#pragma once

#include "AsyncOp.h"

namespace PlayFab
{

class LocalStorage
{
public:
    LocalStorage();
    LocalStorage(LocalStorage const&) = default;
    LocalStorage& operator=(LocalStorage const&) = default;
    ~LocalStorage() = default;

    // Storage operations
    AsyncOp<Vector<uint8_t>> Read(String path, RunContext runContext) noexcept;
    AsyncOp<void> Write(String path, Vector<uint8_t>&& data, RunContext runContext) noexcept;
    AsyncOp<void> Clear(String path, RunContext runContext) noexcept;

    // Wait until all pending storage operations complete
    AsyncOp<void> Flush(RunContext runContext) noexcept;

private:
    class State;

    SharedPtr<State> m_state;
};

}
