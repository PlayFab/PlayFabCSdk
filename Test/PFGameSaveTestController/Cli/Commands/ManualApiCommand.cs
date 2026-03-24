using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli.Commands
{
    internal class ManualApiCommand : ICliCommand
    {
        private readonly ControllerRuntime _runtime;
        private readonly string _commandName;
        private readonly Dictionary<string, object> _parameters = new Dictionary<string, object>();

        // Alphabetically sorted list of all available commands
        private static readonly string[] AllCommands = new[]
        {
            "CaptureSaveContainerSnapshot",
            "ClearHttpMocks",
            "CompareSaveContainerSnapshots",
            "ConfigureHttpMock",
            "ConsumeDiskSpace",
            "CopyTargetFolderToSaveFolder",
            "DeleteLocalFolder",
            "DeleteSaveRoot",
            "DoChaosMode",
            "GatherLogs",
            "GatherSnapshot",
            "GetDebugStats",
            "PFGameSaveFilesAddUserWithUiAsync",
            "PFGameSaveFilesGetFolder",
            "PFGameSaveFilesGetRemainingQuota",
            "PFGameSaveFilesGetSaveDescription",
            "PFGameSaveFilesInitialize",
            "PFGameSaveFilesIsConnectedToCloud",
            "PFGameSaveFilesResetCloudAsync",
            "PFGameSaveFilesSetActiveDeviceChangedCallback",
            "PFGameSaveFilesSetActiveDevicePollForceChangeForDebug",
            "PFGameSaveFilesSetActiveDevicePollIntervalForDebug",
            "PFGameSaveFilesSetForceOutOfStorageErrorForDebug",
            "PFGameSaveFilesSetForceSyncFailedErrorForDebug",
            "PFGameSaveFilesSetMockDeviceIdForDebug",
            "PFGameSaveFilesSetSaveDescriptionAsync",
            "PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse",
            "PFGameSaveFilesSetUiActiveDeviceContentionResponse",
            "PFGameSaveFilesSetUiCallbacks",
            "PFGameSaveFilesSetUiConflictAutoResponse",
            "PFGameSaveFilesSetUiConflictResponse",
            "PFGameSaveFilesSetUiOutOfStorageAutoResponse",
            "PFGameSaveFilesSetUiOutOfStorageResponse",
            "PFGameSaveFilesSetUiProgressAutoResponse",
            "PFGameSaveFilesSetUiProgressResponse",
            "PFGameSaveFilesSetUiSyncFailedAutoResponse",
            "PFGameSaveFilesSetUiSyncFailedResponse",
            "PFGameSaveFilesSetWriteManifestsToDiskForDebug",
            "PFGameSaveFilesUninitializeAsync",
            "PFGameSaveFilesUploadWithUiAsync",
            "PFInitialize",
            "PFLocalUserCloseHandle",
            "PFLocalUserCreateHandleWithPersistedLocalId",
            "PFLocalUserCreateHandleWithXboxUser",
            "PFServiceConfigCloseHandle",
            "PFServiceConfigCreateHandle",
            "PFServicesInitialize",
            "PFServicesUninitializeAsync",
            "PFUninitializeAsync",
            "ReadGameSaveData",
            "ReleaseDiskSpace",
            "SmokeCommandEcho",
            "SmokeDelay",
            "SmokeTestFailure",
            "WaitForUserInput",
            "WriteGameSaveData",
            "XblCleanupAsync",
            "XblContextCloseHandle",
            "XblContextCreateHandle",
            "XblInitialize",
            "XblMultiplayerActivitySendInvitesAsync",
            "InteractiveInviteLoop",
            "XGameInviteAcceptPendingInvite",
            "XGameInviteRegisterForEvent",
            "XGameInviteRegisterForPendingEvent",
            "XGameInviteUnregisterForEvent",
            "XGameInviteUnregisterForPendingEvent",
            "WaitForXGameInviteRegisterForEvent",
            "WaitForXGameInviteRegisterForPendingEvent",
            "XGameRuntimeInitialize",
            "XGameUiShowPlayerPickerAsync",
            "XTaskQueueCloseHandle",
            "XTaskQueueCreate",
            "XUserAddAsync",
            "XUserCloseHandle",
            "XUserGetId",
            "XUserPlatformRemoteConnectSetEventHandlers",
            "XUserPlatformSpopPromptComplete",
            "XUserPlatformSpopPromptSetEventHandlers"
        };

        public ManualApiCommand(ControllerRuntime runtime, string commandName)
        {
            _runtime = runtime;
            _commandName = commandName;
        }

        public string Name => _commandName;
        public string[] Aliases => System.Array.Empty<string>();
        public string Description => $"Execute {_commandName} on the selected device";
        public string Usage => $"{_commandName} [--param=value ...]";

        public async Task<CommandResult> ExecuteAsync(string[] args)
        {
            // Parse arguments into parameters
            var parameters = new Dictionary<string, object>(_parameters);
            foreach (var arg in args)
            {
                if (arg.StartsWith("--"))
                {
                    var parts = arg.Substring(2).Split(new[] { '=' }, 2);
                    if (parts.Length == 2)
                    {
                        var paramName = parts[0];
                        var paramValue = parts[1];

                        // Try to parse as different types
                        if (bool.TryParse(paramValue, out var boolValue))
                        {
                            parameters[paramName] = boolValue;
                        }
                        else if (int.TryParse(paramValue, out var intValue))
                        {
                            parameters[paramName] = intValue;
                        }
                        else
                        {
                            parameters[paramName] = paramValue;
                        }
                    }
                }
            }

            // Get first connected device
            var devices = _runtime.Transport.GetConnectedDeviceDetails();
            if (devices.Count == 0)
            {
                return CommandResult.Error("No devices connected. Use 'list-devices' to check device status.");
            }

            var device = devices.First();
            var deviceName = string.IsNullOrWhiteSpace(device.DisplayName) 
                ? device.ClientId.ToString() 
                : device.DisplayName;
            var deviceIdentifier = device.ClientId.ToString();
            var result = await _runtime.SendManualCommandAsync(deviceIdentifier, _commandName, parameters);

            string hresultText = string.IsNullOrWhiteSpace(result.HResult) ? "0x00000000" : result.HResult;
            
            if (string.Equals(result.Status, "succeeded", StringComparison.OrdinalIgnoreCase))
            {
                var output = $"Command '{_commandName}' executed successfully on {deviceName}. Elapsed: {result.ElapsedMs}ms, HRESULT: {hresultText}";
                
                // Log the full JSON if there's more than just standard fields
                if (!string.IsNullOrWhiteSpace(result.RawJson))
                {
                    try
                    {
                        var jsonDoc = JsonDocument.Parse(result.RawJson);
                        var root = jsonDoc.RootElement;
                        
                        // Check if there are any properties beyond the standard ones
                        var standardProps = new HashSet<string>(StringComparer.OrdinalIgnoreCase)
                        {
                            "command", "commandId", "deviceId", "elapsedMs", "hresult", "status", "timestamp", "type"
                        };
                        
                        bool hasAdditionalData = false;
                        foreach (var prop in root.EnumerateObject())
                        {
                            if (!standardProps.Contains(prop.Name))
                            {
                                hasAdditionalData = true;
                                break;
                            }
                        }
                        
                        if (hasAdditionalData)
                        {
                            output += "\nResponse data:\n" + result.RawJson;
                        }
                    }
                    catch
                    {
                        // Ignore JSON parsing errors
                    }
                }
                
                return CommandResult.Ok(output);
            }
            else
            {
                var errorMsg = !string.IsNullOrWhiteSpace(result.ErrorMessage) 
                    ? result.ErrorMessage 
                    : $"Command failed with HRESULT: {hresultText}";
                return CommandResult.Error($"Command '{_commandName}' failed: {errorMsg}");
            }
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            // Get parameter definitions for this command
            var paramDefs = GetParameterDefinitionsForCommand(_commandName);
            var completions = new List<string>();

            if (args.Length == 0 || (args.Length == 1 && string.IsNullOrEmpty(args[0])))
            {
                // Offer all parameters
                foreach (var param in paramDefs)
                {
                    completions.Add($"--{param.Key}=");
                }
            }
            else
            {
                var lastArg = args[args.Length - 1];
                if (lastArg.StartsWith("--"))
                {
                    var paramName = lastArg.Substring(2).Split('=')[0];
                    
                    // Offer matching parameters
                    foreach (var param in paramDefs)
                    {
                        if (param.Key.StartsWith(paramName, StringComparison.OrdinalIgnoreCase))
                        {
                            completions.Add($"--{param.Key}=");
                        }
                    }
                }
            }

            return completions.ToArray();
        }

        public static string[] GetAllCommandNames()
        {
            return AllCommands;
        }

        private Dictionary<string, object> GetParameterDefinitionsForCommand(string commandName)
        {
            // Return default values based on command - this matches MainForm logic
            return commandName switch
            {
                "WriteGameSaveData" => new Dictionary<string, object>
                {
                    { "fileCount", 10 },
                    { "maxFileSizeBytes", 1024 },
                    { "unicodeFiles", false },
                    { "unicodeFolders", false }
                },
                "CopyTargetFolderToSaveFolder" => new Dictionary<string, object>
                {
                    { "sourceFolder", "C:\\gamesave\\dataset" }
                },
                "DeleteLocalFolder" => new Dictionary<string, object>
                {
                    { "path", "" }
                },
                "SmokeCommandEcho" => new Dictionary<string, object>
                {
                    { "payload", "" }
                },
                "SmokeDelay" => new Dictionary<string, object>
                {
                    { "delayMs", 1000 }
                },
                "ConsumeDiskSpace" => new Dictionary<string, object>
                {
                    { "sizeMb", 100 }
                },
                "PFGameSaveFilesSetActiveDevicePollIntervalForDebug" => new Dictionary<string, object>
                {
                    { "intervalMs", 5000 }
                },
                "PFGameSaveFilesSetMockDeviceIdForDebug" => new Dictionary<string, object>
                {
                    { "deviceId", "" }
                },
                "PFGameSaveFilesSetUiActiveDeviceContentionResponse" => new Dictionary<string, object>
                {
                    { "response", "Cancel" }
                },
                "PFGameSaveFilesSetUiConflictResponse" => new Dictionary<string, object>
                {
                    { "response", "Cancel" }
                },
                "PFGameSaveFilesSetUiOutOfStorageResponse" => new Dictionary<string, object>
                {
                    { "response", "Cancel" }
                },
                "PFGameSaveFilesSetUiProgressResponse" => new Dictionary<string, object>
                {
                    { "response", "Cancel" }
                },
                "PFGameSaveFilesSetUiSyncFailedResponse" => new Dictionary<string, object>
                {
                    { "response", "Cancel" }
                },
                "DoChaosMode" => new Dictionary<string, object>
                {
                    { "fileCreate", true },
                    { "fileModify", true },
                    { "fileDelete", true },
                    { "folderCreate", true },
                    { "folderDelete", true },
                    { "binaryFiles", true },
                    { "textFiles", true },
                    { "largeFiles", true },
                    { "unicodeFiles", true },
                    { "unicodeFolders", true },
                    { "operationsPerUpload", 10 },
                    { "numUploads", 5 }
                },
                _ => new Dictionary<string, object>()
            };
        }
    }
}
