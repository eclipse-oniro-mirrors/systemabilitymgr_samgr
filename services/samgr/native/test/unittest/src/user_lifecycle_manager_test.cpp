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

#include "system_ability_mgr_test.h"

#include "multi_system_ability_manager.h"
#include "sa_status_change_mock.h"
#include "system_ability_manager.h"
#include "system_process_status_change_proxy.h"
#include "test_log.h"

using namespace std;
using namespace testing;
using namespace testing::ext;

namespace OHOS {
/**
 * @tc.name: UserLifecycleStateValidation001
 * @tc.desc: Verify lifecycle state validation before user profiles are initialized.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleStateValidation001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testUserId = 100;
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, static_cast<SamgrUserState>(-1)), ERR_INVALID_VALUE);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_INVALID_VALUE);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_SWITCHING), ERR_INVALID_VALUE);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_INVALID_VALUE);
    EXPECT_FALSE(lifecycleManager.IsValidUser(testUserId));
}

/**
 * @tc.name: UserLifecycleManagerLookup001
 * @tc.desc: Verify manager lookup and switching lifecycle branches.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleManagerLookup001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testUserId = 100;
    auto manager = std::make_shared<MultiSystemAbilityManager>(testUserId);
    ASSERT_NE(manager, nullptr);
    lifecycleManager.multiUserManagers_[testUserId] = manager;
    lifecycleManager.validUserIds_.insert(testUserId);
    EXPECT_EQ(lifecycleManager.GetActiveUserCount(), 1U);
    EXPECT_EQ(lifecycleManager.GetMultiUserManager(testUserId), manager);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_SWITCHING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_SWITCHING), ERR_OK);
    lifecycleManager.userStateMap_[testUserId] = USER_STATE_STOPPING;
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_INVALID_OPERATION);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_SWITCHING), ERR_INVALID_VALUE);
}

/**
 * @tc.name: UserLifecycleAllUsersAbilitySubscription001
 * @tc.desc: Verify global ability subscription validation and deduplication.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleAllUsersAbilitySubscription001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testSaId = 2235;
    constexpr int32_t testPid = 1000;
    sptr<ISystemAbilityStatusChange> listener = new SaStatusChangeMock();
    ASSERT_NE(listener, nullptr);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(testSaId, nullptr, testPid), ERR_INVALID_VALUE);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(testSaId, listener, testPid), ERR_OK);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(testSaId, listener, testPid), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.size(), 1U);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemAbilityForAllUsers(testSaId, listener), ERR_OK);
    EXPECT_TRUE(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.empty());
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemAbilityForAllUsers(testSaId, listener), ERR_OK);
}

/**
 * @tc.name: UserLifecycleAllUsersAbilitySubscriptionLimit001
 * @tc.desc: Verify global ability subscription limit is enforced per caller.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleAllUsersAbilitySubscriptionLimit001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testPid = 1000;
    sptr<ISystemAbilityStatusChange> listener = new SaStatusChangeMock();
    ASSERT_NE(listener, nullptr);
    for (int32_t index = 0; index < BaseSystemAbilityManager::MAX_SUBSCRIBE_COUNT; ++index) {
        ASSERT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(3000 + index, listener, testPid), ERR_OK);
    }
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(4000, listener, testPid), ERR_PERMISSION_DENIED);
}

/**
 * @tc.name: UserLifecycleAllUsersProcessSubscription001
 * @tc.desc: Verify global process subscription validation and deduplication.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleAllUsersProcessSubscription001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    bool subscriptionAdded = true;
    sptr<ISystemProcessStatusChange> listener = new SystemProcessStatusChange();
    ASSERT_NE(listener, nullptr);
    EXPECT_EQ(lifecycleManager.SubscribeSystemProcessForAllUsers(nullptr, subscriptionAdded), ERR_INVALID_VALUE);
    EXPECT_FALSE(subscriptionAdded);
    EXPECT_EQ(lifecycleManager.SubscribeSystemProcessForAllUsers(listener, subscriptionAdded), ERR_OK);
    EXPECT_TRUE(subscriptionAdded);
    EXPECT_EQ(lifecycleManager.SubscribeSystemProcessForAllUsers(listener, subscriptionAdded), ERR_OK);
    EXPECT_FALSE(subscriptionAdded);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemProcessForAllUsers(listener), ERR_OK);
    EXPECT_TRUE(lifecycleManager.globalSubscriptions_.systemProcessSubscriptions.empty());
}

/**
 * @tc.name: UserLifecycleStoppingContext001
 * @tc.desc: Verify stopping context lookup and cleanup paths.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleStoppingContext001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testUserId = 100;
    auto manager = std::make_shared<MultiSystemAbilityManager>(testUserId);
    auto context = std::make_shared<UserLifecycleManager::UserStopping>();
    ASSERT_NE(manager, nullptr);
    ASSERT_NE(context, nullptr);
    context->manager = manager;
    lifecycleManager.userStoppingContexts_[testUserId] = context;
    EXPECT_TRUE(lifecycleManager.IsUserStopping(testUserId));
    EXPECT_EQ(lifecycleManager.GetStoppingMultiUserManager(testUserId), manager);
    lifecycleManager.userStoppingContexts_[testUserId] = nullptr;
    EXPECT_EQ(lifecycleManager.GetStoppingMultiUserManager(testUserId), nullptr);
    lifecycleManager.CancelPendingStopTasks();
    EXPECT_FALSE(lifecycleManager.IsUserStopping(testUserId));
}

/**
 * @tc.name: UserLifecycleSubscriptionManagerLoop001
 * @tc.desc: Verify global subscriptions iterate active managers and retain global state on failure.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleSubscriptionManagerLoop001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    auto firstManager = std::make_shared<MultiSystemAbilityManager>(100);
    auto secondManager = std::make_shared<MultiSystemAbilityManager>(101);
    ASSERT_NE(firstManager, nullptr);
    ASSERT_NE(secondManager, nullptr);
    lifecycleManager.multiUserManagers_[100] = firstManager;
    lifecycleManager.multiUserManagers_[101] = secondManager;
    sptr<ISystemAbilityStatusChange> abilityListener = new SaStatusChangeMock();
    sptr<ISystemProcessStatusChange> processListener = new SystemProcessStatusChange();
    ASSERT_NE(abilityListener, nullptr);
    ASSERT_NE(processListener, nullptr);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(2235, abilityListener, 1000), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.size(), 1U);
    bool added = false;
    EXPECT_NE(lifecycleManager.SubscribeSystemProcessForAllUsers(processListener, added), ERR_OK);
    EXPECT_FALSE(added);
    EXPECT_TRUE(lifecycleManager.globalSubscriptions_.systemProcessSubscriptions.empty());
}

/**
 * @tc.name: UserLifecycleActivateStop001
 * @tc.desc: Verify successful activation and empty-process stop completion.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleActivateStop001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    std::list<SaProfile> profiles;
    lifecycleManager.SetSaProfiles(&profiles);
    constexpr int32_t testUserId = 102;
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    ASSERT_NE(lifecycleManager.GetMultiUserManager(testUserId), nullptr);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_OK);
    EXPECT_FALSE(lifecycleManager.IsUserStopping(testUserId));
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_INVALID_VALUE);
}

/**
 * @tc.name: UserLifecycleExpiredSubscriptionCleanup001
 * @tc.desc: Verify null global subscriptions are removed before registration.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleExpiredSubscriptionCleanup001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.push_back({2235, nullptr, 1});
    lifecycleManager.globalSubscriptions_.systemProcessSubscriptions.push_back(nullptr);
    sptr<ISystemAbilityStatusChange> listener = new SaStatusChangeMock();
    ASSERT_NE(listener, nullptr);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(2236, listener, 1), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.size(), 1U);
    bool added = false;
    sptr<ISystemProcessStatusChange> processListener = new SystemProcessStatusChange();
    ASSERT_NE(processListener, nullptr);
    EXPECT_EQ(lifecycleManager.SubscribeSystemProcessForAllUsers(processListener, added), ERR_OK);
    EXPECT_TRUE(added);
}

/**
 * @tc.name: UserLifecycleQueryEmpty001
 * @tc.desc: Verify empty lifecycle queries and missing manager lookups.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleQueryEmpty001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testUserId = 103;
    ASSERT_EQ(lifecycleManager.GetActiveUserCount(), 0U);
    EXPECT_FALSE(lifecycleManager.IsValidUser(testUserId));
    EXPECT_EQ(lifecycleManager.GetMultiUserManager(testUserId), nullptr);
    EXPECT_EQ(lifecycleManager.GetStoppingMultiUserManager(testUserId), nullptr);
    EXPECT_FALSE(lifecycleManager.IsUserStopping(testUserId));
    EXPECT_TRUE(lifecycleManager.GetValidUserIds().empty());
}

/**
 * @tc.name: UserLifecycleUnsubscribeValidation001
 * @tc.desc: Verify invalid and absent global unsubscribe requests.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleUnsubscribeValidation001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    sptr<ISystemAbilityStatusChange> abilityListener = new SaStatusChangeMock();
    sptr<ISystemProcessStatusChange> processListener = new SystemProcessStatusChange();
    ASSERT_NE(abilityListener, nullptr);
    ASSERT_NE(processListener, nullptr);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemAbilityForAllUsers(2235, nullptr), ERR_INVALID_VALUE);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemProcessForAllUsers(nullptr), ERR_INVALID_VALUE);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemAbilityForAllUsers(2235, abilityListener), ERR_OK);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemProcessForAllUsers(processListener), ERR_OK);
    EXPECT_TRUE(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.empty());
    EXPECT_TRUE(lifecycleManager.globalSubscriptions_.systemProcessSubscriptions.empty());
}

/**
 * @tc.name: UserLifecycleLockReuse001
 * @tc.desc: Verify each user gets a stable lifecycle lock.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleLockReuse001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    auto firstLock = lifecycleManager.GetUserLifecycleLock(104);
    auto sameLock = lifecycleManager.GetUserLifecycleLock(104);
    auto otherLock = lifecycleManager.GetUserLifecycleLock(105);
    ASSERT_NE(firstLock, nullptr);
    ASSERT_NE(otherLock, nullptr);
    EXPECT_EQ(firstLock, sameLock);
    EXPECT_NE(firstLock, otherLock);
}

/**
 * @tc.name: UserLifecycleStopWithoutManager001
 * @tc.desc: Verify stopping a user without a manager restores lifecycle state.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleStopWithoutManager001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testUserId = 106;
    lifecycleManager.validUserIds_.insert(testUserId);
    ASSERT_TRUE(lifecycleManager.IsValidUser(testUserId));
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_INVALID_VALUE);
    EXPECT_FALSE(lifecycleManager.IsValidUser(testUserId));
    EXPECT_EQ(lifecycleManager.userStateMap_.count(testUserId), 0U);
}

/**
 * @tc.name: UserLifecycleCompletionGuard001
 * @tc.desc: Verify completion ignores null and already-finalized contexts.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleCompletionGuard001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testUserId = 107;
    auto context = std::make_shared<UserLifecycleManager::UserStopping>();
    ASSERT_NE(context, nullptr);
    lifecycleManager.CompleteUserStopping(testUserId, nullptr);
    lifecycleManager.CompleteUserStopping(testUserId, context);
    context->finalizing.store(true);
    lifecycleManager.CompleteUserStopping(testUserId, context);
    EXPECT_FALSE(lifecycleManager.IsUserStopping(testUserId));
}

/**
 * @tc.name: UserLifecycleConcurrentStateTransition001
 * @tc.desc: Verify rapid state transitions for the same user converge correctly.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleConcurrentStateTransition001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    std::list<SaProfile> profiles;
    lifecycleManager.SetSaProfiles(&profiles);
    constexpr int32_t testUserId = 200;
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_SWITCHING), ERR_OK);
    EXPECT_EQ(lifecycleManager.GetForegroundUserId(), testUserId);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_SWITCHING), ERR_OK);
    EXPECT_EQ(lifecycleManager.GetForegroundUserId(), testUserId);
    EXPECT_EQ(lifecycleManager.GetActiveUserCount(), 1U);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_OK);
}

/**
 * @tc.name: UserLifecycleForegroundSwitch001
 * @tc.desc: Verify foreground user switches when another user becomes foreground.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleForegroundSwitch001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    std::list<SaProfile> profiles;
    lifecycleManager.SetSaProfiles(&profiles);
    constexpr int32_t firstUserId = 201;
    constexpr int32_t secondUserId = 202;
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(firstUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(firstUserId, USER_STATE_SWITCHING), ERR_OK);
    EXPECT_EQ(lifecycleManager.GetForegroundUserId(), firstUserId);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(secondUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(secondUserId, USER_STATE_SWITCHING), ERR_OK);
    EXPECT_EQ(lifecycleManager.GetForegroundUserId(), secondUserId);
    EXPECT_EQ(lifecycleManager.GetActiveUserCount(), 2U);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(secondUserId, USER_STATE_STOPPING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(firstUserId, USER_STATE_STOPPING), ERR_OK);
}

/**
 * @tc.name: UserLifecycleMultiSaIdSubscriptionDedup001
 * @tc.desc: Verify per-SAId deduplication when subscribing multiple SAIds with same listener.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleMultiSaIdSubscriptionDedup001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testPid = 2000;
    sptr<ISystemAbilityStatusChange> listener = new SaStatusChangeMock();
    ASSERT_NE(listener, nullptr);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(5001, listener, testPid), ERR_OK);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(5002, listener, testPid), ERR_OK);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(5003, listener, testPid), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.size(), 3U);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(5001, listener, testPid), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.size(), 3U);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemAbilityForAllUsers(5002, listener), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.size(), 2U);
}

/**
 * @tc.name: UserLifecycleUnsubscribeNonSubscribedSaId001
 * @tc.desc: Verify unsubscribing a non-subscribed SAId is a safe no-op.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleUnsubscribeNonSubscribedSaId001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    sptr<ISystemAbilityStatusChange> listener = new SaStatusChangeMock();
    ASSERT_NE(listener, nullptr);
    EXPECT_EQ(lifecycleManager.SubscribeSystemAbilityForAllUsers(6001, listener, 3000), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.size(), 1U);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemAbilityForAllUsers(9999, listener), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemAbilitySubscriptions.size(), 1U);
}

/**
 * @tc.name: UserLifecycleMultiProcessListeners001
 * @tc.desc: Verify multiple process listeners can subscribe and unsubscribe independently.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleMultiProcessListeners001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    sptr<ISystemProcessStatusChange> listener1 = new SystemProcessStatusChange();
    sptr<ISystemProcessStatusChange> listener2 = new SystemProcessStatusChange();
    ASSERT_NE(listener1, nullptr);
    ASSERT_NE(listener2, nullptr);
    bool added1 = false;
    bool added2 = false;
    EXPECT_EQ(lifecycleManager.SubscribeSystemProcessForAllUsers(listener1, added1), ERR_OK);
    EXPECT_TRUE(added1);
    EXPECT_EQ(lifecycleManager.SubscribeSystemProcessForAllUsers(listener2, added2), ERR_OK);
    EXPECT_TRUE(added2);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemProcessSubscriptions.size(), 2U);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemProcessForAllUsers(listener1), ERR_OK);
    EXPECT_EQ(lifecycleManager.globalSubscriptions_.systemProcessSubscriptions.size(), 1U);
    EXPECT_EQ(lifecycleManager.UnSubscribeSystemProcessForAllUsers(listener2), ERR_OK);
    EXPECT_TRUE(lifecycleManager.globalSubscriptions_.systemProcessSubscriptions.empty());
}

/**
 * @tc.name: UserLifecycleStopAlreadyStopped001
 * @tc.desc: Verify stopping a user that has already been stopped returns error.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleStopAlreadyStopped001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    std::list<SaProfile> profiles;
    lifecycleManager.SetSaProfiles(&profiles);
    constexpr int32_t testUserId = 203;
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_NE(lifecycleManager.GetMultiUserManager(testUserId), nullptr);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_OK);
    EXPECT_FALSE(lifecycleManager.IsValidUser(testUserId));
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_INVALID_VALUE);
    EXPECT_FALSE(lifecycleManager.IsUserStopping(testUserId));
}

/**
 * @tc.name: UserLifecycleActivateAlreadyActive001
 * @tc.desc: Verify activating an already-active user is a safe no-op.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleActivateAlreadyActive001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    std::list<SaProfile> profiles;
    lifecycleManager.SetSaProfiles(&profiles);
    constexpr int32_t testUserId = 204;
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_NE(lifecycleManager.GetMultiUserManager(testUserId), nullptr);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.GetActiveUserCount(), 1U);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_OK);
}

/**
 * @tc.name: UserLifecycleSwitchSameUser001
 * @tc.desc: Verify switching to the already-foreground user is a no-op.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleSwitchSameUser001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    std::list<SaProfile> profiles;
    lifecycleManager.SetSaProfiles(&profiles);
    constexpr int32_t testUserId = 205;
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_SWITCHING), ERR_OK);
    EXPECT_EQ(lifecycleManager.GetForegroundUserId(), testUserId);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_SWITCHING), ERR_OK);
    EXPECT_EQ(lifecycleManager.GetForegroundUserId(), testUserId);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_OK);
}

/**
 * @tc.name: UserLifecycleValidUserIdsAfterMultiActivate001
 * @tc.desc: Verify GetValidUserIds returns correct set after multiple activations and a stop.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleValidUserIdsAfterMultiActivate001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    std::list<SaProfile> profiles;
    lifecycleManager.SetSaProfiles(&profiles);
    constexpr int32_t userId1 = 206;
    constexpr int32_t userId2 = 207;
    constexpr int32_t userId3 = 208;
    EXPECT_TRUE(lifecycleManager.GetValidUserIds().empty());
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(userId1, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(userId2, USER_STATE_ACTIVATING), ERR_OK);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(userId3, USER_STATE_ACTIVATING), ERR_OK);
    auto validIds = lifecycleManager.GetValidUserIds();
    EXPECT_EQ(validIds.size(), 3U);
    EXPECT_TRUE(validIds.count(userId1) > 0);
    EXPECT_TRUE(validIds.count(userId2) > 0);
    EXPECT_TRUE(validIds.count(userId3) > 0);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(userId2, USER_STATE_STOPPING), ERR_OK);
    validIds = lifecycleManager.GetValidUserIds();
    EXPECT_EQ(validIds.size(), 2U);
    EXPECT_TRUE(validIds.count(userId2) == 0);
}

/**
 * @tc.name: UserLifecycleStopWhileStopping001
 * @tc.desc: Verify stopping a user that is already in stopping state is skipped.
 * @tc.type: FUNC
 */
HWTEST_F(SystemAbilityMgrTest, UserLifecycleStopWhileStopping001, TestSize.Level3)
{
    UserLifecycleManager lifecycleManager;
    constexpr int32_t testUserId = 209;
    auto manager = std::make_shared<MultiSystemAbilityManager>(testUserId);
    ASSERT_NE(manager, nullptr);
    auto context = std::make_shared<UserLifecycleManager::UserStopping>();
    ASSERT_NE(context, nullptr);
    context->manager = manager;
    lifecycleManager.userStoppingContexts_[testUserId] = context;
    lifecycleManager.userStateMap_[testUserId] = USER_STATE_STOPPING;
    lifecycleManager.validUserIds_.insert(testUserId);
    EXPECT_EQ(lifecycleManager.OnUserStateChanged(testUserId, USER_STATE_STOPPING), ERR_OK);
    EXPECT_TRUE(lifecycleManager.IsUserStopping(testUserId));
    lifecycleManager.CancelPendingStopTasks();
}

} // namespace OHOS
