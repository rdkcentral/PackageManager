/*
 * Copyright 2023 Comcast Cable Communications Management, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */ 

#include "../Module.h"

#include <interfaces/IPackageManager.h>

namespace Thunder {
namespace Plugin {

class PackageManagerImplementation : public PluginHost::IPlugin, public Exchange::IPackageManager, public Exchange::IPackageManagerBroker, public Exchange::IPackageManagerCallback {

    private:
        class Notification : public PluginHost::IShell::ICOMLink::INotification {
        public:
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;
            Notification(Notification&&) = delete;
            Notification& operator=(Notification&&) = delete;

            explicit Notification(PackageManagerImplementation* parent)
                : PluginHost::IShell::ICOMLink::INotification()
                , _parent(*parent)
            {
                ASSERT(parent != nullptr);
            }
            ~Notification() override = default;

            void Dangling(const Core::IUnknown* source, const uint32_t interfaceId) override {
                _parent.Dangling(source, interfaceId);
            }
            void Revoked(const Core::IUnknown*, const uint32_t) override {

            }

            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(PluginHost::IShell::ICOMLink::INotification)
            END_INTERFACE_MAP

        private:
            PackageManagerImplementation& _parent;
        };

public:
    PackageManagerImplementation(const PackageManagerImplementation&) = delete;
    PackageManagerImplementation& operator=(const PackageManagerImplementation&) = delete;
    PackageManagerImplementation(PackageManagerImplementation&&) = delete;
    PackageManagerImplementation& operator=(PackageManagerImplementation&&) = delete;
    ~PackageManagerImplementation() override = default;

    PackageManagerImplementation()
        : PluginHost::IPlugin()
        , Exchange::IPackageManager()
        , Exchange::IPackageManagerBroker()
        , Exchange::IPackageManagerCallback()
        , _packagemanager(nullptr)
        , _observers()
        , _adminLock()
        , _notification(this)
        , _service(nullptr)
    {
    }

    BEGIN_INTERFACE_MAP(PackageManagerImplementation)
    INTERFACE_ENTRY(PluginHost::IPlugin)
    INTERFACE_ENTRY(Exchange::IPackageManager)
    INTERFACE_ENTRY(Exchange::IPackageManagerBroker)
    INTERFACE_ENTRY(Exchange::IPackageManagerCallback)
    END_INTERFACE_MAP

    // IPlugin implementation
    const string Initialize(PluginHost::IShell* service) override {
        ASSERT(service != nullptr);
        ASSERT(_service == nullptr);

        _service = service;
        _service->AddRef();

        _service->Register(&_notification);

        return string();
    }

    void Deinitialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED) override {
        ASSERT(service == _service);

        _service->Unregister(&_notification);

        _service->Release();
        _service = nullptr;

    }

    string Information() const override {
        return _T("{\"identifier\":\"ExternalService PackageManager service\"}");
    }

    // IPackageManager implementation

