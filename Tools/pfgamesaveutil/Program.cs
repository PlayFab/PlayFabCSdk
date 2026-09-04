using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Parsing;
using PfGameSaveUtil;
using static PfGameSaveUtil.LocalOperations;
using static PfGameSaveUtil.CloudOperations;
using static PfGameSaveUtil.PlayerResolver;

// ============================================================================
// Shared Options
// ============================================================================

var titleIdOption = new Option<string>(
    name: "--title-id",
    description: "(REQUIRED) PlayFab title ID (e.g., ABCDE)")
{
    IsRequired = true
};

// The secret key may be supplied via --secret-key or, if that's omitted, the
// PLAYFAB_SECRET_KEY environment variable. Using an environment variable keeps the secret
// out of shell history and process argument lists. The CLI flag always takes precedence.
//
// NOTE: the env var is intentionally NOT wired as the option's default value, because
// System.CommandLine prints an option's default in --help output, which would leak the key.
const string SecretKeyEnvVar = "PLAYFAB_SECRET_KEY";
static string? ReadSecretKeyFromEnv()
{
    var v = Environment.GetEnvironmentVariable(SecretKeyEnvVar);
    return string.IsNullOrWhiteSpace(v) ? null : v;
}

var secretKeyOption = new Option<string?>(
    name: "--secret-key",
    description: "(REQUIRED for cloud) PlayFab title secret key (from Game Manager). " +
                 "If omitted, read from the PLAYFAB_SECRET_KEY environment variable.");

var titlePlayerIdOption = new Option<string?>(
    name: "--title-player-id",
    description: "(REQUIRED for cloud) Player's title_player_account entity ID");

var masterPlayerIdOption = new Option<string?>(
    name: "--master-player-id",
    description: "(REQUIRED for cloud) Player's master_player_account ID - use instead of --title-player-id");

var localPathOption = new Option<string>(
    name: "--path",
    description: "Path to the local PGS/save folder, or 'auto' to detect by title ID",
    getDefaultValue: () => "auto");

var forceOption = new Option<bool>(
    name: "--force",
    description: "Skip confirmation prompts");

var verboseOption = new Option<bool>(
    name: "--verbose",
    description: "Log all HTTP requests and responses (to stderr) for diagnostics");
verboseOption.AddAlias("-v");

// ============================================================================
// Helper: Resolve player ID and validate cloud access
// ============================================================================

async Task<string?> ResolvePlayerAsync(string titleId, string? secretKey, string? titlePlayerId, string? masterPlayerId)
{
    if (string.IsNullOrEmpty(secretKey))
    {
        Console.Error.WriteLine("Error: a title secret key is required for this command. " +
            "Pass --secret-key, or set the PLAYFAB_SECRET_KEY environment variable.");
        return null;
    }

    if (string.IsNullOrEmpty(titlePlayerId) && string.IsNullOrEmpty(masterPlayerId))
    {
        Console.Error.WriteLine("Error: Must specify either --player-id or --playfab-id");
        return null;
    }
    if (!string.IsNullOrEmpty(titlePlayerId) && !string.IsNullOrEmpty(masterPlayerId))
    {
        Console.Error.WriteLine("Error: Specify only one of --player-id or --playfab-id, not both");
        return null;
    }

    if (!string.IsNullOrEmpty(masterPlayerId))
    {
        return await ResolveTitlePlayerIdAsync(titleId, secretKey, masterPlayerId);
    }
    return titlePlayerId;
}

// ============================================================================
// Command: download (single manifest version → extract to folder)
// ============================================================================

var versionOption = new Option<string?>(
    name: "--version",
    description: "Manifest version to download (defaults to latest finalized)");

var metadataOption = new Option<string?>(
    name: "--metadata",
    description: "Optional folder for diagnostics (raw service responses, extended manifest, chunk zips). Keeps --path clean for upload.");

var downloadCommand = new Command("download", "Download a single manifest version and extract to a folder [--path (REQUIRED) --version --metadata]")
{
    versionOption,
    localPathOption,
    metadataOption
};

