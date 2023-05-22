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

#include "device_status_collect_manager_test.h"

#include "sa_profiles.h"
#include "string_ex.h"
#include "test_log.h"

#define private public
#include "device_status_collect_manager.h"
#ifdef SUPPORT_COMMON_EVENT
#include "common_event_collect.h"
#endif

using namespace std;
using namespace testing;
using namespace testing::ext;
using namespace OHOS;

namespace OHOS {
namespace {
constexpr int32_t MAX_WAIT_TIME = 10000;
constexpr int64_t EXTRA_ID = 1;
const std::string SA_TAG_DEVICE_ON_LINE = "deviceonline";
const std::string WIFI_NAME = "wifi_status";
constexpr int32_t MOCK_PLUGIN = 20;
}

void DeviceStatusCollectManagerTest::SetUpTestCase()
{
    DTEST_LOG << "SetUpTestCase" << std::endl;
}

void DeviceStatusCollectManagerTest::TearDownTestCase()
{
    DTEST_LOG << "TearDownTestCase" << std::endl;
}

void DeviceStatusCollectManagerTest::SetUp()
{
    DTEST_LOG << "SetUp" << std::endl;
}

void DeviceStatusCollectManagerTest::TearDown()
{
    DTEST_LOG << "TearDown" << std::endl;
}

void DeviceStatusCollectManagerTest::PostTask(
    std::shared_ptr<AppExecFwk::EventHandler>& collectHandler)
{
    isCaseDone = false;
    auto caseDoneNotifyTask = [this]() {
        std::lock_guard<std::mutex> autoLock(caseDoneLock_);
        isCaseDone = true;
        caseDoneCondition_.notify_one();
    };
    if (collectHandler != nullptr) {
        collectHandler->PostTask(caseDoneNotifyTask);
    }
    std::unique_lock<std::mutex> lock(caseDoneLock_);
    caseDoneCondition_.wait_for(lock, std::chrono::milliseconds(MAX_WAIT_TIME),
        [&] () { return isCaseDone; });
}

bool MockCollectPlugin::CheckCondition(const OnDemandCondition& condition)
{
    return mockCheckConditionResult_;
}

/**
 * @tc.name: FilterOnDemandSaProfiles001
 * @tc.desc: test FilterOnDemandSaProfiles with different parameters
 * @tc.type: FUNC
 */
HWTEST_F(DeviceStatusCollectManagerTest, FilterOnDemandSaProfiles001, TestSize.Level3)
{
    DTEST_LOG << " FilterOnDemandSaProfiles001 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;
    collect->FilterOnDemandSaProfiles(saProfiles);
    EXPECT_EQ(true, collect->onDemandSaProfiles_.empty());
    SaProfile saProfile;
    OnDemandEvent event = { DEVICE_ONLINE, SA_TAG_DEVICE_ON_LINE, "on" };
    saProfile.startOnDemand.onDemandEvents.emplace_back(event);
    saProfiles.emplace_back(saProfile);
    collect->FilterOnDemandSaProfiles(saProfiles);
    EXPECT_EQ(false, collect->onDemandSaProfiles_.empty());
    OnDemandEvent event1 = { DEVICE_ONLINE, SA_TAG_DEVICE_ON_LINE, "off" };
    saProfile.stopOnDemand.onDemandEvents.emplace_back(event1);
    saProfiles.emplace_back(saProfile);
    collect->FilterOnDemandSaProfiles(saProfiles);
    EXPECT_EQ(false, collect->onDemandSaProfiles_.empty());
    DTEST_LOG << " FilterOnDemandSaProfiles001 END" << std::endl;
}

/**
 * @tc.name: GetSaControlListByEvent001
 * @tc.desc: test GetSaControlListByEvent with different parameters
 * @tc.type: FUNC
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetSaControlListByEvent001, TestSize.Level3)
{
    DTEST_LOG << " GetSaControlListByEvent001 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    OnDemandEvent event = { DEVICE_ONLINE, SA_TAG_DEVICE_ON_LINE, "on" };
    std::list<SaControlInfo> saControlList;
    collect->GetSaControlListByEvent(event, saControlList);
    EXPECT_EQ(true, saControlList.empty());
    SaProfile saProfile;
    OnDemandEvent event1 = { DEVICE_ONLINE, SA_TAG_DEVICE_ON_LINE, "on" };
    OnDemandEvent event2 = { DEVICE_ONLINE, SA_TAG_DEVICE_ON_LINE, "off" };
    saProfile.startOnDemand.onDemandEvents.emplace_back(event1);
    saProfile.stopOnDemand.onDemandEvents.emplace_back(event2);
    collect->onDemandSaProfiles_.emplace_back(saProfile);
    collect->GetSaControlListByEvent(event, saControlList);
    EXPECT_EQ(false, saControlList.empty());
    saControlList.clear();
    event.value = "off";
    collect->GetSaControlListByEvent(event, saControlList);
    EXPECT_EQ(false, saControlList.empty());
    saControlList.clear();
    event.value = "";
    collect->GetSaControlListByEvent(event, saControlList);
    EXPECT_EQ(true, saControlList.empty());
    event.name = "settingswitch";
    collect->GetSaControlListByEvent(event, saControlList);
    EXPECT_EQ(true, saControlList.empty());
    event.eventId = SETTING_SWITCH;
    collect->GetSaControlListByEvent(event, saControlList);
    EXPECT_EQ(true, saControlList.empty());
    DTEST_LOG << " GetSaControlListByEvent001 END" << std::endl;
}

/**
 * @tc.name: UnInit001
 * @tc.desc: test UnInit
 * @tc.type: FUNC
 */
HWTEST_F(DeviceStatusCollectManagerTest, UnInit001, TestSize.Level3)
{
    DTEST_LOG << " UnInit001 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;
    collect->UnInit();
    EXPECT_EQ(true, collect->collectPluginMap_.empty());
    DTEST_LOG << " UnInit001 END" << std::endl;
}

/**
 * @tc.name: StartCollect001
 * @tc.desc: test StartCollect with empty collectHandler.
 * @tc.type: FUNC
 */
HWTEST_F(DeviceStatusCollectManagerTest, StartCollect001, TestSize.Level3)
{
    DTEST_LOG << " StartCollect001 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    collect->StartCollect();
    EXPECT_EQ(nullptr, collect->collectHandler_);
    DTEST_LOG << " StartCollect001 END" << std::endl;
}

/**
 * @tc.name: CheckConditions001
 * @tc.desc: test CheckConditions, with empty conditions.
 * @tc.type: FUNC
 * @tc.require: I6JE38
 */
HWTEST_F(DeviceStatusCollectManagerTest, CheckConditions001, TestSize.Level3)
{
    DTEST_LOG << " CheckConditions001 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;

    OnDemandEvent event;
    bool result = collect->CheckConditions(event);
    EXPECT_EQ(result, true);
    DTEST_LOG << " CheckConditions001 END" << std::endl;
}

/**
 * @tc.name: CheckConditions002
 * @tc.desc: test CheckConditions, with invalid condition eventId.
 * @tc.type: FUNC
 * @tc.require: I6JE38
 */
HWTEST_F(DeviceStatusCollectManagerTest, CheckConditions002, TestSize.Level3)
{
    DTEST_LOG << " CheckConditions002 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;
    OnDemandCondition condition;
    condition.eventId = -1;
    OnDemandEvent event;
    event.conditions.push_back(condition);
    bool result = collect->CheckConditions(event);
    EXPECT_EQ(result, false);
    DTEST_LOG << " CheckConditions002 END" << std::endl;
}

/**
 * @tc.name: CheckConditions003
 * @tc.desc: test CheckConditions, with collect plugin is nullptr.
 * @tc.type: FUNC
 * @tc.require: I6JE38
 */
HWTEST_F(DeviceStatusCollectManagerTest, CheckConditions003, TestSize.Level3)
{
    DTEST_LOG << " CheckConditions003 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;
    collect->collectPluginMap_[MOCK_PLUGIN] = nullptr;
    OnDemandCondition condition;
    condition.eventId = MOCK_PLUGIN;
    OnDemandEvent event;
    event.conditions.push_back(condition);
    bool result = collect->CheckConditions(event);
    EXPECT_EQ(result, false);
    DTEST_LOG << " CheckConditions003 END" << std::endl;
}

/**
 * @tc.name: CheckConditions004
 * @tc.desc: test CheckConditions, with condition not pass.
 * @tc.type: FUNC
 * @tc.require: I6JE38
 */
HWTEST_F(DeviceStatusCollectManagerTest, CheckConditions004, TestSize.Level3)
{
    DTEST_LOG << " CheckConditions004 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;
    collect->collectPluginMap_[MOCK_PLUGIN] = new MockCollectPlugin(collect);
    OnDemandCondition condition;
    condition.eventId = MOCK_PLUGIN;
    OnDemandEvent event;
    event.conditions.push_back(condition);
    bool result = collect->CheckConditions(event);
    EXPECT_EQ(result, false);
    DTEST_LOG << " CheckConditions004 END" << std::endl;
}

/**
 * @tc.name: CheckConditions005
 * @tc.desc: test CheckConditions, with condition pass.
 * @tc.type: FUNC
 * @tc.require: I6JE38
 */
HWTEST_F(DeviceStatusCollectManagerTest, CheckConditions005, TestSize.Level3)
{
    DTEST_LOG << " CheckConditions005 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;
    sptr<MockCollectPlugin> mockCollectPlugin = new MockCollectPlugin(collect);
    mockCollectPlugin->mockCheckConditionResult_ = true;
    collect->collectPluginMap_[MOCK_PLUGIN] = mockCollectPlugin;
    OnDemandCondition condition;
    condition.eventId = MOCK_PLUGIN;
    OnDemandEvent event;
    event.conditions.push_back(condition);
    bool result = collect->CheckConditions(event);
    EXPECT_EQ(result, true);
    DTEST_LOG << " CheckConditions005 END" << std::endl;
}

/**
 * @tc.name: ReportEvent001
 * @tc.desc: test ReportEvent, with empty collectHandler.
 * @tc.type: FUNC
 */
HWTEST_F(DeviceStatusCollectManagerTest, ReportEvent001, TestSize.Level3)
{
    DTEST_LOG << " ReportEvent001 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    OnDemandEvent event;
    collect->ReportEvent(event);
    EXPECT_EQ(nullptr, collect->collectHandler_);
    DTEST_LOG << " ReportEvent001 END" << std::endl;
}

/**
 * @tc.name: ReportEvent002
 * @tc.desc: test ReportEvent, with empty saControlList.
 * @tc.type: FUNC
 */
HWTEST_F(DeviceStatusCollectManagerTest, ReportEvent002, TestSize.Level3)
{
    DTEST_LOG << " ReportEvent002 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;
    collect->Init(saProfiles);
    OnDemandEvent event;
    collect->ReportEvent(event);
    EXPECT_EQ(true, collect->collectHandler_ != nullptr);
    PostTask(collect->collectHandler_);
    DTEST_LOG << " ReportEvent002 END" << std::endl;
}

/**
 * @tc.name: ReportEvent003
 * @tc.desc: test ReportEvent, report success.
 * @tc.type: FUNC
 */
HWTEST_F(DeviceStatusCollectManagerTest, ReportEvent003, TestSize.Level3)
{
    DTEST_LOG << " ReportEvent003 BEGIN" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::list<SaProfile> saProfiles;
    collect->Init(saProfiles);
    OnDemandEvent event = { DEVICE_ONLINE, SA_TAG_DEVICE_ON_LINE, "on" };
    std::list<SaControlInfo> saControlList;
    SaProfile saProfile;
    OnDemandEvent event1 = { DEVICE_ONLINE, SA_TAG_DEVICE_ON_LINE, "on" };
    OnDemandEvent event2 = { DEVICE_ONLINE, SA_TAG_DEVICE_ON_LINE, "off" };
    saProfile.startOnDemand.onDemandEvents.emplace_back(event1);
    saProfile.stopOnDemand.onDemandEvents.emplace_back(event2);
    collect->onDemandSaProfiles_.emplace_back(saProfile);
    collect->ReportEvent(event);
    EXPECT_EQ(true, collect->collectHandler_ != nullptr);
    PostTask(collect->collectHandler_);
    DTEST_LOG << " ReportEvent003 END" << std::endl;
}

/**
 * @tc.name: AddCollectEvents001
 * @tc.desc: test AddCollectEvents, with events
 * @tc.type: FUNC
 * @tc.require: I6UUNW
 */
HWTEST_F(DeviceStatusCollectManagerTest, AddCollectEvents001, TestSize.Level3)
{
    DTEST_LOG << "AddCollectEvents001 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::vector<OnDemandEvent> events;
    int32_t ret = collect->AddCollectEvents(events);
    EXPECT_EQ(ret, ERR_OK);
    DTEST_LOG << "AddCollectEvents001 end" << std::endl;
}

/**
 * @tc.name: AddCollectEvents002
 * @tc.desc: test AddCollectEvents, with event wifi on
 * @tc.type: FUNC
 * @tc.require: I76X9Q
 */
HWTEST_F(DeviceStatusCollectManagerTest, AddCollectEvents002, TestSize.Level3)
{
    DTEST_LOG << "AddCollectEvents002 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    OnDemandEvent onDemandEvent = {SETTING_SWITCH, WIFI_NAME, "on"};
    std::vector<OnDemandEvent> events {onDemandEvent};
    int32_t ret = collect->AddCollectEvents(events);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "AddCollectEvents002 end" << std::endl;
}

/**
 * @tc.name: AddCollectEvents003
 * @tc.desc: test AddCollectEvents, with eventID is invalid
 * @tc.type: FUNC
 * @tc.require: I76X9Q
 */
HWTEST_F(DeviceStatusCollectManagerTest, AddCollectEvents003, TestSize.Level3)
{
    DTEST_LOG << "AddCollectEvents003 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    OnDemandEvent onDemandEvent = {-1, WIFI_NAME, "on"};
    std::vector<OnDemandEvent> events {onDemandEvent};
    int32_t ret = collect->AddCollectEvents(events);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "AddCollectEvents003 end" << std::endl;
}

/**
 * @tc.name: GetOnDemandEvents001
 * @tc.desc: test GetOnDemandEvents, systemAbilityId is invalid, OnDemandPolicyType is START_POLICY
 * @tc.type: FUNC
 * @tc.require: I6UUNW
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandEvents001, TestSize.Level3)
{
    DTEST_LOG << "GetOnDemandEvents001 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    int32_t systemAbilityId = -1;
    OnDemandPolicyType type = OnDemandPolicyType::START_POLICY;
    std::vector<OnDemandEvent> events;
    int32_t ret = collect->GetOnDemandEvents(systemAbilityId, type, events);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "GetOnDemandEvents001 end" << std::endl;
}

/**
 * @tc.name: GetOnDemandEvents002
 * @tc.desc: test GetOnDemandEvents, systemAbilityId is valid, OnDemandPolicyType is STOP_POLICY
 * @tc.type: FUNC
 * @tc.require: I6UUNW
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandEvents002, TestSize.Level3)
{
    DTEST_LOG << "GetOnDemandEvents002 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    int32_t systemAbilityId = 0;
    OnDemandPolicyType type = OnDemandPolicyType::STOP_POLICY;
    std::vector<OnDemandEvent> events;
    int32_t ret = collect->GetOnDemandEvents(systemAbilityId, type, events);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "GetOnDemandEvents002 end" << std::endl;
}

/**
 * @tc.name: GetOnDemandEvents003
 * @tc.desc: test GetOnDemandEvents, with event wifi on, OnDemandPolicyType is STOP_POLICY
 * @tc.type: FUNC
 * @tc.require: I76X9Q
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandEvents003, TestSize.Level3)
{
    DTEST_LOG << "GetOnDemandEvents003 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    int32_t systemAbilityId = 123;
    OnDemandPolicyType type = OnDemandPolicyType::STOP_POLICY;
    OnDemandEvent onDemandEvent = {SETTING_SWITCH, WIFI_NAME, "on"};
    SaProfile saprofile = {u"test", systemAbilityId};
    collect->onDemandSaProfiles_.push_back(saprofile);
    std::vector<OnDemandEvent> events {onDemandEvent};
    int32_t ret = collect->GetOnDemandEvents(systemAbilityId, type, events);
    EXPECT_EQ(ret, ERR_OK);
    DTEST_LOG << "GetOnDemandEvents003 end" << std::endl;
}

/**
 * @tc.name: GetOnDemandEvents004
 * @tc.desc: test GetOnDemandEvents, with event wifi on, OnDemandPolicyType is START_POLICY
 * @tc.type: FUNC
 * @tc.require: I76X9Q
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandEvents004, TestSize.Level3)
{
    DTEST_LOG << "GetOnDemandEvents004 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    int32_t systemAbilityId = 123;
    OnDemandPolicyType type = OnDemandPolicyType::START_POLICY;
    OnDemandEvent onDemandEvent = {SETTING_SWITCH, WIFI_NAME, "on"};
    SaProfile saprofile = {u"test", systemAbilityId};
    collect->onDemandSaProfiles_.push_back(saprofile);
    std::vector<OnDemandEvent> events {onDemandEvent};
    int32_t ret = collect->GetOnDemandEvents(systemAbilityId, type, events);
    EXPECT_EQ(ret, ERR_OK);
    DTEST_LOG << "GetOnDemandEvents004 end" << std::endl;
}

/**
 * @tc.name: GetOnDemandEvents005
 * @tc.desc: test GetOnDemandEvents, with event wifi on, OnDemandPolicyType is invalid
 * @tc.type: FUNC
 * @tc.require: I76X9Q
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandEvents005, TestSize.Level3)
{
    DTEST_LOG << "GetOnDemandEvents005 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    int32_t systemAbilityId = 123;
    OnDemandPolicyType invalidType = (OnDemandPolicyType)2;
    OnDemandEvent onDemandEvent = {SETTING_SWITCH, WIFI_NAME, "on"};
    std::vector<OnDemandEvent> events {onDemandEvent};
    int32_t ret = collect->GetOnDemandEvents(systemAbilityId, invalidType, events);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "GetOnDemandEvents005 end" << std::endl;
}

/**
 * @tc.name: UpdateOnDemandEvents001
 * @tc.desc: test UpdateOnDemandEvents, systemAbilityId is invalid, OnDemandPolicyType is START_POLICY
 * @tc.type: FUNC
 * @tc.require: I6UUNW
 */
HWTEST_F(DeviceStatusCollectManagerTest, UpdateOnDemandEvents001, TestSize.Level3)
{
    DTEST_LOG << "UpdateOnDemandEvents001 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    int32_t systemAbilityId = -1;
    OnDemandPolicyType type = OnDemandPolicyType::START_POLICY;
    std::vector<OnDemandEvent> events;
    int32_t ret = collect->UpdateOnDemandEvents(systemAbilityId, type, events);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "UpdateOnDemandEvents001 end" << std::endl;
}

/**
 * @tc.name: UpdateOnDemandEvents002
 * @tc.desc: test UpdateOnDemandEvents, systemAbilityId is valid, OnDemandPolicyType is STOP_POLICY
 * @tc.type: FUNC
 * @tc.require: I6UUNW
 */
HWTEST_F(DeviceStatusCollectManagerTest, UpdateOnDemandEvents002, TestSize.Level3)
{
    DTEST_LOG << "UpdateOnDemandEvents002 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    int32_t systemAbilityId = 0;
    OnDemandPolicyType type = OnDemandPolicyType::STOP_POLICY;
    std::vector<OnDemandEvent> events;
    int32_t ret = collect->UpdateOnDemandEvents(systemAbilityId, type, events);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "UpdateOnDemandEvents002 end" << std::endl;
}

/**
 * @tc.name: UpdateOnDemandEvents003
 * @tc.desc: test UpdateOnDemandEvents, with event wifi on
 * @tc.type: FUNC
 * @tc.require: I76X9Q
 */
HWTEST_F(DeviceStatusCollectManagerTest, UpdateOnDemandEvents003, TestSize.Level3)
{
    DTEST_LOG << "UpdateOnDemandEvents003 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    int32_t systemAbilityId = 123;
    OnDemandPolicyType type = OnDemandPolicyType::STOP_POLICY;
    OnDemandEvent onDemandEvent = {SETTING_SWITCH, WIFI_NAME, "on"};
    SaProfile saprofile = {u"test", systemAbilityId};
    collect->onDemandSaProfiles_.push_back(saprofile);
    std::vector<OnDemandEvent> events {onDemandEvent};
    int32_t ret = collect->UpdateOnDemandEvents(systemAbilityId, type, events);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "UpdateOnDemandEvents003 end" << std::endl;
}

/**
 * @tc.name: GetOnDemandReasonExtraData001
 * @tc.desc: test GetOnDemandReasonExtraData with COMMON_EVENT is not in collectPluginMap_
 * @tc.type: FUNC
 * @tc.require: I6W735
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandReasonExtraData001, TestSize.Level3)
{
    sptr<DeviceStatusCollectManager> collectManager = new DeviceStatusCollectManager();
    collectManager->collectPluginMap_.clear();
    OnDemandReasonExtraData onDemandReasonExtraData;
    int32_t ret = collectManager->GetOnDemandReasonExtraData(EXTRA_ID, onDemandReasonExtraData);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
}

/**
 * @tc.name: GetOnDemandReasonExtraData002
 * @tc.desc: test GetOnDemandReasonExtraData with collectPluginMap_[COMMON_EVENT] is nullptr
 * @tc.type: FUNC
 * @tc.require: I6W735
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandReasonExtraData002, TestSize.Level3)
{
    sptr<DeviceStatusCollectManager> collectManager = new DeviceStatusCollectManager();
    collectManager->collectPluginMap_.clear();
    collectManager->collectPluginMap_[COMMON_EVENT] = nullptr;
    OnDemandReasonExtraData onDemandReasonExtraData;
    int32_t ret = collectManager->GetOnDemandReasonExtraData(EXTRA_ID, onDemandReasonExtraData);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
}

/**
 * @tc.name: GetOnDemandReasonExtraData003
 * @tc.desc: test GetOnDemandReasonExtraData with collectPluginMap_[COMMON_EVENT]'s extraDataId is not correct
 * @tc.type: FUNC
 * @tc.require: I6W735
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandReasonExtraData003, TestSize.Level3)
{
    sptr<DeviceStatusCollectManager> collectManager = new DeviceStatusCollectManager();
    sptr<CommonEventCollect> commonEventCollect = new CommonEventCollect(nullptr);
    collectManager->collectPluginMap_.clear();
    collectManager->collectPluginMap_[COMMON_EVENT] = commonEventCollect;
    OnDemandReasonExtraData onDemandReasonExtraData;
    int32_t ret = collectManager->GetOnDemandReasonExtraData(EXTRA_ID, onDemandReasonExtraData);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
}

/**
 * @tc.name: GetOnDemandReasonExtraData004
 * @tc.desc: test GetOnDemandReasonExtraData, get extraData
 * @tc.type: FUNC
 * @tc.require: I6W735
 */
HWTEST_F(DeviceStatusCollectManagerTest, GetOnDemandReasonExtraData004, TestSize.Level3)
{
    sptr<DeviceStatusCollectManager> collectManager = new DeviceStatusCollectManager();
    sptr<CommonEventCollect> commonEventCollect = new CommonEventCollect(nullptr);
    auto runner = AppExecFwk::EventRunner::Create("collect_test1");
    commonEventCollect->workHandler_ = std::make_shared<AppExecFwk::EventHandler>(runner);
    EventFwk::CommonEventData eventData;
    commonEventCollect->SaveOnDemandReasonExtraData(eventData);
    collectManager->collectPluginMap_.clear();
    collectManager->collectPluginMap_[COMMON_EVENT] = commonEventCollect;
    OnDemandReasonExtraData onDemandReasonExtraData;
    int32_t ret = collectManager->GetOnDemandReasonExtraData(EXTRA_ID, onDemandReasonExtraData);
    EXPECT_EQ(ret, ERR_OK);
}
} // namespace OHOS