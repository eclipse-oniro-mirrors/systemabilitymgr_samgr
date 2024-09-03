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

//! System ability definition should be a number between FIRST_SYS_ABILITY_ID
//! and LAST_SYS_ABILITY_ID.
//！ And for vendor, should be a number between VENDOR_SYS_ABILITY_ID_BEGIN and
//！ VENDOR_SYS_ABILITY_ID_END.
//！
//！ If a new SA definition is added, it is necessary to synchronously add a
//！ mapping in the hidumper and synchronously add a mapping in the rust
//！ definition.rs The path of hidumper is
//！ "\base\hiviewdfx\hidumper\frameworks\native\dump_utils.cpp", and map name
//！ is "saNameMap_" The path of definition.rs is
//！ "\foundation\systemabilitymgr\samgr\interfaces\innerkits\rust\src\cxx\
//！ definition.rs"

pub const SAMGR_DUMP_SAID: i32 = 0;
pub const FIRST_SYS_ABILITY_ID: i32 = 0x00000001;
pub const SUBSYS_COMMON_SYS_ABILITY_ID_BEGIN: i32 = 1;
pub const RENDER_SERVICE: i32 = 10;
pub const SUBSYS_AAFWK_SYS_ABILITY_ID_BEGIN: i32 = 100;
pub const ABILITY_TOOLS_SERVICE_ID: i32 = 116;
pub const ABILITY_TEST_SERVICE_ID: i32 = 179;
pub const ABILITY_MGR_SERVICE_ID: i32 = 180;
pub const ABILITY_MST_SERVICE_ID: i32 = 181;
pub const DATAOBS_MGR_SERVICE_SA_ID: i32 = 182;
pub const URI_PERMISSION_MGR_SERVICE_ID: i32 = 183;
pub const QUICK_FIX_MGR_SERVICE_ID: i32 = 184;
pub const SUBSYS_AAFWK_SYS_ABILITY_ID_END: i32 = 199;
pub const SUBSYS_ACCOUNT_SYS_ABILITY_ID_BEGIN: i32 = 200;
pub const SUBSYS_AI_SYS_ABILITY_ID_BEGIN: i32 = 300;
pub const SUBSYS_AI_DS_SYS_ABILITY_ID: i32 = 310;
pub const AIDISPATCHER_ENGINE_SERVICE: i32 = 311;
pub const INTELL_VOICE_SERVICE_ID: i32 = 312;
pub const SUBSYS_APPEXECFWK_SYS_ABILITY_ID_BEGIN: i32 = 400;
pub const BUNDLE_MGR_SERVICE_SYS_ABILITY_ID: i32 = 401;
pub const DISTRIBUTED_BUNDLE_MGR_SERVICE_SYS_ABILITY_ID: i32 = 402;
pub const FORM_MGR_SERVICE_ID: i32 = 403;
pub const SERVICE_ROUTER_MGR_SERVICE_ID: i32 = 404;
pub const SUBSYS_APPLICATIONS_SYS_ABILITY_ID_BEGIN: i32 = 500;
pub const APP_MGR_SERVICE_ID: i32 = 501;
pub const INSTALLD_SERVICE_ID: i32 = 511;
pub const SUBSYS_ARVR_SYS_ABILITY_ID_BEGIN: i32 = 600;
pub const SUBSYS_ARVRHARDWARE_SYS_ABILITY_ID_BEGIN: i32 = 700;
pub const SUBSYS_BARRIERFREE_SYS_ABILITY_ID_BEGIN: i32 = 800;
pub const ACCESSIBILITY_MANAGER_SERVICE_ID: i32 = 801;
pub const SUBSYS_BARRIERFREE_SYS_ABILITY_ID_END: i32 = 899;
pub const SUBSYS_USERIAM_SYS_ABILITY_ID_BEGIN: i32 = 900;
pub const SUBSYS_USERIAM_SYS_ABILITY_USERIDM: i32 = 901;
pub const SUBSYS_USERIAM_SYS_ABILITY_USERAUTH: i32 = 921;
pub const SUBSYS_USERIAM_SYS_ABILITY_AUTHEXECUTORMGR: i32 = 931;
pub const SUBSYS_USERIAM_SYS_ABILITY_PINAUTH: i32 = 941;
pub const SUBSYS_USERIAM_SYS_ABILITY_FACEAUTH: i32 = 942;
pub const SUBSYS_USERIAM_SYS_ABILITY_FINGERPRINTAUTH: i32 = 943;
pub const SUBSYS_USERIAM_SYS_ABILITY_FINGERPRINTAUTH_EX: i32 = 944;
pub const SUBSYS_CCRUNTIME_SYS_ABILITY_ID_BEGIN: i32 = 1000;
pub const SUBSYS_COMMUNICATION_SYS_ABILITY_ID_BEGIN: i32 = 1100;
pub const RPC_UNREGISTERED_TEST_SERVICE: i32 = 1108;
pub const RPC_TEST_SERVICE: i32 = 1109;
pub const RPC_TEST_SERVICE2: i32 = 1110;
pub const IPC_MSG_SEV: i32 = 1114;
pub const IPC_MSG_UNREGISTERED_SERVER: i32 = 1115;
pub const IPC_MSG_REPEAT_SERVER: i32 = 1116;
pub const IPC_MSG_SERVER: i32 = 1117;
pub const IPC_TEST_SERVICE: i32 = 1118;
pub const IPC_EXTRA_TEST_SERVICE: i32 = 1119;
pub const WIFI_DEVICE_SYS_ABILITY_ID: i32 = 1120;
pub const WIFI_HOTSPOT_SYS_ABILITY_ID: i32 = 1121;
pub const WIFI_ENHANCER_SYS_ABILITY_ID: i32 = 1122;
pub const WIFI_P2P_SYS_ABILITY_ID: i32 = 1123;
pub const WIFI_SCAN_SYS_ABILITY_ID: i32 = 1124;
pub const BLUETOOTH_HOST_SYS_ABILITY_ID: i32 = 1130;
pub const NFC_MANAGER_SYS_ABILITY_ID: i32 = 1140;
pub const SE_MANAGER_SYS_ABILITY_ID: i32 = 1141;
pub const DISCOVER_SYS_ABILITY_ID: i32 = 1160;
pub const DNET_SYS_ABILITY_ID: i32 = 1170;
pub const NET_MANAGER_SYS_ABILITY_ID: i32 = 1150;
pub const COMM_NET_CONN_MANAGER_SYS_ABILITY_ID: i32 = 1151;
pub const COMM_NET_POLICY_MANAGER_SYS_ABILITY_ID: i32 = 1152;
pub const COMM_NET_STATS_MANAGER_SYS_ABILITY_ID: i32 = 1153;
pub const COMM_NET_TETHERING_MANAGER_SYS_ABILITY_ID: i32 = 1154;
pub const COMM_VPN_MANAGER_SYS_ABILITY_ID: i32 = 1155;
pub const COMM_DNS_MANAGER_SYS_ABILITY_ID: i32 = 1156;
pub const COMM_ETHERNET_MANAGER_SYS_ABILITY_ID: i32 = 1157;
pub const COMM_NETSYS_NATIVE_SYS_ABILITY_ID: i32 = 1158;
pub const COMM_MDNS_MANAGER_SYS_ABILITY_ID: i32 = 1161;
pub const COMM_NETSYS_EXT_SYS_ABILITY_ID: i32 = 1162;
pub const COMM_DISTRIBUTED_NET_ABILITY_ID: i32 = 1163;
pub const SMART_COMM_SYS_ABILITY_ID: i32 = 1180;
pub const NEARLINK_HOST_SYS_ABILITY_ID: i32 = 1190;
pub const SUBSYS_DFX_SYS_ABILITY_ID_BEGIN: i32 = 1200;
pub const DFX_SYS_HIVIEW_ABILITY_ID: i32 = 1201;
pub const DFX_FAULT_LOGGER_ABILITY_ID: i32 = 1202;
pub const DFX_SYS_EVENT_SERVICE_ABILITY_ID: i32 = 1203;
pub const DFX_SYS_NATIVE_MEMORY_PROFILER_SERVICE_ABILITY_ID: i32 = 1205;
pub const XPERF_SYS_TRACE_SERVICE_ABILITY_ID: i32 = 1208;
pub const XPERF_SYS_IO_SERVICE_ABILITY_ID: i32 = 1209;
pub const XPERF_BIGDATA_MANAGER_SERVICE_ABILITY_ID: i32 = 1210;
pub const DFX_HI_DUMPER_SERVICE_ABILITY_ID: i32 = 1212;
pub const XPOWER_MANAGER_SYSTEM_ABILITY_ID: i32 = 1213;
pub const DFX_HI_PERF_SERVICE_ABILITY_ID: i32 = 1214;
pub const DFX_HI_DUMPER_CPU_SERVICE_ABILITY_ID: i32 = 1215;
pub const SUBSYS_DISTRIBUTEDDATAMNG_SYS_ABILITY_ID_BEGIN: i32 = 1300;
pub const DISTRIBUTED_KV_DATA_SERVICE_ABILITY_ID: i32 = 1301;
pub const DISTRIBUTED_FS_DAEMON_SERVICE_ID: i32 = 1302;
pub const DISTRIBUTED_FS_META_SERVICE_ID: i32 = 1303;
pub const DISTRIBUTED_FS_STORAGE_SERVICE_ID: i32 = 1304;
pub const SUBSYS_DISTRIBUTEDSCHEDULE_SYS_ABILITY_ID_BEGIN: i32 = 1400;
pub const DISTRIBUTED_SCHED_SA_ID: i32 = 1401;
pub const DISTRIBUTED_SCHED_ADAPTER_SA_ID: i32 = 1402;
pub const DISTRIBUTED_SCENARIO_MGR_SA_ID: i32 = 1403;
pub const CONTINUATION_MANAGER_SA_ID: i32 = 1404;
pub const DISTRIBUTED_SCHED_TEST_SO_ID: i32 = 1490;
pub const DISTRIBUTED_SCHED_TEST_OS_ID: i32 = 1491;
pub const DISTRIBUTED_SCHED_TEST_OOS_ID: i32 = 1492;
pub const DISTRIBUTED_SCHED_TEST_TT_ID: i32 = 1493;
pub const DISTRIBUTED_SCHED_TEST_LISTEN_ID: i32 = 1494;
pub const DISTRIBUTED_SCHED_TEST_CONNECTION_ID: i32 = 1495;
pub const DISTRIBUTED_SCHED_TEST_INCOMPLETE_ID: i32 = 1496;
pub const DISTRIBUTED_SCHED_TEST_ONDEMAND_ID: i32 = 1497;
pub const DISTRIBUTED_SCHED_TEST_AUDIO_ID: i32 = 1498;
pub const DISTRIBUTED_SCHED_TEST_MEDIA_ID: i32 = 1499;
pub const SUBSYS_DRIVERS_SYS_ABILITY_ID_BEGIN: i32 = 1500;
pub const SUBSYS_GLOBAL_SYS_ABILITY_ID_BEGIN: i32 = 1600;
pub const ENTERPRISE_DEVICE_MANAGER_SA_ID: i32 = 1601;
pub const I18N_SA_ID: i32 = 1602;
pub const SUBSYS_GRAPHIC_SYS_ABILITY_ID_BEGIN: i32 = 1700;
pub const SUBSYS_HBS_SYS_ABILITY_ID_BEGIN: i32 = 1800;
pub const SUBSYS_IAWARE_SYS_ABILITY_ID_BEGIN: i32 = 1900;
pub const RES_SCHED_SYS_ABILITY_ID: i32 = 1901;
pub const RESSCHEDD_SA_ID: i32 = 1902;
pub const BACKGROUND_TASK_MANAGER_SERVICE_ID: i32 = 1903;
pub const WORK_SCHEDULE_SERVICE_ID: i32 = 1904;
pub const COMPONENT_SCHEDULE_SERVICE_ID: i32 = 1905;
pub const SOC_PERF_SERVICE_SA_ID: i32 = 1906;
pub const DEVICE_USAGE_STATISTICS_SYS_ABILITY_ID: i32 = 1907;
pub const MEMORY_MANAGER_SA_ID: i32 = 1909;
pub const SUSPEND_MANAGER_SYSTEM_ABILITY_ID: i32 = 1910;
pub const ABNORMAL_EFFICIENCY_MGR_SYSTEM_ABILITY_ID: i32 = 1911;
pub const CONCURRENT_TASK_SERVICE_ID: i32 = 1912;
pub const RESOURCE_QUOTA_CONTROL_SYSTEM_ABILITY_ID: i32 = 1913;
pub const DEVICE_STANDBY_SERVICE_SYSTEM_ABILITY_ID: i32 = 1914;
pub const TASK_HEARTBEAT_MGR_SYSTEM_ABILITY_ID: i32 = 1915;
pub const APP_NAP_SYSTEM_ABILITY_ID: i32 = 1916;
pub const SUBSYS_IDE_SYS_ABILITY_ID_BEGIN: i32 = 2000;
pub const SUBSYS_INTELLIACCESSORIES_SYS_ABILITY_ID_BEGIN: i32 = 2100;
pub const SUBSYS_INTELLISPEAKER_SYS_ABILITY_ID_BEGIN: i32 = 2200;
pub const SUBSYS_INTELLITV_SYS_ABILITY_ID_BEGIN: i32 = 2300;
pub const SUBSYS_IOT_SYS_ABILITY_ID_BEGIN: i32 = 2400;
pub const SUBSYS_IOTHARDWARE_SYS_ABILITY_ID_BEGIN: i32 = 2500;
pub const SUBSYS_IVIHARDWARE_SYS_ABILITY_ID_BEGIN: i32 = 2600;
pub const IVIHARDWARE_MANAGER_SA_ID: i32 = 2601;
pub const IVIHARDWARE_ADAS_SA_ID: i32 = 2602;
pub const IVIHARDWARE_TBOX_SA_ID: i32 = 2603;
pub const IVIHARDWARE_CLUSTER_SA_ID: i32 = 2604;
pub const SUBSYS_KERNEL_SYS_ABILITY_ID_BEGIN: i32 = 2700;
pub const SUBSYS_LOCATION_SYS_ABILITY_ID_BEGIN: i32 = 2800;
pub const LOCATION_GEO_CONVERT_SA_ID: i32 = 2801;
pub const LOCATION_LOCATOR_SA_ID: i32 = 2802;
pub const LOCATION_GNSS_SA_ID: i32 = 2803;
pub const LOCATION_NETWORK_LOCATING_SA_ID: i32 = 2804;
pub const LOCATION_NOPOWER_LOCATING_SA_ID: i32 = 2805;
pub const LOCATION_NOTIFICATION_SA_ID: i32 = 2806;
pub const SUBSYS_MSDP_SYS_ABILITY_ID_BEGIN: i32 = 2900;
pub const MSDP_MOTION_SERVICE_ID: i32 = 2901;
pub const MSDP_DEVICESTATUS_SERVICE_ID: i32 = 2902;
pub const MSDP_MOVEMENT_SERVICE_ID: i32 = 2903;
pub const MSDP_SPATIAL_AWARENESS_SERVICE_ID: i32 = 2904;
pub const MSDP_GEOFENCE_SERVICE_ID: i32 = 2905;
pub const MSDP_TIMELINE_SERVICE_ID: i32 = 2906;
pub const MSDP_USER_STATUS_SERVICE_ID: i32 = 2907;
pub const SUBSYS_MULTIMEDIA_SYS_ABILITY_ID_BEGIN: i32 = 3000;
pub const AUDIO_DISTRIBUTED_SERVICE_ID: i32 = 3001;
pub const PLAYER_DISTRIBUTED_SERVICE_ID: i32 = 3002;
pub const RADIO_IVI_SERVICE_ID: i32 = 3003;
pub const AUDIO_SWIFT_SERVICE_ID: i32 = 3004;
pub const UPDATE_DISTRIBUTED_SERVICE_ID: i32 = 3006;
pub const MEDIA_LIBRARY_SERVICE_ID: i32 = 3007;
pub const CAMERA_SERVICE_ID: i32 = 3008;
pub const AUDIO_POLICY_SERVICE_ID: i32 = 3009;
pub const AVSESSION_SERVICE_ID: i32 = 3010;
pub const AV_CODEC_SERVICE_ID: i32 = 3011;
pub const MEDIA_KEY_SYSTEM_SERVICE_ID: i32 = 3012;
pub const SUBSYS_MULTIMODAINPUT_SYS_ABILITY_ID_BEGIN: i32 = 3100;
pub const MULTIMODAL_INPUT_SERVICE_ID: i32 = 3101;
pub const MULTIMODAL_CHANNEL_SERVICE_ID: i32 = 3102;
pub const SUBSYS_NOTIFICATION_SYS_ABILITY_ID_BEGIN: i32 = 3200;
pub const EVENT_MANAGER_SERVICE_ID: i32 = 3201;
pub const COMMON_EVENT_SERVICE_ABILITY_ID: i32 = 3202;
pub const ADVANCED_NOTIFICATION_SERVICE_ABILITY_ID: i32 = 3203;
pub const COMMON_EVENT_SERVICE_ID: i32 = 3299;
pub const SUBSYS_POWERMNG_SYS_ABILITY_ID_BEGIN: i32 = 3300;
pub const POWER_MANAGER_SERVICE_ID: i32 = 3301;
pub const POWER_MANAGER_BATT_SERVICE_ID: i32 = 3302;
pub const POWER_MANAGER_THERMAL_SERVICE_ID: i32 = 3303;
pub const POWER_MANAGER_BATT_STATS_SERVICE_ID: i32 = 3304;
pub const DISPLAY_MANAGER_SERVICE_ID: i32 = 3308;
pub const IVIPOWER_ENHANCED_SERVICE_ID: i32 = 3309;
pub const SUBSYS_ROUTER_SYS_ABILITY_ID_BEGIN: i32 = 3400;
pub const SUBSYS_SECURITY_SYS_ABILITY_ID_BEGIN: i32 = 3500;
pub const ACCESS_TOKEN_MANAGER_SERVICE_ID: i32 = 3503;
pub const TOKEN_SYNC_MANAGER_SERVICE_ID: i32 = 3504;
pub const PRIVACY_MANAGER_SERVICE_ID: i32 = 3505;
pub const SECURITY_COMPONENT_SERVICE_ID: i32 = 3506;
pub const LOCAL_CODE_SIGN_SERVICE_ID: i32 = 3507;
pub const SANDBOX_MANAGER_SERVICE_ID: i32 = 3508;
pub const DEVICE_SECURITY_LEVEL_MANAGER_SA_ID: i32 = 3511;
pub const CERT_MANAGER_SERVICE_SA_ID: i32 = 3512;
pub const DEVICE_THREAT_DETECTION_SERVICE_SA_ID: i32 = 3513;
pub const DLP_PERMISSION_SERVICE_ID: i32 = 3521;
pub const RISK_ANALYSIS_MANAGER_SA_ID: i32 = 3523;
pub const DATA_COLLECT_MANAGER_SA_ID: i32 = 3524;
pub const DLP_CREDENTIAL_SERVICE_ID: i32 = 3553;
pub const SUBSYS_SENSORS_SYS_ABILITY_ID_BEGIN: i32 = 3600;
pub const SENSOR_SERVICE_ABILITY_ID: i32 = 3601;
pub const MISCDEVICE_SERVICE_ABILITY_ID: i32 = 3602;
pub const EXTSHB_SERVICE_ABILITY_ID: i32 = 3603;
pub const MEDICAL_SENSOR_SERVICE_ABILITY_ID: i32 = 3605;
pub const SUBSYS_SMALLSERVICES_SYS_ABILITY_ID_BEGIN: i32 = 3700;
pub const PASTEBOARD_SERVICE_ID: i32 = 3701;
pub const TIME_SERVICE_ID: i32 = 3702;
pub const INPUT_METHOD_SYSTEM_ABILITY_ID: i32 = 3703;
pub const SCREENLOCK_SERVICE_ID: i32 = 3704;
pub const WALLPAPER_MANAGER_SERVICE_ID: i32 = 3705;
pub const DOWNLOAD_SERVICE_ID: i32 = 3706;
pub const PRINT_SERVICE_ID: i32 = 3707;
pub const SCAN_SERVICE_ID: i32 = 3708;
pub const SUBSYS_SOURCECODETRANSFORMER_SYS_ABILITY_ID_BEGIN: i32 = 3800;
pub const SUBSYS_STARTUP_SYS_ABILITY_ID_BEGIN: i32 = 3900;
pub const PARAM_WATCHER_DISTRIBUTED_SERVICE_ID: i32 = 3901;
pub const SYSPARAM_DEVICE_SERVICE_ID: i32 = 3902;
pub const SUBSYS_TELEPONY_SYS_ABILITY_ID_BEGIN: i32 = 4000;
pub const TELEPHONY_SYS_ABILITY_ID: i32 = 4001;
pub const DCALL_SYS_ABILITY_ID: i32 = 4002;
pub const TELEPHONY_CALL_MANAGER_SYS_ABILITY_ID: i32 = 4005;
pub const TELEPHONY_CELLULAR_CALL_SYS_ABILITY_ID: i32 = 4006;
pub const TELEPHONY_CELLULAR_DATA_SYS_ABILITY_ID: i32 = 4007;
pub const TELEPHONY_SMS_MMS_SYS_ABILITY_ID: i32 = 4008;
pub const TELEPHONY_STATE_REGISTRY_SYS_ABILITY_ID: i32 = 4009;
pub const TELEPHONY_CORE_SERVICE_SYS_ABILITY_ID: i32 = 4010;
pub const TELEPHONY_DATA_STORAGE_SYS_ABILITY_ID: i32 = 4012;
pub const TELEPHONY_IMS_SYS_ABILITY_ID: i32 = 4014;
pub const SUBSYS_UPDATE_SYS_ABILITY_ID_BEGIN: i32 = 4100;
pub const SYS_INSTALLER_DISTRIBUTED_SERVICE_ID: i32 = 4101;
pub const QUICKFIX_ENGINE_SERVICE_ID: i32 = 4102;
pub const MODULE_UPDATE_SERVICE_ID: i32 = 4103;
pub const SUBSYS_USB_SYS_ABILITY_ID_BEGIN: i32 = 4200;
pub const USB_SYSTEM_ABILITY_ID: i32 = 4201;
pub const SUBSYS_WEARABLE_SYS_ABILITY_ID_BEGIN: i32 = 4300;
pub const SUBSYS_WEARABLEHARDWARE_SYS_ABILITY_ID_BEGIN: i32 = 4400;
pub const SUBSYS_IVI_SYS_ABILITY_ID_BEGIN: i32 = 4500;
pub const IVI_DRIVINGSAFETY_SA_ID: i32 = 4501;
pub const IVI_CONFIGMANAGER_SA_ID: i32 = 4502;
pub const IVI_COCKPITMONITOR_SA_ID: i32 = 4503;
pub const WINDOW_MANAGER_ID: i32 = 4600;
pub const VSYNC_MANAGER_ID: i32 = 4601;
pub const VSYNC_MANAGER_TEST_ID: i32 = 4602;
pub const GRAPHIC_DUMPER_SERVICE_SA_ID: i32 = 4603;
pub const GRAPHIC_DUMPER_COMMAND_SA_ID: i32 = 4604;
pub const ANIMATION_SERVER_SA_ID: i32 = 4605;
pub const WINDOW_MANAGER_SERVICE_ID: i32 = 4606;
pub const DISPLAY_MANAGER_SERVICE_SA_ID: i32 = 4607;
pub const SOFTBUS_SERVER_SA_ID: i32 = 4700;
pub const DEVICE_AUTH_SERVICE_ID: i32 = 4701;
pub const SUBSYS_DISTRIBUTED_HARDWARE_SYS_ABILITY_ID_BEGIN: i32 = 4800;
pub const DISTRIBUTED_HARDWARE_SA_ID: i32 = 4801;
pub const DISTRIBUTED_HARDWARE_DEVICEMANAGER_SA_ID: i32 = 4802;
pub const DISTRIBUTED_HARDWARE_CAMERA_SOURCE_SA_ID: i32 = 4803;
pub const DISTRIBUTED_HARDWARE_CAMERA_SINK_SA_ID: i32 = 4804;
pub const DISTRIBUTED_HARDWARE_AUDIO_SOURCE_SA_ID: i32 = 4805;
pub const DISTRIBUTED_HARDWARE_AUDIO_SINK_SA_ID: i32 = 4806;
pub const DISTRIBUTED_HARDWARE_SCREEN_SOURCE_SA_ID: i32 = 4807;
pub const DISTRIBUTED_HARDWARE_SCREEN_SINK_SA_ID: i32 = 4808;
pub const DISTRIBUTED_HARDWARE_INPUT_SOURCE_SA_ID: i32 = 4809;
pub const DISTRIBUTED_HARDWARE_INPUT_SINK_SA_ID: i32 = 4810;
pub const DEVICE_STORAGE_MANAGER_SERVICE_ID: i32 = 5000;
pub const STORAGE_SERVICE_ID: i32 = 5001;
pub const STORAGE_MANAGER_DAEMON_ID: i32 = 5002;
pub const STORAGE_MANAGER_MANAGER_ID: i32 = 5003;
pub const FILE_ACCESS_SERVICE_ID: i32 = 5010;
pub const DEVICE_SERVICE_MANAGER_SA_ID: i32 = 5100;
pub const HDF_EXTERNAL_DEVICE_MANAGER_SA_ID: i32 = 5110;
pub const SUBSYS_FILEMANAGEMENT_SYS_ABILITY_ID_BEGIN: i32 = 5200;
pub const FILEMANAGEMENT_DISTRIBUTED_FILE_DAEMON_SA_ID: i32 = 5201;
pub const FILEMANAGEMENT_DISTRIBUTED_FILE_SERVICE_SA_ID: i32 = 5202;
pub const FILEMANAGEMENT_BACKUP_SERVICE_SA_ID: i32 = 5203;
pub const FILEMANAGEMENT_CLOUD_SYNC_SERVICE_SA_ID: i32 = 5204;
pub const FILEMANAGEMENT_CLOUD_DAEMON_SERVICE_SA_ID: i32 = 5205;
pub const FILEMANAGEMENT_CLOUD_BACKUP_SERVICE_SA_ID: i32 = 5206;
pub const SUBSYS_TESTPLATFORM_SYS_ABILITY_ID_BEGIN: i32 = 5500;
pub const DEVICE_ATTEST_PROFILE_SA_ID: i32 = 5501;
pub const DISTRIBUTED_DEVICE_PROFILE_SA_ID: i32 = 6001;
pub const ADVERTISING_SA_ID: i32 = 6104;
pub const ECOLOGICAL_RULE_MANAGER_SA_ID: i32 = 6105;
pub const SUBSYS_ACE_SYS_ABILITY_ID_BEGIN: i32 = 7001;
pub const ARKUI_UI_APPEARANCE_SERVICE_ID: i32 = 7002;
pub const CA_DAEMON_ID: i32 = 8001;
pub const COMM_WATCH_DISTRIBUTED_NET_ABILITY_ID: i32 = 8400;

// reserved for vendor
pub const VENDOR_SYS_ABILITY_ID_BEGIN: i32 = 0x00010000; // 65536
pub const VENDOR_SYS_ABILITY_ID_END: i32 = 0x00020000; // 131072
pub const LAST_SYS_ABILITY_ID: i32 = 0x00ffffff; // 16777215
