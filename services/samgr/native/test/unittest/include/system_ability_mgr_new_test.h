/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef SAMGR_TEST_UNITTEST_INCLUDE_SYSTEM_ABILITY_MGR_NEW_TEST_H
#define SAMGR_TEST_UNITTEST_INCLUDE_SYSTEM_ABILITY_MGR_NEW_TEST_H

#include "gtest/gtest.h"
#include "system_ability_mgr_test.h"

namespace OHOS {
class SystemAbilityMgrNewTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
} // OHOS
#endif /* SAMGR_TEST_UNITTEST_INCLUDE_SYSTEM_ABILITY_MGR_NEW_TEST_H */
