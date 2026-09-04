// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

struct DeviceGameSaveState;

HRESULT Sample_GameSave_Cleanup(DeviceGameSaveState* state);
bool Sample_GameSave_ParseArgs(_In_z_ const char* cmdLine, DeviceGameSaveState* gameSaveState);
