/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_SYSTEM_ABILITY_MANAGER_SYSTEM_ABILITY_STATE_CONTEXT_H
#define OHOS_SYSTEM_ABILITY_MANAGER_SYSTEM_ABILITY_STATE_CONTEXT_H

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <shared_mutex>

#include "refbase.h"
#include "sa_profiles.h"
#include "isystem_ability_load_callback.h"
#include "sa_profiles.h"

namespace OHOS {
enum class SystemAbilityState {
    NOT_LOADED = 0,
    LOADING,
    LOADED,
    UNLOADABLE,
    UNLOADING,
};

enum class SystemProcessState {
    NOT_STARTED = 0,
    STARTED,
    STOPPING,
};

enum class PendingEvent {
    NO_EVENT = 0,
    LOAD_ABILITY_EVENT,
    UNLOAD_ABILITY_EVENT,
};

struct LoadRequestInfo {
    int32_t systemAbilityId = -1;
    std::string deviceId;
    sptr<ISystemAbilityLoadCallback> callback;
    int32_t callingPid = -1;
    OnDemandEvent loadEvent;
};

struct UnloadRequestInfo {
    int32_t systemAbilityId = -1;
    OnDemandEvent unloadEvent;
};

struct SystemProcessContext {
    std::u16string processName;
    int32_t pid = -1;
    int32_t uid = -1;
    std::list<int32_t> saList;
    std::recursive_mutex processLock;
    SystemProcessState state = SystemProcessState::NOT_STARTED;
    std::shared_mutex stateCountLock;
    std::map<SystemAbilityState, uint32_t> abilityStateCountMap;
    std::list<int64_t> restartCountsCtrl;
    bool enableRestart = true;
};

struct SystemAbilityContext {
    int32_t systemAbilityId = -1;
    int32_t delayTime = 0;
    std::shared_ptr<SystemProcessContext> ownProcessContext;
    SystemAbilityState state = SystemAbilityState::NOT_LOADED;
    PendingEvent pendingEvent = PendingEvent::NO_EVENT;
    std::map<int32_t, int32_t> pendingLoadEventCountMap;
    std::list<LoadRequestInfo> pendingLoadEventList;
    UnloadRequestInfo pendingUnloadEvent;
    UnloadRequestInfo unloadRequest;
    bool isAutoRestart = false;
};
} // namespace OHOS

#endif // !defined(OHOS_SYSTEM_ABILITY_MANAGER_SYSTEM_ABILITY_STATE_CONTEXT_H)