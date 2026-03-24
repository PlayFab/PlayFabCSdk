using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Interface for CLI commands
    /// </summary>
    internal interface ICliCommand
    {
        /// <summary>
        /// Primary command name
        /// </summary>
        string Name { get; }

        /// <summary>
        /// Alternative names for the command
        /// </summary>
        string[] Aliases { get; }

        /// <summary>
        /// Brief description of what the command does
        /// </summary>
        string Description { get; }

        /// <summary>
        /// Detailed usage information
        /// </summary>
        string Usage { get; }

        /// <summary>
        /// Execute the command with the given arguments
        /// </summary>
        Task<CommandResult> ExecuteAsync(string[] args);

        /// <summary>
        /// Get possible completions for the given arguments and cursor position
        /// </summary>
        string[] GetCompletions(string[] args, int cursorPosition);
    }

    /// <summary>
    /// Result of executing a CLI command
    /// </summary>
    internal sealed class CommandResult
    {
        public bool Success { get; set; }
        public string Message { get; set; } = string.Empty;
        public bool ShouldExit { get; set; }

        public static CommandResult Ok(string message = "") => new CommandResult { Success = true, Message = message };
        public static CommandResult Error(string message) => new CommandResult { Success = false, Message = message };
        public static CommandResult Exit() => new CommandResult { Success = true, ShouldExit = true };
    }
}
