/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include "system_ability_manager_stub.h"

#include <unistd.h>
#include <cinttypes>

#include "accesstoken_kit.h"
#include "datetime_ex.h"
#include "errors.h"
#include "hitrace_meter.h"
#include "ipc_skeleton.h"
#include "ipc_types.h"
#include "memory_guard.h"
#include "sam_log.h"
#include "string_ex.h"
#include "system_ability_manager.h"
#include "system_ability_on_demand_event.h"
#include "tools.h"

#ifdef WITH_SELINUX
#include "service_checker.h"
#endif

namespace {
#ifdef WITH_SELINUX
    using namespace OHOS::HiviewDFX;
    OHOS::HiviewDFX::HiLogLabel label_ = { LOG_CORE, 0xD001800, "SA_SELINUX" };
    std::unique_ptr<ServiceChecker> selinuxChecker_ = std::make_unique<ServiceChecker>(false);
#endif

    bool CheckGetSAPermission(const int32_t said)
    {
#ifdef WITH_SELINUX
        int64_t begin = OHOS::GetTickCount();
        auto callingPid = OHOS::IPCSkeleton::GetCallingPid();
        auto ret = selinuxChecker_->GetServiceCheck(callingPid, std::to_string(said)) == 0;
        HiLog::Debug(label_, "[Performance] GetServiceCheck SA : %{public}d spend %{public}" PRId64 " ms",
            said, OHOS::GetTickCount() - begin);
        return  ret;
#else
        return true; // if not support selinux, not check selinux permission
#endif
    }

    bool CheckAddOrRemovePermission(const int32_t said)
    {
#ifdef WITH_SELINUX
        int64_t begin = OHOS::GetTickCount();
        auto callingPid = OHOS::IPCSkeleton::GetCallingPid();
        auto ret = selinuxChecker_->AddServiceCheck(callingPid, std::to_string(said)) == 0;
        HiLog::Debug(label_, "[Performance] AddServiceCheck SA : %{public}d spend %{public}" PRId64 " ms",
            said, OHOS::GetTickCount() - begin);
        return ret;
#else
        return true; // if not support selinux, not check selinux permission
#endif
    }

    bool CheckGetRemoteSAPermission(const int32_t said)
    {
#ifdef WITH_SELINUX
        int64_t begin = OHOS::GetTickCount();
        auto callingPid = OHOS::IPCSkeleton::GetCallingPid();
        auto ret = selinuxChecker_->GetRemoteServiceCheck(callingPid, std::to_string(said)) == 0;
        HiLog::Debug(label_, "[Performance] GetRemoteServiceCheck SA : %{public}d spend %{public}" PRId64 " ms",
            said, OHOS::GetTickCount() - begin);
        return ret;
#else
        return true; // if not support selinux, not check selinux permission
#endif
    }

    bool CheckListSAPermission()
    {
#ifdef WITH_SELINUX
        int64_t begin = OHOS::GetTickCount();
        auto callingPid = OHOS::IPCSkeleton::GetCallingPid();
        auto ret = selinuxChecker_->ListServiceCheck(callingPid) == 0;
        HiLog::Debug(label_, "[Performance] ListServiceCheck spend %{public}" PRId64 " ms",
            OHOS::GetTickCount() - begin);
        return ret;
#else
        return true; // if not support selinux, not check selinux permission
#endif
    }
}

using namespace OHOS::Security;
namespace OHOS {
void SystemAbilityManagerStub::SetAbilityFuncMap()
{
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::GET_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::GetSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::CHECK_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::CheckSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::ADD_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::AddSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::REMOVE_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::RemoveSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::LIST_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::ListSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::SUBSCRIBE_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::SubsSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::CHECK_REMOTE_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::CheckRemtSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::ADD_ONDEMAND_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::AddOndemandSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::CHECK_SYSTEM_ABILITY_IMMEDIATELY_TRANSACTION)] =
        &SystemAbilityManagerStub::CheckSystemAbilityImmeInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::UNSUBSCRIBE_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::UnSubsSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::LOAD_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::LoadSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::LOAD_REMOTE_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::LoadRemoteSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::UNLOAD_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::UnloadSystemAbilityInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::CANCEL_UNLOAD_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::CancelUnloadSystemAbilityInner;
}