    uint32_t Install(const string& type,
        const string& id,
        const string& version,
        const string& url,
        const string& appName,
        const string& category,
        string& handle ) override {
            TRACE(Trace::Information, (_T("PackageManager Install call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->Install(type, id, version, url, appName, category, handle);
            }
            return result;
        }

    uint32_t Uninstall(const string& type,
        const string& id,
        const string& version,
        const string& uninstallType,
        string& handle /* @out */) override {
            TRACE(Trace::Information, (_T("PackageManager Uninstall call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->Uninstall(type, id, version, uninstallType, handle);
            }
            return result;
        }

    uint32_t Download(const string& type,
        const string& id,
        const string& version,
        const string& resKey,
        const string& url,
        string& handle /* @out */) override {
            TRACE(Trace::Information, (_T("PackageManager Download call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->Download(type, id, version, resKey, url, handle);
            }
            return result;
        }

    uint32_t Reset(const string& type,
        const string& id,
        const string& version,
        const string& resetType) override {
            TRACE(Trace::Information, (_T("PackageManager Reset call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->Reset(type, id, version, resetType);
            }
            return result;
        }

    uint32_t GetStorageDetails(const string& type,
        const string& id,
        const string& version,
        IPackageManager::StorageInfo& storageinfo /* @out */) const override {
            TRACE(Trace::Information, (_T("PackageManager GetStorageDetails call")));
            Core::ProxyType<const Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->GetStorageDetails(type, id, version, storageinfo);
            }
            return result;
        }

    uint32_t SetAuxMetadata(const string& type,
        const string& id,
        const string& version,
        const string& key,
        const string& value) override {
            TRACE(Trace::Information, (_T("PackageManager SetAuxMetadata call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->SetAuxMetadata(type, id, version, key, value);
            }
            return result;
        }

    uint32_t ClearAuxMetadata(const string& type,
        const string& id,
        const string& version,
        const string& key) override {
            TRACE(Trace::Information, (_T("PackageManager ClearAuxMetadata call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->ClearAuxMetadata(type, id, version, key);
            }
            return result;
        }

    uint32_t GetMetadata(const string& type,
        const string& id,
        const string& version,
        MetadataPayload& metadata /* @out */,
        IPackageManager::IKeyValueIterator*& resources /* @out */,
        IPackageManager::IKeyValueIterator*& auxMetadata /* @out */) const override {
            TRACE(Trace::Information, (_T("PackageManager GetMetadata call")));
            Core::ProxyType<const Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->GetMetadata(type, id, version, metadata, resources, auxMetadata);
            }
            return result;
        }

    uint32_t Cancel(const string& handle) override {
            TRACE(Trace::Information, (_T("PackageManager Cancel call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->Cancel(handle);
            }
            return result;
    }

    uint32_t GetProgress(const string& handle, uint32_t& progress /* @out */) const override {
            TRACE(Trace::Information, (_T("PackageManager GetProgress call")));
            Core::ProxyType<const Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->GetProgress(handle, progress);
            }
            return result;
    }

    uint32_t Register(IPackageManager::INotification* notification) override {
        _adminLock.Lock();

        ASSERT(std::find(_observers.begin(), _observers.end(), notification) == _observers.end());

        _observers.push_back(notification);

        _adminLock.Unlock();

        notification->AddRef();

        return (Core::ERROR_NONE);        
    }

    uint32_t Unregister(IPackageManager::INotification* notification) override {

        _adminLock.Lock();

        auto index(std::find(_observers.begin(), _observers.end(), notification));

        ASSERT(index != _observers.end());

        if (index != _observers.end()) {
            _observers.erase(index);
            _adminLock.Unlock();
            notification->Release();
        } else {
            _adminLock.Unlock();
        }

        return (Core::ERROR_NONE);        
    }

    uint32_t GetList(
        const string& type,
        const string& id,
        const string& version,
        const string& appName,
        const string& category,
        IPackageKeyIterator*& installedIds /* @out */) const override {
            TRACE(Trace::Information, (_T("PackageManager GetList call")));
            Core::ProxyType<const Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->GetList(type, id, version, appName, category, installedIds);
            }
            return result;
    }

    uint32_t Lock(const string& type,
        const string& id,
        const string& version,
        const string& reason,
        const string& owner,
        string& handle /* @out */) override {
            TRACE(Trace::Information, (_T("PackageManager Lock call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->Lock(type, id, version, reason, owner, handle);
            }
            return result;
    }

    uint32_t Unlock(const string& handle) override {
            TRACE(Trace::Information, (_T("PackageManager Unlock call")));
            Core::ProxyType<Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->Unlock(handle);
            }
            return result;
    }

    uint32_t GetLockInfo(const string& type,
        const string& id,
        const string& version,
        LockInfo& info /* @out */) const override  {
            TRACE(Trace::Information, (_T("PackageManager GetLockInfo call")));
            Core::ProxyType<const Exchange::IPackageManager> manager(PackageManager());
            uint32_t result = Core::ERROR_UNAVAILABLE;
            if(manager.IsValid() == true) {
                result = manager->GetLockInfo(type, id, version, info);
            }
            return result;
    }

    // IPackageManagerBroker implementation

    uint32_t Offer(IPackageManager* packagemanager) override {
        TRACE(Trace::Information, (_T("PackageManager implementation offered [%p]"), packagemanager));

        ASSERT(_packagemanager == nullptr);
        ASSERT(packagemanager != nullptr);
        ASSERT(_service != nullptr);

        _adminLock.Lock();

        if(_packagemanager != nullptr) {
            _packagemanager->Release();
        }
        _packagemanager = packagemanager;
        _packagemanager->AddRef();

        _adminLock.Unlock();

        PluginHost::ISubSystem* subSystem = _service->SubSystems();
        if (subSystem != nullptr) {
            if (subSystem->IsActive(PluginHost::ISubSystem::INSTALLATION) == true) {
                SYSLOG(Logging::Startup, (_T("subsystem INSTALLATION is not defined as External !!")));
            }
            else {
                subSystem->Set(PluginHost::ISubSystem::INSTALLATION, nullptr);
            }
            subSystem->Release();
        }

        return Core::ERROR_NONE;
    }

    uint32_t Revoke(const IPackageManager* packagemanager) override {
        TRACE(Trace::Information, (_T("PackageManager implementation revoked [%p]"), packagemanager));

        ASSERT(_service != nullptr);

        PluginHost::ISubSystem* subSystem = _service->SubSystems();
        if (subSystem != nullptr) {
            if (subSystem->IsActive(PluginHost::ISubSystem::INSTALLATION) == true) {
                subSystem->Set(PluginHost::ISubSystem::NOT_INSTALLATION, nullptr);
                subSystem->Release();
            }
        }

        ASSERT(_packagemanager != nullptr);
        ASSERT(packagemanager != nullptr);

        uint32_t result = Core::ERROR_NONE;

        _adminLock.Lock();

        if(_packagemanager == packagemanager) {
            _packagemanager = nullptr;
            _adminLock.Unlock();
            packagemanager->Release();
        } else {
            _adminLock.Unlock();
            result = Core::ERROR_NOT_EXIST;
        }
       
        return result;
    }

    // IPackageManager Callback implementation

    void OperationStatusUpdate(const string& handle, const string& operation, const string& type, const string& id, const string& version, const string& status, const string& details) override {
        OperationStatusNotification(handle, operation, type, id, version, status, details);
    }

private:
    void OperationStatusNotification(const string& handle, const string& operation, const string& type, const string& id, const string& version, const string& status, const string& details) {
        TRACE(Trace::Information, (_T("PackageManager status notification triggered")));
        _adminLock.Lock();

        for (auto const& index : _observers) {
            index->OperationStatus(handle, operation, type, id, version, status, details);
        }

        _adminLock.Unlock();
    }

    Core::ProxyType<Exchange::IPackageManager> PackageManager() {
        Core::ProxyType<Exchange::IPackageManager> packagemanager;
        _adminLock.Lock();
        if(_packagemanager != nullptr) {
            packagemanager = Core::ProxyType<Exchange::IPackageManager>(*_packagemanager, *_packagemanager);
        }
        _adminLock.Unlock();
        return packagemanager;
    }

    Core::ProxyType<const Exchange::IPackageManager> PackageManager() const {
        Core::ProxyType<const Exchange::IPackageManager> packagemanager;
        _adminLock.Lock();
        if(_packagemanager != nullptr) {
            packagemanager = Core::ProxyType<const Exchange::IPackageManager>(*_packagemanager, *_packagemanager);
        }
        _adminLock.Unlock();
        return packagemanager;
    }

    void Dangling(const Core::IUnknown* source, const uint32_t interfaceId)  {
        _adminLock.Lock();
        if( (interfaceId == Exchange::IPackageManager::ID) && (source == _packagemanager) ) {
            TRACE(Trace::Information, (_T("PackageManager dangling pointer cleanup")));
            _packagemanager->Release();
            _packagemanager = nullptr;
        }
        _adminLock.Unlock();
    }


private:
    Exchange::IPackageManager* _packagemanager;
    std::list<Exchange::IPackageManager::INotification*> _observers;
    mutable Core::CriticalSection _adminLock;    
    Core::Sink<Notification> _notification;
    PluginHost::IShell* _service;
};

SERVICE_REGISTRATION(PackageManagerImplementation, 1, 0);

}
}
