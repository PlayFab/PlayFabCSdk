using System;
using System.IO;
using System.Threading.Tasks;

namespace GameTestController.Cli.Commands
{
    internal sealed class BatchCommand : ICliCommand
    {
        private readonly CliEngine _cliEngine;

        public BatchCommand(CliEngine cliEngine)
        {
            _cliEngine = cliEngine;
        }

        public string Name => "batch";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Run manual API commands from a text file";
        public string Usage => "batch <path>";

        public async Task<CommandResult> ExecuteAsync(string[] args)
        {
            if (args.Length != 1)
            {
                return CommandResult.Error($"Usage: {Usage}");
            }

            string path = args[0];
            if (!File.Exists(path))
            {
                return CommandResult.Error($"Batch file not found: {path}");
            }

            string[] lines;
            try
            {
                lines = await File.ReadAllLinesAsync(path).ConfigureAwait(false);
            }
            catch (Exception ex)
            {
                return CommandResult.Error($"Failed to read batch file '{path}': {ex.Message}");
            }

            return await _cliEngine.ExecuteBatchAsync(lines, $"file '{path}'").ConfigureAwait(false);
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            return Array.Empty<string>();
        }
    }
}
