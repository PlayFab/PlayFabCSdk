#pragma once

namespace PlayFab
{
namespace Test
{

class Exception : public std::exception
{
public:
    Exception(String message) : m_message{ std::move(message) }
    {
    }

    const char* what() const noexcept
    {
        return m_message.data();
    }
private:
    String m_message;
};

}
}