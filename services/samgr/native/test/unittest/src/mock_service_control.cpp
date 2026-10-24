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

#include "mock_service_control.h"

MockServiceControlState g_mockServiceControl;

void ResetMockServiceControl()
{
    g_mockServiceControl = MockServiceControlState();
}

void SetMockServiceControlReturn(int32_t val)
{
    g_mockServiceControl.returnVal = val;
}

void SetMockServiceControlReturnSeq(const std::vector<int32_t>& seq)
{
    g_mockServiceControl.returnSeq = seq;
    g_mockServiceControl.useSeq = true;
}

void SetMockServiceControlWaitReturn(int32_t val)
{
    g_mockServiceControl.waitReturnVal = val;
}

extern "C" int ServiceControlWithExtra(const char* name, int action, const char** extra, int argc)
{
    (void)name;
    (void)extra;
    (void)argc;
    g_mockServiceControl.lastAction = action;
    g_mockServiceControl.callCount++;
    if (g_mockServiceControl.useSeq &&
        g_mockServiceControl.callCount - 1 < static_cast<int32_t>(g_mockServiceControl.returnSeq.size())) {
        return g_mockServiceControl.returnSeq[g_mockServiceControl.callCount - 1];
    }
    return g_mockServiceControl.returnVal;
}

extern "C" int ServiceWaitForStatus(const char* name, ServiceStatus status, int waitTimeout)
{
    (void)name;
    g_mockServiceControl.waitStatus = status;
    g_mockServiceControl.waitTimeout = waitTimeout;
    g_mockServiceControl.waitCallCount++;
    return g_mockServiceControl.waitReturnVal;
}
