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
#include <interfaces/IPackageManager.h>
#include <iostream>

MODULE_NAME_DECLARATION(BUILD_REFERENCE);

namespace Thunder = WPEFramework;

using namespace Thunder;

class PackageManagerImplementation : public Thunder::Exchange::IPackageManager {
public:
    ~PackageManagerImplementation() override = default;

    BEGIN_INTERFACE_MAP(PackageManagerImplementation)
        INTERFACE_ENTRY(Thunder::Exchange::IPackageManager)
    END_INTERFACE_MAP

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

};

class PackageManagerClient  {
private:
    using EngineType = RPC::InvokeServerType<1, 0, 8>;

private:
    class Notification : public PluginHost::IPlugin::INotification {
    public:
        Notification(const Notification&) = delete;
        Notification& operator=(const Notification&) = delete;
        Notification(Notification&&) = delete;
        Notification& operator=(Notification&&) = delete;

        Notification(const string& callsign, PackageManagerClient& client) 
        : _callsign(callsign)
        , _client(client)
        , _registering(false){
            
        }
        ~Notification() override = default;

        void Registering(const bool registering) {
            _registering = registering;
        };

    private:
        void StateChange(PluginHost::IShell* plugin) override
        {
            if(_registering == false) {
                ASSERT(plugin != nullptr);
                if(plugin->Callsign() == _callsign) {
                    printf("StateChange PackageManager\n");
                    if(plugin->State() == PluginHost::IShell::ACTIVATED) {
                        printf("StateChange Activated for PackageManager\n");
                        _client.Operational(true);
                    } else if(plugin->State() == PluginHost::IShell::DEACTIVATED) {
                        printf("StateChange Deactivated for PackageManager\n");
                        _client.Operational(false);
                    }
                }
            }
        }

        BEGIN_INTERFACE_MAP(Catalog)
        INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
        END_INTERFACE_MAP

    private:
        const string _callsign;
        PackageManagerClient& _client;
        std::atomic<bool> _registering;
    };

public:
    PackageManagerClient(const uint32_t waitTime, const string& callsign)
        : _adminLock()
        , _broker(nullptr)
        , _callback(nullptr)
        , _packmanChannel(nullptr)
        , _packagemanagerimplementation(Core::Service<PackageManagerImplementation>::Create<Thunder::Exchange::IPackageManager>())
        , _engine()
        , _comChannel()
        , _systemengine(Core::ProxyType<EngineType>::Create())
        , _systemChannel(Core::ProxyType<RPC::CommunicatorClient>::Create(Connector(),Core::ProxyType<Core::IIPCServer>(_systemengine)))
        , _systeminterface(nullptr)
        , _notification(callsign, *this)
        , _callsign(callsign) {

            ASSERT(_systemengine != nullptr);
            ASSERT(_systemChannel != nullptr);
            _systemengine->Announcements(_systemChannel->Announcement());

            _systeminterface = _systemChannel->Open<PluginHost::IShell>(string());
            if(_systeminterface != nullptr) {
                _notification.Registering(true);
                _systeminterface->Register(&_notification); 
                printf("Registered for Plugin Notifications\n");
                _notification.Registering(false);
                // now let's start channel, we cannot use the initial notification so we assume it is running... (all chances for race conditions here but best we can do)
                Operational(true);
            } else {
                printf("Could not get IShell!\n");
            }
    }

    ~PackageManagerClient() {
        // first unregister for the notifications
        if(_systeminterface != nullptr) {
            _systeminterface->Unregister(&_notification); 
            _systeminterface->Release();
            _systeminterface = nullptr;
        }

        if(_broker != nullptr) {
            _broker->Revoke(_packagemanagerimplementation);
            _broker->Release();
            _broker = nullptr;
        }
        if(_callback != nullptr) {
            _callback->Release();
            _callback = nullptr;
        }
        if(_packmanChannel != nullptr) {
            _packmanChannel->Release();
            _packmanChannel = nullptr;
        }
        _packagemanagerimplementation->Release();
    }

