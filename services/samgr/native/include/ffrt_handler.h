/*
* Copyright (C) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_SAMGR_FFRT_HANDLER_H
#define OHOS_SAMGR_FFRT_HANDLER_H

#include <map>
#include <shared_mutex>
#include <string>

#include "ffrt.h"

namespace OHOS {
class FFRTHandler {
public:
    explicit FFRTHandler(const std::string& name);
    ~FFRTHandler() = default;
    bool PostTask(std::function<void()> func);
    bool PostTask(std::function<void()> func, uint64_t delayTime);
    bool PostTask(std::function<void()> func, const std::string& name, uint64_t delayTime);
    void RemoveTask(const std::string& name);
    void DelTask(const std::string& name);
    bool HasInnerEvent(const std::string name);
    void CleanFfrt();
    void SetFfrt(const std::string& name);

private:
    std::shared_mutex mutex_;
    std::map<std::string, ffrt::task_handle> taskMap_;
    std::shared_ptr<ffrt::queue> queue_;
};
} // namespace OHOS
#endif // OHOS_SAMGR_FFRT_HANDLER_H