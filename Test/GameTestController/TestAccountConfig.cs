using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace GameTestController
{
    /// <summary>
    /// Loads and provides access to test account configuration from testAccountConfig.json.
    /// Centralizes account email addresses so they are not hardcoded throughout the codebase.
    /// 
    /// Key constraints enforced:
    /// - No account overlap: PC and Xbox accounts must be distinct to avoid "signed in elsewhere" dialogs.
    /// - Xbox minimum: At least 2 accounts on Xbox for multi-user tests.
    /// - Sandbox alignment: All devices must be in the same Xbox Live sandbox.
    /// - Shared PlayFab entity: All devices share the same PlayFab entity via customId login.
    /// </summary>
    internal sealed class TestAccountConfig
    {
        private static TestAccountConfig? _instance;
        private static readonly object _lock = new();

        public string Sandbox { get; }
        public string PcEmail { get; }
        public string XboxPrimaryEmail { get; }
        public string XboxSecondaryEmail { get; }
        public string PcPassword { get; }
        public string XboxPrimaryPassword { get; }
        public string XboxSecondaryPassword { get; }

        private TestAccountConfig(ConfigFile config)
        {
            Sandbox = config.Sandbox ?? "XDKS.1";
            PcEmail = config.Accounts?.Pc?.Email ?? string.Empty;
            XboxPrimaryEmail = config.Accounts?.XboxPrimary?.Email ?? string.Empty;
            XboxSecondaryEmail = config.Accounts?.XboxSecondary?.Email ?? string.Empty;
            PcPassword = config.Accounts?.Pc?.Password ?? string.Empty;
            XboxPrimaryPassword = config.Accounts?.XboxPrimary?.Password ?? string.Empty;
            XboxSecondaryPassword = config.Accounts?.XboxSecondary?.Password ?? string.Empty;
        }

        /// <summary>
        /// Gets the singleton instance, loading from testAccountConfig.json on first access.
        /// Returns null if the config file is not found (graceful degradation).
        /// </summary>
        public static TestAccountConfig? Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_lock)
                    {
                        if (_instance == null)
                        {
                            _instance = TryLoad();
                        }
                    }
                }
                return _instance;
            }
        }

        /// <summary>
        /// Gets the Xbox account emails to suggest when users need to sign in.
        /// Returns secondary first (it's the one most likely to be missing).
        /// </summary>
        public IEnumerable<string> GetXboxSignInHints()
        {
            if (!string.IsNullOrEmpty(XboxSecondaryEmail))
                yield return XboxSecondaryEmail;
            if (!string.IsNullOrEmpty(XboxPrimaryEmail))
                yield return XboxPrimaryEmail;
        }

        /// <summary>
        /// Validates that the configured accounts satisfy all constraints.
        /// Returns a list of violation messages, or empty if all constraints pass.
        /// </summary>
        public List<string> ValidateConstraints()
        {
            var violations = new List<string>();

            // Constraint: No account overlap between PC and Xbox
            if (!string.IsNullOrEmpty(PcEmail))
            {
                if (string.Equals(PcEmail, XboxPrimaryEmail, StringComparison.OrdinalIgnoreCase))
                    violations.Add($"Account overlap: PC account '{PcEmail}' is also the Xbox primary. " +
                        "This will cause 'signed in on another device' dialogs during two-device tests.");

                if (string.Equals(PcEmail, XboxSecondaryEmail, StringComparison.OrdinalIgnoreCase))
                    violations.Add($"Account overlap: PC account '{PcEmail}' is also the Xbox secondary. " +
                        "This will cause 'signed in on another device' dialogs during multi-user tests.");
            }

            // Constraint: Xbox needs at least 2 distinct accounts for multi-user tests
            if (string.IsNullOrEmpty(XboxPrimaryEmail))
                violations.Add("Xbox primary account is not configured. Most tests require at least one Xbox account.");

            if (string.IsNullOrEmpty(XboxSecondaryEmail))
                violations.Add("Xbox secondary account is not configured. Multi-user tests (70, 104, 105, 106) require two Xbox accounts.");

            if (!string.IsNullOrEmpty(XboxPrimaryEmail) && !string.IsNullOrEmpty(XboxSecondaryEmail) &&
                string.Equals(XboxPrimaryEmail, XboxSecondaryEmail, StringComparison.OrdinalIgnoreCase))
                violations.Add($"Xbox primary and secondary are the same account '{XboxPrimaryEmail}'. " +
                    "Multi-user tests require two distinct accounts on the console.");

            return violations;
        }

        /// <summary>
        /// Checks whether a specific Xbox user email (from xbuser list) matches the PC account,
        /// which would indicate an overlap risk. Used during runtime prerequisite checks.
        /// </summary>
        public bool IsOverlappingWithPc(string xboxUserEmail)
        {
            return !string.IsNullOrEmpty(PcEmail) &&
                   string.Equals(PcEmail, xboxUserEmail, StringComparison.OrdinalIgnoreCase);
        }

        private static TestAccountConfig? TryLoad()
        {
            string? configPath = FindConfigFile();
            if (configPath == null)
                return null;

            try
            {
                string json = File.ReadAllText(configPath);
                var options = new JsonSerializerOptions
                {
                    PropertyNameCaseInsensitive = true,
                    ReadCommentHandling = JsonCommentHandling.Skip
                };
                var config = JsonSerializer.Deserialize<ConfigFile>(json, options);
                return config != null ? new TestAccountConfig(config) : null;
            }
            catch
            {
                return null;
            }
        }

        private static string? FindConfigFile()
        {
            string[] searchPaths = new[]
            {
                Path.Combine(AppContext.BaseDirectory, "..", "testAccountConfig.json"),
                Path.Combine(AppContext.BaseDirectory, "..", "..", "testAccountConfig.json"),
                Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "testAccountConfig.json"),
                Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "Test", "testAccountConfig.json"),
                Path.Combine(Directory.GetCurrentDirectory(), "..", "testAccountConfig.json"),
                Path.Combine(Directory.GetCurrentDirectory(), "testAccountConfig.json"),
            };

            foreach (string path in searchPaths)
            {
                string fullPath = Path.GetFullPath(path);
                if (File.Exists(fullPath))
                    return fullPath;
            }

            return null;
        }

        // JSON deserialization classes
        private sealed class ConfigFile
        {
            public string? Sandbox { get; set; }
            public AccountSet? Accounts { get; set; }
        }

        private sealed class AccountSet
        {
            public AccountEntry? Pc { get; set; }
            public AccountEntry? XboxPrimary { get; set; }
            public AccountEntry? XboxSecondary { get; set; }
        }

        private sealed class AccountEntry
        {
            public string? Email { get; set; }
            public string? Password { get; set; }
            public string? Role { get; set; }
            public string? Device { get; set; }
        }
    }
}
