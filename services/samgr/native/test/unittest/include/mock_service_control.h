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

#ifndef MOCK_SERVICE_CONTROL_H
#define MOCK_SERVICE_CONTROL_H

#include <vector>
#include <string>
#include "service_control.h"

struct MockServiceControlState {
    int32_t lastAction = -1;
    int32_t callCount = 0;
    int32_t returnVal = 0;
    int32_t waitCallCount = 0;
    int32_t waitReturnVal = 0;
    int32_t waitStatus = 0;
    int32_t waitTimeout = 0;
    std::vector<int32_t> returnSeq;
    bool useSeq = false;
};

void ResetMockServiceControl();
void SetMockServiceControlReturn(int32_t val);
void SetMockServiceControlReturnSeq(const std::vector<int32_t>& seq);
void SetMockServiceControlWaitReturn(int32_t val);

extern MockServiceControlState g_mockServiceControl;

#endif
