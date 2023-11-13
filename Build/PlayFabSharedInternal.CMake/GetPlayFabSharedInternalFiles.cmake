cmake_minimum_required(VERSION 3.6)

# file list similar to PlayFabSharedInternal.vcxitems
function(GET_PLAYFAB_SHARED_INTERNAL_FILES
         OUT_PLAYFAB_SHARED_INTERNAL_FILES
         PATH_TO_SOURCE
         )

    set(${OUT_PLAYFAB_SHARED_INTERNAL_FILES}
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/ApiXAsyncProvider.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/AsyncOp.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/BaseModel.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/CancellationToken.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/EnumTraits.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Generated/Error.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/HttpRequest.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/JsonUtils.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Memory.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/ModelBuffer.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/PlatformUser.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/PlatformUtils.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/RapidJson.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Result.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/ResultMacros.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/RunContext.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/SdkVersion.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Trace.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Types.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/XAsyncOperation.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/XAsyncProviderBase.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/pch.h"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/CancellationToken.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/Generated/Error.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/HttpRequest.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/JsonUtils.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/Memory.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/ModelBuffer.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/pch.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/PlatformUtils.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/RunContext.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/Trace.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/XAsyncOperation.cpp"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/XAsyncProviderBase.cpp"
        PARENT_SCOPE
        )

endfunction()
