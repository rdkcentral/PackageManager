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

#pragma once

#include "Module.h"
#include <interfaces/json/JPackageManager.h>

namespace WPEFramework {
namespace Plugin {

    class PackageManager : public PluginHost::IPlugin, public PluginHost::JSONRPC {

    private:
        class Notification : public RPC::IRemoteConnection::INotification {
        public:
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;
            Notification(Notification&&) = delete;
            Notification& operator=(Notification&&) = delete;

            explicit Notification(PackageManager* parent)
                : _parent(*parent)
            {
                ASSERT(parent != nullptr);
            }
            ~Notification() override = default;

            void Activated(RPC::IRemoteConnection* connection VARIABLE_IS_NOT_USED) override {
            }

            void Deactivated(RPC::IRemoteConnection* connection) override
            {
                _parent.Deactivated(connection);
            }

            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
            END_INTERFACE_MAP

        private:
            PackageManager& _parent;
        };

    public:
        PackageManager(const PackageManager&) = delete;
        PackageManager& operator=(const PackageManager&) = delete;
        PackageManager(PackageManager&&) = delete;
        PackageManager& operator=(PackageManager&&) = delete;

        PackageManager()
            : _connectionId(0)
            , _notification(this)
            , _service(nullptr)
        {
        }

        ~PackageManager() override = default;

        BEGIN_INTERFACE_MAP(PackageManager)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_AGGREGATE(Exchange::IPackageManager, _packageManager)
        INTERFACE_ENTRY(PluginHost::IDispatcher)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;

    private:

        void Deactivated(RPC::IRemoteConnection* connection);

    private:
        uint32_t _connectionId;
        Exchange::IPackageManager* _packageManager;
        Core::Sink<Notification> _notification;
        PluginHost::IShell* _service;
    };

} // namespace Plugin
} // namespace WPEFramework
