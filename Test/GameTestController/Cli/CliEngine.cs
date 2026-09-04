using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameTestController.Cli.Commands;

namespace GameTestController.Cli
{
    /// <summary>
    /// Main CLI engine that processes commands and manages the REPL loop
    /// </summary>
    internal sealed class CliEngine
    {
        private readonly ControllerRuntime _runtime;
        private readonly Dictionary<string, ICliCommand> _commands;
        private readonly List<string> _commandHistory;
        private int _historyIndex;
        private string _currentInput = string.Empty;
        private int _currentCursorPosition = 0;
        private bool _isReadingInput = false;
        private readonly ChaosModeScenarioParameters _chaosSettings;
        private int _lastLineLength = 0;
        private int _promptStartRow = -1;
        private string? _targetDeviceName;

        /// <summary>
        /// The display name of the targeted device (e.g., "DeviceA"). Null means target the first connected device.
        /// </summary>
        public string? TargetDeviceName
        {
            get => _targetDeviceName;
            set => _targetDeviceName = value;
        }

        public CliEngine(ControllerRuntime runtime)
        {
            _runtime = runtime;
            _commands = new Dictionary<string, ICliCommand>(StringComparer.OrdinalIgnoreCase);
            _commandHistory = new List<string>();
            _historyIndex = -1;
            _chaosSettings = new ChaosModeScenarioParameters
            {
                FileCreate = true,
                FileModify = true,
                FileDelete = true,
                FolderCreate = true,
                FolderDelete = true,
                BinaryFiles = true,
                TextFiles = true,
                LargeFiles = false,
                UnicodeFiles = false,
                UnicodeFolders = false,
                OperationsPerUpload = 5,
                NumUploads = 3
            };

            RegisterCommands();
        }

        public void OnLogMessage(string message)
        {
            if (_isReadingInput)
            {
                // Clear current prompt line, print log message, redraw prompt and input
                try { Console.Write("\r" + new string(' ', Console.BufferWidth - 1) + "\r"); }
                catch (ArgumentOutOfRangeException) { Console.Write("\r"); }
                Console.WriteLine(message);

                // Update prompt start row since the log message shifted it down
                _promptStartRow = Console.CursorTop;
                CliColors.WriteInfo("> ");
                Console.Write(_currentInput);
                SafeSetCursorPosition(2 + _currentCursorPosition, _promptStartRow);
            }
            else
            {
                Console.WriteLine(message);
            }
        }

        public ChaosModeScenarioParameters GetChaosSettings()
        {
            return _chaosSettings;
        }

        private void RegisterCommands()
        {
            RegisterCommand(new ListDevicesCommand(_runtime));
            RegisterCommand(new LaunchCommand(_runtime));
            RegisterCommand(new RunScenarioCommand(_runtime));
            RegisterCommand(new ChaosCommand(_runtime));
            RegisterCommand(new ChaosForeverCommand(_runtime));
            RegisterCommand(new SetChaosCommand(_chaosSettings));
            RegisterCommand(new BatchCommand(this));
            RegisterCommand(new PasteCommand(this));
            RegisterCommand(new Commands.TargetCommand(this, _runtime));
            RegisterCommand(new Commands.ApiHelpCommand(_runtime));
            RegisterCommand(new HelpCommand(_commands));
            RegisterCommand(new ExitCommand());

            // Any unrecognized command is forwarded to the connected device as an API call.
            // Known API command names are loaded for tab completion.
        }

        private void RegisterCommand(ICliCommand command)
        {
            _commands[command.Name] = command;
            foreach (var alias in command.Aliases)
            {
                _commands[alias] = command;
            }
        }

        public async Task<CommandResult> ExecuteCommandLineAsync(string input, bool addToHistory = true)
        {
            if (string.IsNullOrWhiteSpace(input))
            {
                return CommandResult.Ok();
            }

            if (input.StartsWith("#", StringComparison.Ordinal))
            {
                return CommandResult.Ok();
            }

            if (addToHistory)
            {
                _commandHistory.Add(input);
                _historyIndex = _commandHistory.Count;
            }

            var parts = ParseCommandLine(input);
            if (parts.Length == 0)
            {
                return CommandResult.Ok();
            }

            var commandName = parts[0];
            var args = parts.Skip(1).ToArray();
            var command = ResolveCommand(commandName);

            return await command.ExecuteAsync(args).ConfigureAwait(false);
        }

