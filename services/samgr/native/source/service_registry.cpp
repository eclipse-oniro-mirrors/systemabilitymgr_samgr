/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "iservice_registry.h"

#include <unistd.h>

#include "errors.h"
#include "hilog/log.h"
#include "ipc_skeleton.h"
#include "ipc_types.h"
#include "iremote_broker.h"
#include "iremote_object.h"
#include "iremote_proxy.h"
#include "message_option.h"
#include "message_parcel.h"
#include "refbase.h"
#include "sam_log.h"
#include "string_ex.h"

namespace OHOS {
using namespace OHOS::HiviewDFX;

namespace {
    constexpr int32_t MASK = 0x100;
    constexpr int32_t INTERFACE_TOKEN = 0;
    constexpr int64_t SLEEP_TIME = 1;
    constexpr int32_t RETRY_TIMES = 10;
}

class ServiceRegistryProxy : public IRemoteProxy <IServiceRegistry> {
public:
    explicit ServiceRegistryProxy(const sptr<IRemoteObject>& object)
        : IRemoteProxy<IServiceRegistry>(object)
    {
    }

    ~ServiceRegistryProxy() = default;

    virtual sptr<IRemoteObject> GetService(const std::u16string& name)
    {
        sptr<IRemoteObject> service = CheckService(name);
        if (service != nullptr) {
            return service;
        }
        int32_t retry = RETRY_TIMES;
        HILOGI("Waiting for service %{public}s...", Str16ToStr8(name).data());
        while (retry--) {
            // Sleep and wait for 1 second;
            sleep(SLEEP_TIME);
            service = CheckService(name);
            if (service != nullptr) {
                HILOGI("%{public}s:found service %{public}s", __func__, Str16ToStr8(name).data());
                return service;
            }
        }
        HILOGE("Service %{public}s didn't start. Returning nullptr", Str16ToStr8(name).data());
        return nullptr;
    }

    virtual sptr<IRemoteObject> CheckService(const std::u16string& name)
    {
        HILOGI("%{public}s called", __func__);
        MessageParcel data;
        if (!data.WriteInt32(MASK)) {
            HILOGE("%{public}s write MASK failed!", __func__);
            return nullptr;
        }
        if (!data.WriteInt32(INTERFACE_TOKEN)) {
            HILOGE("%{public}s write INTERFACE_TOKEN failed!", __func__);
            return nullptr;
        }
        if (!data.WriteString16(IServiceRegistry::GetDescriptor())) {
            HILOGE("%{public}s write Descriptor failed!", __func__);
            return nullptr;
        }
        if (!data.WriteString16(name.data())) {
            HILOGE("%{public}s write name failed!", __func__);
            return nullptr;
        }
        sptr<IRemoteObject> remote = Remote();
        if (remote == nullptr) {
            HILOGE("ServiceRegistryProxy::CheckService remote is nullptr !");
            return nullptr;
        }
        MessageOption option;
        MessageParcel reply;
        int32_t err = remote->SendRequest(CHECK_SERVICE_TRANSACTION, data, reply, option);
        HILOGI("%{public}s  available parcel size:%zd", __func__, reply.GetReadableBytes());
        return (err == ERR_NONE) ? reply.ReadRemoteObject() : nullptr;
    }

    virtual int32_t AddService(const std::u16string& name,
        const sptr<IRemoteObject>& service, bool allowIsolated, int32_t dumpsysPriority)
    {
        HILOGD("%{public}s called", __func__);
        MessageParcel data;
        if (!data.WriteInt32(MASK)) {
            HILOGE("%{public}s write MASK failed!", __func__);
            return ERR_FLATTEN_OBJECT;
        }
        if (!data.WriteInt32(INTERFACE_TOKEN)) {
            HILOGE("%{public}s write INTERFACE_TOKEN failed!", __func__);
            return ERR_FLATTEN_OBJECT;
        }
        if (!data.WriteString16(IServiceRegistry::GetDescriptor())) {
            HILOGE("%{public}s write Descriptor failed!", __func__);
            return ERR_FLATTEN_OBJECT;
        }
        if (!data.WriteString16(name.data())) {
            HILOGE("%{public}s write name failed!", __func__);
            return ERR_FLATTEN_OBJECT;
        }
        if (!data.WriteRemoteObject(service)) {
            HILOGE("%{public}s write RemoteObject failed!", __func__);
            return ERR_FLATTEN_OBJECT;
        }
        if (!data.WriteInt32((allowIsolated ? 1 : 0))) {
            HILOGE("%{public}s write allowIsolated failed!", __func__);
            return ERR_FLATTEN_OBJECT;
        }
        if (!data.WriteInt32(dumpsysPriority)) {
            HILOGE("%{public}s write dumpsysPriority failed!", __func__);
            return ERR_FLATTEN_OBJECT;
        }
        sptr<IRemoteObject> remote = Remote();
        if (remote == nullptr) {
            HILOGE("ServiceRegistryProxy::AddService remote is nullptr !");
            return ERR_INVALID_OPERATION;
        }
        MessageOption option;
        MessageParcel reply;
        int32_t err = remote->SendRequest(ADD_SERVICE_TRANSACTION, data, reply, option);

        HILOGI("%{public}s:add service %{public}s %{public}s, return %d",
            __func__, Str16ToStr8(name).data(), err ? "fail" : "succ", err);

        return err;
    }
private:
    static constexpr HiLogLabel LABEL = { LOG_CORE, 0xD001800, "ServiceRegistry" };
};

std::mutex ServiceRegistry::serviceRegistryLock_;
static inline BrokerDelegator<ServiceRegistryProxy> delegator_;

sptr<IServiceRegistry> ServiceRegistry::GetInstance()
{
    static sptr<IServiceRegistry> registryInstance;
    std::lock_guard<std::mutex> lock(serviceRegistryLock_);
    if (registryInstance == nullptr) {
        sptr<IRemoteObject> registryObject = IPCSkeleton::GetContextObject();
        if (registryObject == nullptr) {
            return nullptr;
        }
        registryInstance = iface_cast<IServiceRegistry>(registryObject);
    }
    return registryInstance;
}

SystemAbilityManagerClient& SystemAbilityManagerClient::GetInstance()
{
    static auto instance = new SystemAbilityManagerClient();
    return *instance;
}

sptr<ISystemAbilityManager> SystemAbilityManagerClient::GetSystemAbilityManager()
{
    std::lock_guard<std::mutex> lock(systemAbilityManagerLock_);
    if (systemAbilityManager_ != nullptr) {
        return systemAbilityManager_;
    }
    sptr<IRemoteObject> registryObject = IPCSkeleton::GetContextObject();
    if (registryObject == nullptr) {
        KHILOGD("samgrClient regObject=null,callPid:%{public}d", IPCSkeleton::GetCallingPid());
        return nullptr;
    }
    systemAbilityManager_ = iface_cast<ISystemAbilityManager>(registryObject);
    if (systemAbilityManager_ == nullptr) {
        KHILOGD("samgrClient is null,callPid:%{public}d", IPCSkeleton::GetCallingPid());
    }
    return systemAbilityManager_;
}

void SystemAbilityManagerClient::DestroySystemAbilityManagerObject()
{
    HILOGD("%{public}s called", __func__);
    std::lock_guard<std::mutex> lock(systemAbilityManagerLock_);
    systemAbilityManager_.clear();
}
} // namespace OHOS
