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

#pragma once

#include "Module.h"
#include <interfaces/json/JPackageManager.h>

namespace Thunder {
namespace Plugin {

    class PackageManager : public PluginHost::IPlugin, public PluginHost::JSONRPC {

    private:
        class Notification : public RPC::IRemoteConnection::INotification, public Exchange::IPackageManager::INotification {
        public:
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;
            Notification(Notification&&) = delete;
            Notification& operator=(Notification&&) = delete;

            explicit Notification(PackageManager* parent)
                : RPC::IRemoteConnection::INotification()
                , Exchange::IPackageManager::INotification()
                , _parent(*parent)
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

            void OperationStatus(const string& handle, const string& operation, const string& type, const string& id,
                                         const string& version, const string& status, const string& details) override {
                _parent.OperationStatus(handle, operation, type, id, version, status, details);
            }


            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
            INTERFACE_ENTRY(Exchange::IPackageManager::INotification)
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
            : PluginHost::IPlugin()
            , PluginHost::JSONRPC()
            , _connectionId(0)
            , _packageManager(nullptr)
            , _notification(this)
            , _service(nullptr)
            , _pluginimpl(nullptr)
            , _info()
            , _adminLock()
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
        void OperationStatus(const string& handle, const string& operation, const string& type, const string& id, const string& version, const string& status, const string& details);

    private:
        uint32_t _connectionId;
        Exchange::IPackageManager* _packageManager;
        Core::Sink<Notification> _notification;
        PluginHost::IShell* _service;
        PluginHost::IPlugin* _pluginimpl;
        string _info;
        mutable Core::CriticalSection _adminLock;    
    };

} // namespace Plugin
} // namespace Thunder
