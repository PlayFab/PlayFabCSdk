#pragma once

#include <XTaskQueue.h>

namespace PlayFab
{
namespace Wrappers
{

class TaskQueue
{
public:
    TaskQueue(XTaskQueueHandle handle); // Takes ownership of handle
    TaskQueue(XTaskQueueDispatchMode workDispatchMode, XTaskQueueDispatchMode completionDispatchMode);
    TaskQueue(const TaskQueue& other);
    TaskQueue(TaskQueue&& other) noexcept;
    TaskQueue& operator=(TaskQueue const& other) noexcept = delete;
    TaskQueue& operator=(TaskQueue&& other) noexcept = delete;
    ~TaskQueue() noexcept;

    TaskQueue DeriveWorkQueue() const;
    XTaskQueueHandle Handle() const noexcept;

private:
    XTaskQueueHandle m_handle{ nullptr };
};

}
}