void SystemAbilityManagerStub::SetProcessFuncMap()
{
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::ADD_SYSTEM_PROCESS_TRANSACTION)] =
        &SystemAbilityManagerStub::AddSystemProcessInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::GET_SYSTEM_PROCESS_INFO_TRANSACTION)] =
        &SystemAbilityManagerStub::GetSystemProcessInfoInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::GET_RUNNING_SYSTEM_PROCESS_TRANSACTION)] =
        &SystemAbilityManagerStub::GetRunningSystemProcessInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::SUBSCRIBE_SYSTEM_PROCESS_TRANSACTION)] =
        &SystemAbilityManagerStub::SubscribeSystemProcessInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::UNSUBSCRIBE_SYSTEM_PROCESS_TRANSACTION)] =
        &SystemAbilityManagerStub::UnSubscribeSystemProcessInner;
}

SystemAbilityManagerStub::SystemAbilityManagerStub()
{
    SetAbilityFuncMap();
    SetProcessFuncMap();
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::GET_ONDEMAND_REASON_EXTRA_DATA_TRANSACTION)] =
        &SystemAbilityManagerStub::GetOnDemandReasonExtraDataInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::GET_ONDEAMND_POLICY_TRANSACTION)] =
        &SystemAbilityManagerStub::GetOnDemandPolicyInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::UPDATE_ONDEAMND_POLICY_TRANSACTION)] =
        &SystemAbilityManagerStub::UpdateOnDemandPolicyInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::GET_ONDEMAND_SYSTEM_ABILITY_IDS_TRANSACTION)] =
        &SystemAbilityManagerStub::GetOnDemandSystemAbilityIdsInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::SEND_STRATEGY_TRANASACTION)] =
        &SystemAbilityManagerStub::SendStrategyInner;
    memberFuncMap_[static_cast<uint32_t>(SamgrInterfaceCode::UNLOAD_ALL_IDLE_SYSTEM_ABILITY_TRANSACTION)] =
        &SystemAbilityManagerStub::UnloadAllIdleSystemAbilityInner;
}

int32_t SystemAbilityManagerStub::OnRemoteRequest(uint32_t code,
    MessageParcel& data, MessageParcel& reply, MessageOption &option)
{
    HILOGD("SAMStub::OnReceived, code = %{public}u, callerPid = %{public}d",
        code, IPCSkeleton::GetCallingPid());
    Samgr::MemoryGuard cacheGuard;
    if (!EnforceInterceToken(data)) {
        HILOGE("SAMStub::OnReceived, code = %{public}u, check interfaceToken failed", code);
        return ERR_PERMISSION_DENIED;
    }
    auto itFunc = memberFuncMap_.find(code);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            return (this->*memberFunc)(data, reply);
        }
    }
    HILOGW("SAMStub: default case, need check.");
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}

bool SystemAbilityManagerStub::EnforceInterceToken(MessageParcel& data)
{
    std::u16string interfaceToken = data.ReadInterfaceToken();
    return interfaceToken == SAMANAGER_INTERFACE_TOKEN;
}

int32_t SystemAbilityManagerStub::ListSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("ListSystemAbilityInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }

    if (!CheckListSAPermission()) {
        HILOGE("ListSystemAbilityInner selinux permission denied!");
        return ERR_PERMISSION_DENIED;
    }

    int32_t dumpFlag = 0;
    bool ret = data.ReadInt32(dumpFlag);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::ListSystemAbilityInner read dumpflag failed!");
        return ERR_FLATTEN_OBJECT;
    }

    std::vector<std::u16string> saNameVector = ListSystemAbilities(dumpFlag);
    if (saNameVector.empty()) {
        HILOGI("List System Abilities list errors");
        ret = reply.WriteInt32(ERR_INVALID_VALUE);
    } else {
        HILOGI("SystemAbilityManagerStub::ListSystemAbilityInner list success");
        ret = reply.WriteInt32(ERR_NONE);
        if (ret) {
            ret = reply.WriteString16Vector(saNameVector);
        }
    }

    if (!ret) {
        HILOGW("SystemAbilityManagerStub::ListSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }

    return ERR_NONE;
}

int32_t SystemAbilityManagerStub::SubsSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::SubsSystemAbilityInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }
    sptr<IRemoteObject> remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        HILOGW("SystemAbilityManagerStub::SubsSystemAbilityInner read listener failed!");
        return ERR_NULL_OBJECT;
    }
    sptr<ISystemAbilityStatusChange> listener = iface_cast<ISystemAbilityStatusChange>(remoteObject);
    if (listener == nullptr) {
        HILOGW("SystemAbilityManagerStub::SubsSystemAbilityInner iface_cast failed!");
        return ERR_NULL_OBJECT;
    }
    int32_t result = SubscribeSystemAbility(systemAbilityId, listener);
    HILOGD("SystemAbilityManagerStub::SubsSystemAbilityInner result is %d", result);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::SubsSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }

    return result;
}

