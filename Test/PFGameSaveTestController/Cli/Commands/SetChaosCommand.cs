using System;
using System.Linq;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Configures chaos mode parameters
    /// </summary>
    internal sealed class SetChaosCommand : ICliCommand
    {
        private readonly ChaosModeScenarioParameters _settings;

        public SetChaosCommand(ChaosModeScenarioParameters settings)
        {
            _settings = settings;
        }

        public string Name => "set-chaos";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Configure chaos mode parameters";
        public string Usage => "set-chaos <parameter> <value> | set-chaos (show current settings)";

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            if (args.Length == 0)
            {
                // Show current settings
                var message = new System.Text.StringBuilder();
                message.AppendLine("Current chaos mode settings:");
                message.AppendLine();
                message.AppendLine($"  FileCreate:         {_settings.FileCreate}");
                message.AppendLine($"  FileModify:         {_settings.FileModify}");
                message.AppendLine($"  FileDelete:         {_settings.FileDelete}");
                message.AppendLine($"  FolderCreate:       {_settings.FolderCreate}");
                message.AppendLine($"  FolderDelete:       {_settings.FolderDelete}");
                message.AppendLine($"  BinaryFiles:        {_settings.BinaryFiles}");
                message.AppendLine($"  TextFiles:          {_settings.TextFiles}");
                message.AppendLine($"  LargeFiles:         {_settings.LargeFiles}");
                message.AppendLine($"  UnicodeFiles:       {_settings.UnicodeFiles}");
                message.AppendLine($"  UnicodeFolders:     {_settings.UnicodeFolders}");
                message.AppendLine($"  OperationsPerUpload: {_settings.OperationsPerUpload}");
                message.AppendLine($"  NumUploads:         {_settings.NumUploads}");
                message.AppendLine();
                message.AppendLine("Usage: set-chaos <parameter> <value>");
                message.AppendLine("Parameters: FileCreate, FileModify, FileDelete, FolderCreate, FolderDelete,");
                message.AppendLine("            BinaryFiles, TextFiles, LargeFiles, UnicodeFiles, UnicodeFolders,");
                message.AppendLine("            OperationsPerUpload, NumUploads");

                return Task.FromResult(CommandResult.Ok(message.ToString()));
            }

            if (args.Length < 2)
            {
                return Task.FromResult(CommandResult.Error("Usage: set-chaos <parameter> <value>"));
            }

            var parameter = args[0];
            var value = args[1];

            try
            {
                switch (parameter.ToLowerInvariant())
                {
                    case "filecreate":
                        _settings.FileCreate = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"FileCreate set to {_settings.FileCreate}"));

                    case "filemodify":
                        _settings.FileModify = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"FileModify set to {_settings.FileModify}"));

                    case "filedelete":
                        _settings.FileDelete = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"FileDelete set to {_settings.FileDelete}"));

                    case "foldercreate":
                        _settings.FolderCreate = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"FolderCreate set to {_settings.FolderCreate}"));

                    case "folderdelete":
                        _settings.FolderDelete = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"FolderDelete set to {_settings.FolderDelete}"));

                    case "binaryfiles":
                        _settings.BinaryFiles = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"BinaryFiles set to {_settings.BinaryFiles}"));

                    case "textfiles":
                        _settings.TextFiles = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"TextFiles set to {_settings.TextFiles}"));

                    case "largefiles":
                        _settings.LargeFiles = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"LargeFiles set to {_settings.LargeFiles}"));

                    case "unicodefiles":
                        _settings.UnicodeFiles = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"UnicodeFiles set to {_settings.UnicodeFiles}"));

                    case "unicodefolders":
                        _settings.UnicodeFolders = bool.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"UnicodeFolders set to {_settings.UnicodeFolders}"));

                    case "operationsperupload":
                        _settings.OperationsPerUpload = int.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"OperationsPerUpload set to {_settings.OperationsPerUpload}"));

                    case "numuploads":
                        _settings.NumUploads = int.Parse(value);
                        return Task.FromResult(CommandResult.Ok($"NumUploads set to {_settings.NumUploads}"));

                    default:
                        return Task.FromResult(CommandResult.Error($"Unknown parameter: {parameter}. Type 'set-chaos' to see valid parameters."));
                }
            }
            catch (FormatException)
            {
                return Task.FromResult(CommandResult.Error($"Invalid value '{value}' for parameter '{parameter}'"));
            }
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            if (args.Length == 0 || args.Length == 1)
            {
                // Complete parameter names
                var prefix = args.Length > 0 ? args[0] : string.Empty;
                var parameters = new[] 
                { 
                    "FileCreate", "FileModify", "FileDelete", "FolderCreate", "FolderDelete",
                    "BinaryFiles", "TextFiles", "LargeFiles", "UnicodeFiles", "UnicodeFolders",
                    "OperationsPerUpload", "NumUploads"
                };
                return parameters
                    .Where(p => p.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                    .ToArray();
            }
            else if (args.Length == 2)
            {
                // Complete boolean values for boolean parameters
                var prefix = args[1];
                return new[] { "true", "false" }
                    .Where(v => v.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                    .ToArray();
            }

            return Array.Empty<string>();
        }
    }
}
