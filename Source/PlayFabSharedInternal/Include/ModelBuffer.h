#pragma once

#include <playfab/core/cpp/TypeWrapperHelpers.h>

namespace PlayFab
{

class ModelBuffer
{
public:
    ModelBuffer(void* buffer, size_t bufferSize);
    ModelBuffer(const ModelBuffer&) = delete;
    ModelBuffer(ModelBuffer&&) = default;
    ~ModelBuffer() = default;

    size_t RemainingSpace() const;

    template<typename T>
    Result<T*> Alloc(size_t n);

    template<typename T, typename std::enable_if_t<std::is_fundamental_v<T> || std::is_enum_v<T>, bool> = true>
    Result<T const*> CopyTo(const T* input);

    template<typename T>
    Result<typename T::ModelType const*> CopyTo(typename T::ModelType const* input);

    Result<const char*> CopyTo(const char* input);

    template<typename T, typename std::enable_if_t<std::is_fundamental_v<T> || std::is_enum_v<T>, bool> = true>
    Result<T const*> CopyToArray(const T* input, size_t inputCount);

    template<typename T>
    Result<typename T::ModelType const* const*> CopyToArray(typename T::ModelType const* const* input, size_t inputCount);

    Result<const char* const*> CopyToArray(const char* const* input, size_t inputCount);

    template<typename T, typename std::enable_if_t<Detail::IsDictionaryEntry<T>::value && (std::is_fundamental_v<decltype(T::value)> || std::is_enum_v<decltype(T::value)>), bool> = true>
    Result<T const*> CopyToDictionary(const T* input, size_t inputCount);

    template<typename InternalModelWrapperT>
    Result<typename InternalModelWrapperT::DictionaryEntryType const*> CopyToDictionary(const typename InternalModelWrapperT::DictionaryEntryType* input, size_t inputCount);

    Result<PFStringDictionaryEntry const*> CopyToDictionary(PFStringDictionaryEntry const* input, size_t inputCount);

private:
#if _DEBUG
    char* const m_buffer;
    size_t const m_bufferSize;
#endif
    void* m_bufferPtr;
    size_t m_remaining;
};

//------------------------------------------------------------------------------
// Template implementations
//------------------------------------------------------------------------------

template<typename T>
Result<T*> ModelBuffer::Alloc(size_t n)
{
    size_t const size = sizeof(T) * n;
#if _M_IX86
    // Ignore alignment requirements on x86 builds because of a Visual Studio bug where stack variables don't always honor alignment.
    // This issue has been reported here https://developercommunity.visualstudio.com/t/visual-c-alignof-is-not-conformant-on-x86/1258506
    // but has not yet been fixed.
    if (m_remaining >= size)
#else
    if (std::align(alignof(T), size, m_bufferPtr, m_remaining))
#endif
    {
        T* result = reinterpret_cast<T*>(m_bufferPtr);
        m_bufferPtr = static_cast<char*>(m_bufferPtr) + size;
        m_remaining -= size;
        return result;
    }
    return E_NOT_SUFFICIENT_BUFFER;
}

template<typename T, typename std::enable_if_t<std::is_fundamental_v<T> || std::is_enum_v<T>, bool>>
Result<T const*> ModelBuffer::CopyTo(const T* input)
{
    if (input)
    {
        // Alloc
        auto allocResult = Alloc<T>(1);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
        *outputPtr = *input;
        return std::move(outputPtr);
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

template<typename T>
Result<typename T::ModelType const*> ModelBuffer::CopyTo(const typename T::ModelType* input)
{
    if (input)
    {
        // Alloc
        auto allocResult = Alloc<typename T::ModelType>(1);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
        RETURN_IF_FAILED(T::Copy(*input, *outputPtr, *this));

        return std::move(outputPtr);
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

template<typename T, typename std::enable_if_t<std::is_fundamental_v<T> || std::is_enum_v<T>,bool>>
Result<T const*> ModelBuffer::CopyToArray(const T* input, size_t inputCount)
{
    if (input && inputCount)
    {
        // Alloc
        auto allocResult = Alloc<T>(inputCount);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
        std::memcpy(outputPtr, input, sizeof(T) * inputCount);

        return std::move(outputPtr);
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

template<typename T>
Result<typename T::ModelType const* const*> ModelBuffer::CopyToArray(typename T::ModelType const* const* input, size_t inputCount)
{
    if (input && inputCount)
    {
        // Alloc
        auto allocResult = Alloc<typename T::ModelType const*>(inputCount);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
        for (size_t i = 0; i < inputCount; ++i)
        {
            auto copyResult = this->CopyTo<T>(input[i]);
            RETURN_IF_FAILED(copyResult.hr);
            outputPtr[i] = copyResult.ExtractPayload();
        }
        return std::move(outputPtr);
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

template<typename T, typename std::enable_if_t<Detail::IsDictionaryEntry<T>::value && (std::is_fundamental_v<decltype(T::value)> || std::is_enum_v<decltype(T::value)>), bool>>
Result<T const*> ModelBuffer::CopyToDictionary(const T* input, size_t inputCount)
{
    if (input && inputCount)
    {
        // Alloc
        auto allocResult = Alloc<T>(inputCount);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
        for (size_t i = 0; i < inputCount; ++i)
        {
            auto copyKeyResult = this->CopyTo(input[i].key);
            RETURN_IF_FAILED(copyKeyResult.hr);
            outputPtr[i].key = copyKeyResult.ExtractPayload();
            outputPtr[i].value = input[i].value;
        }
        return std::move(outputPtr);
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

template<typename InternalModelWrapperT>
Result<typename InternalModelWrapperT::DictionaryEntryType const*> ModelBuffer::CopyToDictionary(const typename InternalModelWrapperT::DictionaryEntryType* input, size_t inputCount)
{
    if (input && inputCount)
    {
        // Alloc
        auto allocResult = Alloc<typename InternalModelWrapperT::DictionaryEntryType>(inputCount);
        RETURN_IF_FAILED(allocResult.hr);
        // Copy
        auto outputPtr = allocResult.ExtractPayload();
        for (size_t i = 0; i < inputCount; ++i)
        {
            auto copyKeyResult = this->CopyTo(input[i].key);
            RETURN_IF_FAILED(copyKeyResult.hr);
            outputPtr[i].key = copyKeyResult.ExtractPayload();

            auto copyValueResult = this->CopyTo<InternalModelWrapperT>(input[i].value);
            RETURN_IF_FAILED(copyValueResult.hr);
            outputPtr[i].value = copyValueResult.ExtractPayload();
        }
        return std::move(outputPtr);
    }
    else
    {
        // Nothing to copy
        return nullptr;
    }
}

}
