/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "system_ability_mgr_stub_load_test.h"
#include "samgr_err_code.h"
#include "ability_death_recipient.h"
#include "itest_transaction_service.h"
#include "if_system_ability_manager.h"
#include "ipc_skeleton.h"
#include "sa_profiles.h"
#include "sam_mock_permission.h"
#include "string_ex.h"
#include "system_ability_definition.h"
#include "test_log.h"

#define private public
#include "sa_status_change_mock.h"
#include "system_ability_manager.h"

using namespace std;
using namespace testing;
using namespace testing::ext;
using namespace OHOS;

namespace OHOS {
namespace {

const string DEFAULT_LOAD_NAME = "loadevent";
constexpr uint32_t SAID = 1499;
constexpr int64_t DEFAULT_EVENTID = 0;
constexpr int32_t INVALID_SAID = -1;
}

void SystemAbilityMgrStubLoadTest::SetUpTestCase()
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->abilityStateScheduler_ = std::make_shared<SystemAbilityStateScheduler>();
    std::list<SaProfile> saProfiles;
    saMgr->abilityStateScheduler_->Init(saProfiles);
    DTEST_LOG << "SetUpTestCase" << std::endl;
}

void SystemAbilityMgrStubLoadTest::TearDownTestCase()
{
    DTEST_LOG << "TearDownTestCase" << std::endl;
}

void SystemAbilityMgrStubLoadTest::SetUp()
{
    DTEST_LOG << "SetUp" << std::endl;
}

void SystemAbilityMgrStubLoadTest::TearDown()
{
    DTEST_LOG << "TearDown" << std::endl;
}

void SystemAbilityMgrStubLoadTest::AddSystemAbilityContext(int32_t systemAbilityId, const std::u16string& processName)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    EXPECT_TRUE(saMgr->abilityStateScheduler_ != nullptr);
    std::unique_lock<std::shared_mutex> processWriteLock(saMgr->abilityStateScheduler_->processMapLock_);
    if (saMgr->abilityStateScheduler_->processContextMap_.count(processName) == 0) {
        auto processContext = std::make_shared<SystemProcessContext>();
        processContext->processName = processName;
        processContext->abilityStateCountMap[SystemAbilityState::NOT_LOADED] = 0;
        processContext->abilityStateCountMap[SystemAbilityState::LOADING] = 0;
        processContext->abilityStateCountMap[SystemAbilityState::LOADED] = 0;
        processContext->abilityStateCountMap[SystemAbilityState::UNLOADABLE] = 0;
        processContext->abilityStateCountMap[SystemAbilityState::UNLOADING] = 0;
        saMgr->abilityStateScheduler_->processContextMap_[processName] = processContext;
    }
    saMgr->abilityStateScheduler_->processContextMap_[processName]->saList.push_back(systemAbilityId);
    saMgr->abilityStateScheduler_->processContextMap_[processName]
        ->abilityStateCountMap[SystemAbilityState::NOT_LOADED]++;

    auto abilityContext = std::make_shared<SystemAbilityContext>();
    abilityContext->systemAbilityId = systemAbilityId;
    abilityContext->ownProcessContext = saMgr->abilityStateScheduler_->processContextMap_[processName];
    std::unique_lock<std::shared_mutex> abiltyWriteLock(saMgr->abilityStateScheduler_->abiltyMapLock_);
    saMgr->abilityStateScheduler_->abilityContextMap_[systemAbilityId] = abilityContext;
}

#ifdef SUPPORT_ACCESS_TOKEN
HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemAbilityInner001, TestSize.Level4)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    int32_t result = saMgr->AddSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_PERMISSION_DENIED);
}
#endif

#ifdef SUPPORT_ACCESS_TOKEN
HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemProcessInner001, TestSize.Level4)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    int32_t result = saMgr->AddSystemProcessInner(data, reply);
    EXPECT_EQ(result, ERR_PERMISSION_DENIED);
}
#endif

#ifdef SUPPORT_ACCESS_TOKEN
HWTEST_F(SystemAbilityMgrStubLoadTest, SubscribeSystemProcessInner001, TestSize.Level3)
{
    DTEST_LOG << "SubscribeSystemProcessInner001" << std::endl;
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    int32_t ret = saMgr->SubscribeSystemProcessInner(data, reply);
    EXPECT_EQ(ret, ERR_PERMISSION_DENIED);
}
#endif