    void TriggerOperationStatusUpdate() {
        Core::ProxyType<Exchange::IPackageManagerCallback> callback;

        _adminLock.Lock();
        if(_callback != nullptr) {
            callback = Core::ProxyType<Exchange::IPackageManagerCallback>(_callback,_callback);
        }
        _adminLock.Unlock();

        if(callback.IsValid() == true) {
            callback->OperationStatusUpdate("test", "test", "test", "test", "test", "test", "test");
        }

        printf("Packman operational status update triggered!\n");
    }

private:
    void Operational(const bool upAndRunning) {
        printf("Operational state of PackageManager: %s\n", upAndRunning ? _T("true") : _T("false"));

        if(upAndRunning == true) {
            _adminLock.Lock();
            printf("Staring Package manager communication channel\n");
            _engine = Core::ProxyType<EngineType>::Create();
            _comChannel = Core::ProxyType<RPC::CommunicatorClient>::Create(Connector(),Core::ProxyType<Core::IIPCServer>(_engine));
            _engine->Announcements(_comChannel->Announcement());

            _packmanChannel = _comChannel->Open<Exchange::IPackageManager>(_callsign);
            if(_packmanChannel != nullptr) {
                printf("Opening PackageManager succeeded\n");
                _broker = _packmanChannel->QueryInterface<Exchange::IPackageManagerBroker>();
                if(_broker != nullptr) {
                    printf("Got broker!\n");
                    _broker->Offer(_packagemanagerimplementation);
                    _callback = _packmanChannel->QueryInterface<Exchange::IPackageManagerCallback>();
                    if(_callback == nullptr) {
                        printf("Could not get callback!\n");
                    }
                } else {
                    printf("Could not get broker!\n");
                }
            } else {
                printf("Opening PackageManager failed\n");
            }
            _adminLock.Unlock();
        } else {
            _adminLock.Lock();
            if(_broker != nullptr) {
                // revocing useless as plugin is not running
                _broker->Release();
                _broker = nullptr;
            }
            if(_callback != nullptr) {
                _callback->Release();
                _callback = nullptr;
            }
            if(_packmanChannel != nullptr) {
                _packmanChannel->Release();
                _packmanChannel = nullptr;
            }

            _comChannel.Release();
            _engine.Release();

            _adminLock.Unlock();
        }
    }

    static Core::NodeId Connector() {
        const TCHAR* comPath = ::getenv(_T("COMMUNICATOR_PATH"));

        if (comPath == nullptr) {
#ifdef __WINDOWS__
            comPath = _T("127.0.0.1:62000");
#else
            comPath = _T("/tmp/communicator");
#endif
        }

        return Core::NodeId(comPath);

    }

private:
    mutable Core::CriticalSection _adminLock;    
    Exchange::IPackageManagerBroker* _broker;
    Exchange::IPackageManagerCallback* _callback;
    Exchange::IPackageManager* _packmanChannel;
    Thunder::Exchange::IPackageManager* _packagemanagerimplementation;
    Core::ProxyType<EngineType> _engine;
    Core::ProxyType<RPC::CommunicatorClient> _comChannel;
    Core::ProxyType<EngineType> _systemengine;
    Core::ProxyType<RPC::CommunicatorClient> _systemChannel;
    PluginHost::IShell* _systeminterface;
    Core::Sink<Notification> _notification;
    const string _callsign;

};

int main(int argc, char* argv[])
{
    {
        PackageManagerClient  packmanclient(3000, _T("PackageManager"));
        char keyPress;

        printf("U to trigger event, Q to quit\n");

        // chip.PCD_Init();
        do {
            keyPress = toupper(getchar());

           
            switch (keyPress) {
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


template void WPEFramework::Core::IPCMessageType<2u, WPEFramework::RPC::Data::Input, WPEFramework::RPC::Data::Output>::RawSerializedType<WPEFramework::RPC::Data::Input, 4u>::AddRef() const;
template void WPEFramework::Core::IPCMessageType<2u, WPEFramework::RPC::Data::Input, WPEFramework::RPC::Data::Output>::RawSerializedType<WPEFramework::RPC::Data::Output, 5u>::AddRef() const;