int32_t SystemAbilityManagerStub::UnSubsSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::UnSubsSystemAbilityInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }
    sptr<IRemoteObject> remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        HILOGW("SystemAbilityManagerStub::UnSubscribeSystemAbility read listener failed!");
        return ERR_NULL_OBJECT;
    }
    sptr<ISystemAbilityStatusChange> listener = iface_cast<ISystemAbilityStatusChange>(remoteObject);
    if (listener == nullptr) {
        HILOGW("SystemAbilityManagerStub::UnSubscribeSystemAbility iface_cast failed!");
        return ERR_NULL_OBJECT;
    }
    int32_t result = UnSubscribeSystemAbility(systemAbilityId, listener);
    HILOGD("SystemAbilityManagerStub::UnSubscribeSystemAbility result is %d", result);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::UnSubscribeSystemAbility write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }

    return result;
}

int32_t SystemAbilityManagerStub::CheckRemtSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("CheckRemoteSystemAbilityInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::CheckRemtSystemAbilityInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }

    if (!CheckGetRemoteSAPermission(systemAbilityId)) {
        HILOGE("CheckRemtSystemAbilityInner selinux permission denied!, SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    std::string deviceId;
    ret = data.ReadString(deviceId);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::CheckRemtSystemAbilityInner read deviceId failed!");
        return ERR_FLATTEN_OBJECT;
    }
    std::string uuid = SystemAbilityManager::GetInstance()->TransformDeviceId(deviceId, UUID, false);
    ret = reply.WriteRemoteObject(GetSystemAbility(systemAbilityId, uuid));
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::CheckRemtSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }

    return ERR_NONE;
}

int32_t SystemAbilityManagerStub::AddOndemandSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("AddOndemandSystemAbilityInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::AddOndemandSystemAbilityInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }

    if (!CheckAddOrRemovePermission(systemAbilityId)) {
        HILOGE("AddOndemandSystemAbilityInner selinux permission denied! SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    std::u16string localManagerName = data.ReadString16();
    if (localManagerName.empty()) {
        HILOGW("SystemAbilityManagerStub::AddOndemandSystemAbilityInner read localName failed!");
        return ERR_NULL_OBJECT;
    }

    int32_t result = AddOnDemandSystemAbilityInfo(systemAbilityId, localManagerName);
    HILOGD("SystemAbilityManagerStub::AddOndemandSystemAbilityInner result is %d", result);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::AddOndemandSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }

    return result;
}

int32_t SystemAbilityManagerStub::CheckSystemAbilityImmeInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::CheckSystemAbilityImmeInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }

    if (!CheckGetSAPermission(systemAbilityId)) {
        HILOGE("CheckSystemAbilityImmeInner selinux permission denied! SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    bool isExist = false;
    ret = data.ReadBool(isExist);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::CheckSystemAbilityImmeInner read isExist failed!");
        return ERR_FLATTEN_OBJECT;
    }
    ret = reply.WriteRemoteObject(CheckSystemAbility(systemAbilityId, isExist));
    if (!ret) {
        return ERR_FLATTEN_OBJECT;
    }

    ret = reply.WriteBool(isExist);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::CheckSystemAbilityImmeInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }

    return ERR_NONE;
}

int32_t SystemAbilityManagerStub::UnmarshalingSaExtraProp(MessageParcel& data, SAExtraProp& extraProp)
{
    bool isDistributed = false;
    bool ret = data.ReadBool(isDistributed);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::UnmarshalingSaExtraProp read isDistributed failed!");
        return ERR_FLATTEN_OBJECT;
    }

    int32_t dumpFlags = 0;
    ret = data.ReadInt32(dumpFlags);
    if (!ret || dumpFlags < 0) {
        HILOGW("SystemAbilityManagerStub::UnmarshalingSaExtraProp dumpFlags failed!");
        return ERR_FLATTEN_OBJECT;
    }
    std::u16string capability = data.ReadString16();
    std::u16string permission = data.ReadString16();
    extraProp.isDistributed = isDistributed;
    extraProp.dumpFlags = static_cast<uint32_t>(dumpFlags);
    extraProp.capability = capability;
    extraProp.permission = permission;
    return ERR_OK;
}

