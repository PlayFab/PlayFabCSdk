using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Xml;

namespace PFGameSaveTestController
{
    /// <summary>
    /// Writes test results in JSON and JUnit XML formats for ADO pipeline integration.
    /// </summary>
    internal static class TestResultsWriter
    {
        private static readonly JsonSerializerOptions JsonOptions = new()
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull,
            WriteIndented = true
        };

        /// <summary>
        /// Writes test results to a JSON file.
        /// </summary>
        public static void WriteJsonResults(TestRunResult result, string filePath)
        {
            if (string.IsNullOrWhiteSpace(filePath))
            {
                throw new ArgumentNullException(nameof(filePath));
            }

            var directory = Path.GetDirectoryName(filePath);
            if (!string.IsNullOrEmpty(directory) && !Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
            }

            var json = JsonSerializer.Serialize(result, JsonOptions);
            File.WriteAllText(filePath, json, Encoding.UTF8);
        }

        /// <summary>
        /// Writes test results to a JUnit XML file for ADO pipeline integration.
        /// </summary>
        public static void WriteJUnitResults(TestRunResult result, string filePath)
        {
            if (string.IsNullOrWhiteSpace(filePath))
            {
                throw new ArgumentNullException(nameof(filePath));
            }

            var directory = Path.GetDirectoryName(filePath);
            if (!string.IsNullOrEmpty(directory) && !Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
            }

            var settings = new XmlWriterSettings
            {
                Indent = true,
                Encoding = Encoding.UTF8
            };

            using var writer = XmlWriter.Create(filePath, settings);
            WriteJUnitXml(writer, result);
        }

        private static void WriteJUnitXml(XmlWriter writer, TestRunResult result)
        {
            // JUnit XML format: https://llg.cubic.org/docs/junit/
            writer.WriteStartDocument();

            // <testsuites>
            writer.WriteStartElement("testsuites");
            writer.WriteAttributeString("name", "PFGameSaveTests");
            writer.WriteAttributeString("tests", result.Summary.Total.ToString());
            writer.WriteAttributeString("failures", result.Summary.Failed.ToString());
            writer.WriteAttributeString("errors", "0");
            writer.WriteAttributeString("skipped", result.Summary.Skipped.ToString());
            writer.WriteAttributeString("time", result.TotalDurationSeconds.ToString("F3", CultureInfo.InvariantCulture));
            writer.WriteAttributeString("timestamp", result.StartTime.ToString("o"));

            // <testsuite> - one per scenario
            foreach (var scenario in result.Scenarios)
            {
                writer.WriteStartElement("testsuite");
                writer.WriteAttributeString("name", scenario.Name ?? scenario.Id);
                writer.WriteAttributeString("tests", "1");
                writer.WriteAttributeString("failures", scenario.Status == TestStatus.Failed ? "1" : "0");
                writer.WriteAttributeString("errors", "0");
                writer.WriteAttributeString("skipped", scenario.Status == TestStatus.Skipped ? "1" : "0");
                writer.WriteAttributeString("time", scenario.DurationSeconds.ToString("F3", CultureInfo.InvariantCulture));
                writer.WriteAttributeString("timestamp", scenario.StartTime?.ToString("o") ?? result.StartTime.ToString("o"));

                // <testcase>
                writer.WriteStartElement("testcase");
                writer.WriteAttributeString("name", scenario.Name ?? scenario.Id);
                writer.WriteAttributeString("classname", $"PFGameSaveTests.{SanitizeClassName(scenario.Id)}");
                writer.WriteAttributeString("time", scenario.DurationSeconds.ToString("F3", CultureInfo.InvariantCulture));

                if (scenario.Status == TestStatus.Failed)
                {
                    writer.WriteStartElement("failure");
                    writer.WriteAttributeString("message", scenario.Error ?? "Test failed");
                    writer.WriteAttributeString("type", "AssertionError");

                    // Include step details in the failure text
                    var failureText = BuildFailureText(scenario);
                    if (!string.IsNullOrEmpty(failureText))
                    {
                        writer.WriteCData(failureText);
                    }

                    writer.WriteEndElement(); // </failure>
                }
                else if (scenario.Status == TestStatus.Skipped)
                {
                    writer.WriteStartElement("skipped");
                    writer.WriteAttributeString("message", scenario.Error ?? "Test skipped");
                    writer.WriteEndElement(); // </skipped>
                }

                // System output (optional - include step log)
                if (scenario.Steps != null && scenario.Steps.Count > 0)
                {
                    writer.WriteStartElement("system-out");
                    var stepLog = BuildStepLog(scenario);
                    writer.WriteCData(stepLog);
                    writer.WriteEndElement(); // </system-out>
                }

                writer.WriteEndElement(); // </testcase>
                writer.WriteEndElement(); // </testsuite>
            }

            writer.WriteEndElement(); // </testsuites>
            writer.WriteEndDocument();
        }

        private static string SanitizeClassName(string id)
        {
            if (string.IsNullOrEmpty(id))
            {
                return "UnknownScenario";
            }

            // Replace invalid characters for class names
            var result = new StringBuilder();
            foreach (var c in id)
            {
                if (char.IsLetterOrDigit(c) || c == '_')
                {
                    result.Append(c);
                }
                else if (c == '-' || c == '.')
                {
                    result.Append('_');
                }
            }

            // Ensure it doesn't start with a digit
            if (result.Length > 0 && char.IsDigit(result[0]))
            {
                result.Insert(0, "Scenario_");
            }

            return result.Length > 0 ? result.ToString() : "UnknownScenario";
        }