        public async Task<CommandResult> ExecuteBatchAsync(IReadOnlyList<string> lines, string sourceDescription)
        {
            int executedCount = 0;
            int attemptedCount = 0;

            foreach ((string rawLine, int lineNumber) in lines.Select((line, index) => (line, index + 1)))
            {
                string line = rawLine.Trim();
                if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#", StringComparison.Ordinal))
                {
                    continue;
                }

                attemptedCount++;
                CliColors.WriteInfo("> ");
                Console.WriteLine(line);

                CommandResult result = await ExecuteCommandLineAsync(line, addToHistory: false).ConfigureAwait(false);
                if (!string.IsNullOrEmpty(result.Message))
                {
                    if (result.Success)
                    {
                        Console.WriteLine(result.Message);
                    }
                    else
                    {
                        CliColors.WriteLineError(result.Message);
                    }
                }

                Console.WriteLine();

                if (result.ShouldExit)
                {
                    return result;
                }

                if (!result.Success)
                {
                    return CommandResult.Error($"Batch stopped at line {lineNumber} from {sourceDescription} after {executedCount} successful command(s).");
                }

                executedCount++;
            }

            if (attemptedCount == 0)
            {
                return CommandResult.Error($"No commands were found in {sourceDescription}.");
            }

            return CommandResult.Ok($"Executed {executedCount} command(s) from {sourceDescription}.");
        }

        public async Task RunAsync()
        {
            CliColors.WriteLineInfo("Game Test Controller CLI");
            CliColors.WriteLineMuted("Type 'help' for built-in commands, 'apihelp' for API commands, 'exit' to quit");
            Console.WriteLine();

            while (true)
            {
                CliColors.WriteInfo("> ");
                _promptStartRow = Console.CursorTop;
                var input = ReadLineWithCompletion();

                if (string.IsNullOrWhiteSpace(input))
                {
                    continue;
                }

                try
                {
                    var result = await ExecuteCommandLineAsync(input).ConfigureAwait(false);
                    if (result.ShouldExit)
                    {
                        break;
                    }

                    if (!string.IsNullOrEmpty(result.Message))
                    {
                        if (result.Success)
                        {
                            Console.WriteLine(result.Message);
                        }
                        else
                        {
                            CliColors.WriteLineError(result.Message);
                        }
                    }
                }
                catch (Exception ex)
                {
                    CliColors.WriteLineError($"Error: {ex.Message}");
                }

                Console.WriteLine();
            }
        }

        private ICliCommand ResolveCommand(string commandName)
        {
            if (_commands.TryGetValue(commandName, out var command))
            {
                return command;
            }

            // Unknown command — forward to connected device as an API call
            return new ManualApiCommand(_runtime, commandName, _targetDeviceName);
        }

