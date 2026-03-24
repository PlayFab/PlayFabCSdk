using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace PFGameSaveTestController
{
    internal sealed class ScenarioManifestLoader
    {
        private readonly IDeserializer _deserializer;

        public ScenarioManifestLoader()
        {
            _deserializer = new DeserializerBuilder()
                .WithNamingConvention(CamelCaseNamingConvention.Instance)
                .IgnoreUnmatchedProperties()
                .Build();
        }

        public async Task<ScenarioManifest?> LoadAsync(string path, Action<string, bool, bool>? log = null)
        {
            if (string.IsNullOrWhiteSpace(path))
            {
                throw new ArgumentException("Path must be provided", nameof(path));
            }

            if (!File.Exists(path))
            {
                throw new FileNotFoundException($"Scenario manifest not found: {path}", path);
            }

            log ??= (message, _, _) => Console.WriteLine(message);

            string yaml = await File.ReadAllTextAsync(path).ConfigureAwait(false);
            ScenarioManifest? manifest = _deserializer.Deserialize<ScenarioManifest>(yaml);

            if (manifest == null)
            {
                log($"Scenario manifest '{path}' parsed as null.", false, false);
                return null;
            }

            LogManifest(manifest, log);
            return manifest;
        }

        private static void LogManifest(ScenarioManifest manifest, Action<string, bool, bool> log)
        {
            log($"Scenario Id: {manifest.Id ?? "<missing>"}", true, false);
            log($"Scenario Name: {manifest.Name ?? "<missing>"}", true, false);
            if (manifest.Defaults?.StepTimeoutSeconds != null)
            {
                log($"Default Step Timeout (s): {manifest.Defaults.StepTimeoutSeconds}", true, false);
            }

            if (manifest.Devices != null && manifest.Devices.Count > 0)
            {
                log("Devices:", true, false);
                foreach (KeyValuePair<string, ScenarioDevice> device in manifest.Devices)
                {
                    log($"  - {device.Key}", true, false);
                }
            }

            IReadOnlyList<ScenarioCommandInvocation> commands = BuildCommandList(manifest);
            log($"Total Commands: {commands.Count}", true, false);
            foreach (ScenarioCommandInvocation invocation in commands)
            {
                string parameterSummary = FormatParameters(invocation.Step.Parameters);
                log($"  [{invocation.Role}/{invocation.Block}] {invocation.Step.Command}{parameterSummary}", true, false);
            }

            if (manifest.Cleanup != null && manifest.Cleanup.Count > 0)
            {
                log("Cleanup Steps:", true, false);
                foreach (KeyValuePair<string, List<ScenarioStep>> kv in manifest.Cleanup)
                {
                    foreach (ScenarioStep? step in kv.Value.Where(s => s != null && !string.IsNullOrWhiteSpace(s.Command)))
                    {
                        string parameterSummary = FormatParameters(step!.Parameters);
                        log($"  Role {kv.Key}: {step.Command}{parameterSummary}", true, false);
                    }
                }
            }
        }

        internal static IReadOnlyList<ScenarioCommandInvocation> BuildCommandList(ScenarioManifest manifest)
        {
            if (manifest.ExecutionOrder == null || manifest.Blocks == null)
            {
                return Array.Empty<ScenarioCommandInvocation>();
            }

            List<ScenarioCommandInvocation> results = new List<ScenarioCommandInvocation>();
            foreach (ScenarioExecutionOrder entry in manifest.ExecutionOrder)
            {
                if (entry == null || string.IsNullOrWhiteSpace(entry.Role) || string.IsNullOrWhiteSpace(entry.Block))
                {
                    continue;
                }

                if (!manifest.Blocks.TryGetValue(entry.Block, out List<ScenarioStep>? steps) || steps == null)
                {
                    continue;
                }

                foreach (ScenarioStep? step in steps)
                {
                    if (step == null || string.IsNullOrWhiteSpace(step.Command))
                    {
                        continue;
                    }

                    results.Add(new ScenarioCommandInvocation(entry.Role, entry.Block, step));
                }
            }

            return results;
        }

        private static string FormatParameters(IDictionary<string, object?>? parameters)
        {
            if (parameters == null || parameters.Count == 0)
            {
                return string.Empty;
            }

            IEnumerable<string> segments = parameters.Select(pair => $"{pair.Key}={DescribeValue(pair.Value)}");
            return $" (params: {string.Join(", ", segments)})";
        }

        private static string DescribeValue(object? value)
        {
            if (value == null)
            {
                return "null";
            }

            switch (value)
            {
                case string s:
                    return $"\"{s}\"";
                case IDictionary<string, object?> dict:
                    return "{" + string.Join(", ", dict.Select(kvp => $"{kvp.Key}:{DescribeValue(kvp.Value)}")) + "}";
                case IDictionary<object, object> map:
                    return "{" + string.Join(", ", map.Select(kvp => $"{kvp.Key}:{DescribeValue(kvp.Value)}")) + "}";
                case IEnumerable enumerable when value is not string:
                    return "[" + string.Join(", ", enumerable.Cast<object?>().Select(DescribeValue)) + "]";
                default:
                    return value.ToString() ?? string.Empty;
            }
        }
    }

    internal sealed class ScenarioManifest
    {
        [YamlMember(Alias = "id")]
        public string? Id { get; set; }

        [YamlMember(Alias = "name")]
        public string? Name { get; set; }

        [YamlMember(Alias = "tags")]
        public List<string> Tags { get; set; } = new();

        [YamlMember(Alias = "devices")]
        public Dictionary<string, ScenarioDevice> Devices { get; set; } = new();

        [YamlMember(Alias = "defaults")]
        public ScenarioDefaults? Defaults { get; set; }

        [YamlMember(Alias = "blocks")]
        public Dictionary<string, List<ScenarioStep>> Blocks { get; set; } = new();

        [YamlMember(Alias = "executionOrder")]
        public List<ScenarioExecutionOrder> ExecutionOrder { get; set; } = new();

        [YamlMember(Alias = "cleanup")]
        public Dictionary<string, List<ScenarioStep>> Cleanup { get; set; } = new();
    }

    internal sealed class ScenarioDefaults
    {
        [YamlMember(Alias = "stepTimeoutSeconds")]
        public int? StepTimeoutSeconds { get; set; }
    }

    internal sealed class ScenarioDevice
    {
    }

    internal sealed class ScenarioStep
    {
        [YamlMember(Alias = "command")]
        public string? Command { get; set; }

        [YamlMember(Alias = "parameters")]
        public IDictionary<string, object?>? Parameters { get; set; }

        [YamlMember(Alias = "timeoutSeconds")]
        public int? TimeoutSeconds { get; set; }
    }

    internal sealed class ScenarioExecutionOrder
    {
        [YamlMember(Alias = "role")]
        public string? Role { get; set; }

        [YamlMember(Alias = "block")]
        public string? Block { get; set; }
    }

    internal readonly record struct ScenarioCommandInvocation(string Role, string Block, ScenarioStep Step);
}