downloadCommand.SetHandler(async (context) =>
{
    var titleId = context.ParseResult.GetValueForOption(titleIdOption)!;
    var secretKey = context.ParseResult.GetValueForOption(secretKeyOption) ?? ReadSecretKeyFromEnv();
    var titlePlayerId = context.ParseResult.GetValueForOption(titlePlayerIdOption);
    var masterPlayerId = context.ParseResult.GetValueForOption(masterPlayerIdOption);
    var localPath = context.ParseResult.GetValueForOption(localPathOption)!;
    var version = context.ParseResult.GetValueForOption(versionOption);
    var metadata = context.ParseResult.GetValueForOption(metadataOption);

    // download requires an explicit destination ('auto' is the local-PGS auto-detect
    // sentinel used by the other verbs and is not a valid download target).
    if (string.IsNullOrWhiteSpace(localPath) || string.Equals(localPath, "auto", StringComparison.OrdinalIgnoreCase))
    {
        Console.Error.WriteLine("Error: download requires --path <folder> specifying where to download to.");
        Environment.Exit(1);
        return;
    }

    var entityId = await ResolvePlayerAsync(titleId, secretKey, titlePlayerId, masterPlayerId);
    if (entityId == null) return;

    await DownloadSingleSaveAsync(titleId, secretKey!, entityId, localPath, version, metadata);
});

// ============================================================================
// Command: upload (compress a folder → new finalized manifest version)
// ============================================================================

var uploadPathOption = new Option<string>(
    name: "--path",
    description: "(REQUIRED) Path to the local folder whose contents will be uploaded")
{
    IsRequired = true
};

var descriptionOption = new Option<string?>(
    name: "--description",
    description: "Optional short save description to attach to the manifest");

var maxZipMbOption = new Option<int>(
    name: "--max-zip-mb",
    description: "Maximum size of each zip chunk in MB (default 64; the console cannot extract larger zips)",
    getDefaultValue: () => 64);

var uploadCommand = new Command("upload", "Compress a folder and upload it as a new finalized save version [--path --description --force --max-zip-mb]")
{
    uploadPathOption,
    descriptionOption,
    forceOption,
    maxZipMbOption
};

uploadCommand.SetHandler(async (context) =>
{
    var titleId = context.ParseResult.GetValueForOption(titleIdOption)!;
    var secretKey = context.ParseResult.GetValueForOption(secretKeyOption) ?? ReadSecretKeyFromEnv();
    var titlePlayerId = context.ParseResult.GetValueForOption(titlePlayerIdOption);
    var masterPlayerId = context.ParseResult.GetValueForOption(masterPlayerIdOption);
    var path = context.ParseResult.GetValueForOption(uploadPathOption)!;
    var description = context.ParseResult.GetValueForOption(descriptionOption);
    var force = context.ParseResult.GetValueForOption(forceOption);
    var maxZipMb = context.ParseResult.GetValueForOption(maxZipMbOption);

    var entityId = await ResolvePlayerAsync(titleId, secretKey, titlePlayerId, masterPlayerId);
    if (entityId == null) return;

    await UploadSaveAsync(titleId, secretKey!, entityId, path, description, force, (long)maxZipMb * 1024 * 1024);
});

// ============================================================================
// Command: downloadall (all manifest versions → per-version folders)
// ============================================================================
var downloadAllCommand = new Command("downloadall", "Download all manifest versions into per-version folders (clean saves in extracted/, diagnostics in diag/) [--path (REQUIRED)]");

downloadAllCommand.SetHandler(async (context) =>
{
    var titleId = context.ParseResult.GetValueForOption(titleIdOption)!;
    var secretKey = context.ParseResult.GetValueForOption(secretKeyOption) ?? ReadSecretKeyFromEnv();
    var titlePlayerId = context.ParseResult.GetValueForOption(titlePlayerIdOption);
    var masterPlayerId = context.ParseResult.GetValueForOption(masterPlayerIdOption);
    var localPath = context.ParseResult.GetValueForOption(localPathOption)!;

    // downloadall requires an explicit destination ('auto' is the local-PGS auto-detect
    // sentinel used by the other verbs and is not a valid download target).
    if (string.IsNullOrWhiteSpace(localPath) || string.Equals(localPath, "auto", StringComparison.OrdinalIgnoreCase))
    {
        Console.Error.WriteLine("Error: downloadall requires --path <folder> specifying where to download to.");
        Environment.Exit(1);
        return;
    }

    var entityId = await ResolvePlayerAsync(titleId, secretKey, titlePlayerId, masterPlayerId);
    if (entityId == null) return;

    await DownloadAllSavesAsync(titleId, secretKey!, entityId, localPath, null);
});

// ============================================================================
// Command: info
// ============================================================================

var jsonOption = new Option<bool>(
    name: "--json",
    description: "Output in JSON format");

var pendingDeleteOption = new Option<bool>(
    name: "--pending-delete",
    description: "Include manifests in PendingDeletion state (hidden by default; they cannot be downloaded)");

var infoCommand = new Command("info", "Show player's save state info [--json --pending-delete]")
{
    jsonOption,
    pendingDeleteOption
};

