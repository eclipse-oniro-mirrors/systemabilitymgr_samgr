// Copyright (C) 2024 Huawei Device Co., Ltd.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#![allow(missing_docs)]
#![allow(clippy::missing_safety_doc)]
//! Safe Rust interface to OHOS samgr
// Export types of this crate

#[macro_use]
#[allow(unused)]
mod hilog;

pub mod definition;
mod dump_flag;
pub mod manage;
pub use dump_flag::DumpFlagPriority;
mod wrapper;

pub const LOG_LABEL: hilog_rust::HiLogLabel = hilog_rust::HiLogLabel {
    log_type: hilog_rust::LogType::LogCore,
    domain: 0xD001800,
    tag: "SAMGR_RUST",
};
