#include "pch.h"

#include "XStoreHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XStore.h>
#include "CommandRegistry.h"

// Shared state for handles that persist between handler calls
static XStoreContextHandle s_storeContext = nullptr;
static XStoreProductQueryHandle s_productQuery = nullptr;
static XStoreLicenseHandle s_storeLicense = nullptr;
static XTaskQueueRegistrationToken s_gameLicenseChangedToken = {};
static XTaskQueueRegistrationToken s_packageLicenseLostToken = {};

CommandResultPayload HandleXStoreCreateContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_storeContext)
            {
                XStoreCloseContextHandle(s_storeContext);
                s_storeContext = nullptr;
            }
            HRESULT hr = XStoreCreateContext(state->xuser, &s_storeContext);
            LogToWindowFormat("XStoreCreateContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreCloseContextHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_storeContext)
            {
                XStoreCloseContextHandle(s_storeContext);
                s_storeContext = nullptr;
                LogToWindow("XStoreCloseContextHandle executed");
            }
            else
            {
                LogToWindow("XStoreCloseContextHandle skipped (no handle)");
            }
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryAssociatedProductsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryAssociatedProductsAsync(
                s_storeContext,
                XStoreProductKind::Game | XStoreProductKind::Durable | XStoreProductKind::Consumable,
                25,
                &async);
            LogToWindowFormat("XStoreQueryAssociatedProductsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_productQuery)
            {
                XStoreCloseProductsQueryHandle(s_productQuery);
                s_productQuery = nullptr;
            }
            HRESULT hr = XStoreQueryAssociatedProductsResult(&async, &s_productQuery);
            LogToWindowFormat("XStoreQueryAssociatedProductsResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryAssociatedProductsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryAssociatedProductsResult: result retrieved in Async handler");
            return s_productQuery ? S_OK : E_UNEXPECTED;
        });
}

CommandResultPayload HandleXStoreQueryProductsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryProductsAsync(
                s_storeContext,
                XStoreProductKind::Game | XStoreProductKind::Durable | XStoreProductKind::Consumable,
                nullptr,
                0,
                nullptr,
                0,
                &async);
            LogToWindowFormat("XStoreQueryProductsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_productQuery)
            {
                XStoreCloseProductsQueryHandle(s_productQuery);
                s_productQuery = nullptr;
            }
            HRESULT hr = XStoreQueryProductsResult(&async, &s_productQuery);
            LogToWindowFormat("XStoreQueryProductsResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryProductsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryProductsResult: result retrieved in Async handler");
            return s_productQuery ? S_OK : E_UNEXPECTED;
        });
}

CommandResultPayload HandleXStoreQueryEntitledProductsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryEntitledProductsAsync(
                s_storeContext,
                XStoreProductKind::Game | XStoreProductKind::Durable | XStoreProductKind::Consumable,
                25,
                &async);
            LogToWindowFormat("XStoreQueryEntitledProductsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_productQuery)
            {
                XStoreCloseProductsQueryHandle(s_productQuery);
                s_productQuery = nullptr;
            }
            HRESULT hr = XStoreQueryEntitledProductsResult(&async, &s_productQuery);
            LogToWindowFormat("XStoreQueryEntitledProductsResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryEntitledProductsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryEntitledProductsResult: result retrieved in Async handler");
            return s_productQuery ? S_OK : E_UNEXPECTED;
        });
}

CommandResultPayload HandleXStoreQueryProductForCurrentGameAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryProductForCurrentGameAsync(s_storeContext, &async);
            LogToWindowFormat("XStoreQueryProductForCurrentGameAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_productQuery)
            {
                XStoreCloseProductsQueryHandle(s_productQuery);
                s_productQuery = nullptr;
            }
            HRESULT hr = XStoreQueryProductForCurrentGameResult(&async, &s_productQuery);
            LogToWindowFormat("XStoreQueryProductForCurrentGameResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryProductForCurrentGameResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryProductForCurrentGameResult: result retrieved in Async handler");
            return s_productQuery ? S_OK : E_UNEXPECTED;
        });
}

CommandResultPayload HandleXStoreQueryProductForPackageAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryProductForPackageAsync(
                s_storeContext,
                XStoreProductKind::Game | XStoreProductKind::Durable,
                "",
                &async);
            LogToWindowFormat("XStoreQueryProductForPackageAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_productQuery)
            {
                XStoreCloseProductsQueryHandle(s_productQuery);
                s_productQuery = nullptr;
            }
            HRESULT hr = XStoreQueryProductForPackageResult(&async, &s_productQuery);
            LogToWindowFormat("XStoreQueryProductForPackageResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryProductForPackageResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryProductForPackageResult: result retrieved in Async handler");
            return s_productQuery ? S_OK : E_UNEXPECTED;
        });
}

