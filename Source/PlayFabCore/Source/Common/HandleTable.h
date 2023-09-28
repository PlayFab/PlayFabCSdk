namespace PlayFab
{

// Simple handle table used to track and map open client handles 
template<typename HandleT, typename ObjectT>
class HandleTable
{
public:
    static_assert(std::is_pointer_v<HandleT>, "HandleT must be a pointer type");

    HandleTable(uintptr_t firstHandleValue);
    HandleTable(HandleTable const&) = delete;
    HandleTable(HandleTable&&) = delete;
    HandleTable& operator=(HandleTable const&) = delete;
    HandleTable& operator=(HandleTable&&) = delete;
    ~HandleTable();

    HRESULT MakeHandle(SharedPtr<ObjectT> object, HandleT& handle);
    HRESULT FromHandle(HandleT handle, SharedPtr<ObjectT>& object) const;
    void CloseHandle(HandleT handle);

private:
    mutable std::mutex m_mutex;
    Map<HandleT, SharedPtr<ObjectT>> m_handles;
    uintptr_t m_nextHandle;
};

//------------------------------------------------------------------------------
// Template implementations
//------------------------------------------------------------------------------

template<typename HandleT, typename ObjectT>
HandleTable<HandleT, ObjectT>::HandleTable(uintptr_t firstHandleValue) :
    m_nextHandle{ firstHandleValue }
{
}

template<typename HandleT, typename ObjectT>
HandleTable<HandleT, ObjectT>::~HandleTable()
{
    if (!m_handles.empty())
    {
        TRACE_WARNING("Warning: Unclosed handles remain during cleanup (%s)", __FUNCTION__);
    }
}

template<typename HandleT, typename ObjectT>
HRESULT HandleTable<HandleT, ObjectT>::MakeHandle(SharedPtr<ObjectT> object, HandleT& handle)
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    RETURN_HR_IF(E_INVALIDARG, !object);

    handle = (HandleT)m_nextHandle++;
    m_handles.emplace(handle, std::move(object));

    return S_OK;
}

template<typename HandleT, typename ObjectT>
HRESULT HandleTable<HandleT, ObjectT>::FromHandle(HandleT handle, SharedPtr<ObjectT>& object) const
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    auto it = m_handles.find(handle);
    if (it == m_handles.end())
    {
        TRACE_ERROR("%s: Attempted to access invalid handle %llu", __FUNCTION__, handle);
        return E_PF_INVALIDHANDLE;
    }

    assert(it->second);
    object = it->second;

    return S_OK;
}

template<typename HandleT, typename ObjectT>
void HandleTable<HandleT, ObjectT>::CloseHandle(HandleT handle)
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    auto it = m_handles.find(handle);
    if (it == m_handles.end())
    {
        TRACE_WARNING("%s: Attempted to close invalid handle %llu", __FUNCTION__, handle);
        return;
    }

    m_handles.erase(it);
}

}