infoCommand.SetHandler(async (context) =>
{
    var titleId = context.ParseResult.GetValueForOption(titleIdOption)!;
    var secretKey = context.ParseResult.GetValueForOption(secretKeyOption) ?? ReadSecretKeyFromEnv();
    var titlePlayerId = context.ParseResult.GetValueForOption(titlePlayerIdOption);
    var masterPlayerId = context.ParseResult.GetValueForOption(masterPlayerIdOption);
    var json = context.ParseResult.GetValueForOption(jsonOption);
    var includePendingDelete = context.ParseResult.GetValueForOption(pendingDeleteOption);

    var entityId = await ResolvePlayerAsync(titleId, secretKey, titlePlayerId, masterPlayerId);
    if (entityId == null) return;

    await ShowInfoAsync(titleId, secretKey!, entityId, true, json, includePendingDelete);
});

// ============================================================================
// Command: compare
// ============================================================================

var compareCommand = new Command("compare", "Compare cloud vs local files [--path]");

compareCommand.SetHandler(async (context) =>
{
    var titleId = context.ParseResult.GetValueForOption(titleIdOption)!;
    var secretKey = context.ParseResult.GetValueForOption(secretKeyOption) ?? ReadSecretKeyFromEnv();
    var titlePlayerId = context.ParseResult.GetValueForOption(titlePlayerIdOption);
    var masterPlayerId = context.ParseResult.GetValueForOption(masterPlayerIdOption);
    var localPath = context.ParseResult.GetValueForOption(localPathOption)!;

    var entityId = await ResolvePlayerAsync(titleId, secretKey, titlePlayerId, masterPlayerId);
    if (entityId == null) return;

    await CompareWithLocalAsync(titleId, secretKey!, entityId, localPath, null);
});

// ============================================================================
// Command: reset
// ============================================================================

var cloudOption = new Option<bool>(
    name: "--cloud",
    description: "Delete cloud save data");

var localOption = new Option<bool>(
    name: "--local",
    description: "Delete local PGS folder");

var resetCommand = new Command("reset", "Delete save data [--path --force --cloud --local]")
{
    forceOption,
    cloudOption,
    localOption
};

resetCommand.SetHandler(async (context) =>
{
    var titleId = context.ParseResult.GetValueForOption(titleIdOption)!;
    var secretKey = context.ParseResult.GetValueForOption(secretKeyOption) ?? ReadSecretKeyFromEnv();
    var titlePlayerId = context.ParseResult.GetValueForOption(titlePlayerIdOption);
    var masterPlayerId = context.ParseResult.GetValueForOption(masterPlayerIdOption);
    var localPath = context.ParseResult.GetValueForOption(localPathOption)!;
    var force = context.ParseResult.GetValueForOption(forceOption);
    var resetCloud = context.ParseResult.GetValueForOption(cloudOption);
    var resetLocal = context.ParseResult.GetValueForOption(localOption);

    // Local-only reset doesn't require cloud access
    if (resetLocal && !resetCloud)
    {
        ResetLocalOnly(titleId, localPath, force);
        return;
    }

    // Determine scope (default to cloud if none specified)
    var doCloud = resetCloud || (!resetCloud && !resetLocal);
    var doLocal = resetLocal;

    // Cloud operations require credentials
    if (doCloud)
    {
        var entityId = await ResolvePlayerAsync(titleId, secretKey, titlePlayerId, masterPlayerId);
        if (entityId == null) return;
        await ResetAsync(titleId, secretKey!, entityId, force, doCloud, doLocal, localPath);
    }
    else
    {
        ResetLocalOnly(titleId, localPath, force);
    }
});

// ============================================================================
// Command: collect
// ============================================================================

var collectCommand = new Command("collect", "Zip local PGS folder for debugging [--path]");

collectCommand.SetHandler((context) =>
{
    var titleId = context.ParseResult.GetValueForOption(titleIdOption)!;
    var localPath = context.ParseResult.GetValueForOption(localPathOption)!;
    CollectLocalPgsFolder(titleId, localPath);
});

// ============================================================================
// Command: status
// ============================================================================

var inprocOption = new Option<string?>(
    name: "--force-inproc",
    description: "Set ForceUseInprocGameSaves (true=enable, false=delete)");
inprocOption.ArgumentHelpName = "true|false";

var localServicesOption = new Option<string?>(
    name: "--force-local-services",
    description: "Set ForceUseLocalServices (true=enable, false=delete)");
localServicesOption.ArgumentHelpName = "true|false";

var traceOption = new Option<string?>(
    name: "--trace-to-debugger",
    description: "Set TraceToDebugger (true=enable, false=delete)");
