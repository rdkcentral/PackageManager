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

// Since lower levels 
// #define _TRACE_LEVEL 5

#define MODULE_NAME PackageManagerClient

#include <iostream>
#include <core/core.h>
#include <com/com.h>
#include <plugins/Types.h>
#include <interfaces/IPackageManager.h>
#include <iostream>

MODULE_NAME_DECLARATION(BUILD_REFERENCE);

using namespace Thunder;

class PackageManagerImplementation : public Thunder::Exchange::IPackageManager {
public:
    ~PackageManagerImplementation() override = default;

    BEGIN_INTERFACE_MAP(PackageManagerImplementation)
        INTERFACE_ENTRY(Thunder::Exchange::IPackageManager)
    END_INTERFACE_MAP

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
        printf("PackageManager cancel called: %s\n", handle.c_str());
        return Core::ERROR_NONE;
    }

    uint32_t GetProgress(const string& handle, uint32_t& progress /* @out */) const override {
        return Core::ERROR_NONE;
    }

    uint32_t Register(IPackageManager::INotification* notification) override {
        return Core::ERROR_BAD_REQUEST;
    }

    uint32_t Unregister(IPackageManager::INotification* notification) override {
        return Core::ERROR_BAD_REQUEST;
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

};



class PackageManagerClient : public Thunder::RPC::SmartInterfaceType<Thunder::Exchange::IPackageManager> {
private:
    using BaseClass = Thunder::RPC::SmartInterfaceType<Thunder::Exchange::IPackageManager>;
public:
    PackageManagerClient(const uint32_t waitTime, const string& callsign)
        : BaseClass()
        , _adminLock()
        , _broker(nullptr)
        , _callback(nullptr)
        , _packagemanagerimplementation(Core::Service<PackageManagerImplementation>::Create<Thunder::Exchange::IPackageManager>()) {
        uint32_t result = BaseClass::Open(waitTime, BaseClass::Connector(), callsign);
        printf("Opening PackageManager: %u\n", result);
    }

    ~PackageManagerClient() override {
        _adminLock.Lock();
            if(_broker != nullptr) {
                _broker->Revoke(_packagemanagerimplementation);
            }
            _broker->Release();
            _broker = nullptr;
            if(_callback != nullptr) {
                _callback->Release();
                _callback = nullptr;
            }
        _adminLock.Unlock();
        BaseClass::Close(Thunder::Core::infinite);
        _packagemanagerimplementation->Release();
    }

    void TriggerOperationStatusUpdate() {
        Core::ProxyType<Exchange::IPackageManagerCallback> callback;

        _adminLock.Lock();
        if(_callback != nullptr) {
            callback = Core::ProxyType<Exchange::IPackageManagerCallback>(*_callback,*_callback);
        }
        _adminLock.Unlock();

        if(callback.IsValid() == true) {
            callback->OperationStatusUpdate("test", "test", "test", "test", "test", "test", "test");
        }

        printf("Packman operational status update triggered!\n");
    }

private:
    void Operational(const bool upAndRunning) override {
        printf("Operational state of PackageManager: %s\n", upAndRunning ? _T("true") : _T("false"));

        if(upAndRunning == true) {
            Exchange::IPackageManager* packman = BaseClass::Interface();
            if(packman != nullptr) {
                ASSERT(_broker == nullptr);
                ASSERT(_callback == nullptr);
                _adminLock.Lock();
                _broker = packman->QueryInterface<Exchange::IPackageManagerBroker>();
                if(_broker != nullptr) {
                    printf("Got broker!\n");
                    // we still need lock here as it could be in the destructor
                    _broker->Offer(_packagemanagerimplementation);
                    _callback = packman->QueryInterface<Exchange::IPackageManagerCallback>();
                    if(_callback != nullptr) {
                        _adminLock.Unlock();
                    } else {
                        _adminLock.Unlock();
                        printf("Could not get callback!\n");
                    }
                } else {
                    _adminLock.Unlock();
                    printf("Could not get broker!\n");
                }
                packman->Release();
            } else {
                printf("Could not get packagemanager!\n");
            }
        } else {
            // _broker and _callback could already be nullptr when triggered by the channel close in the destructor
            _adminLock.Lock();
            if(_broker != nullptr) {
                _broker->Release();
                _broker = nullptr;
            }
            if(_callback != nullptr) {
                _callback->Release();
                _callback = nullptr;
            }
            _adminLock.Unlock();
        }
    }

private:
    mutable Core::CriticalSection _adminLock;    
    Exchange::IPackageManagerBroker* _broker;
    Exchange::IPackageManagerCallback* _callback;
    Thunder::Exchange::IPackageManager* _packagemanagerimplementation;
};

int main(int, char*)
{
    {
        PackageManagerClient  packmanclient(3000, _T("org.rdk.PackageManager"));
        char keyPress;

        // chip.PCD_Init();
        do {
            keyPress = toupper(getchar());
            
            switch (keyPress) {
            case 'O': {
                printf("Operations state issue: %s\n", packmanclient.IsOperational() ? _T("true") : _T("false"));
                break;
            }
            case 'U': {
                packmanclient.TriggerOperationStatusUpdate();
                break;
            }
            case 'Q': break;
            default: break;
            };
        } while (keyPress != 'Q');
    }

    Thunder::Core::Singleton::Dispose();

    return 0;
}
