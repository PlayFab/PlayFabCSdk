using System;

namespace GameTestController.Cli
{
    /// <summary>
    /// Helpers for colorized console output in CLI mode.
    /// </summary>
    internal static class CliColors
    {
        public static void WriteSuccess(string text)
        {
            Write(text, ConsoleColor.Green);
        }

        public static void WriteError(string text)
        {
            Write(text, ConsoleColor.Red);
        }

        public static void WriteWarning(string text)
        {
            Write(text, ConsoleColor.Yellow);
        }

        public static void WriteInfo(string text)
        {
            Write(text, ConsoleColor.Cyan);
        }

        public static void WriteMuted(string text)
        {
            Write(text, ConsoleColor.DarkGray);
        }

        public static void WriteLineSuccess(string text)
        {
            WriteLine(text, ConsoleColor.Green);
        }

        public static void WriteLineError(string text)
        {
            WriteLine(text, ConsoleColor.Red);
        }

        public static void WriteLineWarning(string text)
        {
            WriteLine(text, ConsoleColor.Yellow);
        }

        public static void WriteLineInfo(string text)
        {
            WriteLine(text, ConsoleColor.Cyan);
        }

        public static void WriteLineMuted(string text)
        {
            WriteLine(text, ConsoleColor.DarkGray);
        }

        private static void Write(string text, ConsoleColor color)
        {
            var prev = Console.ForegroundColor;
            Console.ForegroundColor = color;
            Console.Write(text);
            Console.ForegroundColor = prev;
        }

        private static void WriteLine(string text, ConsoleColor color)
        {
            var prev = Console.ForegroundColor;
            Console.ForegroundColor = color;
            Console.WriteLine(text);
            Console.ForegroundColor = prev;
        }
    }
}