        private string ReadLineWithCompletion()
        {
            var input = new StringBuilder();
            int cursorPosition = 0;
            int promptLength = 2; // "> ".Length
            string[]? lastCompletions = null;
            int lastCompletionIndex = -1;
            string? lastCompletionPrefix = null;
            int lastCompletionWordStart = -1;

            _isReadingInput = true;
            _currentInput = string.Empty;
            _currentCursorPosition = 0;

            while (true)
            {
                var key = Console.ReadKey(intercept: true);

                if (key.Key == ConsoleKey.Enter)
                {
                    Console.WriteLine();
                    _isReadingInput = false;
                    _lastLineLength = 0;
                    _promptStartRow = -1;
                    return input.ToString();
                }
                else if (key.Key == ConsoleKey.Tab)
                {
                    var beforeCursor = input.ToString().Substring(0, cursorPosition);
                    var wordStart = GetWordStart(beforeCursor);
                    var currentPrefix = beforeCursor.Substring(wordStart);
                    
                    // Check if we're continuing the same completion session
                    bool isContinuation = lastCompletions != null &&
                                         lastCompletionPrefix != null &&
                                         lastCompletionWordStart == wordStart &&
                                         currentPrefix.StartsWith(lastCompletionPrefix, StringComparison.Ordinal);
                    
                    if (!isContinuation)
                    {
                        // New completion session
                        lastCompletions = GetCompletions(input.ToString(), cursorPosition);
                        lastCompletionIndex = -1;
                        lastCompletionPrefix = currentPrefix;
                        lastCompletionWordStart = wordStart;
                    }
                    
                    if (lastCompletions == null || lastCompletions.Length == 0)
                    {
                        // No completions available
                        continue;
                    }
                    else if (lastCompletions.Length == 1)
                    {
                        // Single completion - apply it
                        var completion = lastCompletions[0];
                        
                        input.Remove(wordStart, cursorPosition - wordStart);
                        input.Insert(wordStart, completion);
                        
                        cursorPosition = wordStart + completion.Length;
                        RedrawLine(input.ToString(), cursorPosition);
                        
                        // Reset completion state after applying single match
                        lastCompletions = null;
                        lastCompletionIndex = -1;
                        lastCompletionPrefix = null;
                    }
                    else
                    {
                        // Multiple completions - cycle through them
                        lastCompletionIndex = (lastCompletionIndex + 1) % lastCompletions.Length;
                        var completion = lastCompletions[lastCompletionIndex];
                        
                        input.Remove(wordStart, cursorPosition - wordStart);
                        input.Insert(wordStart, completion);
                        
                        cursorPosition = wordStart + completion.Length;
                        RedrawLine(input.ToString(), cursorPosition);
                    }
                }
                else if (key.Key == ConsoleKey.Backspace)
                {
                    if (cursorPosition > 0)
                    {
                        input.Remove(cursorPosition - 1, 1);
                        cursorPosition--;
                        RedrawLine(input.ToString(), cursorPosition);
                    }
                }
                else if (key.Key == ConsoleKey.Delete)
                {
                    if (cursorPosition < input.Length)
                    {
                        input.Remove(cursorPosition, 1);
                        RedrawLine(input.ToString(), cursorPosition);
                    }
                }
                else if (key.Key == ConsoleKey.LeftArrow)
                {
                    if (cursorPosition > 0)
                    {
                        cursorPosition--;
                        SafeSetCursorPosition(promptLength + cursorPosition, Console.CursorTop);
                    }
                }
                else if (key.Key == ConsoleKey.RightArrow)
                {
                    if (cursorPosition < input.Length)
                    {
                        cursorPosition++;
                        SafeSetCursorPosition(promptLength + cursorPosition, Console.CursorTop);
                    }
                }
                else if (key.Key == ConsoleKey.UpArrow)
                {
                    if (_historyIndex > 0)
                    {
                        _historyIndex--;
                        input.Clear();
                        input.Append(_commandHistory[_historyIndex]);
                        cursorPosition = input.Length;
                        RedrawLine(input.ToString(), cursorPosition);
                    }
                }
                else if (key.Key == ConsoleKey.DownArrow)
                {
                    if (_historyIndex < _commandHistory.Count - 1)
                    {
                        _historyIndex++;
                        input.Clear();
                        input.Append(_commandHistory[_historyIndex]);
                        cursorPosition = input.Length;
                        RedrawLine(input.ToString(), cursorPosition);
                    }
                    else if (_historyIndex == _commandHistory.Count - 1)
                    {
                        _historyIndex = _commandHistory.Count;
                        input.Clear();
                        cursorPosition = 0;
                        RedrawLine(input.ToString(), cursorPosition);
                    }
                }
                else if (key.Key == ConsoleKey.Home)
                {
                    cursorPosition = 0;
                    SafeSetCursorPosition(promptLength, Console.CursorTop);
                }
                else if (key.Key == ConsoleKey.End)
                {
                    cursorPosition = input.Length;
                    SafeSetCursorPosition(promptLength + cursorPosition, Console.CursorTop);
                }
                else if (key.Key == ConsoleKey.Escape)
                {
                    // Clear the current line
                    input.Clear();
                    cursorPosition = 0;
                    RedrawLine(input.ToString(), cursorPosition);
                    
                    // Reset completion state
                    lastCompletions = null;
                    lastCompletionIndex = -1;
                    lastCompletionPrefix = null;
                }
                else if (!char.IsControl(key.KeyChar))
                {
                    input.Insert(cursorPosition, key.KeyChar);
                    cursorPosition++;
                    _currentInput = input.ToString();
                    _currentCursorPosition = cursorPosition;
                    RedrawLine(input.ToString(), cursorPosition);
                    
                    // Reset completion state on any character input
                    lastCompletions = null;
                    lastCompletionIndex = -1;
                    lastCompletionPrefix = null;
                }
                else
                {
                    // Reset completion state on any other key
                    lastCompletions = null;
                    lastCompletionIndex = -1;
                    lastCompletionPrefix = null;
                }
                
                // Update current state for log interception
                _currentInput = input.ToString();
                _currentCursorPosition = cursorPosition;
            }
        }

