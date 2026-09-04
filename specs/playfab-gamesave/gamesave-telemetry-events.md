GameSave PowerBI telemetry 

1) Activation
Metric: Activation Error Failure Rate. The failure rate of the specific error, based on all errors.
EventName: ContextActivationFailure
Calculation: (COUNT(HResultUint) / COUNT(All ContextActivationFailures))
Shows by PlatformType

2) Sync
Metric: Sync Error Failure Rate. The failure rate of the specific error, based on all errors.
EventName: ContextSyncError
Calculation: (COUNT(HResultUint) / COUNT(All ContextSyncErrors))
Filter: SyncDownload (0 = upload / 1 = download)
Options:
    CR_KeepLocal
    CR_NoConflictsExpected
    CR_NoResolutionChosen
    CR_SelectVersion
    CR_TakeRemote

3) Delete
Metric: Delete Failure Rate. The failure rate for the ContextDelete event.
EventName: ContextDelete
Calculation: (COUNT(HResultUint) (where HResultUint <> 0) / COUNT(All ContextDelete))

4) Sync Compression
Metric: Compression Rate
EventName: ContextSync
Calculation: SyncSizeBytes / OriginalSizeBytes

5) Users
Daily report streams contain a Distinct Count of EntityId called Users
SUM(Users)

6) Xuids
Daily report streams contain a Distinct Count of Xuid called Xuids
SUM(Xuids)

7) Devices
Daily report streams contain a Distinct Count of either HardwareId (Console) or device_id (Desktop) called Devices
SUM(Devices)

8) Events
Daily report streams contain a count of events received called EventCount
SUM(EventCount)
