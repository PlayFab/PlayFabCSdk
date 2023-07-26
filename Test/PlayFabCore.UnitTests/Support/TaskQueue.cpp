#include "stdafx.h"
#include "TaskQueue.h"
#include <playfab/core/cpp/PlayFabException.h>

namespace PlayFab
{
namespace Wrappers
{

TaskQueue::TaskQueue(XTaskQueueHandle handle) : m_handle{ handle }
{
}

TaskQueue::TaskQueue(XTaskQueueDispatchMode workDispatchMode, XTaskQueueDispatchMode completionDispatchMode)
{
    THROW_IF_FAILED(XTaskQueueCreate(workDispatchMode, completionDispatchMode, &m_handle));
}

TaskQueue::TaskQueue(const TaskQueue& other)
{
    THROW_IF_FAILED(XTaskQueueDuplicateHandle(other.m_handle, &m_handle));
}

TaskQueue::TaskQueue(TaskQueue&& other) noexcept
{
    std::swap(m_handle, other.m_handle);
}

TaskQueue::~TaskQueue() noexcept
{
    if (m_handle)
    {
        XTaskQueueCloseHandle(m_handle);
    }
}

TaskQueue TaskQueue::DeriveWorkQueue() const
{
    XTaskQueuePortHandle workPort{ nullptr };
    THROW_IF_FAILED(XTaskQueueGetPort(m_handle, XTaskQueuePort::Work, &workPort));

    XTaskQueueHandle derivedHandle{ nullptr };
    THROW_IF_FAILED(XTaskQueueCreateComposite(workPort, workPort, &derivedHandle));

    return TaskQueue{ derivedHandle };
}

XTaskQueueHandle TaskQueue::Handle() const noexcept
{
    return m_handle;
}

}
}
