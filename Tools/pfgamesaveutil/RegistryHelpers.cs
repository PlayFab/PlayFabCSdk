using Microsoft.Win32;

namespace PfGameSaveUtil;

public static class RegistryHelpers
{
    public static void ShowRegistryKey(string path, string displayName)
    {
        var netPath = path.Replace(@"HKLM:\", "");
        
        try
        {
            using var key = Registry.LocalMachine.OpenSubKey(netPath);
            Console.Write($"  {displayName}: ");
            if (key != null)
            {
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine("EXISTS");
                Console.ResetColor();
            }
            else
            {
                Console.ForegroundColor = ConsoleColor.DarkGray;
                Console.WriteLine("NOT FOUND");
                Console.ResetColor();
            }
        }
        catch (Exception)
        {
            Console.ForegroundColor = ConsoleColor.DarkGray;
            Console.WriteLine("NOT FOUND");
            Console.ResetColor();
        }
    }

    public static void ShowRegistryValue(string path, string valueName)
    {
        var netPath = path.Replace(@"HKLM:\", "");
        
        try
        {
            using var key = Registry.LocalMachine.OpenSubKey(netPath);
            Console.Write($"  {valueName}: ");
            
            if (key != null)
            {
                var value = key.GetValue(valueName);
                if (value != null)
                {
                    if (value is int intVal && intVal == 1)
                    {
                        Console.ForegroundColor = ConsoleColor.Green;
                        Console.WriteLine($"ENABLED ({value})");
                    }
                    else
                    {
                        Console.ForegroundColor = ConsoleColor.Yellow;
                        Console.WriteLine($"SET ({value})");
                    }
                    Console.ResetColor();
                }
                else
                {
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    Console.WriteLine("NOT SET");
                    Console.ResetColor();
                }
            }
            else
            {
                Console.ForegroundColor = ConsoleColor.DarkGray;
                Console.WriteLine("NOT SET");
                Console.ResetColor();
            }
        }
        catch (Exception)
        {
            Console.ForegroundColor = ConsoleColor.DarkGray;
            Console.WriteLine("NOT SET");
            Console.ResetColor();
        }
    }

    public static string GetRegistryKeyStatus(string path)
    {
        try
        {
            using var key = Registry.LocalMachine.OpenSubKey(path);
            return key != null ? "EXISTS" : "NOT FOUND";
        }
        catch
        {
            return "NOT FOUND";
        }
    }

    public static string GetRegistryValueStatus(string path, string valueName)
    {
        try
        {
            using var key = Registry.LocalMachine.OpenSubKey(path);
            if (key != null)
            {
                var value = key.GetValue(valueName);
                if (value != null)
                {
                    return value is int intVal && intVal == 1 ? $"ENABLED ({value})" : $"SET ({value})";
                }
            }
            return "NOT SET";
        }
        catch
        {
            return "NOT SET";
        }
    }

    public static bool SetRegistryValue(string path, string valueName, int value)
    {
        try
        {
            using var key = Registry.LocalMachine.CreateSubKey(path, true);
            if (key != null)
            {
                key.SetValue(valueName, value, RegistryValueKind.DWord);
                Console.WriteLine($"  Set {valueName} = {value}");
                return true;
            }
            Console.Error.WriteLine($"  Failed to open/create key: {path}");
            return false;
        }
        catch (UnauthorizedAccessException)
        {
            Console.Error.WriteLine($"  Access denied. Run as Administrator to modify registry.");
            return false;
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"  Error setting {valueName}: {ex.Message}");
            return false;
        }
    }

    public static bool DeleteRegistryValue(string path, string valueName)
    {
        try
        {
            using var key = Registry.LocalMachine.OpenSubKey(path, true);
            if (key != null)
            {
                var value = key.GetValue(valueName);
                if (value != null)
                {
                    key.DeleteValue(valueName);
                    Console.WriteLine($"  Deleted {valueName}");
                    return true;
                }
                Console.WriteLine($"  {valueName} not set (nothing to delete)");
                return true;
            }
            Console.WriteLine($"  Key not found: {path}");
            return true;
        }
        catch (UnauthorizedAccessException)
        {
            Console.Error.WriteLine($"  Access denied. Run as Administrator to modify registry.");
            return false;
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"  Error deleting {valueName}: {ex.Message}");
            return false;
        }
    }
}