        private void RedrawLine(string line, int cursorPosition)
        {
            var newLine = "> " + line;
            var newLength = newLine.Length;
            int bufferWidth = Console.BufferWidth;

            // On first draw, record where the prompt starts
            if (_promptStartRow < 0)
            {
                _promptStartRow = Console.CursorTop;
            }

            // Move cursor to the row where the prompt started
            SafeSetCursorPosition(0, _promptStartRow);

            // Write the new content (prompt in cyan, input in default)
            CliColors.WriteInfo("> ");
            Console.Write(line);

            // Clear any leftover characters from the previous (longer) line
            if (_lastLineLength > newLength)
            {
                Console.Write(new string(' ', _lastLineLength - newLength));
            }

            // Update last line length
            _lastLineLength = newLength;

            // Position cursor at the correct location within the (possibly wrapped) line
            int absolutePos = 2 + cursorPosition;
            int targetRow = _promptStartRow + (absolutePos / bufferWidth);
            int targetCol = absolutePos % bufferWidth;
            SafeSetCursorPosition(targetCol, targetRow);
        }

        private static void SafeSetCursorPosition(int left, int top)
        {
            try
            {
                int bufferWidth = Console.BufferWidth;
                int col = left % bufferWidth;
                Console.SetCursorPosition(col, Math.Max(0, top));
            }
            catch (ArgumentOutOfRangeException)
            {
                // Console resized or edge case — ignore
            }
        }

        private string[] GetCompletions(string input, int cursorPosition)
        {
            var beforeCursor = input.Substring(0, cursorPosition);
            var parts = ParseCommandLine(beforeCursor);

            if (parts.Length == 0 || (parts.Length == 1 && !beforeCursor.EndsWith(" ")))
            {
                // Completing command name — combine built-in commands with API commands
                var prefix = parts.Length > 0 ? parts[0] : string.Empty;
                var apiCommands = ManualApiCommand.GetAllCommandNames(_runtime);
                return _commands.Keys.Concat(apiCommands)
                    .Distinct(StringComparer.OrdinalIgnoreCase)
                    .Where(cmd => cmd.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                    .OrderBy(cmd => cmd)
                    .ToArray();
            }
            else
            {
                // Completing arguments
                var commandName = parts[0];
                
                if (_commands.TryGetValue(commandName, out var command))
                {
                    // Pass all args after command name to GetCompletions
                    var args = parts.Skip(1).ToArray();
                    return command.GetCompletions(args, cursorPosition);
                }
                else
                {
                    // Unknown built-in command — try API command completions
                    var args = parts.Skip(1).ToArray();
                    var manualCommand = new ManualApiCommand(_runtime, commandName);
                    return manualCommand.GetCompletions(args, cursorPosition);
                }
            }
        }

        private int GetWordStart(string text)
        {
            int pos = text.Length - 1;
            while (pos >= 0 && !char.IsWhiteSpace(text[pos]))
            {
                pos--;
            }
            return pos + 1;
        }

        private string[] ParseCommandLine(string input)
        {
            var parts = new List<string>();
            var current = new StringBuilder();
            bool inQuotes = false;

            for (int i = 0; i < input.Length; i++)
            {
                char c = input[i];

                if (c == '"')
                {
                    inQuotes = !inQuotes;
                }
                else if (char.IsWhiteSpace(c) && !inQuotes)
                {
                    if (current.Length > 0)
                    {
                        parts.Add(current.ToString());
                        current.Clear();
                    }
                }
                else
                {
                    current.Append(c);
                }
            }

            if (current.Length > 0)
            {
                parts.Add(current.ToString());
            }

            return parts.ToArray();
        }
    }
}