        private static string BuildFailureText(ScenarioResult scenario)
        {
            var sb = new StringBuilder();

            if (!string.IsNullOrEmpty(scenario.Error))
            {
                sb.AppendLine(scenario.Error);
                sb.AppendLine();
            }

            if (scenario.Steps != null)
            {
                var failedStep = scenario.Steps.FirstOrDefault(s => s.Status == TestStatus.Failed);
                if (failedStep != null)
                {
                    sb.AppendLine($"Failed at step: {failedStep.Command}");
                    if (!string.IsNullOrEmpty(failedStep.Error))
                    {
                        sb.AppendLine($"Error: {failedStep.Error}");
                    }
                    if (!string.IsNullOrEmpty(failedStep.HResult))
                    {
                        sb.AppendLine($"HResult: {failedStep.HResult}");
                    }
                }
            }

            return sb.ToString();
        }

        private static string BuildStepLog(ScenarioResult scenario)
        {
            var sb = new StringBuilder();

            if (scenario.Steps == null || scenario.Steps.Count == 0)
            {
                return "No steps recorded.";
            }

            int stepNum = 0;
            foreach (var step in scenario.Steps)
            {
                stepNum++;
                var status = step.Status switch
                {
                    TestStatus.Passed => "PASS",
                    TestStatus.Failed => "FAIL",
                    TestStatus.Skipped => "SKIP",
                    _ => "????"
                };

                sb.AppendLine($"[{stepNum}] {status} {step.Command} ({step.DurationMs}ms) hr={step.HResult ?? "0x00000000"}");
            }

            return sb.ToString();
        }
    }

    /// <summary>
    /// Represents the result of a complete test run.
    /// </summary>
    internal sealed class TestRunResult
    {
        /// <summary>
        /// Unique identifier for this test run.
        /// </summary>
        public string RunId { get; set; } = Guid.NewGuid().ToString("N")[..12];

        /// <summary>
        /// When the test run started.
        /// </summary>
        public DateTime StartTime { get; set; } = DateTime.UtcNow;

        /// <summary>
        /// When the test run ended.
        /// </summary>
        public DateTime EndTime { get; set; }

        /// <summary>
        /// Total duration in seconds.
        /// </summary>
        public double TotalDurationSeconds => (EndTime - StartTime).TotalSeconds;

        /// <summary>
        /// Summary of pass/fail counts.
        /// </summary>
        public TestRunSummary Summary { get; set; } = new();

        /// <summary>
        /// Individual scenario results.
        /// </summary>
        public List<ScenarioResult> Scenarios { get; set; } = new();

        /// <summary>
        /// The tag used to filter scenarios, if any.
        /// </summary>
        public string? Tag { get; set; }

        /// <summary>
        /// Any error that caused the run to abort.
        /// </summary>
        public string? FatalError { get; set; }
    }

    /// <summary>
    /// Summary statistics for a test run.
    /// </summary>
    internal sealed class TestRunSummary
    {
        public int Total { get; set; }
        public int Passed { get; set; }
        public int Failed { get; set; }
        public int Skipped { get; set; }
    }

    /// <summary>
    /// Result of running a single scenario.
    /// </summary>
    internal sealed class ScenarioResult
    {
        /// <summary>
        /// Scenario ID from the YAML manifest.
        /// </summary>
        public string Id { get; set; } = string.Empty;

        /// <summary>
        /// Human-readable scenario name.
        /// </summary>
        public string? Name { get; set; }

        /// <summary>
        /// Tags associated with this scenario.
        /// </summary>
        public List<string>? Tags { get; set; }

        /// <summary>
        /// Overall status of the scenario.
        /// </summary>
        public TestStatus Status { get; set; }

        /// <summary>
        /// Duration in seconds.
        /// </summary>
        public double DurationSeconds { get; set; }

        /// <summary>
        /// Error message if the scenario failed.
        /// </summary>
        public string? Error { get; set; }

        /// <summary>
        /// When the scenario started.
        /// </summary>
        public DateTime? StartTime { get; set; }

        /// <summary>
        /// Individual step results.
        /// </summary>
        public List<StepResult>? Steps { get; set; }
    }

    /// <summary>
    /// Result of a single step within a scenario.
    /// </summary>
    internal sealed class StepResult
    {
        /// <summary>
        /// Command name.
        /// </summary>
        public string Command { get; set; } = string.Empty;

        /// <summary>
        /// Device or role that executed this step.
        /// </summary>
        public string? Device { get; set; }

        /// <summary>
        /// Step status.
        /// </summary>
        public TestStatus Status { get; set; }

        /// <summary>
        /// Duration in milliseconds.
        /// </summary>
        public long DurationMs { get; set; }

        /// <summary>
        /// HRESULT if available.
        /// </summary>
        public string? HResult { get; set; }

        /// <summary>
        /// Error message if step failed.
        /// </summary>
        public string? Error { get; set; }
    }

    /// <summary>
    /// Test status enumeration.
    /// </summary>
    internal enum TestStatus
    {
        Passed,
        Failed,
        Skipped
    }
}
