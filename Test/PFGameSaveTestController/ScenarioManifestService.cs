using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace PFGameSaveTestController
{
    internal sealed class ScenarioManifestService
    {
        private readonly ScenarioManifestLoader _loader;
        private readonly Action<string, bool, bool> _logger;

        public ScenarioManifestService(Action<string, bool, bool> logger)
            : this(new ScenarioManifestLoader(), logger)
        {
        }

        public ScenarioManifestService(ScenarioManifestLoader loader, Action<string, bool, bool> logger)
        {
            _loader = loader;
            _logger = logger;
        }

        public async Task<ScenarioPlan?> LoadAsync(string path)
        {
            if (string.IsNullOrWhiteSpace(path))
            {
                throw new ArgumentException("Path must be provided", nameof(path));
            }

            string resolvedPath = ResolvePath(path);
            _logger($"Loading scenario manifest '{resolvedPath}'...", false, false);

            ScenarioManifest? manifest = await _loader.LoadAsync(resolvedPath, _logger).ConfigureAwait(false);
            if (manifest == null)
            {
                _logger("Scenario manifest parsed to null.", false, false);
                return null;
            }

            IReadOnlyList<ScenarioCommandInvocation> commands = ScenarioManifestLoader.BuildCommandList(manifest);
            IReadOnlyList<ScenarioCleanupInvocation> cleanup = ScenarioPlan.BuildCleanupCommands(manifest);

            _logger("Scenario manifest load complete.", false, false);
            return new ScenarioPlan(manifest, commands, cleanup);
        }

        private static string ResolvePath(string path)
        {
            return Path.IsPathRooted(path)
                ? path
                : Path.Combine(AppContext.BaseDirectory, path);
        }
    }

    internal sealed class ScenarioPlan
    {
        public ScenarioPlan(ScenarioManifest manifest, IReadOnlyList<ScenarioCommandInvocation> commands, IReadOnlyList<ScenarioCleanupInvocation> cleanupCommands)
        {
            Manifest = manifest;
            Commands = commands;
            CleanupCommands = cleanupCommands;
            DefaultTimeoutSeconds = manifest.Defaults?.StepTimeoutSeconds ?? 30;
            RequiredDevices = BuildRequiredDevices(manifest);
        }

        public ScenarioManifest Manifest { get; }

        public IReadOnlyList<ScenarioCommandInvocation> Commands { get; }

        public IReadOnlyList<ScenarioCleanupInvocation> CleanupCommands { get; }

        public int DefaultTimeoutSeconds { get; }

        public IReadOnlyCollection<string> RequiredDevices { get; }

        private static IReadOnlyCollection<string> BuildRequiredDevices(ScenarioManifest manifest)
        {
            var devices = new HashSet<string>(StringComparer.OrdinalIgnoreCase);

            if (manifest.Devices != null)
            {
                foreach (KeyValuePair<string, ScenarioDevice> entry in manifest.Devices)
                {
                    if (!string.IsNullOrWhiteSpace(entry.Key))
                    {
                        devices.Add(entry.Key);
                    }
                }
            }

            if (manifest.ExecutionOrder != null)
            {
                foreach (ScenarioExecutionOrder? order in manifest.ExecutionOrder)
                {
                    if (!string.IsNullOrWhiteSpace(order?.Role))
                    {
                        devices.Add(order!.Role!);
                    }
                }
            }

            if (manifest.Cleanup != null)
            {
                foreach (KeyValuePair<string, List<ScenarioStep>> entry in manifest.Cleanup)
                {
                    if (!string.IsNullOrWhiteSpace(entry.Key))
                    {
                        devices.Add(entry.Key);
                    }
                }
            }

            return devices.Count > 0
                ? devices.ToArray()
                : Array.Empty<string>();
        }

        internal static IReadOnlyList<ScenarioCleanupInvocation> BuildCleanupCommands(ScenarioManifest manifest)
        {
            if (manifest.Cleanup == null || manifest.Cleanup.Count == 0)
            {
                return Array.Empty<ScenarioCleanupInvocation>();
            }

            var steps = new List<ScenarioCleanupInvocation>();
            foreach (KeyValuePair<string, List<ScenarioStep>> entry in manifest.Cleanup)
            {
                if (string.IsNullOrWhiteSpace(entry.Key) || entry.Value == null)
                {
                    continue;
                }

                foreach (ScenarioStep? step in entry.Value)
                {
                    if (step == null || string.IsNullOrWhiteSpace(step.Command))
                    {
                        continue;
                    }

                    steps.Add(new ScenarioCleanupInvocation(entry.Key, step));
                }
            }

            return steps.Count > 0 ? steps : Array.Empty<ScenarioCleanupInvocation>();
        }
    }

    internal readonly record struct ScenarioCleanupInvocation(string Role, ScenarioStep Step);
}
