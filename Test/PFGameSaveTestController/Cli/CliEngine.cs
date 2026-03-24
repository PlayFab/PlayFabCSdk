using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PFGameSaveTestController.Cli.Commands;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Main CLI engine that processes commands and manages the REPL loop
    /// </summary>
    internal sealed class CliEngine
    {
        private readonly ControllerRuntime _runtime;
        private readonly Dictionary<string, ICliCommand> _commands;
        private readonly Dictionary<string, ICliCommand> _manualCommands;
        private readonly List<string> _commandHistory;
        private int _historyIndex;
        private string _currentInput = string.Empty;
        private int _currentCursorPosition = 0;
        private bool _isReadingInput = false;
        private readonly ChaosModeScenarioParameters _chaosSettings;
        private bool _isManualMode = false;
        private int _lastLineLength = 0;

        public CliEngine(ControllerRuntime runtime)
        {
            _runtime = runtime;
            _commands = new Dictionary<string, ICliCommand>(StringComparer.OrdinalIgnoreCase);
            _manualCommands = new Dictionary<string, ICliCommand>(StringComparer.OrdinalIgnoreCase);
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
                // Clear current line, print log message, redraw prompt and input
                var prompt = _isManualMode ? "manual> " : "> ";
                Console.Write("\r" + new string(' ', Console.BufferWidth - 1) + "\r");
                Console.WriteLine(message);
                Console.Write(prompt + _currentInput);
                Console.SetCursorPosition(prompt.Length + _currentCursorPosition, Console.CursorTop);
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
            // Normal mode commands
            RegisterCommand(new ListDevicesCommand(_runtime));
            RegisterCommand(new RunScenarioCommand(_runtime));
            RegisterCommand(new ChaosCommand(_runtime));
            RegisterCommand(new ChaosForeverCommand(_runtime));
            RegisterCommand(new SetChaosCommand(_chaosSettings));
            RegisterCommand(new ManualModeCommand(this));
            RegisterCommand(new HelpCommand(_commands));
            RegisterCommand(new ExitCommand());

            // Manual mode commands
            RegisterManualCommand(new ManualModeExitCommand(this));
            RegisterManualCommand(new HelpCommand(_manualCommands));
            RegisterManualCommand(new ExitCommand());
            
            // Register all manual API commands
            foreach (var commandName in ManualApiCommand.GetAllCommandNames())
            {
                RegisterManualCommand(new ManualApiCommand(_runtime, commandName));
            }
        }

        private void RegisterCommand(ICliCommand command)
        {
            _commands[command.Name] = command;
            foreach (var alias in command.Aliases)
            {
                _commands[alias] = command;
            }
        }

        private void RegisterManualCommand(ICliCommand command)
        {
            _manualCommands[command.Name] = command;
            foreach (var alias in command.Aliases)
            {
                _manualCommands[alias] = command;
            }
        }

        public void EnterManualMode()
        {
            _isManualMode = true;
        }

        public void ExitManualMode()
        {
            _isManualMode = false;
        }

        public async Task RunAsync()
        {
            Console.WriteLine("PlayFab GameSave Test Controller CLI");
            Console.WriteLine("Type 'help' for available commands, 'exit' to quit");
            Console.WriteLine();

            while (true)
            {
                var prompt = _isManualMode ? "manual> " : "> ";
                Console.Write(prompt);
                var input = ReadLineWithCompletion();

                if (string.IsNullOrWhiteSpace(input))
                {
                    continue;
                }

                _commandHistory.Add(input);
                _historyIndex = _commandHistory.Count;

                var parts = ParseCommandLine(input);
                if (parts.Length == 0)
                {
                    continue;
                }

                var commandName = parts[0];
                var args = parts.Skip(1).ToArray();

                var commandDict = _isManualMode ? _manualCommands : _commands;
                if (!commandDict.TryGetValue(commandName, out var command))
                {
                    Console.WriteLine($"Unknown command: {commandName}");
                    Console.WriteLine("Type 'help' for available commands");
                    continue;
                }

                try
                {
                    var result = await command!.ExecuteAsync(args);
                    if (result.ShouldExit)
                    {
                        break;
                    }

                    if (!string.IsNullOrEmpty(result.Message))
                    {
                        Console.WriteLine(result.Message);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error: {ex.Message}");
                }

                Console.WriteLine();
            }
        }

        private string ReadLineWithCompletion()
        {
            var input = new StringBuilder();
            int cursorPosition = 0;
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
                    return input.ToString();
                }
                else if (key.Key == ConsoleKey.Tab)
                {
                    var beforeCursor = input.ToString().Substring(0, cursorPosition);
                    var wordStart = GetWordStart(beforeCursor);
                    var currentPrefix = beforeCursor.Substring(wordStart);
                    
                    // Check if we're continuing the same completion session
                    bool isContinuation = lastCompletions != null && 
                                         lastCompletionPrefix == currentPrefix.Substring(0, Math.Min(currentPrefix.Length, lastCompletionPrefix?.Length ?? 0)) &&
                                         lastCompletionWordStart == wordStart;
                    
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
                        Console.SetCursorPosition(2 + cursorPosition, Console.CursorTop);
                    }
                }
                else if (key.Key == ConsoleKey.RightArrow)
                {
                    if (cursorPosition < input.Length)
                    {
                        cursorPosition++;
                        Console.SetCursorPosition(2 + cursorPosition, Console.CursorTop);
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
                    Console.SetCursorPosition(2, Console.CursorTop);
                }
                else if (key.Key == ConsoleKey.End)
                {
                    cursorPosition = input.Length;
                    Console.SetCursorPosition(2 + cursorPosition, Console.CursorTop);
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
            var prompt = _isManualMode ? "manual> " : "> ";
            var newLine = prompt + line;
            var newLength = newLine.Length;
            
            // Move cursor to start of line
            Console.CursorLeft = 0;
            
            // Write the new content
            Console.Write(newLine);
            
            // Clear any remaining characters from the previous line
            if (_lastLineLength > newLength)
            {
                Console.Write(new string(' ', _lastLineLength - newLength));
            }
            
            // Update last line length
            _lastLineLength = newLength;
            
            // Position cursor
            Console.SetCursorPosition(prompt.Length + cursorPosition, Console.CursorTop);
        }

        private string[] GetCompletions(string input, int cursorPosition)
        {
            var beforeCursor = input.Substring(0, cursorPosition);
            var parts = ParseCommandLine(beforeCursor);
            var commandDict = _isManualMode ? _manualCommands : _commands;

            if (parts.Length == 0 || (parts.Length == 1 && !beforeCursor.EndsWith(" ")))
            {
                // Completing command name
                var prefix = parts.Length > 0 ? parts[0] : string.Empty;
                return commandDict.Keys
                    .Distinct(StringComparer.OrdinalIgnoreCase)
                    .Where(cmd => cmd.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                    .OrderBy(cmd => cmd)
                    .ToArray();
            }
            else
            {
                // Completing arguments
                var commandName = parts[0];
                
                if (commandDict.TryGetValue(commandName, out var command))
                {
                    // Pass all args after command name to GetCompletions
                    var args = parts.Skip(1).ToArray();
                    return command.GetCompletions(args, cursorPosition);
                }
            }

            return Array.Empty<string>();
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
