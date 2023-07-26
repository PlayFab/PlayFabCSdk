#pragma once

#if _HAS_CXX17
// use std::string_view implementation if available
#include <string_view>
#endif

namespace PlayFab
{

template <typename E>
struct EnumRange
{
    // Provide an arbitrary range for enum values by default. If enum has values greater that this default,
    // specialize this class to provide a custom value.
    static constexpr E maxValue = static_cast<E>(100);
};

namespace Detail
{

#if _HAS_CXX17
using StringView = std::string_view;
#else
// Very minimal StringView definition
class StringView
{
public:
    using const_pointer = const char*;
    using size_type = size_t;

    constexpr StringView() 
        : m_data{ nullptr },
        m_size{ 0 }
    {
    }
    constexpr StringView(const_pointer data, size_type size)
        : m_data{ data },
        m_size{ size }
    {
    }
    constexpr StringView(const StringView&) = default;
    constexpr StringView& operator=(const StringView&) = default;

    constexpr const_pointer data() const
    {
        return m_data;
    }

    constexpr size_type length() const
    {
        return m_size;
    }

private:
    const_pointer m_data;
    size_type m_size;
};
#endif

template<size_t n>
class StaticString
{
public:
    constexpr StaticString(const char* name) noexcept
        : StaticString{ name, std::make_index_sequence<n>{} }
    {
    }

    constexpr operator StringView() const noexcept
    {
        return StringView{ chars, n };
    }

private:
    template<std::size_t... I>
    constexpr StaticString(const char* name, std::index_sequence<I...>) noexcept
        : chars{ name[I]..., 0 }
    {
    }

    const char chars[n + 1]{};
};

template<>
class StaticString<0>
{
public:
    constexpr StaticString(const char*) noexcept {};
    constexpr operator StringView() const noexcept
    {
        return StringView{};
    }
};

constexpr StringView ParseName(const char* funcName) noexcept
{
#if defined(_MSC_VER)
    constexpr auto delim{ '>' };
#elif defined(__clang__)
    constexpr auto delim{ ']' };
#endif
    size_t end{ 0 };
    for (; funcName[end] && funcName[end] != delim; ++end) {}

    size_t begin{ end };
    for (; begin > 0; --begin)
    {
        if (!((funcName[begin - 1] >= '0' && funcName[begin - 1] <= '9') ||
            (funcName[begin - 1] >= 'a' && funcName[begin - 1] <= 'z') ||
            (funcName[begin - 1] >= 'A' && funcName[begin - 1] <= 'Z') ||
            (funcName[begin - 1] == '_')))
        {
            break;
        }
    }

    // Symbol names cannot begin with a number
    if (funcName[begin] >= '0' && funcName[begin] <= '9')
    {
        // Invalid enum value
        return StringView{};
    }

    return StringView{ funcName + begin, end - begin };
}

template<typename E, E V>
static constexpr auto N() noexcept
{
    static_assert(std::is_enum<E>::value, "E must be an enum type.");
#if defined(_MSC_VER)
    constexpr auto name = ParseName(__FUNCSIG__);
#elif defined(__clang__)
    constexpr auto name = ParseName(__PRETTY_FUNCTION__);
#else
    static_assert(false, "Unrecognized platform");
#endif
    return StaticString<name.length()>{ name.data() };
}

template<typename E, E V>
constexpr auto EnumValueName = N<E, V>();

template<typename E>
struct EnumTraits
{
private:
    static_assert(std::is_enum_v<E>, "EnumTraits can only be instantiated for enum types.");

    static constexpr size_t maxValue = static_cast<size_t>(EnumRange<E>::maxValue);

    template<std::size_t... I>
    static constexpr auto ValueNames(std::index_sequence<I...>) noexcept
    {
        return std::array<StringView, sizeof...(I)>{ { EnumValueName<E, static_cast<E>(I)>... } };
    }
    static constexpr std::array<StringView, maxValue + 1> valueNames = ValueNames(std::make_index_sequence<maxValue + 1>());

public:
    static const char* Name(E value) noexcept
    {
        auto i{ static_cast<size_t>(value) };
        if (i > maxValue)
        {
            assert(false);
            return nullptr;
        }
        return valueNames[i].data();
    }

    static E Value(const char* name) noexcept
    {
        for (auto i = 0u; i < valueNames.size(); ++i)
        {
            if (strcmp(name, valueNames[i].data()) == 0)
            {
                return static_cast<E>(i);
            }
        }
        assert(false);
        return E{};
    }
};

} // namespace Detail

template<typename E>
constexpr auto EnumName(E value) noexcept -> std::enable_if_t<std::is_enum<E>::value, const char*>
{
    return Detail::EnumTraits<E>::Name(value);
}

template<typename E>
constexpr auto EnumValue(const char* name) noexcept -> std::enable_if_t<std::is_enum<E>::value, E>
{
    return Detail::EnumTraits<E>::Value(name);
}
}
