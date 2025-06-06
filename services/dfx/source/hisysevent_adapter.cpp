/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include "hisysevent_adapter.h"
#include "hisysevent_easy.h"

#include <string>

#include "def.h"

// hisysevent report 500 times per 5s
#define HISYSEVENT_PERIOD 5
#define HISYSEVENT_THRESHOLD 500
#include "hisysevent.h"
#include "sam_log.h"

namespace OHOS {
using namespace OHOS::HiviewDFX;
namespace {
constexpr const char* ADD_SYSTEMABILITY_FAIL = "ADD_SYSTEMABILITY_FAIL";
constexpr const char* CALLER_UID = "CALLER_UID";
constexpr const char* SAID = "SAID";
constexpr const char* COUNT = "COUNT";
constexpr const char* FILE_NAME = "FILE_NAME";
constexpr const char* GETSA__TAG = "GETSA_FREQUENCY";

constexpr const char* REASON = "REASON";
constexpr const char* ONDEMAND_SA_LOAD_FAIL = "ONDEMAND_SA_LOAD_FAIL";
constexpr const char* ONDEMAND_SA_LOAD = "ONDEMAND_SA_LOAD";
constexpr const char* EVENT = "EVENT";
constexpr const char* SA_CRASH = "SA_CRASH";
constexpr const char* ONDEMAND_SA_UNLOAD = "ONDEMAND_SA_UNLOAD";
constexpr const char* SA_UNLOAD_FAIL = "SA_UNLOAD_FAIL";
constexpr const char* SA_LOAD_DURATION = "SA_LOAD_DURATION";
constexpr const char* SA_UNLOAD_DURATION = "SA_UNLOAD_DURATION";
constexpr const char* SA_MAIN_EXIT = "SA_MAIN_EXIT";
constexpr const char* PROCESS_START_FAIL = "PROCESS_START_FAIL";
constexpr const char* PROCESS_STOP_FAIL = "PROCESS_STOP_FAIL";
constexpr const char* PROCESS_START_DURATION = "PROCESS_START_DURATION";
constexpr const char* PROCESS_STOP_DURATION = "PROCESS_STOP_DURATION";
constexpr const char* SA_IDLE = "SA_IDLE";
constexpr const char* PROCESS_NAME = "PROCESS_NAME";
constexpr const char* PID = "PID";
constexpr const char* UID = "UID";
constexpr const char* DURATION = "DURATION";
constexpr const char* KEY_STAGE = "KEY_STAGE";
constexpr int32_t CONTAINER_SA_MIN = 0x00010500; //66816
constexpr int32_t CONTAINER_SA_MAX = 0x0001055f; //66911
}

static bool IsInCrashWhiteList(int32_t saId)
{
    std::vector<int> whiteList = { 10, 1201, 1202, 1203, 1204, 1205, 1213, 1215, 5002, 5100, 5110, 9999, 65537, 65830,
        65850, 66064, 66186, 69930, 131071, 345135 };
    for (auto sa : whiteList) {
        if (saId == sa) {
            return true;
        }
    }
    if (saId >= CONTAINER_SA_MIN && saId <= CONTAINER_SA_MAX) {
        return true;
    }
    return false;
}

void ReportSaCrash(int32_t saId)
{
    if (IsInCrashWhiteList(saId)) {
        HILOGD("report SA:%{public}d is in crash whitelist.", saId);
        return;
    }
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        SA_CRASH,
        HiSysEvent::EventType::FAULT,
        SAID, saId);
    if (ret != 0) {
        HILOGE("report sa crash failed! SA:%{public}d, ret:%{public}d.", saId, ret);
    }
}

void ReportSaUnLoadFail(int32_t saId, int32_t pid, int32_t uid, const std::string& reason)
{
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        SA_UNLOAD_FAIL,
        HiSysEvent::EventType::FAULT,
        SAID, saId,
        PID, pid,
        UID, uid,
        REASON, reason);
    if (ret != 0) {
        HILOGE("report sa unload fail event failed! SA:%{public}d, ret %{public}d.", saId, ret);
    }
}

static void ReportSaDuration(const std::string& eventName, int32_t saId, int32_t keyStage, int64_t duration)
{
    if (duration <= 0) {
        return;
    }
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        eventName,
        HiSysEvent::EventType::BEHAVIOR,
        SAID, saId,
        KEY_STAGE, keyStage,
        DURATION, duration);
    if (ret != 0) {
        HILOGE("report event:%{public}s failed! SA:%{public}d, ret:%{public}d.",
            eventName.c_str(), saId, ret);
    }
}

void ReportSaMainExit(const std::string& reason)
{
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        SA_MAIN_EXIT,
        HiSysEvent::EventType::FAULT,
        REASON, reason);
    if (ret != 0) {
        HILOGE("report sa main exit event failed! ret:%{public}d.", ret);
    }
}

void ReportSaLoadDuration(int32_t saId, int32_t keyStage, int64_t duration)
{
    ReportSaDuration(SA_LOAD_DURATION, saId, keyStage, duration);
}

void ReportSaUnLoadDuration(int32_t saId, int32_t keyStage, int64_t duration)
{
    ReportSaDuration(SA_UNLOAD_DURATION, saId, keyStage, duration);
}