traceOption.ArgumentHelpName = "true|false";

var statusCommand = new Command("status", "Show/modify local device status [--force-inproc --force-local-services --trace-to-debugger]")
{
    inprocOption,
    localServicesOption,
    traceOption
};

statusCommand.SetHandler((context) =>
{
    var titleId = context.ParseResult.GetValueForOption(titleIdOption)!;
    var localPath = context.ParseResult.GetValueForOption(localPathOption)!;
    var inproc = context.ParseResult.GetValueForOption(inprocOption);
    var localServices = context.ParseResult.GetValueForOption(localServicesOption);
    var trace = context.ParseResult.GetValueForOption(traceOption);

    // If any toggle options specified, apply them
    if (inproc != null || localServices != null || trace != null)
    {
        Console.WriteLine("Modifying registry settings:");
        if (inproc != null)
        {
            if (bool.TryParse(inproc, out var val))
            {
                if (val)
                    RegistryHelpers.SetRegistryValue(@"SOFTWARE\Microsoft\GamingServices", "ForceUseInprocGameSaves", 1);
                else
                    RegistryHelpers.DeleteRegistryValue(@"SOFTWARE\Microsoft\GamingServices", "ForceUseInprocGameSaves");
            }
            else
            {
                Console.Error.WriteLine("Error: Invalid value for --force-inproc. Expected 'true' or 'false'.");
                Environment.Exit(1);
            }
        }
        if (localServices != null)
        {
            if (bool.TryParse(localServices, out var val))
            {
                if (val)
                    RegistryHelpers.SetRegistryValue(@"SOFTWARE\Microsoft\GamingServices", "ForceUseLocalServices", 1);
                else
                    RegistryHelpers.DeleteRegistryValue(@"SOFTWARE\Microsoft\GamingServices", "ForceUseLocalServices");
            }
            else
            {
                Console.Error.WriteLine("Error: Invalid value for --force-local-services. Expected 'true' or 'false'.");
                Environment.Exit(1);
            }
        }
        if (trace != null)
        {
            if (bool.TryParse(trace, out var val))
            {
                if (val)
                    RegistryHelpers.SetRegistryValue(@"SOFTWARE\Microsoft\GamingServices\Auth", "TraceToDebugger", 1);
                else
                    RegistryHelpers.DeleteRegistryValue(@"SOFTWARE\Microsoft\GamingServices\Auth", "TraceToDebugger");
            }
            else
            {
                Console.Error.WriteLine("Error: Invalid value for --trace-to-debugger. Expected 'true' or 'false'.");
                Environment.Exit(1);
            }
        }
        Console.WriteLine();
    }

    ShowLocalStatus(titleId, localPath);
});

// ============================================================================
// Root Command
// ============================================================================

var rootCommand = new RootCommand("pfgamesaveutil - download, upload, inspect, compare, and manage PlayFab Game Save data")
{
    downloadCommand,
    downloadAllCommand,
    uploadCommand,
    infoCommand,
    compareCommand,
    resetCommand,
    collectCommand,
    statusCommand
};

// Add shared options as global so they're available to all subcommands
rootCommand.AddGlobalOption(titleIdOption);
rootCommand.AddGlobalOption(secretKeyOption);
rootCommand.AddGlobalOption(titlePlayerIdOption);
rootCommand.AddGlobalOption(masterPlayerIdOption);
rootCommand.AddGlobalOption(verboseOption);

// --path applies to every cloud/local verb (download uses it via the command list above).
downloadAllCommand.AddOption(localPathOption);
compareCommand.AddOption(localPathOption);
resetCommand.AddOption(localPathOption);
collectCommand.AddOption(localPathOption);
statusCommand.AddOption(localPathOption);

// Build parser without the --version directive
var parser = new CommandLineBuilder(rootCommand)
    .UseHelp()
    .UseEnvironmentVariableDirective()
    .UseParseDirective()
    .UseSuggestDirective()
    .RegisterWithDotnetSuggest()
    .UseTypoCorrections()
    .UseParseErrorReporting()
    .UseExceptionHandler()
    .CancelOnProcessTermination()
    // Enable HTTP traffic logging (and install the PlayFab SDK transport logger) when
    // --verbose is set, before any command handler runs.
    .AddMiddleware(async (context, next) =>
    {
        if (context.ParseResult.GetValueForOption(verboseOption))
        {
            HttpLogger.Enabled = true;
            LoggingTransportPlugin.Install();
            HttpLogger.Line("[verbose] HTTP logging enabled. Requests/responses are written to stderr; secrets are redacted.");
        }
        await next(context);
    })
    .Build();

return await parser.InvokeAsync(args);
