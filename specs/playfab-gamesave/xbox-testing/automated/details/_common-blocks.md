# Common Xbox Test Blocks

Shared initialization, cleanup, and reset sequences referenced by all scenario detail files.

---

### Common Xbox Init Block
Most scenarios share this initialization sequence. Referenced below as "Xbox Init Block."

```yaml
- command: XGameRuntimeInitialize
- command: PFInitialize
- command: PFServicesInitialize
- command: PFServiceConfigCreateHandle
  parameters:
    endpoint: "https://E18D7.playfabapi.com"
    titleId: "E18D7"
- command: XTaskQueueCreate
  parameters:
    workMode: ThreadPool
    completionMode: ThreadPool
    setAsProcessQueue: true
- command: PFGameSaveFilesInitialize
- command: PFGameSaveFilesSetUiCallbacks
  parameters:
    enable: true
- command: PFGameSaveFilesSetActiveDeviceChangedCallback
- command: XUserAddAsync
- command: PFLocalUserCreateHandleWithXboxUser
```

### Common Xbox Cleanup Block
Referenced below as "Xbox Cleanup Block."

```yaml
- command: PFLocalUserCloseHandle
- command: XUserCloseHandle
- command: PFGameSaveFilesUninitializeAsync
- command: PFServicesUninitializeAsync
- command: PFUninitializeAsync
- command: PFServiceConfigCloseHandle
- command: XTaskQueueCloseHandle
```

### Common Account Reset (Xbox)
The in-proc plan uses `PFGameSaveFilesResetCloudAsync` to clear the account, but this returns `E_NOTIMPL` on the GRTS provider. For Xbox/GRTS, account reset requires:

1. Perform Xbox Init Block to initialize and authenticate.
2. `PFGameSaveFilesAddUserWithUiAsync` to acquire the active lock.
3. `PFGameSaveFilesGetFolder` to locate the save root.
4. `DeleteSaveRoot` with `preserveManifest: false` to clear all local data.
5. Upload the empty state with `PFGameSaveFilesUploadWithUiAsync` using `ReleaseDeviceAsActive` to publish a blank manifest.
6. Xbox Cleanup Block to tear down.

Alternatively, if a service-side reset API or support tool is available, use that. Document the chosen approach per environment.

---

