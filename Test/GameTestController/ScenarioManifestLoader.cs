using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using YamlDotNet.Core;
using YamlDotNet.Core.Events;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace GameTestController
{
    internal sealed class ScenarioManifestLoader
    {
        private readonly IDeserializer _deserializer;

        public ScenarioManifestLoader()
        {
            _deserializer = new DeserializerBuilder()
                .WithNamingConvention(CamelCaseNamingConvention.Instance)
                .WithTypeConverter(new ScalarOrStringListConverter())
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
                    string engineNote = (device.Value?.HasEngineConstraint ?? false) ? $" (requires engine: {device.Value!.EngineDisplay})" : "";
                    log($"  - {device.Key}{engineNote}", true, false);
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

                // A block may be repeated N times via `repeat:` in executionOrder.
                int repeat = entry.Repeat > 0 ? entry.Repeat : 1;
                for (int iteration = 0; iteration < repeat; iteration++)
                {
                    foreach (ScenarioStep? step in steps)
                    {
                        if (step == null || string.IsNullOrWhiteSpace(step.Command))
                        {
                            continue;
                        }

                        results.Add(new ScenarioCommandInvocation(entry.Role, entry.Block, step));
                    }
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

        [YamlMember(Alias = "skip")]
        public bool? Skip { get; set; }

        [YamlMember(Alias = "tags")]
        public List<string> Tags { get; set; } = new();

        [YamlMember(Alias = "platforms")]
        public Dictionary<string, string> Platforms { get; set; } = new();

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

        /// <summary>
        /// Returns the platform status for the given platform key
        /// (xbox, pc-grts, pc-inproc, psx). Returns "untested" if not specified.
        /// </summary>
        public string GetPlatformStatus(string platformKey)
        {
            string? status = Platforms
                .FirstOrDefault(entry => string.Equals(entry.Key, platformKey, StringComparison.OrdinalIgnoreCase))
                .Value;
            if (!string.IsNullOrEmpty(status))
            {
                // Strip inline comments (e.g., "failing  # reason")
                int commentIdx = status.IndexOf('#');
                string clean = commentIdx >= 0 ? status.Substring(0, commentIdx).Trim() : status.Trim();
                return clean.ToLowerInvariant();
            }
            return "untested";
        }
    }

    internal sealed class ScenarioDefaults
    {
        [YamlMember(Alias = "stepTimeoutSeconds")]
        public int? StepTimeoutSeconds { get; set; }
    }

    internal sealed class ScenarioDevice
    {
        /// <summary>
        /// When set, the device assigned to this role must report one of these engine types
        /// (e.g., "xbox", "pc-grts", "pc-inproc-gamesaves").
        /// Accepts a single string or a YAML list: <c>engine: xbox</c> or <c>engine: [xbox, pc-grts]</c>.
        /// Scenarios are skipped when no connected device satisfies the constraint.
        /// </summary>
        [YamlMember(Alias = "engine")]
        public List<string>? Engine { get; set; }

        /// <summary>
        /// Environment prerequisites that must be met on the device assigned to this role.
        /// The controller validates these before executing the scenario and provides clear
        /// guidance when they are not met (instead of failing with cryptic runtime errors).
        /// </summary>
        [YamlMember(Alias = "prerequisites")]
        public ScenarioPrerequisites? Prerequisites { get; set; }

        /// <summary>
        /// Returns true when this device spec has at least one engine constraint.
        /// </summary>
        public bool HasEngineConstraint => Engine != null && Engine.Count > 0;

        /// <summary>
        /// Returns true when the given device engine satisfies this spec's constraint.
        /// If no constraint is set, returns true (unconstrained).
        /// </summary>
        public bool EngineMatches(string? deviceEngine)
        {
            if (!HasEngineConstraint)
            {
                return true;
            }

            return Engine!.Any(e => string.Equals(e, deviceEngine, StringComparison.OrdinalIgnoreCase));
        }

        /// <summary>
        /// Returns true when this spec requires an in-process engine (pc-inproc or pc-inproc-gamesaves).
        /// Used to pass /forceinproc when auto-launching a local device.
        /// </summary>
        public bool RequiresInproc =>
            HasEngineConstraint
            && Engine!.Any(e => e.StartsWith("pc-inproc", StringComparison.OrdinalIgnoreCase));

        /// <summary>
        /// Returns a display string for the engine constraint (e.g., "xbox" or "xbox|pc-grts").
        /// </summary>
        public string EngineDisplay => HasEngineConstraint ? string.Join("|", Engine!) : "";
    }

    internal sealed class ScenarioStep
    {
        [YamlMember(Alias = "command")]
        public string? Command { get; set; }

        [YamlMember(Alias = "parameters")]
        public IDictionary<string, object?>? Parameters { get; set; }

        [YamlMember(Alias = "timeoutSeconds")]
        public int? TimeoutSeconds { get; set; }

        [YamlMember(Alias = "expectFailure")]
        public bool? ExpectFailure { get; set; }
    }

    internal sealed class ScenarioExecutionOrder
    {
        [YamlMember(Alias = "role")]
        public string? Role { get; set; }

        [YamlMember(Alias = "block")]
        public string? Block { get; set; }

        /// <summary>
        /// Number of times to run this block consecutively. Defaults to 1.
        /// Lets a scenario loop an upload/download/verify cycle without listing the
        /// same entry dozens of times (e.g. a 50x chaos soak).
        /// </summary>
        [YamlMember(Alias = "repeat")]
        public int Repeat { get; set; } = 1;
    }

    /// <summary>
    /// Environment prerequisites for a device role. Validated by the controller before
    /// executing the scenario. When prerequisites are not met, the scenario is skipped
    /// with a clear message explaining what's needed and how to fix it.
    /// </summary>
    internal sealed class ScenarioPrerequisites
    {
        /// <summary>
        /// Number of Xbox users that must be signed in on the device (e.g., 2 for multi-user tests).
        /// Checked via <c>xbuser list</c> on Xbox devices.
        /// </summary>
        [YamlMember(Alias = "xboxUsers")]
        public int? XboxUsers { get; set; }

        /// <summary>
        /// Required Xbox Live sandbox ID (e.g., "XDKS.1").
        /// Checked via <c>xbconfig SandboxId</c> on Xbox devices.
        /// </summary>
        [YamlMember(Alias = "sandbox")]
        public string? Sandbox { get; set; }

        /// <summary>
        /// Whether this scenario requires sflash.exe for hardware-level device control
        /// (connected standby, shutdown, power-loss simulation). When true, the controller
        /// verifies sflash.exe is available on the PATH before running the scenario.
        /// </summary>
        [YamlMember(Alias = "sflash")]
        public bool? Sflash { get; set; }
    }

    internal readonly record struct ScenarioCommandInvocation(string Role, string Block, ScenarioStep Step);

    /// <summary>
    /// YamlDotNet type converter that deserializes a YAML node into <see cref="List{String}"/>
    /// whether the source is a scalar (<c>engine: xbox</c>) or a sequence (<c>engine: [xbox, pc-grts]</c>).
    /// </summary>
    internal sealed class ScalarOrStringListConverter : IYamlTypeConverter
    {
        public bool Accepts(Type type) => type == typeof(List<string>);

        public object? ReadYaml(IParser parser, Type type)
        {
            if (parser.TryConsume<Scalar>(out Scalar? scalar))
            {
                return string.IsNullOrWhiteSpace(scalar.Value)
                    ? null
                    : new List<string> { scalar.Value };
            }

            if (parser.TryConsume<SequenceStart>(out _))
            {
                var list = new List<string>();
                while (!parser.TryConsume<SequenceEnd>(out _))
                {
                    Scalar item = parser.Consume<Scalar>();
                    if (!string.IsNullOrWhiteSpace(item.Value))
                    {
                        list.Add(item.Value);
                    }
                }
                return list.Count > 0 ? list : null;
            }

            // Unexpected node type — skip it and return null.
            parser.SkipThisAndNestedEvents();
            return null;
        }

        public void WriteYaml(IEmitter emitter, object? value, Type type)
        {
            if (value is not List<string> list || list.Count == 0)
            {
                emitter.Emit(new Scalar(null, null, "", ScalarStyle.Plain, true, false));
                return;
            }

            if (list.Count == 1)
            {
                emitter.Emit(new Scalar(list[0]));
                return;
            }

            emitter.Emit(new SequenceStart(null, null, false, SequenceStyle.Flow));
            foreach (string item in list)
            {
                emitter.Emit(new Scalar(item));
            }
            emitter.Emit(new SequenceEnd());
        }
    }
}