static void ReportProcessDuration(const std::string& eventName, const std::string& processName,
    int32_t pid, int32_t uid, int64_t duration)
{
    if (duration <= 0) {
        return;
    }
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        eventName,
        HiSysEvent::EventType::BEHAVIOR,
        PROCESS_NAME, processName,
        PID, pid,
        UID, uid,
        DURATION, duration);
    if (ret != 0) {
        HILOGE("report event:%{public}s failed! process:%{public}s, ret:%{public}d.",
            eventName.c_str(), processName.c_str(), ret);
    }
}

void ReportProcessStartDuration(const std::string& processName, int32_t pid, int32_t uid, int64_t duration)
{
    ReportProcessDuration(PROCESS_START_DURATION, processName, pid, uid, duration);
}

void ReportProcessStopDuration(const std::string& processName, int32_t pid, int32_t uid, int64_t duration)
{
    ReportProcessDuration(PROCESS_STOP_DURATION, processName, pid, uid, duration);
}

static void ReportProcessFail(const std::string& eventName, const std::string& processName,
    int32_t pid, int32_t uid, const std::string& reason)
{
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        eventName,
        HiSysEvent::EventType::FAULT,
        PROCESS_NAME, processName,
        PID, pid,
        UID, uid,
        REASON, reason);
    if (ret != 0) {
        HILOGE("report event:%{public}s failed! process:%{public}s, ret:%{public}d.",
            eventName.c_str(), processName.c_str(), ret);
    }
}

void ReportProcessStartFail(const std::string& processName, int32_t pid, int32_t uid, const std::string& reason)
{
    ReportProcessFail(PROCESS_START_FAIL, processName, pid, uid, reason);
}

void ReportProcessStopFail(const std::string& processName, int32_t pid, int32_t uid, const std::string& reason)
{
    ReportProcessFail(PROCESS_STOP_FAIL, processName, pid, uid, reason);
}

void ReportSamgrSaLoadFail(int32_t said, int32_t pid, int32_t uid, const std::string& reason)
{
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        ONDEMAND_SA_LOAD_FAIL,
        HiSysEvent::EventType::FAULT,
        SAID, said,
        PID, pid,
        UID, uid,
        REASON, reason);
    if (ret != 0) {
        HILOGE("hisysevent report samgr sa load fail event failed! ret %{public}d.", ret);
    }
}

void ReportSamgrSaLoad(int32_t said, int32_t pid, int32_t uid, int32_t eventId)
{
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        ONDEMAND_SA_LOAD,
        HiSysEvent::EventType::BEHAVIOR,
        SAID, said,
        PID, pid,
        UID, uid,
        EVENT, eventId);
    if (ret != 0) {
        HILOGE("hisysevent report samgr sa load event failed! ret %{public}d.", ret);
    }
}

void ReportSamgrSaUnload(int32_t said, int32_t pid, int32_t uid, int32_t eventId)
{
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        ONDEMAND_SA_UNLOAD,
        HiSysEvent::EventType::BEHAVIOR,
        SAID, said,
        PID, pid,
        UID, uid,
        EVENT, eventId);
    if (ret != 0) {
        HILOGE("hisysevent report samgr sa unload event failed! ret %{public}d.", ret);
    }
}

static bool IsOpenSoWhiteList(int32_t saId)
{
    std::vector<int> whiteList = { 4606 };
    for (auto sa : whiteList) {
        if (saId == sa) {
            return true;
        }
    }
    return false;
}

void ReportAddSystemAbilityFailed(int32_t said, int32_t pid, int32_t uid, const std::string& filaName)
{
    if (IsOpenSoWhiteList(said)) {
        HILOGD("report SA:%{public}d is open so whitelist.", said);
        return;
    }
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        ADD_SYSTEMABILITY_FAIL,
        HiSysEvent::EventType::FAULT,
        SAID, said,
        PID, pid,
        UID, uid,
        FILE_NAME, filaName);
    if (ret != 0) {
        HILOGE("hisysevent report add system ability event failed! ret %{public}d.", ret);
    }
}

void ReportGetSAFrequency(uint32_t callerUid, uint32_t said, int32_t count)
{
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        GETSA__TAG,
        HiSysEvent::EventType::STATISTIC,
        CALLER_UID, callerUid,
        SAID, said,
        COUNT, count);
    if (ret != 0) {
        HILOGD("hisysevent report get sa frequency failed! ret %{public}d.", ret);
    }
}

void WatchDogSendEvent(int32_t pid, uint32_t uid, const std::string& sendMsg,
    const std::string& eventName)
{
    int ret = HiSysEventWrite(HiSysEvent::Domain::SAMGR,
        eventName,
        HiSysEvent::EventType::FAULT,
        "PID", pid,
        "UID", uid,
        "MSG", sendMsg);
    if (ret != 0) {
        HILOGE("hisysevent report watchdog failed! ret %{public}d.", ret);
    }
}

void ReportSAIdle(const char* reason)
{
    HiSysEventEasyWrite(HiSysEvent::Domain::SAMGR,
        SA_IDLE,
        HiSysEventEasyType::EASY_EVENT_TYPE_BEHAVIOR,
        reason);
}
} // OHOS