CommandResultPayload HandleXStoreEnumerateProductsQuery(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_productQuery)
            {
                LogToWindow("XStoreEnumerateProductsQuery: no product query handle");
                return E_UNEXPECTED;
            }
            HRESULT hr = XStoreEnumerateProductsQuery(
                s_productQuery,
                nullptr,
                [](const XStoreProduct* product, void*) -> bool
                {
                    if (product)
                    {
                        LogToWindowFormat("  Product: %s (%s)", product->storeId, product->title);
                    }
                    return true;
                });
            LogToWindowFormat("XStoreEnumerateProductsQuery (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreProductsQueryHasMorePages(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_productQuery)
            {
                LogToWindow("XStoreProductsQueryHasMorePages: no product query handle");
                return E_UNEXPECTED;
            }
            bool hasMore = XStoreProductsQueryHasMorePages(s_productQuery);
            LogToWindowFormat("XStoreProductsQueryHasMorePages: %s", hasMore ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreProductsQueryNextPageAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (!s_productQuery)
            {
                LogToWindow("XStoreProductsQueryNextPageAsync: no product query handle");
                return E_UNEXPECTED;
            }
            HRESULT hr = XStoreProductsQueryNextPageAsync(s_productQuery, &async);
            LogToWindowFormat("XStoreProductsQueryNextPageAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_productQuery)
            {
                XStoreCloseProductsQueryHandle(s_productQuery);
                s_productQuery = nullptr;
            }
            HRESULT hr = XStoreProductsQueryNextPageResult(&async, &s_productQuery);
            LogToWindowFormat("XStoreProductsQueryNextPageResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreProductsQueryNextPageResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreProductsQueryNextPageResult: result retrieved in Async handler");
            return s_productQuery ? S_OK : E_UNEXPECTED;
        });
}

CommandResultPayload HandleXStoreCloseProductsQueryHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_productQuery)
            {
                XStoreCloseProductsQueryHandle(s_productQuery);
                s_productQuery = nullptr;
                LogToWindow("XStoreCloseProductsQueryHandle executed");
            }
            else
            {
                LogToWindow("XStoreCloseProductsQueryHandle skipped (no handle)");
            }
            return S_OK;
        });
}

CommandResultPayload HandleXStoreAcquireLicenseForPackageAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreAcquireLicenseForPackageAsync(s_storeContext, "", &async);
            LogToWindowFormat("XStoreAcquireLicenseForPackageAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_storeLicense)
            {
                XStoreCloseLicenseHandle(s_storeLicense);
                s_storeLicense = nullptr;
            }
            HRESULT hr = XStoreAcquireLicenseForPackageResult(&async, &s_storeLicense);
            LogToWindowFormat("XStoreAcquireLicenseForPackageResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreAcquireLicenseForPackageResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreAcquireLicenseForPackageResult: result retrieved in Async handler");
            return s_storeLicense ? S_OK : E_UNEXPECTED;
        });
}

CommandResultPayload HandleXStoreIsLicenseValid(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_storeLicense)
            {
                LogToWindow("XStoreIsLicenseValid: no license handle");
                return E_UNEXPECTED;
            }
            bool valid = XStoreIsLicenseValid(s_storeLicense);
            LogToWindowFormat("XStoreIsLicenseValid: %s", valid ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreCloseLicenseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_storeLicense)
            {
                XStoreCloseLicenseHandle(s_storeLicense);
                s_storeLicense = nullptr;
                LogToWindow("XStoreCloseLicenseHandle executed");
            }
            else
            {
                LogToWindow("XStoreCloseLicenseHandle skipped (no handle)");
            }
            return S_OK;
        });
}

CommandResultPayload HandleXStoreCanAcquireLicenseForStoreIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreCanAcquireLicenseForStoreIdAsync(s_storeContext, "", &async);
            LogToWindowFormat("XStoreCanAcquireLicenseForStoreIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XStoreCanAcquireLicenseResult result{};
            HRESULT hr = XStoreCanAcquireLicenseForStoreIdResult(&async, &result);
            LogToWindowFormat("XStoreCanAcquireLicenseForStoreIdResult (hr=0x%08X, status=%u)",
                static_cast<uint32_t>(hr), static_cast<uint32_t>(result.status));
            return hr;
        });
}

