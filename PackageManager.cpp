/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
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
 */

#include "PackageManager.h"

namespace WPEFramework {
namespace Plugin {

    namespace {

        static Metadata<PackageManager> metadata(
            // Version
            1, 0, 0,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            {}
        );
    }

    const string PackageManager::Initialize(PluginHost::IShell* service)
    {
        string message;

        ASSERT(service != nullptr);
        ASSERT(_service == nullptr);
        ASSERT(_connectionId == 0);
        ASSERT(_packageManager == nullptr);

        _service = service;
        _service->AddRef();

        _service->Register(&_notification);

        _packageManager = service->Root<Exchange::IPackageManager>(_connectionId, 2000, _T("PackageManagerImplementation"));
        if (_packageManager != nullptr) {
            Exchange::JPackageManager::Register(*this, _packageManager);
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
                Exchange::JPackageManager::Unregister(*this);

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
        // No additional info to report.
        return (string());
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

} // namespace Plugin
} // namespace WPEFramework
