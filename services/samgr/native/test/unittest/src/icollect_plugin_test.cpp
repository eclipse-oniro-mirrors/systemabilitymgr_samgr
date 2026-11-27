/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "icollect_plugin_test.h"

#include "test_log.h"
#include "icollect_plugin.h"
#include "ireport.h"
#include "base_system_ability_manager.h"

using namespace std;
using namespace testing;
using namespace testing::ext;
using namespace OHOS;

namespace OHOS {
class MockIReport : public IReport {
public:
    void ReportEvent(const OnDemandEvent& event) override
    {
        reportEventCalled = true;
        lastEvent = event;
    }

    void PostTask(std::function<void()> callback) override
    {
        postTaskCalled = true;
        savedCallback = std::move(callback);
    }

    void PostDelayTask(std::function<void()> callback, int32_t delayTime) override
    {
        postDelayTaskCalled = true;
        lastDelayTime = delayTime;
        savedDelayCallback = std::move(callback);
    }

    bool reportEventCalled = false;
    bool postTaskCalled = false;
    bool postDelayTaskCalled = false;
    OnDemandEvent lastEvent;
    int32_t lastDelayTime = -1;
    std::function<void()> savedCallback;
    std::function<void()> savedDelayCallback;
};

class TestCollectPlugin : public ICollectPlugin {
public:
    using ICollectPlugin::ICollectPlugin;
    int32_t OnStart() override { return ERR_OK; }
    int32_t OnStop() override { return ERR_OK; }
};

class TestBaseManager final : public BaseSystemAbilityManager {
public:
    TestBaseManager() = default;
};
} // namespace OHOS

void ICollectPluginTest::SetUpTestCase()
{
    DTEST_LOG << "SetUpTestCase" << std::endl;
}

void ICollectPluginTest::TearDownTestCase()
{
    DTEST_LOG << "TearDownTestCase" << std::endl;
}

void ICollectPluginTest::SetUp()
{
    DTEST_LOG << "SetUp" << std::endl;
}

void ICollectPluginTest::TearDown()
{
    DTEST_LOG << "TearDown" << std::endl;
}

