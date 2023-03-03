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

#include "local_ability_manager_proxy.h"

#include "ipc_types.h"
#include "iremote_object.h"
#include "message_option.h"
#include "message_parcel.h"
#include "nlohmann/json.hpp"
#include "refbase.h"

using namespace std;
using namespace OHOS::HiviewDFX;

namespace OHOS {
bool LocalAbilityManagerProxy::StartAbility(int32_t systemAbilityId, const std::string& eventStr)
{
    if (systemAbilityId <= 0) {
        HiLog::Warn(label_, "StartAbility systemAbilityId invalid.");
        return false;
    }

    if (eventStr.empty()) {
        HiLog::Warn(label_, "StartAbility eventStr invalid.");
        return false;
    }

    sptr<IRemoteObject> iro = Remote();
    if (iro == nullptr) {
        HiLog::Error(label_, "StartAbility Remote return null");
        return false;
    }

    MessageParcel data;
    if (!data.WriteInterfaceToken(LOCAL_ABILITY_MANAGER_INTERFACE_TOKEN)) {
        HiLog::Warn(label_, "StartAbility interface token check failed");
        return false;
    }
    bool ret = data.WriteInt32(systemAbilityId);
    if (!ret) {
        HiLog::Warn(label_, "StartAbility write systemAbilityId failed!");
        return false;
    }
    ret = data.WriteString(eventStr);
    if (!ret) {
        HiLog::Warn(label_, "StartAbility write eventStr failed!");
        return false;
    }
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);
    int32_t status = iro->SendRequest(START_ABILITY_TRANSACTION, data, reply, option);
    if (status != NO_ERROR) {
        HiLog::Error(label_, "StartAbility SendRequest failed, return value : %{public}d", status);
        return false;
    }
    return true;
}

bool LocalAbilityManagerProxy::StopAbility(int32_t systemAbilityId, const std::string& eventStr)
{
    if (systemAbilityId <= 0) {
        HiLog::Warn(label_, "StopAbility systemAbilityId invalid.");
        return false;
    }

    if (eventStr.empty()) {
        HiLog::Warn(label_, "StartAbility eventStr invalid.");
        return false;
    }
    
    sptr<IRemoteObject> iro = Remote();
    if (iro == nullptr) {
        HiLog::Error(label_, "StopAbility Remote return null");
        return false;
    }

    MessageParcel data;
    if (!data.WriteInterfaceToken(LOCAL_ABILITY_MANAGER_INTERFACE_TOKEN)) {
        HiLog::Warn(label_, "StopAbility interface token check failed");
        return false;
    }
    bool ret = data.WriteInt32(systemAbilityId);
    if (!ret) {
        HiLog::Warn(label_, "StopAbility write systemAbilityId failed!");
        return false;
    }
    ret = data.WriteString(eventStr);
    if (!ret) {
        HiLog::Warn(label_, "StopAbility write eventStr failed!");
        return false;
    }
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);
    int32_t status = iro->SendRequest(STOP_ABILITY_TRANSACTION, data, reply, option);
    if (status != NO_ERROR) {
        HiLog::Error(label_, "StopAbility SendRequest failed, return value : %{public}d", status);
        return false;
    }
    return true;
}

bool LocalAbilityManagerProxy::ActiveAbility(int32_t systemAbilityId,
    const std::unordered_map<std::string, std::string>& activeReason)
{
    if (systemAbilityId <= 0) {
        HiLog::Warn(label_, "ActiveAbility systemAbilityId invalid.");
        return false;
    }

    sptr<IRemoteObject> iro = Remote();
    if (iro == nullptr) {
        HiLog::Error(label_, "ActiveAbility Remote return null");
        return false;
    }

    MessageParcel data;
    if (!data.WriteInterfaceToken(LOCAL_ABILITY_MANAGER_INTERFACE_TOKEN)) {
        HiLog::Warn(label_, "ActiveAbility interface token check failed");
        return false;
    }
    if (!data.WriteInt32(systemAbilityId)) {
        HiLog::Warn(label_, "ActiveAbility write systemAbilityId failed!");
        return false;
    }
    nlohmann::json payload;
    for (auto it = activeReason.begin(); it != activeReason.end(); ++it) {
        payload[it->first] = it->second;
    }
    if (!data.WriteString(payload.dump())) {
        HiLog::Warn(label_, "ActiveAbility write activeReason failed!");
        return false;
    }

    MessageParcel reply;
    MessageOption option;
    int32_t status = iro->SendRequest(ACTIVE_ABILITY_TRANSACTION, data, reply, option);
    if (status != NO_ERROR) {
        HiLog::Error(label_, "ActiveAbility SendRequest failed, return value : %{public}d", status);
        return false;
    }
    bool result = false;
    if (!reply.ReadBool(result)) {
        HiLog::Warn(label_, "ActiveAbility read result failed!");
        return false;
    }
    return result;
}

bool LocalAbilityManagerProxy::IdleAbility(int32_t systemAbilityId,
    const std::unordered_map<std::string, std::string>& idleReason, int32_t& delayTime)
{
    if (systemAbilityId <= 0) {
        HiLog::Warn(label_, "IdleAbility systemAbilityId invalid.");
        return false;
    }

    sptr<IRemoteObject> iro = Remote();
    if (iro == nullptr) {
        HiLog::Error(label_, "IdleAbility Remote return null");
        return false;
    }

    MessageParcel data;
    if (!data.WriteInterfaceToken(LOCAL_ABILITY_MANAGER_INTERFACE_TOKEN)) {
        HiLog::Warn(label_, "IdleAbility interface token check failed");
        return false;
    }
    if (!data.WriteInt32(systemAbilityId)) {
        HiLog::Warn(label_, "IdleAbility write systemAbilityId failed!");
        return false;
    }
    nlohmann::json payload;
    for (auto it = idleReason.begin(); it != idleReason.end(); ++it) {
        payload[it->first] = it->second;
    }
    if (!data.WriteString(payload.dump())) {
        HiLog::Warn(label_, "IdleAbility write ildeReason failed!");
        return false;
    }

    MessageParcel reply;
    MessageOption option;
    int32_t status = iro->SendRequest(IDLE_ABILITY_TRANSACTION, data, reply, option);
    if (status != NO_ERROR) {
        HiLog::Error(label_, "IdleAbility SendRequest failed, return value : %{public}d", status);
        return false;
    }
    bool result = false;
    if (!reply.ReadBool(result)) {
        HiLog::Warn(label_, "IdleAbility read result failed!");
        return false;
    }
    if (!reply.ReadInt32(delayTime)) {
        HiLog::Warn(label_, "IdleAbility read delayTime failed!");
        return false;
    }
    return result;
}
}
