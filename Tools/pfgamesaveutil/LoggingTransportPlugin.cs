using System.Diagnostics;
using PlayFab;

namespace PfGameSaveUtil;

/// <summary>
/// Wraps the PlayFab C# SDK's default transport plugin so that the SDK-driven HTTP
/// calls (authentication, profile/player resolution) are logged when --verbose is set.
///
/// The PlayFab GameSave REST calls and blob transfers do NOT go through this plugin —
/// they are logged directly in <see cref="GameSaveClient"/>. This plugin only covers
/// the calls the PlayFab SDK itself makes (e.g. GetEntityToken, GetTitlePlayers...).
/// </summary>
public sealed class LoggingTransportPlugin : ITransportPlugin
{
    private readonly ITransportPlugin _inner;

    public LoggingTransportPlugin(ITransportPlugin inner)
    {
        _inner = inner;
    }

    public async Task<object> DoPost(string fullUrl, object request, Dictionary<string, string> extraHeaders)
    {
        if (HttpLogger.Enabled)
        {
            HttpLogger.Request("POST", fullUrl, extraHeaders, HttpLogger.Serialize(request));
        }

        var sw = Stopwatch.StartNew();
        object result;
        try
        {
            result = await _inner.DoPost(fullUrl, request, extraHeaders);
        }
        catch (Exception ex)
        {
            if (HttpLogger.Enabled)
            {
                HttpLogger.Line($"<< (exception after {sw.ElapsedMilliseconds} ms) {HttpLogger.RedactUrl(fullUrl)}: {ex.Message}");
            }
            throw;
        }

        if (HttpLogger.Enabled)
        {
            var body = result as string ?? HttpLogger.Serialize(result);
            HttpLogger.Response(fullUrl, 200, "OK (SDK)", sw.ElapsedMilliseconds, headers: null, body: body);
        }

        return result;
    }

    /// <summary>
    /// Installs this logging wrapper around the SDK's current transport plugin (idempotent).
    /// </summary>
    public static void Install()
    {
        var current = PluginManager.GetPlugin<ITransportPlugin>(PluginContract.PlayFab_Transport, "");
        if (current is LoggingTransportPlugin)
        {
            return;
        }
        PluginManager.SetPlugin(new LoggingTransportPlugin(current), PluginContract.PlayFab_Transport, "");
    }
}
