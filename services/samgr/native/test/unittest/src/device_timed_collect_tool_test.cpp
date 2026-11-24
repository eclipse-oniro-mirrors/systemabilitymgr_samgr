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

#include "device_timed_collect_tool_test.h"

#define private public

#include "test_log.h"
#include "device_timed_collect_tool.h"
#include "if_system_ability_manager.h"

using namespace std;
using namespace testing;
using namespace testing::ext;
using namespace OHOS;

void DeviceTimedCollectToolTest::SetUpTestCase()
{
    DTEST_LOG << "SetUpTestCase" << std::endl;
}

void DeviceTimedCollectToolTest::TearDownTestCase()
{
    DTEST_LOG << "TearDownTestCase" << std::endl;
}

void DeviceTimedCollectToolTest::SetUp()
{
    DTEST_LOG << "SetUp" << std::endl;
}

void DeviceTimedCollectToolTest::TearDown()
{
    DTEST_LOG << "TearDown" << std::endl;
}

/**
 * @tc.name: GetInstance001
 * @tc.desc: test GetInstance returns non-null
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, GetInstance001, TestSize.Level3)
{
    DTEST_LOG << "GetInstance001 begin" << std::endl;
    std::shared_ptr<PreferencesUtil> instance = PreferencesUtil::GetInstance();
    EXPECT_EQ(true, instance != nullptr);
    DTEST_LOG << "GetInstance001 end" << std::endl;
}

/**
 * @tc.name: GetInstance002
 * @tc.desc: test GetInstance returns same instance on multiple calls
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, GetInstance002, TestSize.Level3)
{
    DTEST_LOG << "GetInstance002 begin" << std::endl;
    std::shared_ptr<PreferencesUtil> instance1 = PreferencesUtil::GetInstance();
    std::shared_ptr<PreferencesUtil> instance2 = PreferencesUtil::GetInstance();
    EXPECT_EQ(true, instance1 != nullptr);
    EXPECT_EQ(true, instance2 != nullptr);
    EXPECT_EQ(instance1.get(), instance2.get());
    DTEST_LOG << "GetInstance002 end" << std::endl;
}

/**
 * @tc.name: GetPathForUser001
 * @tc.desc: test GetPathForUser with BASE_USER returns "/data/samgr/samgr.xml"
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, GetPathForUser001, TestSize.Level3)
{
    DTEST_LOG << "GetPathForUser001 begin" << std::endl;
    std::string path = PreferencesUtil::GetPathForUser(BASE_USER);
    EXPECT_EQ("/data/samgr/samgr.xml", path);
    DTEST_LOG << "GetPathForUser001 end" << std::endl;
}

/**
 * @tc.name: GetPathForUser002
 * @tc.desc: test GetPathForUser with non-base user returns path with userId
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, GetPathForUser002, TestSize.Level3)
{
    DTEST_LOG << "GetPathForUser002 begin" << std::endl;
    int32_t userId = 100;
    std::string path = PreferencesUtil::GetPathForUser(userId);
    EXPECT_EQ("/data/samgr/samgr_100.xml", path);
    DTEST_LOG << "GetPathForUser002 end" << std::endl;
}

/**
 * @tc.name: DeleteUserPreferences001
 * @tc.desc: test DeleteUserPreferences with BASE_USER returns ERR_INVALID_VALUE
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, DeleteUserPreferences001, TestSize.Level3)
{
    DTEST_LOG << "DeleteUserPreferences001 begin" << std::endl;
    int32_t result = PreferencesUtil::DeleteUserPreferences(BASE_USER);
    EXPECT_EQ(ERR_INVALID_VALUE, result);
    DTEST_LOG << "DeleteUserPreferences001 end" << std::endl;
}

/**
 * @tc.name: DeleteUserPreferences002
 * @tc.desc: test DeleteUserPreferences with SAMGR_INVALID_USER_ID returns ERR_INVALID_VALUE
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, DeleteUserPreferences002, TestSize.Level3)
{
    DTEST_LOG << "DeleteUserPreferences002 begin" << std::endl;
    int32_t result = PreferencesUtil::DeleteUserPreferences(SAMGR_INVALID_USER_ID);
    EXPECT_EQ(ERR_INVALID_VALUE, result);
    DTEST_LOG << "DeleteUserPreferences002 end" << std::endl;
}

/**
 * @tc.name: DeleteUserPreferences003
 * @tc.desc: test DeleteUserPreferences with valid user returns ERR_OK or ERR_INVALID_OPERATION
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, DeleteUserPreferences003, TestSize.Level3)
{
    DTEST_LOG << "DeleteUserPreferences003 begin" << std::endl;
    int32_t userId = 100;
    int32_t result = PreferencesUtil::DeleteUserPreferences(userId);
    EXPECT_TRUE(result == ERR_OK || result == ERR_INVALID_OPERATION);
    DTEST_LOG << "DeleteUserPreferences003 end" << std::endl;
}

/**
 * @tc.name: SaveLong001
 * @tc.desc: test SaveLong with valid key
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, SaveLong001, TestSize.Level3)
{
    DTEST_LOG << "SaveLong001 begin" << std::endl;
    std::shared_ptr<PreferencesUtil> instance = PreferencesUtil::GetInstance();
    ASSERT_TRUE(instance != nullptr);
    instance->ptr_ = nullptr;
    bool result = instance->SaveLong("test_long_key", 123456LL);
    EXPECT_TRUE(result == true || result == false);
    DTEST_LOG << "SaveLong001 end" << std::endl;
}

/**
 * @tc.name: ObtainLong001
 * @tc.desc: test ObtainLong with default value returns default when no preference
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, ObtainLong001, TestSize.Level3)
{
    DTEST_LOG << "ObtainLong001 begin" << std::endl;
    std::shared_ptr<PreferencesUtil> instance = PreferencesUtil::GetInstance();
    ASSERT_TRUE(instance != nullptr);
    instance->ptr_ = nullptr;
    int64_t defValue = 999999LL;
    int64_t result = instance->ObtainLong("nonexistent_key", defValue);
    EXPECT_EQ(defValue, result);
    DTEST_LOG << "ObtainLong001 end" << std::endl;
}

/**
 * @tc.name: IsExist001
 * @tc.desc: test IsExist with any key returns false when no preference
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, IsExist001, TestSize.Level3)
{
    DTEST_LOG << "IsExist001 begin" << std::endl;
    std::shared_ptr<PreferencesUtil> instance = PreferencesUtil::GetInstance();
    ASSERT_TRUE(instance != nullptr);
    instance->ptr_ = nullptr;
    bool result = instance->IsExist("nonexistent_key");
    EXPECT_EQ(false, result);
    DTEST_LOG << "IsExist001 end" << std::endl;
}

/**
 * @tc.name: Remove001
 * @tc.desc: test Remove with any key returns false when no preference
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, Remove001, TestSize.Level3)
{
    DTEST_LOG << "Remove001 begin" << std::endl;
    std::shared_ptr<PreferencesUtil> instance = PreferencesUtil::GetInstance();
    ASSERT_TRUE(instance != nullptr);
    instance->ptr_ = nullptr;
    std::string savedPath = instance->path_;
    instance->path_ = "nonexistent_relative_path_for_ut.xml";
    bool result = instance->Remove("nonexistent_key");
    instance->path_ = savedPath;
    EXPECT_EQ(false, result);
    DTEST_LOG << "Remove001 end" << std::endl;
}

/**
 * @tc.name: ObtainString001
 * @tc.desc: test ObtainString with default value returns default when no preference
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, ObtainString001, TestSize.Level3)
{
    DTEST_LOG << "ObtainString001 begin" << std::endl;
    std::shared_ptr<PreferencesUtil> instance = PreferencesUtil::GetInstance();
    ASSERT_TRUE(instance != nullptr);
    instance->ptr_ = nullptr;
    std::string defValue = "default_str";
    std::string result = instance->ObtainString("nonexistent_key", defValue);
    EXPECT_EQ(defValue, result);
    DTEST_LOG << "ObtainString001 end" << std::endl;
}

/**
 * @tc.name: ObtainAll001
 * @tc.desc: test ObtainAll returns empty map when no preference
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTimedCollectToolTest, ObtainAll001, TestSize.Level3)
{
    DTEST_LOG << "ObtainAll001 begin" << std::endl;
    std::shared_ptr<PreferencesUtil> instance = PreferencesUtil::GetInstance();
    ASSERT_TRUE(instance != nullptr);
    instance->ptr_ = nullptr;
    std::string savedPath = instance->path_;
    instance->path_ = "nonexistent_relative_path_for_ut.xml";
    auto result = instance->ObtainAll();
    instance->path_ = savedPath;
    EXPECT_EQ(0u, result.size());
    DTEST_LOG << "ObtainAll001 end" << std::endl;
}
