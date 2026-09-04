using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using GameTestController.Cli;
using GameTestController.Xbox;

namespace GameTestController
{
    internal static class Program
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool AllocConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool FreeConsole();

        [STAThread]
        private static int Main(string[] args)
        {
            // Check for help first
            if (args.Any(arg => arg == "--help" || arg == "-help" || arg == "-h" || arg == "-?"))
            {
                HeadlessRunner.ShowHelp();
                return ExitCodes.Success;
            }

            // Pre-load XTF DLLs from GDK bin directory early so P/Invoke resolves them
            GdkDllResolver.EnsureLoaded();

            // Check for --ui-debug mode
            var uiDebugOptions = UiDebugRunner.TryParse(args);
            if (uiDebugOptions != null)
            {
                var runner = new UiDebugRunner();
                return runner.Run(uiDebugOptions);
            }

            // Parse headless options
            var headlessOptions = HeadlessOptions.Parse(args);

            // If headless options specified, run in headless mode
            if (headlessOptions.HasHeadlessOptions)
            {
                var runner = new HeadlessRunner();
                return runner.Run(headlessOptions);
            }

            // Check for legacy CLI mode
            if (args.Length > 0 && args.Any(arg => string.Equals(arg, "-cli", StringComparison.OrdinalIgnoreCase)))
            {
                RunCliMode();
                return ExitCodes.Success;
            }

            // Normal GUI mode
            Application.SetHighDpiMode(HighDpiMode.DpiUnawareGdiScaled);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
            return ExitCodes.Success;
        }

        private static void RunCliMode()
        {
            // Allocate a console for CLI mode
            AllocConsole();

            try
            {
                // Create CLI engine first so we can use it in the logger and access chaos settings
                CliEngine? cliEngine = null;
                
                // Create runtime with logger that integrates with CLI and chaos settings provider
                var runtime = new ControllerRuntime(
                    logger: (message, isError, skipTimestamp) =>
                    {
                        var timestamp = skipTimestamp ? "" : $"[{DateTime.Now:HH:mm:ss}] ";
                        var fullMessage = $"{timestamp}{message}";
                        
                        if (cliEngine != null)
                        {
                            cliEngine.OnLogMessage(fullMessage);
                        }
                        else
                        {
                            Console.WriteLine(fullMessage);
                        }
                    },
                    chaosSettingsProvider: () => cliEngine?.GetChaosSettings() ?? new ChaosModeScenarioParameters()
                );
                
                // Start the WebSocket server
                runtime.StartAsync().GetAwaiter().GetResult();
                
                // Create and run CLI engine
                cliEngine = new CliEngine(runtime);
                cliEngine.RunAsync().GetAwaiter().GetResult();
                
                // Cleanup
                runtime.StopAsync().GetAwaiter().GetResult();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Fatal error: {ex.Message}");
                Console.WriteLine($"Stack trace: {ex.StackTrace}");
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
                Environment.Exit(1);
            }
            finally
            {
                FreeConsole();
            }
        }
    }
}
