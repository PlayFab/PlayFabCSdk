// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>
#include <playfab/core/PFTypes.h>
#include <playfab/services/PFTypes.h>

extern "C"
{

#pragma push_macro("IN")
#undef IN

/// <summary>
/// PFCloudScriptExecuteFunctionRequest data model. Executes an Azure Function with the profile of the
/// entity that is defined in the request.
/// </summary>
typedef struct PFCloudScriptExecuteFunctionRequest
{
    /// <summary>
    /// (Optional) The optional custom tags associated with the request (e.g. build number, external
    /// trace identifiers, etc.).
    /// </summary>
    _Maybenull_ _Field_size_(customTagsCount) struct PFStringDictionaryEntry const* customTags;

    /// <summary>
    /// Count of customTags
    /// </summary>
    uint32_t customTagsCount;

    /// <summary>
    /// (Optional) The optional entity to perform this action on. Defaults to the currently logged in
    /// entity.
    /// </summary>
    _Maybenull_ PFEntityKey const* entity;

    /// <summary>
    /// The name of the CloudScript function to execute.
    /// </summary>
    _Null_terminated_ const char* functionName;

    /// <summary>
    /// (Optional) Object that is passed in to the function as the FunctionArgument field of the FunctionExecutionContext
    /// data structure.
    /// </summary>
    PFJsonObject functionParameter;

    /// <summary>
    /// (Optional) Generate a 'entity_executed_cloudscript_function' PlayStream event containing the
    /// results of the function execution and other contextual information. This event will show up in
    /// the PlayStream debugger console for the player in Game Manager.
    /// </summary>
    _Maybenull_ bool const* generatePlayStreamEvent;

} PFCloudScriptExecuteFunctionRequest;

/// <summary>
/// PFCloudScriptFunctionExecutionError data model.
/// </summary>
typedef struct PFCloudScriptFunctionExecutionError
{
    /// <summary>
    /// (Optional) Error code, such as CloudScriptAzureFunctionsExecutionTimeLimitExceeded, CloudScriptAzureFunctionsArgumentSizeExceeded,
    /// CloudScriptAzureFunctionsReturnSizeExceeded or CloudScriptAzureFunctionsHTTPRequestError.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* error;

    /// <summary>
    /// (Optional) Details about the error.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* message;

    /// <summary>
    /// (Optional) Point during the execution of the function at which the error occurred, if any.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* stackTrace;

} PFCloudScriptFunctionExecutionError;

/// <summary>
/// PFCloudScriptExecuteFunctionResult data model.
/// </summary>
typedef struct PFCloudScriptExecuteFunctionResult
{
    /// <summary>
    /// (Optional) Error from the CloudScript Azure Function.
    /// </summary>
    _Maybenull_ PFCloudScriptFunctionExecutionError const* error;

    /// <summary>
    /// The amount of time the function took to execute.
    /// </summary>
    int32_t executionTimeMilliseconds;

    /// <summary>
    /// (Optional) The name of the function that executed.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* functionName;

    /// <summary>
    /// (Optional) The object returned from the function, if any.
    /// </summary>
    PFJsonObject functionResult;

    /// <summary>
    /// (Optional) Flag indicating if the FunctionResult was too large and was subsequently dropped from
    /// this event.
    /// </summary>
    _Maybenull_ bool const* functionResultTooLarge;

} PFCloudScriptExecuteFunctionResult;

#pragma pop_macro("IN")

}
