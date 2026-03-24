using System;
using System.Globalization;
using System.Text.Json;

namespace PFGameSaveTestController
{
    internal sealed class ActionResultHandler
    {
        private readonly CommandProcessor _commandProcessor;
        private readonly Action<string, bool, bool> _logger;

        public ActionResultHandler(CommandProcessor commandProcessor, Action<string, bool, bool> logger)
        {
            _commandProcessor = commandProcessor;
            _logger = logger;
        }

        public bool TryHandle(string deviceName, string message)
        {
            if (string.IsNullOrWhiteSpace(message))
            {
                return false;
            }

            try
            {
                using JsonDocument doc = JsonDocument.Parse(message);
                JsonElement root = doc.RootElement;
                if (root.ValueKind != JsonValueKind.Object)
                {
                    return false;
                }

                if (!root.TryGetProperty("type", out JsonElement typeElement) || typeElement.ValueKind != JsonValueKind.String)
                {
                    return false;
                }

                if (!string.Equals(typeElement.GetString(), "actionResult", StringComparison.OrdinalIgnoreCase))
                {
                    return false;
                }

                ActionResult result = ParseActionResult(root, message);

                bool matched = _commandProcessor.HandleResult(result);
                if (!matched)
                {
                    _logger($"[{deviceName}] Untracked ActionResult: commandId={result.CommandId}, status={result.Status}, deviceId={result.DeviceId}, elapsedMs={result.ElapsedMs}, timestamp={result.Timestamp:O}", false, false);
                }

                return true;
            }
            catch (JsonException ex)
            {
                _logger($"[{deviceName}] Failed to parse action result JSON: {ex.Message}", false, false);
                return false;
            }
        }

        private static ActionResult ParseActionResult(JsonElement root, string rawJson)
        {
            var result = new ActionResult
            {
                RawJson = rawJson ?? string.Empty
            };

            if (root.TryGetProperty("type", out JsonElement typeElement) && typeElement.ValueKind == JsonValueKind.String)
            {
                result.Type = typeElement.GetString() ?? string.Empty;
            }

            if (root.TryGetProperty("commandId", out JsonElement commandIdElement) && commandIdElement.ValueKind == JsonValueKind.String)
            {
                result.CommandId = commandIdElement.GetString() ?? string.Empty;
            }

            if (root.TryGetProperty("command", out JsonElement commandElement) && commandElement.ValueKind == JsonValueKind.String)
            {
                result.Command = commandElement.GetString() ?? string.Empty;
            }

            if (root.TryGetProperty("deviceId", out JsonElement deviceIdElement) && deviceIdElement.ValueKind == JsonValueKind.String)
            {
                result.DeviceId = deviceIdElement.GetString() ?? string.Empty;
            }

            if (root.TryGetProperty("status", out JsonElement statusElement) && statusElement.ValueKind == JsonValueKind.String)
            {
                result.Status = statusElement.GetString() ?? string.Empty;
            }

            if (root.TryGetProperty("elapsedMs", out JsonElement elapsedElement))
            {
                if (elapsedElement.TryGetInt32(out int elapsed))
                {
                    result.ElapsedMs = elapsed;
                }
                else if (elapsedElement.ValueKind == JsonValueKind.String && int.TryParse(elapsedElement.GetString(), NumberStyles.Integer, CultureInfo.InvariantCulture, out int parsedElapsed))
                {
                    result.ElapsedMs = parsedElapsed;
                }
            }

            if (root.TryGetProperty("timestamp", out JsonElement timestampElement) && timestampElement.ValueKind == JsonValueKind.String)
            {
                string? timestampText = timestampElement.GetString();
                if (!string.IsNullOrEmpty(timestampText) && DateTimeOffset.TryParse(timestampText, CultureInfo.InvariantCulture, DateTimeStyles.RoundtripKind, out DateTimeOffset parsedTimestamp))
                {
                    result.Timestamp = parsedTimestamp;
                }
            }

            if (root.TryGetProperty("hresult", out JsonElement hresultElement))
            {
                string? normalized = NormalizeHResult(hresultElement);
                if (!string.IsNullOrWhiteSpace(normalized))
                {
                    result.HResult = normalized.Trim();
                }
            }

            if (root.TryGetProperty("errorMessage", out JsonElement errorMessageElement))
            {
                string? errorMessage = ExtractStringValue(errorMessageElement);
                if (!string.IsNullOrWhiteSpace(errorMessage))
                {
                    result.ErrorMessage = errorMessage.Trim();
                }
            }

            if (string.IsNullOrEmpty(result.ErrorMessage)
                && root.TryGetProperty("message", out JsonElement messageElement))
            {
                string? message = ExtractStringValue(messageElement);
                if (!string.IsNullOrWhiteSpace(message))
                {
                    result.ErrorMessage = message.Trim();
                }
            }

            return result;
        }

        private static string? ExtractStringValue(JsonElement element)
        {
            switch (element.ValueKind)
            {
                case JsonValueKind.String:
                    return element.GetString();
                case JsonValueKind.Number:
                case JsonValueKind.True:
                case JsonValueKind.False:
                    return element.GetRawText();
                default:
                    return null;
            }
        }

        private static string? NormalizeHResult(JsonElement element)
        {
            switch (element.ValueKind)
            {
                case JsonValueKind.String:
                    return element.GetString();
                case JsonValueKind.Number:
                    if (element.TryGetInt64(out long value))
                    {
                        int truncated = unchecked((int)value);
                        return string.Format(CultureInfo.InvariantCulture, "0x{0:X8}", truncated);
                    }

                    return element.GetRawText();
                default:
                    return null;
            }
        }
    }
}
