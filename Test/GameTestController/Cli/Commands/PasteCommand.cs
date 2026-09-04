using System;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GameTestController.Cli.Commands
{
    internal sealed class PasteCommand : ICliCommand
    {
        private readonly CliEngine _cliEngine;

        public PasteCommand(CliEngine cliEngine)
        {
            _cliEngine = cliEngine;
        }

        public string Name => "paste";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Run manual API commands from clipboard text";
        public string Usage => "paste";

        public async Task<CommandResult> ExecuteAsync(string[] args)
        {
            if (args.Length != 0)
            {
                return CommandResult.Error($"Usage: {Usage}");
            }

            string? clipboardText;
            try
            {
                clipboardText = await GetClipboardTextAsync().ConfigureAwait(false);
            }
            catch (Exception ex)
            {
                return CommandResult.Error($"Failed to read clipboard text: {ex.Message}");
            }

            if (clipboardText == null)
            {
                return CommandResult.Error("Clipboard does not contain text.");
            }

            if (string.IsNullOrWhiteSpace(clipboardText))
            {
                return CommandResult.Error("Clipboard text is empty.");
            }

            string[] lines = clipboardText.Split(new[] { "\r\n", "\n", "\r" }, StringSplitOptions.None);
            return await _cliEngine.ExecuteBatchAsync(lines, "clipboard").ConfigureAwait(false);
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            return Array.Empty<string>();
        }

        private static Task<string?> GetClipboardTextAsync()
        {
            var completionSource = new TaskCompletionSource<string?>(TaskCreationOptions.RunContinuationsAsynchronously);

            var thread = new Thread(() =>
            {
                try
                {
                    if (!Clipboard.ContainsText())
                    {
                        completionSource.SetResult(null);
                        return;
                    }

                    completionSource.SetResult(Clipboard.GetText(TextDataFormat.Text));
                }
                catch (Exception ex)
                {
                    completionSource.SetException(ex);
                }
            });

            thread.SetApartmentState(ApartmentState.STA);
            thread.IsBackground = true;
            thread.Start();
            return completionSource.Task;
        }
    }
}