CommandResultPayload HandleXStoreCanAcquireLicenseForStoreIdResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreCanAcquireLicenseForStoreIdResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreCanAcquireLicenseForPackageAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreCanAcquireLicenseForPackageAsync(s_storeContext, "", &async);
            LogToWindowFormat("XStoreCanAcquireLicenseForPackageAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XStoreCanAcquireLicenseResult result{};
            HRESULT hr = XStoreCanAcquireLicenseForPackageResult(&async, &result);
            LogToWindowFormat("XStoreCanAcquireLicenseForPackageResult (hr=0x%08X, status=%u)",
                static_cast<uint32_t>(hr), static_cast<uint32_t>(result.status));
            return hr;
        });
}

CommandResultPayload HandleXStoreCanAcquireLicenseForPackageResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreCanAcquireLicenseForPackageResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryGameLicenseAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryGameLicenseAsync(s_storeContext, &async);
            LogToWindowFormat("XStoreQueryGameLicenseAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XStoreGameLicense license{};
            HRESULT hr = XStoreQueryGameLicenseResult(&async, &license);
            LogToWindowFormat("XStoreQueryGameLicenseResult (hr=0x%08X, isActive=%s, isTrial=%s)",
                static_cast<uint32_t>(hr),
                license.isActive ? "true" : "false",
                license.isTrial ? "true" : "false");
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryGameLicenseResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryGameLicenseResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryAddOnLicensesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryAddOnLicensesAsync(s_storeContext, &async);
            LogToWindowFormat("XStoreQueryAddOnLicensesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            uint32_t count = 0;
            HRESULT hr = XStoreQueryAddOnLicensesResultCount(&async, &count);
            if (SUCCEEDED(hr) && count > 0)
            {
                std::vector<XStoreAddonLicense> licenses(count);
                hr = XStoreQueryAddOnLicensesResult(&async, count, licenses.data());
                LogToWindowFormat("XStoreQueryAddOnLicensesResult (hr=0x%08X, count=%u)",
                    static_cast<uint32_t>(hr), count);
            }
            else
            {
                LogToWindowFormat("XStoreQueryAddOnLicensesResultCount (hr=0x%08X, count=%u)",
                    static_cast<uint32_t>(hr), count);
            }
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryAddOnLicensesResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryAddOnLicensesResultCount: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryAddOnLicensesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryAddOnLicensesResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryConsumableBalanceRemainingAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryConsumableBalanceRemainingAsync(s_storeContext, "", &async);
            LogToWindowFormat("XStoreQueryConsumableBalanceRemainingAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XStoreConsumableResult result{};
            HRESULT hr = XStoreQueryConsumableBalanceRemainingResult(&async, &result);
            LogToWindowFormat("XStoreQueryConsumableBalanceRemainingResult (hr=0x%08X, quantity=%u)",
                static_cast<uint32_t>(hr), result.quantity);
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryConsumableBalanceRemainingResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryConsumableBalanceRemainingResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreReportConsumableFulfillmentAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            GUID trackingId = {};
            HRESULT hr = XStoreReportConsumableFulfillmentAsync(
                s_storeContext, "", 1, trackingId, &async);
            LogToWindowFormat("XStoreReportConsumableFulfillmentAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XStoreConsumableResult result{};
            HRESULT hr = XStoreReportConsumableFulfillmentResult(&async, &result);
            LogToWindowFormat("XStoreReportConsumableFulfillmentResult (hr=0x%08X, quantity=%u)",
                static_cast<uint32_t>(hr), result.quantity);
            return hr;
        });
}

CommandResultPayload HandleXStoreReportConsumableFulfillmentResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreReportConsumableFulfillmentResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreGetUserCollectionsIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreGetUserCollectionsIdAsync(s_storeContext, "", "", &async);
            LogToWindowFormat("XStoreGetUserCollectionsIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            size_t size = 0;
            HRESULT hr = XStoreGetUserCollectionsIdResultSize(&async, &size);
            if (SUCCEEDED(hr) && size > 0)
            {
                std::vector<char> buffer(size);
                hr = XStoreGetUserCollectionsIdResult(&async, size, buffer.data());
                LogToWindowFormat("XStoreGetUserCollectionsIdResult (hr=0x%08X, size=%zu)",
                    static_cast<uint32_t>(hr), size);
            }
            else
            {
                LogToWindowFormat("XStoreGetUserCollectionsIdResultSize (hr=0x%08X, size=%zu)",
                    static_cast<uint32_t>(hr), size);
            }
            return hr;
        });
}

