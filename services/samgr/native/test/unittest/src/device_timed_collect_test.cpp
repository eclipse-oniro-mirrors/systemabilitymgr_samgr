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

#include "device_timed_collect_test.h"

#include "sa_profiles.h"
#include "test_log.h"

#define private public
#include "device_status_collect_manager.h"
#include "device_timed_collect.h"

using namespace std;
using namespace testing;
using namespace testing::ext;
using namespace OHOS;

namespace OHOS {
void DeviceTimedCollectTest::SetUpTestCase()
{
    DTEST_LOG << "SetUpTestCase" << std::endl;
}

void DeviceTimedCollectTest::TearDownTestCase()
{
    DTEST_LOG << "TearDownTestCase" << std::endl;
}

void DeviceTimedCollectTest::SetUp()
{
    DTEST_LOG << "SetUp" << std::endl;
}

void DeviceTimedCollectTest::TearDown()
{
    DTEST_LOG << "TearDown" << std::endl;
}

/**
 * @tc.name: Init001
 * @tc.desc: test Init
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectTest, Init001, TestSize.Level3)
{
    std::list<SaProfile> saProfiles;
    SaProfile saProfile;
    OnDemandEvent onDemandEvent = {TIMED_EVENT, "loopevent", "40"};
    saProfile.startOnDemand.onDemandEvents.push_back(onDemandEvent);
    saProfiles.push_back(saProfile);
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->Init(saProfiles);
    EXPECT_EQ(deviceTimedCollect->timedSet_.size(), 1);
}

/**
 * @tc.name: Init002
 * @tc.desc: test Init
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectTest, Init002, TestSize.Level3)
{
    std::list<SaProfile> saProfiles;
    SaProfile saProfile;
    OnDemandEvent onDemandEvent = {TIMED_EVENT, "mockevent", "40"};
    saProfile.startOnDemand.onDemandEvents.push_back(onDemandEvent);
    saProfiles.push_back(saProfile);
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->Init(saProfiles);
    EXPECT_EQ(deviceTimedCollect->timedSet_.size(), 0);
}

/**
 * @tc.name: Init003
 * @tc.desc: test Init
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectTest, Init003, TestSize.Level3)
{
    std::list<SaProfile> saProfiles;
    SaProfile saProfile;
    OnDemandEvent onDemandEvent = {TIMED_EVENT, "loopevent", "40"};
    saProfile.stopOnDemand.onDemandEvents.push_back(onDemandEvent);
    saProfiles.push_back(saProfile);
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->Init(saProfiles);
    EXPECT_EQ(deviceTimedCollect->timedSet_.size(), 1);
}

/**
 * @tc.name: Init004
 * @tc.desc: test Init
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectTest, Init004, TestSize.Level3)
{
    std::list<SaProfile> saProfiles;
    SaProfile saProfile;
    OnDemandEvent onDemandEvent = {TIMED_EVENT, "mockevent", "40"};
    saProfile.stopOnDemand.onDemandEvents.push_back(onDemandEvent);
    saProfiles.push_back(saProfile);
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->Init(saProfiles);
    EXPECT_EQ(deviceTimedCollect->timedSet_.size(), 0);
}

/**
 * @tc.name: Init005
 * @tc.desc: test Init
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectTest, Init005, TestSize.Level3)
{
    std::list<SaProfile> saProfiles;
    SaProfile saProfile;
    OnDemandEvent onDemandEvent = {TIMED_EVENT, "loopevent", "20"};
    saProfile.stopOnDemand.onDemandEvents.push_back(onDemandEvent);
    saProfiles.push_back(saProfile);
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->Init(saProfiles);
    EXPECT_EQ(deviceTimedCollect->timedSet_.size(), 0);
}

/**
 * @tc.name: Init006
 * @tc.desc: test Init
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectTest, Init006, TestSize.Level3)
{
    std::list<SaProfile> saProfiles;
    SaProfile saProfile;
    OnDemandEvent onDemandEvent = {TIMED_EVENT, "loopevent", "20"};
    saProfile.startOnDemand.onDemandEvents.push_back(onDemandEvent);
    saProfiles.push_back(saProfile);
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->Init(saProfiles);
    EXPECT_EQ(deviceTimedCollect->timedSet_.size(), 0);
}

/**
 * @tc.name: Init007
 * @tc.desc: test Init
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectTest, Init007, TestSize.Level3)
{
    std::list<SaProfile> saProfiles;
    SaProfile saProfile;
    OnDemandEvent onDemandEvent = {TIMED_EVENT, "loopevent", "invalid"};
    saProfile.startOnDemand.onDemandEvents.push_back(onDemandEvent);
    saProfiles.push_back(saProfile);
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->Init(saProfiles);
    EXPECT_EQ(deviceTimedCollect->timedSet_.size(), 0);
}

/**
 * @tc.name: ReportEvent001
 * @tc.desc: test ReportEvent, report is nullptr
 * @tc.type: FUNC
 * @tc.require: I6OU0A
 */
HWTEST_F(DeviceTimedCollectTest, ReportEvent001, TestSize.Level3)
{
    sptr<DeviceStatusCollectManager> collect = nullptr;
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    OnDemandEvent event;
    deviceTimedCollect->ReportEvent(event);
    EXPECT_EQ(collect, nullptr);
}

/**
 * @tc.name: PostDelayTask001
 * @tc.desc: test ReportEvent, report is nullptr
 * @tc.type: FUNC
 * @tc.require: I6OU0A
 */
HWTEST_F(DeviceTimedCollectTest, PostDelayTask001, TestSize.Level3)
{
    sptr<DeviceStatusCollectManager> collect = nullptr;
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->PostDelayTask(deviceTimedCollect->loopTasks_[0], 0);
    EXPECT_EQ(collect, nullptr);
}

/**
 * @tc.name: PostDelayTask002
 * @tc.desc: test ReportEvent, report is not nullptr
 * @tc.type: FUNC
 * @tc.require: I6OU0A
 */
HWTEST_F(DeviceTimedCollectTest, PostDelayTask002, TestSize.Level3)
{
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    deviceTimedCollect->PostDelayTask(deviceTimedCollect->loopTasks_[0], 0);
    EXPECT_NE(collect, nullptr);
}

/**
 * @tc.name: AddCollectEvent001
 * @tc.desc: test AddCollectEvent, with event
 * @tc.type: FUNC
 * @tc.require: I6UUNW
 */
HWTEST_F(DeviceTimedCollectTest, AddCollectEvent001, TestSize.Level3)
{
    DTEST_LOG << "AddCollectEvent001 begin" << std::endl;
    sptr<DeviceStatusCollectManager> collect = new DeviceStatusCollectManager();
    std::shared_ptr<DeviceTimedCollect> deviceTimedCollect =
        std::make_shared<DeviceTimedCollect>(collect);
    OnDemandEvent event;
    int32_t ret = deviceTimedCollect->AddCollectEvent(event);
    EXPECT_EQ(ret, ERR_INVALID_VALUE);
    DTEST_LOG << "AddCollectEvent001 end" << std::endl;
}
}