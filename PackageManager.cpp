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

#include "PackageManager.h"

namespace Thunder {
namespace Plugin {

    namespace {

        static Metadata<PackageManager> metadata(
            // Version
            1, 1, 0,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            { subsystem::INSTALLATION }
        );
    }

    const string PackageManager::Initialize(PluginHost::IShell* service)
    {
        string message;

        ASSERT(service != nullptr);
        ASSERT(_service == nullptr);
        ASSERT(_connectionId == 0);
        ASSERT(_packageManager == nullptr);
        ASSERT(_pluginimpl == nullptr);

        _service = service;
        _service->AddRef();

        _service->Register(&_notification);

        _packageManager = service->Root<Exchange::IPackageManager>(_connectionId, 2000, _T("PackageManagerImplementation"));
        if (_packageManager != nullptr) {
            Exchange::JPackageManager::Register(*this, _packageManager);
            _packageManager->Register(&_notification); 

            _pluginimpl = _packageManager->QueryInterface<PluginHost::IPlugin>();
            if (_pluginimpl != nullptr) {
                _adminLock.Lock();
                _info = _pluginimpl->Information();
                _adminLock.Unlock();
                message = _pluginimpl->Initialize(service);
            }

        } else {
            message = _T("PackageManager could not be instantiated. Could not acquire PackageManager interface");
        }

        return message;
    }

    void PackageManager::Deinitialize(PluginHost::IShell* service)
    {
        if (_service != nullptr) {

            ASSERT(service == _service);

            _service->Unregister(&_notification);

            if (_packageManager != nullptr) {
                _packageManager->Unregister(&_notification); 
                Exchange::JPackageManager::Unregister(*this);

                if (_pluginimpl != nullptr) {
                    _pluginimpl->Deinitialize(service);
                    _pluginimpl->Release();
                    _pluginimpl = nullptr;
                }

                // Stop processing:
                RPC::IRemoteConnection* connection = service->RemoteConnection(_connectionId);
                VARIABLE_IS_NOT_USED uint32_t result = _packageManager->Release();
                _packageManager = nullptr;

                // It should have been the last reference we are releasing, 
                // so it should endup in a DESTRUCTION_SUCCEEDED, if not we
                // are leaking...
                ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);

                // If this was running in a (container) process...
                if (connection != nullptr) {
                    // Lets trigger the cleanup sequence for
                    // out-of-process code. Which will guard
                    // that unwilling processes, get shot if
                    // not stopped friendly :-)
                    connection->Terminate();
                    connection->Release();
                }
            }

            _connectionId = 0;
            _service->Release();
            _service = nullptr;
        }
    }

    string PackageManager::Information() const
    {
        Core::SafeSyncType<Core::CriticalSection> _guard(_adminLock);
        return _info;
    }

    void PackageManager::Deactivated(RPC::IRemoteConnection* connection)
    {
        // This can potentially be called on a socket thread, so the deactivation (wich in turn kills this object) must be done
        // on a seperate thread. Also make sure this call-stack can be unwound before we are totally destructed.
        if (_connectionId == connection->Id()) {

            ASSERT(_service != nullptr);

            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service, PluginHost::IShell::DEACTIVATED, PluginHost::IShell::FAILURE));
        }
    }

    void PackageManager::OperationStatus(const string& handle, const string& operation, const string& type, const string& id, const string& version, const string& status, const string& details) {
        Exchange::JPackageManager::Event::OperationStatus(*this, handle, operation, type, id, version, status, details);
    }

} // namespace Plugin
} // namespace Thunder