CommandResultPayload HandleXStoreGetUserCollectionsIdResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreGetUserCollectionsIdResultSize: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreGetUserCollectionsIdResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreGetUserCollectionsIdResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreGetUserPurchaseIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreGetUserPurchaseIdAsync(s_storeContext, "", "", &async);
            LogToWindowFormat("XStoreGetUserPurchaseIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            size_t size = 0;
            HRESULT hr = XStoreGetUserPurchaseIdResultSize(&async, &size);
            if (SUCCEEDED(hr) && size > 0)
            {
                std::vector<char> buffer(size);
                hr = XStoreGetUserPurchaseIdResult(&async, size, buffer.data());
                LogToWindowFormat("XStoreGetUserPurchaseIdResult (hr=0x%08X, size=%zu)",
                    static_cast<uint32_t>(hr), size);
            }
            else
            {
                LogToWindowFormat("XStoreGetUserPurchaseIdResultSize (hr=0x%08X, size=%zu)",
                    static_cast<uint32_t>(hr), size);
            }
            return hr;
        });
}

CommandResultPayload HandleXStoreGetUserPurchaseIdResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreGetUserPurchaseIdResultSize: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreGetUserPurchaseIdResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreGetUserPurchaseIdResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryLicenseTokenAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryLicenseTokenAsync(s_storeContext, nullptr, 0, "", &async);
            LogToWindowFormat("XStoreQueryLicenseTokenAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            size_t size = 0;
            HRESULT hr = XStoreQueryLicenseTokenResultSize(&async, &size);
            if (SUCCEEDED(hr) && size > 0)
            {
                std::vector<char> buffer(size);
                hr = XStoreQueryLicenseTokenResult(&async, size, buffer.data());
                LogToWindowFormat("XStoreQueryLicenseTokenResult (hr=0x%08X, size=%zu)",
                    static_cast<uint32_t>(hr), size);
            }
            else
            {
                LogToWindowFormat("XStoreQueryLicenseTokenResultSize (hr=0x%08X, size=%zu)",
                    static_cast<uint32_t>(hr), size);
            }
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryLicenseTokenResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryLicenseTokenResultSize: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryLicenseTokenResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryLicenseTokenResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreShowPurchaseUIAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowPurchaseUIAsync(s_storeContext, "", nullptr, nullptr, &async);
            LogToWindowFormat("XStoreShowPurchaseUIAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowPurchaseUIResult(&async);
            LogToWindowFormat("XStoreShowPurchaseUIResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreShowPurchaseUIResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreShowPurchaseUIResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreShowProductPageUIAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowProductPageUIAsync(s_storeContext, "", &async);
            LogToWindowFormat("XStoreShowProductPageUIAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowProductPageUIResult(&async);
            LogToWindowFormat("XStoreShowProductPageUIResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreShowProductPageUIResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreShowProductPageUIResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreShowAssociatedProductsUIAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowAssociatedProductsUIAsync(
                s_storeContext,
                "",
                XStoreProductKind::Game | XStoreProductKind::Durable,
                &async);
            LogToWindowFormat("XStoreShowAssociatedProductsUIAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowAssociatedProductsUIResult(&async);
            LogToWindowFormat("XStoreShowAssociatedProductsUIResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreShowAssociatedProductsUIResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreShowAssociatedProductsUIResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreShowRateAndReviewUIAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowRateAndReviewUIAsync(s_storeContext, &async);
            LogToWindowFormat("XStoreShowRateAndReviewUIAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XStoreRateAndReviewResult result{};
            HRESULT hr = XStoreShowRateAndReviewUIResult(&async, &result);
            LogToWindowFormat("XStoreShowRateAndReviewUIResult (hr=0x%08X, wasUpdated=%s)",
                static_cast<uint32_t>(hr), result.wasUpdated ? "true" : "false");
            return hr;
        });
}

CommandResultPayload HandleXStoreShowRateAndReviewUIResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreShowRateAndReviewUIResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreShowRedeemTokenUIAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowRedeemTokenUIAsync(s_storeContext, "", nullptr, 0, false, &async);
            LogToWindowFormat("XStoreShowRedeemTokenUIAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowRedeemTokenUIResult(&async);
            LogToWindowFormat("XStoreShowRedeemTokenUIResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreShowRedeemTokenUIResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreShowRedeemTokenUIResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreShowGiftingUIAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowGiftingUIAsync(s_storeContext, "", nullptr, nullptr, &async);
            LogToWindowFormat("XStoreShowGiftingUIAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreShowGiftingUIResult(&async);
            LogToWindowFormat("XStoreShowGiftingUIResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreShowGiftingUIResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreShowGiftingUIResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryGameAndDlcPackageUpdatesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryGameAndDlcPackageUpdatesAsync(s_storeContext, &async);
            LogToWindowFormat("XStoreQueryGameAndDlcPackageUpdatesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            uint32_t count = 0;
            HRESULT hr = XStoreQueryGameAndDlcPackageUpdatesResultCount(&async, &count);
            if (SUCCEEDED(hr) && count > 0)
            {
                std::vector<XStorePackageUpdate> updates(count);
                hr = XStoreQueryGameAndDlcPackageUpdatesResult(&async, count, updates.data());
                LogToWindowFormat("XStoreQueryGameAndDlcPackageUpdatesResult (hr=0x%08X, count=%u)",
                    static_cast<uint32_t>(hr), count);
            }
            else
            {
                LogToWindowFormat("XStoreQueryGameAndDlcPackageUpdatesResultCount (hr=0x%08X, count=%u)",
                    static_cast<uint32_t>(hr), count);
            }
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryGameAndDlcPackageUpdatesResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryGameAndDlcPackageUpdatesResultCount: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryGameAndDlcPackageUpdatesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryGameAndDlcPackageUpdatesResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryPackageUpdatesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryPackageUpdatesAsync(s_storeContext, nullptr, 0, &async);
            LogToWindowFormat("XStoreQueryPackageUpdatesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            uint32_t count = 0;
            HRESULT hr = XStoreQueryPackageUpdatesResultCount(&async, &count);
            if (SUCCEEDED(hr) && count > 0)
            {
                std::vector<XStorePackageUpdate> updates(count);
                hr = XStoreQueryPackageUpdatesResult(&async, count, updates.data());
                LogToWindowFormat("XStoreQueryPackageUpdatesResult (hr=0x%08X, count=%u)",
                    static_cast<uint32_t>(hr), count);
            }
            else
            {
                LogToWindowFormat("XStoreQueryPackageUpdatesResultCount (hr=0x%08X, count=%u)",
                    static_cast<uint32_t>(hr), count);
            }
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryPackageUpdatesResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryPackageUpdatesResultCount: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryPackageUpdatesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryPackageUpdatesResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreDownloadPackageUpdatesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreDownloadPackageUpdatesAsync(s_storeContext, nullptr, 0, &async);
            LogToWindowFormat("XStoreDownloadPackageUpdatesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreDownloadPackageUpdatesResult(&async);
            LogToWindowFormat("XStoreDownloadPackageUpdatesResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreDownloadPackageUpdatesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreDownloadPackageUpdatesResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreDownloadAndInstallPackageUpdatesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreDownloadAndInstallPackageUpdatesAsync(s_storeContext, nullptr, 0, &async);
            LogToWindowFormat("XStoreDownloadAndInstallPackageUpdatesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreDownloadAndInstallPackageUpdatesResult(&async);
            LogToWindowFormat("XStoreDownloadAndInstallPackageUpdatesResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreDownloadAndInstallPackageUpdatesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreDownloadAndInstallPackageUpdatesResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreDownloadAndInstallPackagesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreDownloadAndInstallPackagesAsync(s_storeContext, nullptr, 0, &async);
            LogToWindowFormat("XStoreDownloadAndInstallPackagesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            uint32_t count = 0;
            HRESULT hr = XStoreDownloadAndInstallPackagesResultCount(&async, &count);
            if (SUCCEEDED(hr) && count > 0)
            {
                std::vector<char> idBuf(static_cast<size_t>(count) * XPACKAGE_IDENTIFIER_MAX_LENGTH);
                hr = XStoreDownloadAndInstallPackagesResult(
                    &async, count,
                    reinterpret_cast<char(*)[XPACKAGE_IDENTIFIER_MAX_LENGTH]>(idBuf.data()));
                LogToWindowFormat("XStoreDownloadAndInstallPackagesResult (hr=0x%08X, count=%u)",
                    static_cast<uint32_t>(hr), count);
            }
            else
            {
                LogToWindowFormat("XStoreDownloadAndInstallPackagesResultCount (hr=0x%08X, count=%u)",
                    static_cast<uint32_t>(hr), count);
            }
            return hr;
        });
}

CommandResultPayload HandleXStoreDownloadAndInstallPackagesResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreDownloadAndInstallPackagesResultCount: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreDownloadAndInstallPackagesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreDownloadAndInstallPackagesResult: result retrieved in Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreQueryPackageIdentifier(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH] = {};
            HRESULT hr = XStoreQueryPackageIdentifier("", sizeof(packageId), packageId);
            LogToWindowFormat("XStoreQueryPackageIdentifier (hr=0x%08X, packageId=%s)",
                static_cast<uint32_t>(hr), packageId);
            return hr;
        });
}

CommandResultPayload HandleXStoreRegisterGameLicenseChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_storeContext)
            {
                LogToWindow("XStoreRegisterGameLicenseChanged: no store context");
                return E_UNEXPECTED;
            }
            HRESULT hr = XStoreRegisterGameLicenseChanged(
                s_storeContext,
                state->taskQueue,
                nullptr,
                [](void*) { LogToWindow("GameLicenseChanged callback fired"); },
                &s_gameLicenseChangedToken);
            LogToWindowFormat("XStoreRegisterGameLicenseChanged (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreUnregisterGameLicenseChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_storeContext)
            {
                LogToWindow("XStoreUnregisterGameLicenseChanged: no store context");
                return E_UNEXPECTED;
            }
            bool result = XStoreUnregisterGameLicenseChanged(s_storeContext, s_gameLicenseChangedToken, true);
            s_gameLicenseChangedToken = {};
            LogToWindowFormat("XStoreUnregisterGameLicenseChanged: %s", result ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreRegisterPackageLicenseLost(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_storeLicense)
            {
                LogToWindow("XStoreRegisterPackageLicenseLost: no license handle");
                return E_UNEXPECTED;
            }
            HRESULT hr = XStoreRegisterPackageLicenseLost(
                s_storeLicense,
                state->taskQueue,
                nullptr,
                [](void*) { LogToWindow("PackageLicenseLost callback fired"); },
                &s_packageLicenseLostToken);
            LogToWindowFormat("XStoreRegisterPackageLicenseLost (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreUnregisterPackageLicenseLost(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_storeLicense)
            {
                LogToWindow("XStoreUnregisterPackageLicenseLost: no license handle");
                return E_UNEXPECTED;
            }
            bool result = XStoreUnregisterPackageLicenseLost(s_storeLicense, s_packageLicenseLostToken, true);
            s_packageLicenseLostToken = {};
            LogToWindowFormat("XStoreUnregisterPackageLicenseLost: %s", result ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreIsAvailabilityPurchasable(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XStoreAvailability availability{};
            bool purchasable = XStoreIsAvailabilityPurchasable(availability);
            LogToWindowFormat("XStoreIsAvailabilityPurchasable: %s", purchasable ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXStoreAcquireLicenseForDurablesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreAcquireLicenseForDurablesAsync(s_storeContext, "", &async);
            LogToWindowFormat("XStoreAcquireLicenseForDurablesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_storeLicense)
            {
                XStoreCloseLicenseHandle(s_storeLicense);
                s_storeLicense = nullptr;
            }
            HRESULT hr = XStoreAcquireLicenseForDurablesResult(&async, &s_storeLicense);
            LogToWindowFormat("XStoreAcquireLicenseForDurablesResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreAcquireLicenseForDurablesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreAcquireLicenseForDurablesResult: result retrieved in Async handler");
            return s_storeLicense ? S_OK : E_UNEXPECTED;
        });
}

CommandResultPayload HandleXStoreQueryAssociatedProductsForStoreIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XStoreQueryAssociatedProductsForStoreIdAsync(
                s_storeContext,
                "",
                XStoreProductKind::Game | XStoreProductKind::Durable | XStoreProductKind::Consumable,
                25,
                &async);
            LogToWindowFormat("XStoreQueryAssociatedProductsForStoreIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_productQuery)
            {
                XStoreCloseProductsQueryHandle(s_productQuery);
                s_productQuery = nullptr;
            }
            HRESULT hr = XStoreQueryAssociatedProductsForStoreIdResult(&async, &s_productQuery);
            LogToWindowFormat("XStoreQueryAssociatedProductsForStoreIdResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXStoreQueryAssociatedProductsForStoreIdResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XStoreQueryAssociatedProductsForStoreIdResult: result retrieved in Async handler");
            return s_productQuery ? S_OK : E_UNEXPECTED;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XStoreAcquireLicenseForDurablesAsync", HandleXStoreAcquireLicenseForDurablesAsync },
    { "XStoreAcquireLicenseForDurablesResult", HandleXStoreAcquireLicenseForDurablesResult },
    { "XStoreAcquireLicenseForPackageAsync", HandleXStoreAcquireLicenseForPackageAsync },
    { "XStoreAcquireLicenseForPackageResult", HandleXStoreAcquireLicenseForPackageResult },
    { "XStoreCanAcquireLicenseForPackageAsync", HandleXStoreCanAcquireLicenseForPackageAsync },
    { "XStoreCanAcquireLicenseForPackageResult", HandleXStoreCanAcquireLicenseForPackageResult },
    { "XStoreCanAcquireLicenseForStoreIdAsync", HandleXStoreCanAcquireLicenseForStoreIdAsync },
    { "XStoreCanAcquireLicenseForStoreIdResult", HandleXStoreCanAcquireLicenseForStoreIdResult },
    { "XStoreCloseContextHandle", HandleXStoreCloseContextHandle },
    { "XStoreCloseLicenseHandle", HandleXStoreCloseLicenseHandle },
    { "XStoreCloseProductsQueryHandle", HandleXStoreCloseProductsQueryHandle },
    { "XStoreCreateContext", HandleXStoreCreateContext },
    { "XStoreDownloadAndInstallPackageUpdatesAsync", HandleXStoreDownloadAndInstallPackageUpdatesAsync },
    { "XStoreDownloadAndInstallPackageUpdatesResult", HandleXStoreDownloadAndInstallPackageUpdatesResult },
    { "XStoreDownloadAndInstallPackagesAsync", HandleXStoreDownloadAndInstallPackagesAsync },
    { "XStoreDownloadAndInstallPackagesResult", HandleXStoreDownloadAndInstallPackagesResult },
    { "XStoreDownloadAndInstallPackagesResultCount", HandleXStoreDownloadAndInstallPackagesResultCount },
    { "XStoreDownloadPackageUpdatesAsync", HandleXStoreDownloadPackageUpdatesAsync },
    { "XStoreDownloadPackageUpdatesResult", HandleXStoreDownloadPackageUpdatesResult },
    { "XStoreEnumerateProductsQuery", HandleXStoreEnumerateProductsQuery },
    { "XStoreGetUserCollectionsIdAsync", HandleXStoreGetUserCollectionsIdAsync },
    { "XStoreGetUserCollectionsIdResult", HandleXStoreGetUserCollectionsIdResult },
    { "XStoreGetUserCollectionsIdResultSize", HandleXStoreGetUserCollectionsIdResultSize },
    { "XStoreGetUserPurchaseIdAsync", HandleXStoreGetUserPurchaseIdAsync },
    { "XStoreGetUserPurchaseIdResult", HandleXStoreGetUserPurchaseIdResult },
    { "XStoreGetUserPurchaseIdResultSize", HandleXStoreGetUserPurchaseIdResultSize },
    { "XStoreIsAvailabilityPurchasable", HandleXStoreIsAvailabilityPurchasable },
    { "XStoreIsLicenseValid", HandleXStoreIsLicenseValid },
    { "XStoreProductsQueryHasMorePages", HandleXStoreProductsQueryHasMorePages },
    { "XStoreProductsQueryNextPageAsync", HandleXStoreProductsQueryNextPageAsync },
    { "XStoreProductsQueryNextPageResult", HandleXStoreProductsQueryNextPageResult },
    { "XStoreQueryAddOnLicensesAsync", HandleXStoreQueryAddOnLicensesAsync },
    { "XStoreQueryAddOnLicensesResult", HandleXStoreQueryAddOnLicensesResult },
    { "XStoreQueryAddOnLicensesResultCount", HandleXStoreQueryAddOnLicensesResultCount },
    { "XStoreQueryAssociatedProductsAsync", HandleXStoreQueryAssociatedProductsAsync },
    { "XStoreQueryAssociatedProductsForStoreIdAsync", HandleXStoreQueryAssociatedProductsForStoreIdAsync },
    { "XStoreQueryAssociatedProductsForStoreIdResult", HandleXStoreQueryAssociatedProductsForStoreIdResult },
    { "XStoreQueryAssociatedProductsResult", HandleXStoreQueryAssociatedProductsResult },
    { "XStoreQueryConsumableBalanceRemainingAsync", HandleXStoreQueryConsumableBalanceRemainingAsync },
    { "XStoreQueryConsumableBalanceRemainingResult", HandleXStoreQueryConsumableBalanceRemainingResult },
    { "XStoreQueryEntitledProductsAsync", HandleXStoreQueryEntitledProductsAsync },
    { "XStoreQueryEntitledProductsResult", HandleXStoreQueryEntitledProductsResult },
    { "XStoreQueryGameAndDlcPackageUpdatesAsync", HandleXStoreQueryGameAndDlcPackageUpdatesAsync },
    { "XStoreQueryGameAndDlcPackageUpdatesResult", HandleXStoreQueryGameAndDlcPackageUpdatesResult },
    { "XStoreQueryGameAndDlcPackageUpdatesResultCount", HandleXStoreQueryGameAndDlcPackageUpdatesResultCount },
    { "XStoreQueryGameLicenseAsync", HandleXStoreQueryGameLicenseAsync },
    { "XStoreQueryGameLicenseResult", HandleXStoreQueryGameLicenseResult },
    { "XStoreQueryLicenseTokenAsync", HandleXStoreQueryLicenseTokenAsync },
    { "XStoreQueryLicenseTokenResult", HandleXStoreQueryLicenseTokenResult },
    { "XStoreQueryLicenseTokenResultSize", HandleXStoreQueryLicenseTokenResultSize },
    { "XStoreQueryPackageIdentifier", HandleXStoreQueryPackageIdentifier },
    { "XStoreQueryPackageUpdatesAsync", HandleXStoreQueryPackageUpdatesAsync },
    { "XStoreQueryPackageUpdatesResult", HandleXStoreQueryPackageUpdatesResult },
    { "XStoreQueryPackageUpdatesResultCount", HandleXStoreQueryPackageUpdatesResultCount },
    { "XStoreQueryProductForCurrentGameAsync", HandleXStoreQueryProductForCurrentGameAsync },
    { "XStoreQueryProductForCurrentGameResult", HandleXStoreQueryProductForCurrentGameResult },
    { "XStoreQueryProductForPackageAsync", HandleXStoreQueryProductForPackageAsync },
    { "XStoreQueryProductForPackageResult", HandleXStoreQueryProductForPackageResult },
    { "XStoreQueryProductsAsync", HandleXStoreQueryProductsAsync },
    { "XStoreQueryProductsResult", HandleXStoreQueryProductsResult },
    { "XStoreRegisterGameLicenseChanged", HandleXStoreRegisterGameLicenseChanged },
    { "XStoreRegisterPackageLicenseLost", HandleXStoreRegisterPackageLicenseLost },
    { "XStoreReportConsumableFulfillmentAsync", HandleXStoreReportConsumableFulfillmentAsync },
    { "XStoreReportConsumableFulfillmentResult", HandleXStoreReportConsumableFulfillmentResult },
    { "XStoreShowAssociatedProductsUIAsync", HandleXStoreShowAssociatedProductsUIAsync },
    { "XStoreShowAssociatedProductsUIResult", HandleXStoreShowAssociatedProductsUIResult },
    { "XStoreShowGiftingUIAsync", HandleXStoreShowGiftingUIAsync },
    { "XStoreShowGiftingUIResult", HandleXStoreShowGiftingUIResult },
    { "XStoreShowProductPageUIAsync", HandleXStoreShowProductPageUIAsync },
    { "XStoreShowProductPageUIResult", HandleXStoreShowProductPageUIResult },
    { "XStoreShowPurchaseUIAsync", HandleXStoreShowPurchaseUIAsync },
    { "XStoreShowPurchaseUIResult", HandleXStoreShowPurchaseUIResult },
    { "XStoreShowRateAndReviewUIAsync", HandleXStoreShowRateAndReviewUIAsync },
    { "XStoreShowRateAndReviewUIResult", HandleXStoreShowRateAndReviewUIResult },
    { "XStoreShowRedeemTokenUIAsync", HandleXStoreShowRedeemTokenUIAsync },
    { "XStoreShowRedeemTokenUIResult", HandleXStoreShowRedeemTokenUIResult },
    { "XStoreUnregisterGameLicenseChanged", HandleXStoreUnregisterGameLicenseChanged },
    { "XStoreUnregisterPackageLicenseLost", HandleXStoreUnregisterPackageLicenseLost }
});
