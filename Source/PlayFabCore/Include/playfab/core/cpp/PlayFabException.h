// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <exception>
#include <string>

namespace PlayFab
{
namespace Wrappers
{

struct Exception : public std::exception
{
    Exception(HRESULT _hr) : hr{ _hr }
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "PlayFabException: 0x%08X", static_cast<unsigned int>(_hr));
        m_what = buf;
    }
    HRESULT const hr;

    const char* what() const noexcept override
    {
        return m_what.c_str();
    }

private:
    std::string m_what;
};

#define THROW_IF_FAILED(hr) do { HRESULT __hrRet = hr; if (FAILED(__hrRet)) { throw PlayFab::Wrappers::Exception{ __hrRet }; }} while (0)

}
}
