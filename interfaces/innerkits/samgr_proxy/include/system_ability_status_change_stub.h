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

#ifndef SAMGR_INTERFACES_INNERKITS_SAMGR_PROXY_INCLUDE_SYSTEM_ABILITY_STATUS_CHANGE_STUB_H
#define SAMGR_INTERFACES_INNERKITS_SAMGR_PROXY_INCLUDE_SYSTEM_ABILITY_STATUS_CHANGE_STUB_H

#include <map>
#include "iremote_stub.h"
#include "isystem_ability_status_change.h"

namespace OHOS {
class SystemAbilityStatusChangeStub : public IRemoteStub<ISystemAbilityStatusChange> {
public:
    SystemAbilityStatusChangeStub();
    ~SystemAbilityStatusChangeStub() = default;
    int32_t OnRemoteRequest(uint32_t code, MessageParcel& data, MessageParcel& reply, MessageOption& option) override;

protected:
    static bool CheckInputSysAbilityId(int32_t systemAbilityId);

private:
    static int32_t LocalOnAddSystemAbility(SystemAbilityStatusChangeStub* stub, MessageParcel& data,
        MessageParcel& reply)
    {
        return stub->OnAddSystemAbilityInner(data, reply);
    }
    static int32_t LocalOnRemoveSystemAbility(SystemAbilityStatusChangeStub* stub, MessageParcel& data,
        MessageParcel& reply)
    {
        return stub->OnRemoveSystemAbilityInner(data, reply);
    }
    int32_t OnAddSystemAbilityInner(MessageParcel& data, MessageParcel& reply);
    int32_t OnRemoveSystemAbilityInner(MessageParcel& data, MessageParcel& reply);
    static bool CanRequest();
    static bool EnforceInterceToken(MessageParcel& data);

    using SystemAbilityStatusChangeStubFunc =
        int32_t (*)(SystemAbilityStatusChangeStub* stub, MessageParcel& data, MessageParcel& reply);
    std::map<uint32_t, SystemAbilityStatusChangeStubFunc> memberFuncMap_;
};
}
#endif /* SAMGR_INTERFACES_INNERKITS_SAMGR_PROXY_INCLUDE_SYSTEM_ABILITY_STATUS_CHANGE_STUB_H */
