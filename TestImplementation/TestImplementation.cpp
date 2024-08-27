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
#include <interfaces/IConfiguration.h>

namespace Thunder {
namespace Plugin {

class PackageManagerImplementation : public PluginHost::IPlugin, public Exchange::IPackageManager {

public:
    PackageManagerImplementation(const PackageManagerImplementation&) = delete;
    PackageManagerImplementation& operator=(const PackageManagerImplementation&) = delete;
    PackageManagerImplementation(PackageManagerImplementation&&) = delete;
    PackageManagerImplementation& operator=(PackageManagerImplementation&&) = delete;
    ~PackageManagerImplementation() override = default;

    PackageManagerImplementation()
        : PluginHost::IPlugin()
        , Exchange::IPackageManager()
        , _observers()
        , _adminLock()
    {
    }

    BEGIN_INTERFACE_MAP(PackageManagerImplementation)
    INTERFACE_ENTRY(PluginHost::IPlugin)
    INTERFACE_ENTRY(Exchange::IPackageManager)
    END_INTERFACE_MAP

    // IPlugin implementation
    const string Initialize(PluginHost::IShell* service) override {

        PluginHost::ISubSystem* subSystem = service->SubSystems();
        if (subSystem != nullptr) {
            if (subSystem->IsActive(PluginHost::ISubSystem::INSTALLATION) == true) {
                SYSLOG(Logging::Startup, (_T("subsystem INSTALLATION is not defined as External !!")));
            }
            else {
                subSystem->Set(PluginHost::ISubSystem::INSTALLATION, nullptr);
            }
            subSystem->Release();
        }

        return string();
    }

    void Deinitialize(PluginHost::IShell* service) override {
        PluginHost::ISubSystem* subSystem = service->SubSystems();
        if (subSystem != nullptr) {
            if (subSystem->IsActive(PluginHost::ISubSystem::INSTALLATION) == true) {
                subSystem->Set(PluginHost::ISubSystem::NOT_INSTALLATION, nullptr);
                subSystem->Release();
            }
        }
    }

    string Information() const override {
        return _T("{\"identifier\":\"Test PackageManager service\"}");
    }

PUSH_WARNING(DISABLE_WARNING_UNUSED_PARAMETERS)

    // IPackageManager implementation
    uint32_t Install(const string& type,
        const string& id,
        const string& version,
        const string& url,
        const string& appName,
        const string& category,
        string& handle ) override {
            return Core::ERROR_NONE;
        }

    uint32_t Uninstall(const string& type,
        const string& id,
        const string& version,
        const string& uninstallType,
        string& handle /* @out */) override {
            return Core::ERROR_NONE;
        }

    uint32_t Download(const string& type,
        const string& id,
        const string& version,
        const string& resKey,
        const string& url,
        string& handle /* @out */) override {
            return Core::ERROR_NONE;
        }

    uint32_t Reset(const string& type,
        const string& id,
        const string& version,
        const string& resetType) override {
            return Core::ERROR_NONE;
        }

    uint32_t GetStorageDetails(const string& type,
        const string& id,
        const string& version,
        IPackageManager::StorageInfo& storageinfo /* @out */) const override {
            return Core::ERROR_NONE;
        }

    uint32_t SetAuxMetadata(const string& type,
        const string& id,
        const string& version,
        const string& key,
        const string& value) override {
            return Core::ERROR_NONE;
        }

    uint32_t ClearAuxMetadata(const string& type,
        const string& id,
        const string& version,
        const string& key) override {
            return Core::ERROR_NONE;
        }

    uint32_t GetMetadata(const string& type,
        const string& id,
        const string& version,
        MetadataPayload& metadata /* @out */,
        IPackageManager::IKeyValueIterator*& resources /* @out */,
        IPackageManager::IKeyValueIterator*& auxMetadata /* @out */) const override {
            return Core::ERROR_NONE;
        }

    uint32_t Cancel(const string& handle) override {
        return Core::ERROR_NONE;
    }

    uint32_t GetProgress(const string& handle, uint32_t& progress /* @out */) const override {
        return Core::ERROR_NONE;
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
            return Core::ERROR_NONE;
    }

    uint32_t Lock(const string& type,
        const string& id,
        const string& version,
        const string& reason,
        const string& owner,
        string& handle /* @out */) override {
            return Core::ERROR_NONE;
    }

    uint32_t Unlock(const string& handle) override {
        return Core::ERROR_NONE;
    }

    uint32_t GetLockInfo(const string& type,
            const string& id,
            const string& version,
            LockInfo& result /* @out */) const override  {
        return Core::ERROR_NONE;
    }

POP_WARNING()

private:
    void OperationStatusNotification(const string& handle, const string& operation, const string& type, const string& id, const string& version, const string& status, const string& details) {
        _adminLock.Lock();

        for (auto const& index : _observers) {
            index->OperationStatus(handle, operation, type, id, version, status, details);
        }

        _adminLock.Unlock();
    }

private:
    std::list<Exchange::IPackageManager::INotification*> _observers;
    mutable Core::CriticalSection _adminLock;    
};

SERVICE_REGISTRATION(PackageManagerImplementation, 1, 0);

}
}