HWTEST_F(SystemAbilityMgrStubLoadTest, SubsSystemAbilityInner002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(INVALID_SAID);
    int32_t result = saMgr->SubsSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_NULL_OBJECT);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, SubsSystemAbilityInner003, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    int32_t result = saMgr->SubsSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_NULL_OBJECT);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, SubsSystemAbilityInner004, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    SAInfo saInfo;
    saMgr->abilityMap_[SAID] = saInfo;
    saMgr->workHandler_ = make_shared<FFRTHandler>("workHandler");
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    data.WriteRemoteObject(testAbility);
    int32_t result = saMgr->SubsSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbilityImmeInner004, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    EXPECT_TRUE(testAbility != nullptr);
    SAInfo saInfo;
    saInfo.remoteObj = testAbility;
    saMgr->abilityMap_[SAID] = saInfo;
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    data.WriteBool(false);
    int32_t result = saMgr->CheckSystemAbilityImmeInner(data, reply);
    EXPECT_EQ(result, ERR_NONE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, UnmarshalingSaExtraProp003, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    SystemAbilityManager::SAExtraProp extraProp;
    bool isExist = false;
    int32_t dumpFlags = 0;
    std::u16string capability = u"capability";
    std::u16string permission = u"permission";
    data.WriteBool(isExist);
    data.WriteInt32(dumpFlags);
    data.WriteString16(capability);
    data.WriteString16(permission);
    int32_t result = saMgr->UnmarshalingSaExtraProp(data, extraProp);
    EXPECT_EQ(result, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbilityInner002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    saMgr->abilityMap_.clear();
    int32_t result = saMgr->GetSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_NULL_OBJECT);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbilityInner003, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    MessageParcel data;
    MessageParcel reply;
    SAInfo saInfo;
    saInfo.remoteObj = testAbility;
    saMgr->abilityMap_[SAID] = saInfo;
    data.WriteInt32(SAID);
    int32_t result = saMgr->GetSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_NONE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbilityInner002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    saMgr->abilityMap_.clear();
    int32_t result = saMgr->CheckSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_NULL_OBJECT);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbilityInner003, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    MessageParcel data;
    MessageParcel reply;
    SAInfo saInfo;
    saInfo.remoteObj = testAbility;
    saMgr->abilityMap_[SAID] = saInfo;
    data.WriteInt32(SAID);
    int32_t result = saMgr->CheckSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_NONE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, LoadRemoteSystemAbilityInner001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    int32_t result = saMgr->LoadRemoteSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, LoadRemoteSystemAbilityInner002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    int32_t result = saMgr->LoadRemoteSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, LoadRemoteSystemAbilityInner003, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    string deviceId = "test";
    data.WriteString(deviceId);
    int32_t result = saMgr->LoadRemoteSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, LoadRemoteSystemAbilityInner004, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    string deviceId = "test";
    data.WriteString(deviceId);
    data.WriteRemoteObject(callback);
    int32_t result = saMgr->LoadRemoteSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, LoadRemoteSystemAbilityInner005, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(INVALID_SAID);
    int32_t result = saMgr->LoadRemoteSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, InitSaProfile001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->workHandler_ = make_shared<FFRTHandler>("workHandler1");
    saMgr->InitSaProfile();
    EXPECT_NE(saMgr->workHandler_, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, InitSaProfile002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->workHandler_ = nullptr;
    saMgr->InitSaProfile();
    EXPECT_EQ(saMgr->workHandler_, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSaProfile001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    CommonSaProfile saProfilein;
    CommonSaProfile SaProfileout;
    saMgr->saProfileMap_[SAID] = saProfilein;
    bool res = saMgr->GetSaProfile(SAID, SaProfileout);
    saMgr->saProfileMap_.clear();
    EXPECT_EQ(res, true);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbility001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> res = saMgr->GetSystemAbility(INVALID_SAID);
    EXPECT_EQ(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbility002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    string deviceId = "test";
    sptr<IRemoteObject> res = saMgr->GetSystemAbility(SAID, deviceId);
    EXPECT_EQ(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbilityFromRemote001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> res = saMgr->GetSystemAbilityFromRemote(INVALID_SAID);
    EXPECT_EQ(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbilityFromRemote002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> res = saMgr->GetSystemAbilityFromRemote(SAID);
    EXPECT_EQ(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbilityFromRemote003, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    SAInfo saInfo;
    saInfo.isDistributed = false;
    saMgr->abilityMap_[SAID] = saInfo;
    sptr<IRemoteObject> res = saMgr->GetSystemAbilityFromRemote(SAID);
    saMgr->abilityMap_.clear();
    EXPECT_EQ(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbilityFromRemote004, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    SAInfo saInfo;
    saInfo.isDistributed = true;
    saInfo.remoteObj = saMgr;
    saMgr->abilityMap_[SAID] = saInfo;
    sptr<IRemoteObject> res = saMgr->GetSystemAbilityFromRemote(SAID);
    saMgr->abilityMap_.clear();
    EXPECT_NE(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbility001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> res = saMgr->CheckSystemAbility(INVALID_SAID);
    EXPECT_EQ(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbility002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    SAInfo saInfo;
    saInfo.remoteObj = saMgr;
    saMgr->abilityMap_[SAID] = saInfo;
    sptr<IRemoteObject> res = saMgr->CheckSystemAbility(SAID);
    saMgr->abilityMap_.clear();
    EXPECT_NE(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, NotifySystemAbilityChanged001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    string deviceId = "test";
    int32_t code = 1;
    saMgr->NotifySystemAbilityChanged(SAID, deviceId, code, nullptr);
    EXPECT_NE(saMgr, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, NotifySystemAbilityChanged002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SaStatusChangeMock> testAbility(new SaStatusChangeMock());
    string deviceId = "test";
    int32_t code = 1;
    saMgr->NotifySystemAbilityChanged(SAID, deviceId, code, testAbility);
    EXPECT_NE(saMgr, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, FindSystemAbilityNotify001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t pid = 1;
    saMgr->listenerMap_[SAID].push_back({nullptr, pid});
    string deviceId = "test";
    int32_t code = 1;
    bool res = saMgr->FindSystemAbilityNotify(SAID, deviceId, code);
    saMgr->listenerMap_.clear();
    EXPECT_EQ(res, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, FindSystemAbilityNotify002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->listenerMap_.clear();
    string deviceId = "test";
    int32_t code = 1;
    bool res = saMgr->FindSystemAbilityNotify(SAID, deviceId, code);
    saMgr->listenerMap_.clear();
    EXPECT_EQ(res, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, StartOnDemandAbility001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"test";
    saMgr->StartOnDemandAbility(procName, SAID);
    EXPECT_NE(saMgr, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, StartOnDemandAbility002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"test";
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::STARTING;
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    saMgr->StartOnDemandAbility(procName, SAID);
    saMgr->startingAbilityMap_.clear();
    EXPECT_NE(saMgr, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, StartOnDemandAbilityInner001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"test";
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::STARTING;
    saMgr->StartOnDemandAbilityInner(procName, SAID, abilityItem);
    EXPECT_EQ(abilityItem.state, SystemAbilityManager::AbilityState::STARTING);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, StartOnDemandAbilityInner002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"";
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::INIT;
    saMgr->StartOnDemandAbilityInner(procName, SAID, abilityItem);
    EXPECT_EQ(abilityItem.state, SystemAbilityManager::AbilityState::INIT);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, StartOnDemandAbilityInner003, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"";
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::STARTING;
    int32_t ret = saMgr->StartOnDemandAbilityInner(procName, SAID, abilityItem);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, StartOnDemandAbilityInner004, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"";
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::STARTED;
    int32_t ret = saMgr->StartOnDemandAbilityInner(procName, SAID, abilityItem);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, AddOnDemandSystemAbilityInfo001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"test";
    int maxService = 1002;
    for (int tempSaid = 1; tempSaid < maxService; tempSaid++) {
        saMgr->onDemandAbilityMap_[tempSaid] = procName;
    }
    int32_t res = saMgr->AddOnDemandSystemAbilityInfo(SAID, procName);
    saMgr->onDemandAbilityMap_.clear();
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, AddOnDemandSystemAbilityInfo002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"test";
    saMgr->systemProcessMap_[procName] = saMgr;
    int32_t res = saMgr->AddOnDemandSystemAbilityInfo(SAID, procName);
    saMgr->systemProcessMap_.clear();
    EXPECT_EQ(res, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, AddOnDemandSystemAbilityInfo003, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"test";
    saMgr->systemProcessMap_[procName] = saMgr;
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::STARTING;
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    int32_t res = saMgr->AddOnDemandSystemAbilityInfo(SAID, procName);
    saMgr->systemProcessMap_.clear();
    saMgr->startingAbilityMap_.clear();
    EXPECT_EQ(res, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, AddOnDemandSystemAbilityInfo004, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"test";
    saMgr->systemProcessMap_[procName] = saMgr;
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::STARTING;
    saMgr->workHandler_ = make_shared<FFRTHandler>("workHandler2");
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    int32_t res = saMgr->AddOnDemandSystemAbilityInfo(SAID, procName);
    saMgr->systemProcessMap_.clear();
    saMgr->startingAbilityMap_.clear();
    EXPECT_EQ(res, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, StartOnDemandAbility003, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"test";
    saMgr->onDemandAbilityMap_[SAID] = procName;
    bool isExist = false;
    int32_t res = saMgr->StartOnDemandAbility(SAID, isExist);
    saMgr->onDemandAbilityMap_.clear();
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbility003, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    bool isExist = false;
    sptr<IRemoteObject> res = saMgr->CheckSystemAbility(INVALID_SAID, isExist);
    EXPECT_EQ(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbility004, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    SAInfo saInfo;
    saInfo.remoteObj = saMgr;
    saMgr->abilityMap_[SAID] = saInfo;
    bool isExist = false;
    sptr<IRemoteObject> res = saMgr->CheckSystemAbility(SAID, isExist);
    saMgr->abilityMap_.clear();
    EXPECT_NE(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbility005, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    bool isExist = false;
    sptr<IRemoteObject> res = saMgr->CheckSystemAbility(SAID, isExist);
    EXPECT_EQ(res, nullptr);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbility006, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::STARTING;
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    AddSystemAbilityContext(SAID, u"test");
    saMgr->abilityStateScheduler_->processContextMap_[u"test"]->state = SystemProcessState::STARTED;
    bool isExist = false;
    sptr<IRemoteObject> res = saMgr->CheckSystemAbility(SAID, isExist);
    saMgr->startingAbilityMap_.clear();
    saMgr->abilityStateScheduler_->processContextMap_.clear();
    saMgr->abilityStateScheduler_->abilityContextMap_.clear();
    EXPECT_EQ(isExist, true);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, CheckSystemAbility007, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.state = SystemAbilityManager::AbilityState::INIT;
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    u16string proName = u"test";
    saMgr->onDemandAbilityMap_[SAID] = proName;
    AddSystemAbilityContext(SAID, u"test");
    saMgr->abilityStateScheduler_->processContextMap_[u"test"]->state = SystemProcessState::STARTED;
    bool isExist = false;
    sptr<IRemoteObject> res = saMgr->CheckSystemAbility(SAID, isExist);
    saMgr->startingAbilityMap_.clear();
    saMgr->onDemandAbilityMap_.clear();
    saMgr->abilityStateScheduler_->processContextMap_.clear();
    saMgr->abilityStateScheduler_->abilityContextMap_.clear();
    EXPECT_EQ(isExist, true);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, SubscribeSystemAbility001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SaStatusChangeMock> listener(new SaStatusChangeMock());
    int32_t res = saMgr->SubscribeSystemAbility(INVALID_SAID, listener);
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, SubscribeSystemAbility002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SaStatusChangeMock> listener(nullptr);
    int32_t res = saMgr->SubscribeSystemAbility(SAID, listener);
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, SubscribeSystemAbility003, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SaStatusChangeMock> listener(nullptr);
    int32_t res = saMgr->SubscribeSystemAbility(INVALID_SAID, listener);
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, SubscribeSystemAbility004, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SaStatusChangeMock> listener(new SaStatusChangeMock());
    saMgr->listenerMap_[SAID].push_back({listener, SAID});
    int32_t res = saMgr->SubscribeSystemAbility(SAID, listener);
    EXPECT_EQ(res, ERR_OK);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemAbility001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    SystemAbilityManager::SAExtraProp extraProp;
    int32_t res = saMgr->AddSystemAbility(INVALID_SAID, testAbility, extraProp);
    saMgr->workHandler_ = nullptr;
    saMgr->SendSystemAbilityRemovedMsg(SAID);
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemAbility002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(nullptr);
    SystemAbilityManager::SAExtraProp extraProp;
    int32_t res = saMgr->AddSystemAbility(SAID, testAbility, extraProp);
    u16string name;
    string srcDeviceId;
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    saMgr->SendCheckLoadedMsg(SAID, name, srcDeviceId, callback);
    saMgr->SendLoadedSystemAbilityMsg(SAID, testAbility, callback);
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemAbility003, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(nullptr);
    SystemAbilityManager::SAExtraProp extraProp;
    int32_t res = saMgr->AddSystemAbility(INVALID_SAID, testAbility, extraProp);
    saMgr->workHandler_ = make_shared<FFRTHandler>("workHandler3");
    u16string name;
    string srcDeviceId;
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    saMgr->SendCheckLoadedMsg(SAID, name, srcDeviceId, callback);
    EXPECT_EQ(res, ERR_INVALID_VALUE);
    saMgr->workHandler_->CleanFfrt();
    saMgr->workHandler_ = nullptr;
}

/**
 * @tc.name: AddSystemAbility004
 * @tc.desc: test AddSystemAbility, return ERR_OK,SendCheckLoadedMsg SA loaded
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemAbility004, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    SystemAbilityManager::SAExtraProp extraProp;
    extraProp.isDistributed = false;
    saMgr->dBinderService_ = nullptr;
    int32_t res = saMgr->AddSystemAbility(SAID, testAbility, extraProp);
    saMgr->dBinderService_ = DBinderService::GetInstance();
    u16string name;
    string srcDeviceId;
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    SAInfo saInfo;
    saMgr->abilityMap_[SAID] = saInfo;
    saMgr->SendCheckLoadedMsg(SAID, name, srcDeviceId, callback);
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: AddSystemAbility005
 * @tc.desc: test AddSystemAbility, return ERR_OK
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemAbility005, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->workHandler_ = make_shared<FFRTHandler>("workHandler");
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    SystemAbilityManager::SAExtraProp extraProp;
    saMgr->dBinderService_ = nullptr;
    int32_t res = saMgr->AddSystemAbility(SOFTBUS_SERVER_SA_ID, testAbility, extraProp);
    saMgr->dBinderService_ = DBinderService::GetInstance();
    u16string name = u"test";
    string srcDeviceId;
    int64_t id = 1;
    saMgr->startingProcessMap_[name] = id;
    saMgr->startingAbilityMap_.clear();
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    saMgr->CleanCallbackForLoadFailed(SAID, name, srcDeviceId, callback);
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: AddSystemAbility006
 * @tc.desc: test AddSystemAbility, return ERR_OK
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemAbility006, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->workHandler_ = make_shared<FFRTHandler>("workHandler");
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    SystemAbilityManager::SAExtraProp extraProp;
    saMgr->rpcCallbackImp_ = nullptr;
    int32_t res = saMgr->AddSystemAbility(SOFTBUS_SERVER_SA_ID, testAbility, extraProp);
    saMgr->rpcCallbackImp_ = make_shared<RpcCallbackImp>();
    u16string name = u"test";
    string srcDeviceId = "srcDeviceId";
    saMgr->startingProcessMap_.clear();
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.callbackMap[srcDeviceId].push_back(make_pair(callback, SAID));
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    saMgr->CleanCallbackForLoadFailed(SAID, name, srcDeviceId, callback);
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: AddSystemProcess001
 * @tc.desc: test AddSystemProcess, return ERR_INVALID_VALUE
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemProcess001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->workHandler_ = make_shared<FFRTHandler>("workHandler");
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    u16string procName = u"";
    int32_t res = saMgr->AddSystemProcess(procName, testAbility);
    u16string name = u"test";
    string srcDeviceId = "srcDeviceId";
    saMgr->startingProcessMap_.clear();
    sptr<SystemAbilityLoadCallbackMock> callbackOne = new SystemAbilityLoadCallbackMock();
    sptr<SystemAbilityLoadCallbackMock> callbackTwo = new SystemAbilityLoadCallbackMock();
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.callbackMap[srcDeviceId].push_back(make_pair(callbackOne, SAID));
    abilityItem.callbackMap[srcDeviceId].push_back(make_pair(callbackTwo, SAID));
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    saMgr->CleanCallbackForLoadFailed(SAID, name, srcDeviceId, callbackOne);
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

/**
 * @tc.name: AddSystemProcess002
 * @tc.desc: test AddSystemProcess, return ERR_INVALID_VALUE
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, AddSystemProcess002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->workHandler_ = make_shared<FFRTHandler>("workHandler");
    sptr<IRemoteObject> testAbility(nullptr);
    u16string procName = u"proname";
    int32_t res = saMgr->AddSystemProcess(procName, testAbility);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    saMgr->SendLoadedSystemAbilityMsg(SAID, testAbility, callback);
    u16string name = u"test";
    string srcDeviceId = "srcDeviceId";
    saMgr->startingProcessMap_.clear();
    sptr<SystemAbilityLoadCallbackMock> callbackOne = new SystemAbilityLoadCallbackMock();
    sptr<SystemAbilityLoadCallbackMock> callbackTwo = new SystemAbilityLoadCallbackMock();
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.callbackMap[srcDeviceId].push_back(make_pair(callbackOne, SAID));
    abilityItem.callbackMap[srcDeviceId].push_back(make_pair(callbackTwo, SAID));
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    saMgr->CleanCallbackForLoadFailed(SAID, name, srcDeviceId, callbackTwo);
    EXPECT_EQ(res, ERR_INVALID_VALUE);
}

/**
 * @tc.name: GetSystemProcess001
 * @tc.desc: test GetSystemProcess, process found
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemProcess001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    u16string procName = u"proTest";
    int32_t res = saMgr->AddSystemProcess(procName, testAbility);
    EXPECT_EQ(res, ERR_OK);
    sptr<IRemoteObject> resObj = saMgr->GetSystemProcess(procName);
    EXPECT_NE(resObj, nullptr);
}

/**
 * @tc.name: StartingSystemProcess001
 * @tc.desc: test StartingSystemProcess, process already starting!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, StartingSystemProcess001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    u16string procName = u"proTest";
    saMgr->startingProcessMap_.clear();
    int countNum = 2;
    saMgr->startingProcessMap_[procName] = countNum;
    OnDemandEvent defaultevent = {DEFAULT_EVENTID, DEFAULT_LOAD_NAME, ""};
    int32_t res = saMgr->StartingSystemProcess(procName, SAID, defaultevent);
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: StartingSystemProcess002
 * @tc.desc: test StartingSystemProcess, return ERR_OK!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, StartingSystemProcess002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    u16string procName = u"proTest";
    saMgr->startingProcessMap_.clear();
    saMgr->systemProcessMap_[procName] = testAbility;
    OnDemandEvent defaultevent = {DEFAULT_EVENTID, DEFAULT_LOAD_NAME, ""};
    int32_t res = saMgr->StartingSystemProcess(procName, SAID, defaultevent);
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: LoadSystemAbilityFromRpc001
 * @tc.desc: test LoadSystemAbilityFromRpc, not supported!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbilityFromRpc001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    SaProfile saProfile;
    string srcDeviceId;
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    saMgr->saProfileMap_.clear();
    bool res = saMgr->LoadSystemAbilityFromRpc(srcDeviceId, SAID, callback);
    EXPECT_FALSE(res);
}

/**
 * @tc.name: LoadSystemAbilityFromRpc002
 * @tc.desc: test LoadSystemAbilityFromRpc, not distributed!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbilityFromRpc002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    CommonSaProfile saProfile;
    saProfile.distributed = false;
    string srcDeviceId;
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    saMgr->saProfileMap_[SAID] = saProfile;
    bool res = saMgr->LoadSystemAbilityFromRpc(srcDeviceId, SAID, callback);
    EXPECT_FALSE(res);
}

/**
 * @tc.name: LoadSystemAbilityFromRpc003
 * @tc.desc: test LoadSystemAbilityFromRpc, return true!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbilityFromRpc003, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<IRemoteObject> testAbility(new SaStatusChangeMock());
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    CommonSaProfile saProfile;
    saProfile.distributed = true;
    SAInfo saInfo;
    saInfo.remoteObj = testAbility;
    string srcDeviceId;
    saMgr->saProfileMap_[SAID] = saProfile;
    saMgr->abilityMap_[SAID] = saInfo;
    AddSystemAbilityContext(SAID, u"listen_test");
    bool res = saMgr->LoadSystemAbilityFromRpc(srcDeviceId, SAID, callback);
    saMgr->abilityStateScheduler_->processContextMap_.clear();
    saMgr->abilityStateScheduler_->abilityContextMap_.clear();
    EXPECT_TRUE(res);
}

/**
 * @tc.name: LoadSystemAbilityFromRpc004
 * @tc.desc: test LoadSystemAbilityFromRpc, return true!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbilityFromRpc004, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    CommonSaProfile saProfile;
    u16string procName = u"procname";
    int countNum = 2;
    saProfile.distributed = true;
    saProfile.process = procName;
    SAInfo saInfo;
    SystemAbilityManager::AbilityItem abilityItem;
    saInfo.remoteObj = nullptr;
    string srcDeviceId = "srcDeviceId";
    saMgr->saProfileMap_[SAID] = saProfile;
    saMgr->abilityMap_[SAID] = saInfo;
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    saMgr->startingProcessMap_[procName] = countNum;
    AddSystemAbilityContext(SAID, procName);
    bool res = saMgr->LoadSystemAbilityFromRpc(srcDeviceId, SAID, callback);
    saMgr->abilityStateScheduler_->processContextMap_.clear();
    saMgr->abilityStateScheduler_->abilityContextMap_.clear();
    EXPECT_TRUE(res);
}

/**
 * @tc.name: LoadSystemAbilityFromRpc005
 * @tc.desc: test LoadSystemAbilityFromRpc, LoadSystemAbility said or callback invalid!!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbilityFromRpc005, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = nullptr;
    string srcDeviceId = "srcDeviceId";
    bool res = saMgr->LoadSystemAbilityFromRpc(srcDeviceId, SAID, callback);
    EXPECT_FALSE(res);
}

/**
 * @tc.name: LoadSystemAbilityFromRpc006
 * @tc.desc: test LoadSystemAbilityFromRpc, LoadSystemAbility said or callback invalid!!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbilityFromRpc006, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    string srcDeviceId = "srcDeviceId";
    bool res = saMgr->LoadSystemAbilityFromRpc(srcDeviceId, INVALID_SAID, callback);
    EXPECT_FALSE(res);
}

/**
 * @tc.name: LoadSystemAbility001
 * @tc.desc: test LoadSystemAbility001, systemAbilityId or callback invalid!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbility001, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    pair<sptr<ISystemAbilityLoadCallback>, int32_t> itemPair;
    int countNum = 2;
    itemPair.first = callback;
    itemPair.second = SAID;
    saMgr->callbackCountMap_[itemPair.second] = countNum;
    saMgr->RemoveStartingAbilityCallbackLocked(itemPair);
    int32_t res = saMgr->LoadSystemAbility(INVALID_SAID, callback);
    EXPECT_EQ(res, INVALID_INPUT_PARA);
}

/**
 * @tc.name: LoadSystemAbility002
 * @tc.desc: test LoadSystemAbility002, systemAbilityId or callback invalid!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbility002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    saMgr->OnRemoteCallbackDied(nullptr);
    int32_t res = saMgr->LoadSystemAbility(SAID, nullptr);
    EXPECT_EQ(res, INVALID_INPUT_PARA);
}

/**
 * @tc.name: LoadSystemAbility003
 * @tc.desc: test LoadSystemAbility003, systemAbilityId or callback invalid!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbility003, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    list<sptr<ISystemAbilityLoadCallback>> callbacks;
    callbacks.push_back(callback);
    saMgr->remoteCallbackDeath_ = sptr<IRemoteObject::DeathRecipient>(new RemoteCallbackDeathRecipient());
    saMgr->RemoveRemoteCallbackLocked(callbacks, callback);
    int32_t res = saMgr->LoadSystemAbility(INVALID_SAID, nullptr);
    EXPECT_EQ(res, INVALID_INPUT_PARA);
}

/**
 * @tc.name: LoadSystemAbility004
 * @tc.desc: test LoadSystemAbility004, LoadSystemAbility already existed callback object systemAbilityId!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbility004, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    CommonSaProfile saProfile;
    SAInfo saInfo;
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    SystemAbilityManager::AbilityItem abilityItem;
    saMgr->saProfileMap_[SAID] = saProfile;
    saMgr->abilityMap_[SAID] = saInfo;
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    abilityItem.callbackMap["local"].push_back(make_pair(callback, SAID));
    AddSystemAbilityContext(SAID, u"listen_test");
    int32_t res = saMgr->LoadSystemAbility(SAID, callback);
    saMgr->abilityStateScheduler_->processContextMap_.clear();
    saMgr->abilityStateScheduler_->abilityContextMap_.clear();
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: LoadSystemAbility005
 * @tc.desc: test LoadSystemAbility005, LoadSystemAbility systemAbilityId!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbility005, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    CommonSaProfile saProfile;
    SAInfo saInfo;
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    SystemAbilityManager::AbilityItem abilityItem;
    saMgr->saProfileMap_[SAID] = saProfile;
    saMgr->abilityMap_[SAID] = saInfo;
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    saMgr->abilityCallbackDeath_ = nullptr;
    AddSystemAbilityContext(SAID, u"listen_test");
    int32_t res = saMgr->LoadSystemAbility(SAID, callback);
    saMgr->abilityStateScheduler_->processContextMap_.clear();
    saMgr->abilityStateScheduler_->abilityContextMap_.clear();
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: LoadSystemAbility006
 * @tc.desc: test LoadSystemAbility006, LoadSystemAbility already existed callback!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbility006, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    string deviceId = "deviceId";
    saMgr->remoteCallbacks_["1499_deviceId"].push_back(callback);
    int32_t res = saMgr->LoadSystemAbility(SAID, deviceId, callback);
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: LoadSystemAbility007
 * @tc.desc: test LoadSystemAbility007, LoadSystemAbility AddDeathRecipient succeed!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbility007, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    string deviceId = "deviceId";
    saMgr->remoteCallbacks_.clear();
    saMgr->remoteCallbackDeath_ = sptr<IRemoteObject::DeathRecipient>(new RemoteCallbackDeathRecipient());
    int32_t res = saMgr->LoadSystemAbility(SAID, deviceId, callback);
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: LoadSystemAbility008
 * @tc.desc: test LoadSystemAbility008, LoadSystemAbility already existed callback!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, LoadSystemAbility008, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    CommonSaProfile saProfile;
    SAInfo saInfo;
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    sptr<SystemAbilityLoadCallbackMock> callbackTwo = new SystemAbilityLoadCallbackMock();
    SystemAbilityManager::AbilityItem abilityItem;
    abilityItem.callbackMap["local"].push_back(make_pair(callback, SAID));
    abilityItem.callbackMap["local"].push_back(make_pair(callbackTwo, SAID));
    saMgr->saProfileMap_[SAID] = saProfile;
    saMgr->startingAbilityMap_[SAID] = abilityItem;
    AddSystemAbilityContext(SAID, u"listen_test");
    int32_t res = saMgr->LoadSystemAbility(SAID, callbackTwo);
    saMgr->abilityStateScheduler_->processContextMap_.clear();
    saMgr->abilityStateScheduler_->abilityContextMap_.clear();
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.name: DoMakeRemoteBinder001
 * @tc.desc: test DoMakeRemoteBinder, callback is nullptr
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, DoMakeRemoteBinder001, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = nullptr;
    string deviceId = "deviceId";
    auto callingPid = IPCSkeleton::GetCallingPid();
    auto callingUid = IPCSkeleton::GetCallingUid();
    saMgr->dBinderService_ = nullptr;
    saMgr->DoLoadRemoteSystemAbility(SAID, callingPid, callingUid, deviceId, callback);
    sptr<DBinderServiceStub> res = saMgr->DoMakeRemoteBinder(SAID, callingPid, callingUid, deviceId);
    EXPECT_EQ(res, nullptr);
}

/**
 * @tc.name: DoMakeRemoteBinder002
 * @tc.desc: test DoMakeRemoteBinder, callback is nullptr
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, DoMakeRemoteBinder002, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    string deviceId = "deviceId";
    auto callingPid = IPCSkeleton::GetCallingPid();
    auto callingUid = IPCSkeleton::GetCallingUid();
    saMgr->dBinderService_ = nullptr;
    saMgr->remoteCallbackDeath_ = sptr<IRemoteObject::DeathRecipient>(new RemoteCallbackDeathRecipient());
    saMgr->DoLoadRemoteSystemAbility(SAID, callingPid, callingUid, deviceId, callback);
    sptr<DBinderServiceStub> res = saMgr->DoMakeRemoteBinder(SAID, callingPid, callingUid, deviceId);
    EXPECT_EQ(res, nullptr);
}

/**
 * @tc.name: OnRemoteRequest002
 * @tc.desc: test SystemAbilityStatusChangeStub::OnRemoteRequest002
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, OnRemoteRequest002, TestSize.Level3)
{
    sptr<SaStatusChangeMock> testAbility(new SaStatusChangeMock());
    EXPECT_TRUE(testAbility != nullptr);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    int32_t code = 1;
    int32_t result = testAbility->OnRemoteRequest(code, data, reply, option);
    EXPECT_EQ(result, ERR_PERMISSION_DENIED);
}

/**
 * @tc.name: OnRemoteRequest003
 * @tc.desc: test SystemAbilityStatusChangeStub::OnRemoteRequest003
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, OnRemoteRequest003, TestSize.Level3)
{
    std::u16string StatuschangeToken = u"OHOS.ISystemAbilityStatusChange";
    uint32_t code = 3;
    sptr<SaStatusChangeMock> testAbility(new SaStatusChangeMock());
    EXPECT_TRUE(testAbility != nullptr);
    MessageParcel data;
    data.WriteInterfaceToken(StatuschangeToken);
    MessageParcel reply;
    MessageOption option;
    int32_t result = testAbility->OnRemoteRequest(code, data, reply, option);
    EXPECT_EQ(result, IPC_STUB_UNKNOW_TRANS_ERR);
}

/**
 * @tc.name: OnRemoteRequest004
 * @tc.desc: test SystemAbilityLoadCallbackStub::OnRemoteRequest004
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, OnRemoteRequest004, TestSize.Level3)
{
    int32_t code = 1;
    sptr<SystemAbilityLoadCallbackMock> testAbility(new SystemAbilityLoadCallbackMock());
    EXPECT_TRUE(testAbility != nullptr);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    int32_t result = testAbility->OnRemoteRequest(code, data, reply, option);
    EXPECT_EQ(result, ERR_PERMISSION_DENIED);
}

/**
 * @tc.name: OnRemoteRequest005
 * @tc.desc: test SystemAbilityLoadCallbackStub::OnRemoteRequest005
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, OnRemoteRequest005, TestSize.Level3)
{
    std::u16string loadToken = u"OHOS.ISystemAbilityLoadCallback";
    uint32_t code = 4;
    sptr<SystemAbilityLoadCallbackMock> testAbility(new SystemAbilityLoadCallbackMock());
    EXPECT_TRUE(testAbility != nullptr);
    MessageParcel data;
    data.WriteInterfaceToken(loadToken);
    MessageParcel reply;
    MessageOption option;
    int32_t result = testAbility->OnRemoteRequest(code, data, reply, option);
    EXPECT_EQ(result, IPC_STUB_UNKNOW_TRANS_ERR);
}

/**
 * @tc.name: OnAddSystemAbilityInner001
 * @tc.desc: test SystemAbilityStatusChangeStub::OnAddSystemAbilityInner
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, OnAddSystemAbilityInner001, TestSize.Level3)
{
    sptr<SaStatusChangeMock> testAbility(new SaStatusChangeMock());
    EXPECT_TRUE(testAbility != nullptr);
    MessageParcel data;
    data.WriteInt32(INVALID_SAID);
    MessageParcel reply;
    int32_t result = testAbility->OnAddSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_NULL_OBJECT);
}

/**
 * @tc.name: OnLoadSystemAbilitySuccessInner001
 * @tc.desc: test SystemAbilityLoadCallbackStub::OnLoadSystemAbilitySuccessInner
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, OnLoadSystemAbilitySuccessInner001, TestSize.Level3)
{
    sptr<SystemAbilityLoadCallbackMock> testAbility(new SystemAbilityLoadCallbackMock());
    EXPECT_TRUE(testAbility != nullptr);
    MessageParcel data;
    data.WriteInt32(INVALID_SAID);
    MessageParcel reply;
    int32_t result = testAbility-> OnLoadSystemAbilitySuccessInner(data, reply);
    EXPECT_EQ(result, ERR_INVALID_VALUE);
}

/**
 * @tc.name: OnLoadSystemAbilityFailInner001
 * @tc.desc: test SystemAbilityLoadCallbackStub::OnLoadSystemAbilityFailInner
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, OnLoadSystemAbilityFailInner001, TestSize.Level3)
{
    sptr<SystemAbilityLoadCallbackMock> testAbility(new SystemAbilityLoadCallbackMock());
    EXPECT_TRUE(testAbility != nullptr);
    MessageParcel data;
    data.WriteInt32(INVALID_SAID);
    MessageParcel reply;
    int32_t result = testAbility->OnLoadSystemAbilityFailInner(data, reply);
    EXPECT_EQ(result, ERR_INVALID_VALUE);
}

/**
 * @tc.name: OnLoadSACompleteForRemoteInner001
 * @tc.desc: test SystemAbilityLoadCallbackStub::OnLoadSACompleteForRemoteInner001
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, OnLoadSACompleteForRemoteInner001, TestSize.Level3)
{
    sptr<SystemAbilityLoadCallbackMock> testAbility(new SystemAbilityLoadCallbackMock());
    EXPECT_TRUE(testAbility != nullptr);
    MessageParcel data;
    data.WriteInt32(INVALID_SAID);
    MessageParcel reply;
    int32_t result = testAbility->OnLoadSACompleteForRemoteInner(data, reply);
    EXPECT_EQ(result, ERR_INVALID_VALUE);
}

/**
 * @tc.name: Test CancelUnloadSystemAbilityInner001
 * @tc.desc: CancelUnloadSystemAbilityInner001, permission denied!
 * @tc.type: FUNC
 * @tc.require: I7VQQG
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, CancelUnloadSystemAbilityInner001, TestSize.Level3)
{
    DTEST_LOG << "CancelUnloadSystemAbilityInner001" << std::endl;
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(INVALID_SAID);
    int32_t ret = saMgr->CancelUnloadSystemAbilityInner(data, reply);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
}

/**
 * @tc.name: Test CancelUnloadSystemAbilityInner002
 * @tc.desc: CancelUnloadSystemAbilityInner002, permission denied!
 * @tc.type: FUNC
 * @tc.require: I7VQQG
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, CancelUnloadSystemAbilityInner002, TestSize.Level3)
{
    DTEST_LOG << "CancelUnloadSystemAbilityInner002" << std::endl;
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(SAID);
    int32_t ret = saMgr->CancelUnloadSystemAbilityInner(data, reply);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
}

/**
 * @tc.name: GetSystemAbilityInner004
 * @tc.desc: test GetSystemAbilityInner, read systemAbilityId failed!
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrStubLoadTest, GetSystemAbilityInner004, TestSize.Level3)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(INVALID_SAID);
    int32_t result = saMgr->GetSystemAbilityInner(data, reply);
    EXPECT_EQ(result, ERR_NULL_OBJECT);
}
}