int32_t SystemAbilityManagerStub::AddSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("AddSystemAbilityInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::AddSystemAbilityExtraInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }

    if (!CheckAddOrRemovePermission(systemAbilityId)) {
        HILOGE("AddSystemAbilityInner selinux permission denied! SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    auto object = data.ReadRemoteObject();
    if (object == nullptr) {
        HILOGW("SystemAbilityManagerStub::AddSystemAbilityExtraInner readParcelable failed!");
        return ERR_NULL_OBJECT;
    }
    SAExtraProp extraProp;
    int32_t result = UnmarshalingSaExtraProp(data, extraProp);
    if (result != ERR_OK) {
        HILOGW("SystemAbilityManagerStub::AddSystemAbilityExtraInner UnmarshalingSaExtraProp failed!");
        return result;
    }
    result = AddSystemAbility(systemAbilityId, object, extraProp);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::AddSystemAbilityExtraInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}

int32_t SystemAbilityManagerStub::GetSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::GetSystemAbilityInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }

    if (!CheckGetSAPermission(systemAbilityId)) {
        HILOGE("GetSystemAbilityInner selinux permission denied! SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    ret = reply.WriteRemoteObject(GetSystemAbility(systemAbilityId));
    if (!ret) {
        HILOGW("SystemAbilityManagerStub:GetSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return ERR_NONE;
}

int32_t SystemAbilityManagerStub::CheckSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::CheckSystemAbilityInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }

    if (!CheckGetSAPermission(systemAbilityId)) {
        HILOGE("CheckSystemAbilityInner selinux permission denied! SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    ret = reply.WriteRemoteObject(CheckSystemAbility(systemAbilityId));
    if (!ret) {
        return ERR_FLATTEN_OBJECT;
    }
    return ERR_NONE;
}

int32_t SystemAbilityManagerStub::RemoveSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("RemoveSystemAbilityInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::RemoveSystemAbilityInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }

    if (!CheckAddOrRemovePermission(systemAbilityId)) {
        HILOGE("RemoveSystemAbilityInner selinux permission denied!SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    int32_t result = RemoveSystemAbility(systemAbilityId);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::RemoveSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}


int32_t SystemAbilityManagerStub::AddSystemProcessInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("AddSystemProcessInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    std::u16string procName = data.ReadString16();
    if (procName.empty()) {
        HILOGW("SystemAbilityManagerStub::AddSystemProcessInner read process name failed!");
        return ERR_NULL_OBJECT;
    }

    sptr<IRemoteObject> procObject = data.ReadRemoteObject();
    if (procObject == nullptr) {
        HILOGW("SystemAbilityManagerStub::AddSystemProcessInner readParcelable failed!");
        return ERR_NULL_OBJECT;
    }

    int32_t result = AddSystemProcess(procName, procObject);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::AddSystemProcessInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}

int32_t SystemAbilityManagerStub::LoadSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    std::string loadSystemAbilityTag = ToString(systemAbilityId) + "_LoadSystemAbility";
    HITRACE_METER_NAME(HITRACE_TAG_SAMGR, loadSystemAbilityTag);
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::LoadSystemAbilityInner read systemAbilityId failed!");
        return ERR_INVALID_VALUE;
    }

    if (!CheckGetSAPermission(systemAbilityId)) {
        HILOGE("LoadSystemAbilityInner selinux permission denied!SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    sptr<IRemoteObject> remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        HILOGW("SystemAbilityManagerStub::LoadSystemAbilityInner read callback failed!");
        return ERR_INVALID_VALUE;
    }
    sptr<ISystemAbilityLoadCallback> callback = iface_cast<ISystemAbilityLoadCallback>(remoteObject);
    if (callback == nullptr) {
        HILOGW("SystemAbilityManagerStub::LoadSystemAbilityInner iface_cast failed!");
        return ERR_INVALID_VALUE;
    }
    int32_t result = LoadSystemAbility(systemAbilityId, callback);
    HILOGD("SystemAbilityManagerStub::LoadSystemAbilityInner result is %{public}d", result);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::LoadSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}

int32_t SystemAbilityManagerStub::LoadRemoteSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::LoadRemoteSystemAbilityInner systemAbilityId invalid");
        return ERR_INVALID_VALUE;
    }

    if (!CheckGetRemoteSAPermission(systemAbilityId)) {
        HILOGE("LoadRemoteSystemAbilityInner selinux permission denied!SA : %{public}d", systemAbilityId);
        return ERR_PERMISSION_DENIED;
    }

    std::string deviceId = data.ReadString();
    if (deviceId.empty()) {
        HILOGW("SystemAbilityManagerStub::LoadRemoteSystemAbilityInner read deviceId failed");
        return ERR_INVALID_VALUE;
    }
    sptr<IRemoteObject> remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        HILOGW("SystemAbilityManagerStub::LoadRemoteSystemAbilityInner read callback failed!");
        return ERR_INVALID_VALUE;
    }
    sptr<ISystemAbilityLoadCallback> callback = iface_cast<ISystemAbilityLoadCallback>(remoteObject);
    if (callback == nullptr) {
        HILOGW("SystemAbilityManagerStub::LoadRemoteSystemAbilityInner iface_cast failed!");
        return ERR_INVALID_VALUE;
    }
    int32_t result = LoadSystemAbility(systemAbilityId, deviceId, callback);
    HILOGD("SystemAbilityManagerStub::LoadRemoteSystemAbilityInner result is %{public}d", result);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::LoadRemoteSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}

int32_t SystemAbilityManagerStub::UnloadSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::UnloadSystemAbilityInner systemAbilityId invalid");
        return ERR_INVALID_VALUE;
    }
    int32_t result = UnloadSystemAbility(systemAbilityId);
    HILOGD("SystemAbilityManagerStub::UnloadSystemAbilityInner result is %{public}d", result);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::UnloadSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}

int32_t SystemAbilityManagerStub::UnloadAllIdleSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t result = UnloadAllIdleSystemAbility();
    HILOGI("SystemAbilityManagerStub::UnloadAllIdleSystemAbilityInner result is %{public}d", result);
    return result;
}

