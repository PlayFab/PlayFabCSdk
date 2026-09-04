using System.Text;
using System.Text.Encodings.Web;
using System.Text.Json;
using System.Text.Json.Nodes;

namespace PfGameSaveUtil;

/// <summary>
/// Lightweight HTTP traffic logger toggled by the global --verbose switch.
///
/// All output goes to stderr so it never pollutes parseable stdout (e.g. `info --json`).
/// Secrets are redacted: sensitive headers (secret key / entity token / authorization)
/// and Azure Storage SAS query strings are never printed in full.
/// </summary>
public static class HttpLogger
{
    public static bool Enabled { get; set; }

    private static readonly JsonSerializerOptions s_pretty = new()
    {
        WriteIndented = true,
        // Render '<redacted>' and URL characters literally instead of \uXXXX escapes
        // (this output is for human diagnostics, not for re-parsing as a web payload).
        Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
    };

    private static readonly string[] s_sensitiveHeaderHints = { "secret", "token", "authoriz", "x-ms-encryption" };

    // JSON property names whose values must be redacted in logged request/response bodies.
    private static readonly HashSet<string> s_sensitiveJsonKeys = new(StringComparer.OrdinalIgnoreCase)
    {
        "EntityToken", "SecretKey", "DeveloperSecretKey", "SessionTicket", "Password", "TitleSharedSecret"
    };

    // SAS query parameter names that should never be printed.
    private static readonly HashSet<string> s_sasParams = new(StringComparer.OrdinalIgnoreCase)
    {
        "sig", "se", "sp", "sv", "sr", "st", "skoid", "sktid", "skt", "ske", "sks", "skv", "spr", "si", "rscd"
    };

    public static void Line(string message)
    {
        if (!Enabled) return;
        Console.Error.WriteLine(message);
    }

    public static void Request(string method, string url, IEnumerable<KeyValuePair<string, string>>? headers, string? body)
    {
        if (!Enabled) return;
        Console.Error.WriteLine();
        Console.Error.WriteLine($">> {method} {RedactUrl(url)}");
        if (headers != null)
        {
            foreach (var h in headers)
            {
                Console.Error.WriteLine($">>   {h.Key}: {RedactHeader(h.Key, h.Value)}");
            }
        }
        if (!string.IsNullOrEmpty(body))
        {
            Console.Error.WriteLine(">>   body:");
            Console.Error.WriteLine(Indent(Pretty(body), "        "));
        }
    }

    public static void Response(string url, int statusCode, string statusText, long elapsedMs,
        IEnumerable<KeyValuePair<string, string>>? headers = null, string? body = null)
    {
        if (!Enabled) return;
        Console.Error.WriteLine($"<< {statusCode} {statusText}  ({elapsedMs} ms)  {RedactUrl(url)}");
        if (headers != null)
        {
            foreach (var h in headers)
            {
                Console.Error.WriteLine($"<<   {h.Key}: {RedactHeader(h.Key, h.Value)}");
            }
        }
        if (!string.IsNullOrEmpty(body))
        {
            Console.Error.WriteLine("<<   body:");
            Console.Error.WriteLine(Indent(Pretty(body), "        "));
        }
    }

    public static string Serialize(object? value)
    {
        if (value == null) return "null";
        try { return JsonSerializer.Serialize(value, s_pretty); }
        catch { return value.ToString() ?? "null"; }
    }

    // --- redaction helpers ---------------------------------------------------

    public static string RedactUrl(string url)
    {
        if (string.IsNullOrEmpty(url)) return url;
        int q = url.IndexOf('?');
        if (q < 0) return url;

        var baseUrl = url.Substring(0, q);
        var query = url.Substring(q + 1);
        var parts = query.Split('&');
        var rebuilt = new List<string>(parts.Length);
        bool redactedAny = false;
        foreach (var part in parts)
        {
            var eq = part.IndexOf('=');
            var key = eq >= 0 ? part.Substring(0, eq) : part;
            if (s_sasParams.Contains(key))
            {
                rebuilt.Add($"{key}=<redacted>");
                redactedAny = true;
            }
            else
            {
                rebuilt.Add(part);
            }
        }
        var suffix = redactedAny ? "  (SAS)" : "";
        return $"{baseUrl}?{string.Join("&", rebuilt)}{suffix}";
    }

    private static string RedactHeader(string name, string value)
    {
        foreach (var hint in s_sensitiveHeaderHints)
        {
            if (name.IndexOf(hint, StringComparison.OrdinalIgnoreCase) >= 0)
            {
                return "<redacted>";
            }
        }
        return value;
    }

    private static string Pretty(string body)
    {
        var trimmed = body.TrimStart();
        if (trimmed.StartsWith("{") || trimmed.StartsWith("["))
        {
            try
            {
                var node = JsonNode.Parse(body);
                RedactJson(node);
                return node?.ToJsonString(s_pretty) ?? body;
            }
            catch { /* not valid JSON; print as-is */ }
        }
        return body;
    }

    // Recursively redacts sensitive values in a JSON tree: values of known sensitive
    // keys (e.g. EntityToken, SecretKey) and any string that is a SAS-bearing URL.
    private static void RedactJson(JsonNode? node)
    {
        switch (node)
        {
            case JsonObject obj:
                foreach (var key in obj.Select(kv => kv.Key).ToList())
                {
                    var child = obj[key];
                    if (s_sensitiveJsonKeys.Contains(key) && child is JsonValue)
                    {
                        obj[key] = "<redacted>";
                    }
                    else if (child is JsonValue v && v.TryGetValue<string>(out var s) && IsSasUrl(s))
                    {
                        obj[key] = RedactUrl(s);
                    }
                    else
                    {
                        RedactJson(child);
                    }
                }
                break;
            case JsonArray arr:
                for (int i = 0; i < arr.Count; i++)
                {
                    var child = arr[i];
                    if (child is JsonValue v && v.TryGetValue<string>(out var s) && IsSasUrl(s))
                    {
                        arr[i] = RedactUrl(s);
                    }
                    else
                    {
                        RedactJson(child);
                    }
                }
                break;
        }
    }

    private static bool IsSasUrl(string value)
    {
        return value.IndexOf("sig=", StringComparison.OrdinalIgnoreCase) >= 0 && value.Contains('?');
    }

    private static string Indent(string text, string indent)
    {
        var sb = new StringBuilder();
        foreach (var line in text.Replace("\r\n", "\n").Split('\n'))
        {
            sb.Append(indent).AppendLine(line);
        }
        return sb.ToString().TrimEnd('\r', '\n');
    }
}
