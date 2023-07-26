// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <exception>

namespace PlayFab
{
namespace Wrappers
{

struct Exception : public std::exception
{
    Exception(HRESULT _hr) : hr{ _hr }
    {
    }
    HRESULT const hr;
};

#define THROW_IF_FAILED(hr) do { HRESULT __hrRet = hr; if (FAILED(__hrRet)) { throw PlayFab::Wrappers::Exception{ __hrRet }; }} while (0, 0)

}
}