int32_t SystemAbilityManagerStub::GetSystemProcessInfoInner(MessageParcel& data, MessageParcel& reply)
{
    HILOGI("GetSystemProcessInfoInner called");
    if (!CanRequest()) {
        HILOGE("GetSystemProcessInfoInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_NULL_OBJECT;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::GetSystemProcessInfoInner read systemAbilityId failed!");
        return ERR_NULL_OBJECT;
    }
    SystemProcessInfo processInfo;
    int32_t result = GetSystemProcessInfo(systemAbilityId, processInfo);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("GetSystemProcessInfoInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    if (result != ERR_OK) {
        return ERR_OK;
    }

    ret = reply.WriteString(processInfo.processName);
    if (!ret) {
        HILOGW("GetSystemProcessInfoInner write processName failed.");
        return ERR_FLATTEN_OBJECT;
    }
    ret = reply.WriteInt32(processInfo.pid);
    if (!ret) {
        HILOGW("GetSystemProcessInfoInner write pid failed.");
        return ERR_FLATTEN_OBJECT;
    }
    ret = reply.WriteInt32(processInfo.uid);
    if (!ret) {
        HILOGW("GetSystemProcessInfoInner write uid failed.");
            return ERR_FLATTEN_OBJECT;
    }
    return ERR_OK;
}