/**
 * @tc.name: GetManager001
 * @tc.desc: test constructor with valid report and empty manager
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, GetManager001, TestSize.Level3)
{
    DTEST_LOG << "GetManager001 begin" << std::endl;
    sptr<MockIReport> report = new MockIReport();
    std::weak_ptr<BaseSystemAbilityManager> emptyManager;
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(report, emptyManager);
    EXPECT_EQ(true, plugin != nullptr);
    EXPECT_EQ(true, plugin->GetManager().expired());
    DTEST_LOG << "GetManager001 end" << std::endl;
}

/**
 * @tc.name: GetManager002
 * @tc.desc: test constructor with valid report and valid manager
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, GetManager002, TestSize.Level3)
{
    DTEST_LOG << "GetManager002 begin" << std::endl;
    sptr<MockIReport> report = new MockIReport();
    std::shared_ptr<TestBaseManager> manager = std::make_shared<TestBaseManager>();
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(report, manager);
    EXPECT_EQ(true, plugin != nullptr);
    EXPECT_EQ(false, plugin->GetManager().expired());
    auto lockedManager = plugin->GetManager().lock();
    EXPECT_EQ(true, lockedManager != nullptr);
    DTEST_LOG << "GetManager002 end" << std::endl;
}

/**
 * @tc.name: ReportEvent001
 * @tc.desc: test ReportEvent with valid report delegates to report_->ReportEvent
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, ReportEvent001, TestSize.Level3)
{
    DTEST_LOG << "ReportEvent001 begin" << std::endl;
    sptr<MockIReport> report = new MockIReport();
    std::weak_ptr<BaseSystemAbilityManager> emptyManager;
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(report, emptyManager);
    OnDemandEvent event;
    event.eventId = 1;
    event.name = "test_event";
    event.value = "test_value";
    plugin->ReportEvent(event);
    EXPECT_EQ(true, report->reportEventCalled);
    EXPECT_EQ(1, report->lastEvent.eventId);
    EXPECT_EQ("test_event", report->lastEvent.name);
    EXPECT_EQ("test_value", report->lastEvent.value);
    DTEST_LOG << "ReportEvent001 end" << std::endl;
}

/**
 * @tc.name: ReportEvent002
 * @tc.desc: test ReportEvent with null report does not crash
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, ReportEvent002, TestSize.Level3)
{
    DTEST_LOG << "ReportEvent002 begin" << std::endl;
    sptr<IReport> nullReport = nullptr;
    std::weak_ptr<BaseSystemAbilityManager> emptyManager;
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(nullReport, emptyManager);
    OnDemandEvent event;
    event.eventId = 1;
    event.name = "test_event";
    event.value = "test_value";
    plugin->ReportEvent(event);
    EXPECT_EQ(true, plugin != nullptr);
    DTEST_LOG << "ReportEvent002 end" << std::endl;
}

/**
 * @tc.name: PostTask001
 * @tc.desc: test PostTask with valid report delegates to report_->PostTask
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, PostTask001, TestSize.Level3)
{
    DTEST_LOG << "PostTask001 begin" << std::endl;
    sptr<MockIReport> report = new MockIReport();
    std::weak_ptr<BaseSystemAbilityManager> emptyManager;
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(report, emptyManager);
    bool callbackCalled = false;
    plugin->PostTask([&callbackCalled]() { callbackCalled = true; });
    EXPECT_EQ(true, report->postTaskCalled);
    ASSERT_TRUE(report->savedCallback != nullptr);
    report->savedCallback();
    EXPECT_EQ(true, callbackCalled);
    DTEST_LOG << "PostTask001 end" << std::endl;
}

/**
 * @tc.name: PostTask002
 * @tc.desc: test PostTask with null report does not crash
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, PostTask002, TestSize.Level3)
{
    DTEST_LOG << "PostTask002 begin" << std::endl;
    sptr<IReport> nullReport = nullptr;
    std::weak_ptr<BaseSystemAbilityManager> emptyManager;
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(nullReport, emptyManager);
    plugin->PostTask([]() {});
    EXPECT_EQ(true, plugin != nullptr);
    DTEST_LOG << "PostTask002 end" << std::endl;
}

/**
 * @tc.name: PostDelayTask001
 * @tc.desc: test PostDelayTask with valid report delegates to report_->PostDelayTask
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, PostDelayTask001, TestSize.Level3)
{
    DTEST_LOG << "PostDelayTask001 begin" << std::endl;
    sptr<MockIReport> report = new MockIReport();
    std::weak_ptr<BaseSystemAbilityManager> emptyManager;
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(report, emptyManager);
    int32_t delayTime = 100;
    bool callbackCalled = false;
    plugin->PostDelayTask([&callbackCalled]() { callbackCalled = true; }, delayTime);
    EXPECT_EQ(true, report->postDelayTaskCalled);
    EXPECT_EQ(100, report->lastDelayTime);
    ASSERT_TRUE(report->savedDelayCallback != nullptr);
    report->savedDelayCallback();
    EXPECT_EQ(true, callbackCalled);
    DTEST_LOG << "PostDelayTask001 end" << std::endl;
}

/**
 * @tc.name: PostDelayTask002
 * @tc.desc: test PostDelayTask with null report does not crash
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, PostDelayTask002, TestSize.Level3)
{
    DTEST_LOG << "PostDelayTask002 begin" << std::endl;
    sptr<IReport> nullReport = nullptr;
    std::weak_ptr<BaseSystemAbilityManager> emptyManager;
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(nullReport, emptyManager);
    plugin->PostDelayTask([]() {}, 200);
    EXPECT_EQ(true, plugin != nullptr);
    DTEST_LOG << "PostDelayTask002 end" << std::endl;
}

/**
 * @tc.name: OnStartAndOnStop001
 * @tc.desc: test OnStart and OnStop return ERR_OK
 * @tc.type: FUNC
 */
HWTEST_F(ICollectPluginTest, OnStartAndOnStop001, TestSize.Level3)
{
    DTEST_LOG << "OnStartAndOnStop001 begin" << std::endl;
    sptr<MockIReport> report = new MockIReport();
    std::weak_ptr<BaseSystemAbilityManager> emptyManager;
    sptr<TestCollectPlugin> plugin = new TestCollectPlugin(report, emptyManager);
    EXPECT_EQ(ERR_OK, plugin->OnStart());
    EXPECT_EQ(ERR_OK, plugin->OnStop());
    DTEST_LOG << "OnStartAndOnStop001 end" << std::endl;
}
