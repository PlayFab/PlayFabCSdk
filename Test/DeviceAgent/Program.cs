using DeviceAgent;

// --- Argument parsing ---
string controllerHost = "localhost";
int controllerPort = 15081;
string? testAppPath = null;
string machineName = Environment.MachineName;

for (int i = 0; i < args.Length; i++)
{
    switch (args[i])
    {
        case "--controller-host" when i + 1 < args.Length:
            controllerHost = args[++i];
            break;
        case "--controller-port" when i + 1 < args.Length:
            controllerPort = int.Parse(args[++i]);
            break;
        case "--test-app" when i + 1 < args.Length:
            testAppPath = args[++i];
            break;
        case "--machine-name" when i + 1 < args.Length:
            machineName = args[++i];
            break;
        case "--help" or "-h":
            Console.WriteLine("DeviceAgent — Remote device lifecycle manager for GameTestController");
            Console.WriteLine();
            Console.WriteLine("Options:");
            Console.WriteLine("  --controller-host <ip>    Controller host (default: localhost)");
            Console.WriteLine("  --controller-port <port>  Controller agent port (default: 15081)");
            Console.WriteLine("  --test-app <path>         Path to test app executable");
            Console.WriteLine("  --machine-name <name>     Friendly name (default: machine name)");
            Console.WriteLine("  --help, -h                Show this help");
            return 0;
    }
}

Console.WriteLine($"DeviceAgent v1.0");
Console.WriteLine($"  Machine:    {machineName}");
Console.WriteLine($"  Controller: ws://{controllerHost}:{controllerPort}/agent/");
if (testAppPath != null)
    Console.WriteLine($"  Test App:   {testAppPath}");
Console.WriteLine();

var agent = new AgentRuntime(controllerHost, controllerPort, machineName, testAppPath);

// Handle Ctrl+C gracefully
var cts = new CancellationTokenSource();
Console.CancelKeyPress += (_, e) =>
{
    e.Cancel = true;
    Console.WriteLine("Shutting down...");
    cts.Cancel();
};

await agent.RunAsync(cts.Token);
return 0;