int32_t SystemAbilityManagerStub::GetRunningSystemProcessInner(MessageParcel& data, MessageParcel& reply)
{
    HILOGI("GetRunningSystemProcessInner called");
    if (!CanRequest()) {
        HILOGE("GetRunningSystemProcessInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    std::list<SystemProcessInfo> systemProcessInfos;
    int32_t result = GetRunningSystemProcess(systemProcessInfos);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("GetRunningSystemProcessInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    if (result != ERR_OK) {
        return ERR_OK;
    }

    size_t size = systemProcessInfos.size();
    ret = reply.WriteInt32(size);
    if (!ret) {
        HILOGW("GetRunningSystemProcessInner write systemProcessInfos size failed.");
        return ERR_FLATTEN_OBJECT;
    }
    for (auto& systemProcessInfo : systemProcessInfos) {
        ret = reply.WriteString(systemProcessInfo.processName);
        if (!ret) {
            HILOGW("GetRunningSystemProcessInner write processName failed.");
            return ERR_FLATTEN_OBJECT;
        }
        ret = reply.WriteInt32(systemProcessInfo.pid);
        if (!ret) {
            HILOGW("GetRunningSystemProcessInner write pid failed.");
            return ERR_FLATTEN_OBJECT;
        }
        ret = reply.WriteInt32(systemProcessInfo.uid);
        if (!ret) {
            HILOGW("GetRunningSystemProcessInner write uid failed.");
            return ERR_FLATTEN_OBJECT;
        }
    }
    return ERR_OK;
}

int32_t SystemAbilityManagerStub::SubscribeSystemProcessInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("SubscribeSystemProcessInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    sptr<IRemoteObject> remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        HILOGW("SubscribeSystemProcessInner read listener failed!");
        return ERR_NULL_OBJECT;
    }
    sptr<ISystemProcessStatusChange> listener = iface_cast<ISystemProcessStatusChange>(remoteObject);
    if (listener == nullptr) {
        HILOGW("SubscribeSystemProcessInner iface_cast failed!");
        return ERR_NULL_OBJECT;
    }
    int32_t result = SubscribeSystemProcess(listener);
    HILOGD("SubscribeSystemProcess result is %{public}d", result);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SubscribeSystemProcessInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}

int32_t SystemAbilityManagerStub::UnSubscribeSystemProcessInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("UnSubscribeSystemProcessInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    sptr<IRemoteObject> remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        HILOGW("UnSubscribeSystemProcessInner read listener failed!");
        return ERR_NULL_OBJECT;
    }
    sptr<ISystemProcessStatusChange> listener = iface_cast<ISystemProcessStatusChange>(remoteObject);
    if (listener == nullptr) {
        HILOGW("UnSubscribeSystemProcessInner iface_cast failed!");
        return ERR_NULL_OBJECT;
    }
    int32_t result = UnSubscribeSystemProcess(listener);
    HILOGD("UnSubscribeSystemProcessInner result is %{public}d", result);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("UnSubscribeSystemProcessInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}

int32_t SystemAbilityManagerStub::CancelUnloadSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = -1;
    bool ret = data.ReadInt32(systemAbilityId);
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGW("SystemAbilityManagerStub::CancelUnloadSystemAbilityInner systemAbilityId invalid");
        return ERR_INVALID_VALUE;
    }
    int32_t result = CancelUnloadSystemAbility(systemAbilityId);
    HILOGD("SystemAbilityManagerStub::CancelUnloadSystemAbilityInner result is %{public}d", result);
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGW("SystemAbilityManagerStub::CancelUnloadSystemAbilityInner write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return result;
}

int32_t SystemAbilityManagerStub::GetOnDemandReasonExtraDataInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("GetOnDemandReasonExtraData PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int64_t extraDataId = -1;
    if (!data.ReadInt64(extraDataId)) {
        HILOGW("SystemAbilityManagerStub::GetOnDemandReasonExtraData read extraDataId failed.");
        return ERR_FLATTEN_OBJECT;
    }
    MessageParcel extraDataParcel;
    int32_t result = GetOnDemandReasonExtraData(extraDataId, extraDataParcel);
    HILOGD("SystemAbilityManagerStub::GetOnDemandReasonExtraData result is %{public}d", result);
    if (!reply.WriteInt32(result)) {
        HILOGW("SystemAbilityManagerStub::GetOnDemandReasonExtraData write reply failed.");
        return ERR_FLATTEN_OBJECT;
    }
    sptr<OnDemandReasonExtraData> extraData;
    extraData = extraDataParcel.ReadParcelable<OnDemandReasonExtraData>();
    if (extraData == nullptr) {
        HILOGW("SystemAbilityManagerStub::GetOnDemandReasonExtraData read extraData failed.");
        return ERR_FLATTEN_OBJECT;
    }
    if (!reply.WriteParcelable(extraData)) {
        HILOGW("SystemAbilityManagerStub::GetOnDemandReasonExtraData write extraData failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return ERR_OK;
}

int32_t SystemAbilityManagerStub::GetOnDemandPolicyInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("GetOnDemandPolicyInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int32_t systemAbilityId = -1;
    if (!data.ReadInt32(systemAbilityId)) {
        HILOGW("GetOnDemandPolicyInner read saId failed.");
        return ERR_FLATTEN_OBJECT;
    }
    int32_t type = 0;
    if (!data.ReadInt32(type)) {
        HILOGW("GetOnDemandPolicyInner read type failed.");
        return ERR_FLATTEN_OBJECT;
    }
    OnDemandPolicyType typeEnum = static_cast<OnDemandPolicyType>(type);
    std::vector<SystemAbilityOnDemandEvent> abilityOnDemandEvents;
    int32_t result = GetOnDemandPolicy(systemAbilityId, typeEnum, abilityOnDemandEvents);
    if (!reply.WriteInt32(result)) {
        HILOGW("GetOnDemandPolicyInner write result failed.");
        return ERR_FLATTEN_OBJECT;
    }
    if (!OnDemandEventToParcel::WriteOnDemandEventsToParcel(abilityOnDemandEvents, reply)) {
        HILOGW("GetOnDemandPolicyInner write on demand event failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return ERR_OK;
}

int32_t SystemAbilityManagerStub::UpdateOnDemandPolicyInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("UpdateOnDemandPolicyInner PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int32_t systemAbilityId = -1;
    if (!data.ReadInt32(systemAbilityId)) {
        HILOGW("UpdateOnDemandPolicyInner read saId failed.");
        return ERR_FLATTEN_OBJECT;
    }
    int32_t type = 0;
    if (!data.ReadInt32(type)) {
        HILOGW("GetOnDemandPolicyInner read type failed.");
        return ERR_FLATTEN_OBJECT;
    }
    OnDemandPolicyType typeEnum = static_cast<OnDemandPolicyType>(type);
    std::vector<SystemAbilityOnDemandEvent> abilityOnDemandEvents;
    if (!OnDemandEventToParcel::ReadOnDemandEventsFromParcel(abilityOnDemandEvents, data)) {
        HILOGW("UpdateOnDemandPolicyInner read on demand event failed.");
        return ERR_FLATTEN_OBJECT;
    }
    int32_t result = UpdateOnDemandPolicy(systemAbilityId, typeEnum, abilityOnDemandEvents);
    if (!reply.WriteInt32(result)) {
        HILOGW("UpdateOnDemandPolicyInner write result failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return ERR_OK;
}

int32_t SystemAbilityManagerStub::SendStrategyInner(MessageParcel& data, MessageParcel& reply)
{
    if (!CanRequest()) {
        HILOGE("SendStrategy PERMISSION DENIED!");
        return ERR_PERMISSION_DENIED;
    }
    int32_t type = 0;
    if (!data.ReadInt32(type)) {
        HILOGW("SendStrategy read type failed.");
        return ERR_FLATTEN_OBJECT;
    }
    std::vector<int32_t> systemAbilityIds;
    if (!data.ReadInt32Vector(&systemAbilityIds)) {
        HILOGW("SendStrategy read saId failed.");
        return ERR_FLATTEN_OBJECT;
    }
    int32_t level = -1;
    if (!data.ReadInt32(level)) {
        HILOGW("SendStrategy read level failed.");
        return ERR_FLATTEN_OBJECT;
    }
    std::string action;
    if (!data.ReadString(action)) {
        HILOGW("SendStrategy read action failed!");
        return ERR_FLATTEN_OBJECT;
    }
    int32_t result = SendStrategy(type, systemAbilityIds, level, action);
    if (!reply.WriteInt32(result)) {
        HILOGW("SendStrategy write result failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return ERR_OK;
}

int32_t SystemAbilityManagerStub::GetOnDemandSystemAbilityIdsInner(MessageParcel& data, MessageParcel& reply)
{
    std::vector<int32_t> systemAbilityIds;
    int32_t result = GetOnDemandSystemAbilityIds(systemAbilityIds);
    if (!reply.WriteInt32(result)) {
        HILOGE("GetOnDemandSystemAbilityIdsInner write result failed.");
        return ERR_FLATTEN_OBJECT;
    }
    if (!reply.WriteInt32Vector(systemAbilityIds)) {
        HILOGE("GetOnDemandSystemAbilityIdsInner write result failed.");
        return ERR_FLATTEN_OBJECT;
    }
    return ERR_OK;
}

bool SystemAbilityManagerStub::CanRequest()
{
    auto accessTokenId = IPCSkeleton::GetCallingTokenID();
    AccessToken::ATokenTypeEnum tokenType = AccessToken::AccessTokenKit::GetTokenTypeFlag(accessTokenId);
    HILOGD("SystemAbilityManagerStub::CanRequest tokenId:%{private}u, tokenType:%{public}d",
        accessTokenId, tokenType);
    return (tokenType == AccessToken::ATokenTypeEnum::TOKEN_NATIVE);
}
} // namespace OHOS
