#include "pch.h"
#include "ModelBuffer.h"

namespace PlayFab
{

ModelBuffer::ModelBuffer(void* buffer, size_t bufferSize) :
#if _DEBUG
    m_buffer{ static_cast<char*>(buffer) },
    m_bufferSize{ bufferSize },
#endif
    m_bufferPtr{ buffer },
    m_remaining{ bufferSize }
{
}

size_t ModelBuffer::RemainingSpace() const
{
    return m_remaining;
}

Result<const char*> ModelBuffer::CopyTo(const char* input)
{
    if (input)
    {
        // Alloc
        size_t bytesNeeded = std::strlen(input) + 1; // null terminator
        auto allocResult = Alloc<char>(bytesNeeded);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
#if HC_PLATFORM_IS_MICROSOFT
        auto error = strcpy_s(outputPtr, bytesNeeded, input);
        RETURN_HR_IF(E_FAIL, error);

        return outputPtr;
#else
        return std::strcpy(outputPtr, input);
#endif
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

Result<const char* const*> ModelBuffer::CopyToArray(const char* const* input, size_t inputCount)
{
    if (input && inputCount)
    {
        // Alloc
        auto allocResult = Alloc<const char*>(inputCount);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
        for (size_t i = 0; i < inputCount; ++i)
        {
            auto copyResult = this->CopyTo(input[i]);
            RETURN_IF_FAILED(copyResult.hr);
            outputPtr[i] = copyResult.ExtractPayload();
        }
        return outputPtr;
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

Result<PFStringDictionaryEntry const*> ModelBuffer::CopyToDictionary(PFStringDictionaryEntry const* input, size_t inputCount)
{
    if (input && inputCount)
    {
        // Alloc
        auto allocResult = Alloc<PFStringDictionaryEntry>(inputCount);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
        for (size_t i = 0; i < inputCount; ++i)
        {
            auto copyKeyResult = this->CopyTo(input[i].key);
            RETURN_IF_FAILED(copyKeyResult.hr);
            outputPtr[i].key = copyKeyResult.ExtractPayload();

            auto copyValueResult = this->CopyTo(input[i].value);
            RETURN_IF_FAILED(copyValueResult.hr);
            outputPtr[i].value = copyValueResult.ExtractPayload();
        }
        return outputPtr;
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

}
