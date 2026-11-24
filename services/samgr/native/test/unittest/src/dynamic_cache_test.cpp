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

#include "dynamic_cache_test.h"

#define private public

#include "test_log.h"
#include "dynamic_cache.h"

using namespace std;
using namespace testing;
using namespace testing::ext;
using namespace OHOS;

namespace OHOS {
class TestDynamicCache : public DynamicCache {
public:
    using DynamicCache::DynamicCache;
    sptr<IRemoteObject> Recompute(int32_t querySaId, int32_t code) override
    {
        recomputeCalled = true;
        return DynamicCache::Recompute(querySaId, code);
    }
    bool recomputeCalled = false;
};
} // namespace OHOS

void DynamicCacheTest::SetUpTestCase()
{
    DTEST_LOG << "SetUpTestCase" << std::endl;
}

void DynamicCacheTest::TearDownTestCase()
{
    DTEST_LOG << "TearDownTestCase" << std::endl;
}

void DynamicCacheTest::SetUp()
{
    DTEST_LOG << "SetUp" << std::endl;
}

void DynamicCacheTest::TearDown()
{
    DTEST_LOG << "TearDown" << std::endl;
}

/**
 * @tc.name: CanUseCache001
 * @tc.desc: test CanUseCache with no prior cache returns false
 * @tc.type: FUNC
 */
HWTEST_F(DynamicCacheTest, CanUseCache001, TestSize.Level3)
{
    DTEST_LOG << "CanUseCache001 begin" << std::endl;
    TestDynamicCache cache;
    char waterLine[128] = {0};
    std::string defaultValue = "default";
    bool result = cache.CanUseCache(1, waterLine, defaultValue);
    EXPECT_EQ(false, result);
    DTEST_LOG << "CanUseCache001 end" << std::endl;
}

/**
 * @tc.name: ClearCache001
 * @tc.desc: test ClearCache clears lastQuerySaId_ and lastQuerySaProxy_
 * @tc.type: FUNC
 */
HWTEST_F(DynamicCacheTest, ClearCache001, TestSize.Level3)
{
    DTEST_LOG << "ClearCache001 begin" << std::endl;
    TestDynamicCache cache;
    cache.lastQuerySaId_ = 100;
    cache.lastQuerySaProxy_ = nullptr;
    cache.ClearCache();
    EXPECT_EQ(-1, cache.lastQuerySaId_);
    EXPECT_EQ(true, cache.lastQuerySaProxy_ == nullptr);
    DTEST_LOG << "ClearCache001 end" << std::endl;
}

/**
 * @tc.name: QueryResult001
 * @tc.desc: test QueryResult with no cache calls Recompute and returns nullptr
 * @tc.type: FUNC
 */
HWTEST_F(DynamicCacheTest, QueryResult001, TestSize.Level3)
{
    DTEST_LOG << "QueryResult001 begin" << std::endl;
    TestDynamicCache cache;
    sptr<IRemoteObject> result = cache.QueryResult(1, 0);
    EXPECT_EQ(true, result == nullptr);
    EXPECT_EQ(true, cache.recomputeCalled);
    DTEST_LOG << "QueryResult001 end" << std::endl;
}

/**
 * @tc.name: OnRemoteDied001
 * @tc.desc: test OnRemoteDied calls ClearCache with no crash on empty cache
 * @tc.type: FUNC
 */
HWTEST_F(DynamicCacheTest, OnRemoteDied001, TestSize.Level3)
{
    DTEST_LOG << "OnRemoteDied001 begin" << std::endl;
    TestDynamicCache cache;
    wptr<IRemoteObject> remote(nullptr);
    cache.OnRemoteDied(remote);
    EXPECT_EQ(-1, cache.lastQuerySaId_);
    EXPECT_EQ(true, cache.lastQuerySaProxy_ == nullptr);
    DTEST_LOG << "OnRemoteDied001 end" << std::endl;
}

/**
 * @tc.name: Recompute001
 * @tc.desc: test Recompute with mismatched SAId returns nullptr
 * @tc.type: FUNC
 */
HWTEST_F(DynamicCacheTest, Recompute001, TestSize.Level3)
{
    DTEST_LOG << "Recompute001 begin" << std::endl;
    TestDynamicCache cache;
    cache.lastQuerySaId_ = 100;
    sptr<IRemoteObject> result = cache.Recompute(200, 0);
    EXPECT_EQ(true, result == nullptr);
    DTEST_LOG << "Recompute001 end" << std::endl;
}

/**
 * @tc.name: Recompute002
 * @tc.desc: test Recompute with matching SAId but null proxy returns nullptr
 * @tc.type: FUNC
 */
HWTEST_F(DynamicCacheTest, Recompute002, TestSize.Level3)
{
    DTEST_LOG << "Recompute002 begin" << std::endl;
    TestDynamicCache cache;
    cache.lastQuerySaId_ = 100;
    cache.lastQuerySaProxy_ = nullptr;
    sptr<IRemoteObject> result = cache.Recompute(100, 0);
    EXPECT_EQ(true, result == nullptr);
    DTEST_LOG << "Recompute002 end" << std::endl;
}

/**
 * @tc.name: CanUseCache002
 * @tc.desc: test CanUseCache with defaultValue equal to waterLine returns false
 * @tc.type: FUNC
 */
HWTEST_F(DynamicCacheTest, CanUseCache002, TestSize.Level3)
{
    DTEST_LOG << "CanUseCache002 begin" << std::endl;
    TestDynamicCache cache;
    char waterLine[128] = "default";
    std::string defaultValue = "default";
    bool result = cache.CanUseCache(1, waterLine, defaultValue);
    EXPECT_EQ(false, result);
    DTEST_LOG << "CanUseCache002 end" << std::endl;
}

/**
 * @tc.name: QueryResult002
 * @tc.desc: test QueryResult with CanUseCache returning false triggers Recompute
 * @tc.type: FUNC
 */
HWTEST_F(DynamicCacheTest, QueryResult002, TestSize.Level3)
{
    DTEST_LOG << "QueryResult002 begin" << std::endl;
    TestDynamicCache cache;
    cache.lastQuerySaId_ = 1;
    sptr<IRemoteObject> result = cache.QueryResult(1, 0);
    EXPECT_EQ(true, result == nullptr);
    EXPECT_EQ(true, cache.recomputeCalled);
    DTEST_LOG << "QueryResult002 end" << std::endl;
}
