//-------------------------------------------------------------------------
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//-------------------------------------------------------------------------

namespace GameSave
{

    // simple class to manage the lifetime of the service runtime
    class ServiceProvider
    {
    public:
        ServiceProvider();
        ~ServiceProvider() = default;
        ServiceProvider(ServiceProvider&&) = default;
        ServiceProvider& operator=(ServiceProvider&&) = default;
    
        HRESULT Initialize();
        HRESULT Start();
        HRESULT Stop();
    private:
        ServiceProvider(const ServiceProvider&);
        ServiceProvider &operator=(const ServiceProvider&);
        bool _xblShimConfigured = false;
        HRESULT _initResult = E_PENDING;
        wil::critical_section m_lock;

    };
}