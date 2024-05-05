/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#ifndef UTILS_SYSTEM_ABILITY_DEFINITION_H
#define UTILS_SYSTEM_ABILITY_DEFINITION_H

#include<map>
#include<string>

namespace OHOS {
/*
System ability definition should be a number between FIRST_SYS_ABILITY_ID and LAST_SYS_ABILITY_ID.
And for vendor, should be a number between VENDOR_SYS_ABILITY_ID_BEGIN and VENDOR_SYS_ABILITY_ID_END.

If a new SA definition is added, it is necessary to synchronously add a mapping in the hidumper and
synchronously add a mapping in the rust definition.
The path of hidumper is "\base\hiviewdfx\hidumper\frameworks\native\dump_utils.cpp", and map name is "saNameMap_"
The path of definition.rs is "\foundation\systemabilitymgr\samgr\interfaces\innerkits\rust\src\cxx\definition.rs"
*/
enum {
    SAMGR_DUMP_SAID                                  = 0,
    FIRST_SYS_ABILITY_ID                             = 0x00000001,
    SUBSYS_COMMON_SYS_ABILITY_ID_BEGIN               = 1,
    RENDER_SERVICE                                   = 10,
    SUBSYS_AAFWK_SYS_ABILITY_ID_BEGIN                = 100,
    ABILITY_TOOLS_SERVICE_ID                         = 116,
    ABILITY_TEST_SERVICE_ID                          = 179,
    ABILITY_MGR_SERVICE_ID                           = 180,
    ABILITY_MST_SERVICE_ID                           = 181,
    DATAOBS_MGR_SERVICE_SA_ID                        = 182,
    URI_PERMISSION_MGR_SERVICE_ID                    = 183,
    QUICK_FIX_MGR_SERVICE_ID                         = 184,
    SUBSYS_AAFWK_SYS_ABILITY_ID_END                  = 199,
    SUBSYS_ACCOUNT_SYS_ABILITY_ID_BEGIN              = 200,
    SUBSYS_AI_SYS_ABILITY_ID_BEGIN                   = 300,
    SUBSYS_AI_DS_SYS_ABILITY_ID                      = 310,
    AIDISPATCHER_ENGINE_SERVICE                      = 311,
    INTELL_VOICE_SERVICE_ID                          = 312,
    SUBSYS_APPEXECFWK_SYS_ABILITY_ID_BEGIN           = 400,
    BUNDLE_MGR_SERVICE_SYS_ABILITY_ID                = 401,
    DISTRIBUTED_BUNDLE_MGR_SERVICE_SYS_ABILITY_ID    = 402,
    FORM_MGR_SERVICE_ID                              = 403,
    SERVICE_ROUTER_MGR_SERVICE_ID                    = 404,
    SUBSYS_APPLICATIONS_SYS_ABILITY_ID_BEGIN         = 500,
    APP_MGR_SERVICE_ID                               = 501,
    INSTALLD_SERVICE_ID                              = 511,
    SUBSYS_ARVR_SYS_ABILITY_ID_BEGIN                 = 600,
    SUBSYS_ARVRHARDWARE_SYS_ABILITY_ID_BEGIN         = 700,
    SUBSYS_BARRIERFREE_SYS_ABILITY_ID_BEGIN          = 800,
    ACCESSIBILITY_MANAGER_SERVICE_ID                 = 801,
    SUBSYS_BARRIERFREE_SYS_ABILITY_ID_END            = 899,
    SUBSYS_USERIAM_SYS_ABILITY_ID_BEGIN              = 900,
    SUBSYS_USERIAM_SYS_ABILITY_USERIDM               = 901,
    SUBSYS_USERIAM_SYS_ABILITY_USERAUTH              = 921,
    SUBSYS_USERIAM_SYS_ABILITY_AUTHEXECUTORMGR       = 931,
    SUBSYS_USERIAM_SYS_ABILITY_PINAUTH               = 941,
    SUBSYS_USERIAM_SYS_ABILITY_FACEAUTH              = 942,
    SUBSYS_USERIAM_SYS_ABILITY_FINGERPRINTAUTH       = 943,
    SUBSYS_USERIAM_SYS_ABILITY_FINGERPRINTAUTH_EX    = 944,
    SUBSYS_CCRUNTIME_SYS_ABILITY_ID_BEGIN            = 1000,
    SUBSYS_COMMUNICATION_SYS_ABILITY_ID_BEGIN        = 1100,
    RPC_UNREGISTERED_TEST_SERVICE                    = 1108,
    RPC_TEST_SERVICE                                 = 1109,
    RPC_TEST_SERVICE2                                = 1110,
    IPC_MSG_SEV                                      = 1114,
    IPC_MSG_UNREGISTERED_SERVER                      = 1115,
    IPC_MSG_REPEAT_SERVER                            = 1116,
    IPC_MSG_SERVER                                   = 1117,
    IPC_TEST_SERVICE                                 = 1118,
    IPC_EXTRA_TEST_SERVICE                           = 1119,
    WIFI_DEVICE_SYS_ABILITY_ID                       = 1120,
    WIFI_HOTSPOT_SYS_ABILITY_ID                      = 1121,
    WIFI_ENHANCER_SYS_ABILITY_ID                     = 1122,
    WIFI_P2P_SYS_ABILITY_ID                          = 1123,
    WIFI_SCAN_SYS_ABILITY_ID                         = 1124,
    BLUETOOTH_HOST_SYS_ABILITY_ID                    = 1130,
    NFC_MANAGER_SYS_ABILITY_ID                       = 1140,
    SE_MANAGER_SYS_ABILITY_ID                        = 1141,
    DISCOVER_SYS_ABILITY_ID                          = 1160,
    DNET_SYS_ABILITY_ID                              = 1170,
    NET_MANAGER_SYS_ABILITY_ID                       = 1150,
    COMM_NET_CONN_MANAGER_SYS_ABILITY_ID             = 1151,
    COMM_NET_POLICY_MANAGER_SYS_ABILITY_ID           = 1152,
    COMM_NET_STATS_MANAGER_SYS_ABILITY_ID            = 1153,
    COMM_NET_TETHERING_MANAGER_SYS_ABILITY_ID        = 1154,
    COMM_VPN_MANAGER_SYS_ABILITY_ID                  = 1155,
    COMM_DNS_MANAGER_SYS_ABILITY_ID                  = 1156,
    COMM_ETHERNET_MANAGER_SYS_ABILITY_ID             = 1157,
    COMM_NETSYS_NATIVE_SYS_ABILITY_ID                = 1158,
    COMM_MDNS_MANAGER_SYS_ABILITY_ID                 = 1161,
    COMM_NETSYS_EXT_SYS_ABILITY_ID                   = 1162,
    COMM_DISTRIBUTED_NET_ABILITY_ID                  = 1163,
    SMART_COMM_SYS_ABILITY_ID                        = 1180,
    NEARLINK_HOST_SYS_ABILITY_ID                     = 1190,
    SUBSYS_DFX_SYS_ABILITY_ID_BEGIN                  = 1200,
    DFX_SYS_HIVIEW_ABILITY_ID                        = 1201,
    DFX_FAULT_LOGGER_ABILITY_ID                      = 1202,
    DFX_SYS_EVENT_SERVICE_ABILITY_ID                 = 1203,
    DFX_SYS_NATIVE_MEMORY_PROFILER_SERVICE_ABILITY_ID = 1205,
    XPERF_SYS_TRACE_SERVICE_ABILITY_ID               = 1208,
    XPERF_SYS_IO_SERVICE_ABILITY_ID                  = 1209,
    XPERF_BIGDATA_MANAGER_SERVICE_ABILITY_ID         = 1210,
    DFX_HI_DUMPER_SERVICE_ABILITY_ID                 = 1212,
    XPOWER_MANAGER_SYSTEM_ABILITY_ID                 = 1213,
    DFX_HI_PERF_SERVICE_ABILITY_ID                   = 1214,
    DFX_HI_DUMPER_CPU_SERVICE_ABILITY_ID             = 1215,
    SUBSYS_DISTRIBUTEDDATAMNG_SYS_ABILITY_ID_BEGIN   = 1300,
    DISTRIBUTED_KV_DATA_SERVICE_ABILITY_ID           = 1301,
    DISTRIBUTED_FS_DAEMON_SERVICE_ID                 = 1302,
    DISTRIBUTED_FS_META_SERVICE_ID                   = 1303,
    DISTRIBUTED_FS_STORAGE_SERVICE_ID                = 1304,
    SUBSYS_DISTRIBUTEDSCHEDULE_SYS_ABILITY_ID_BEGIN  = 1400,
    DISTRIBUTED_SCHED_SA_ID                          = 1401,
    DISTRIBUTED_SCHED_ADAPTER_SA_ID                  = 1402,
    DISTRIBUTED_SCENARIO_MGR_SA_ID                   = 1403,
    CONTINUATION_MANAGER_SA_ID                       = 1404,
    DISTRIBUTED_SCHED_TEST_SO_ID                     = 1490,
    DISTRIBUTED_SCHED_TEST_OS_ID                     = 1491,
    DISTRIBUTED_SCHED_TEST_OOS_ID                    = 1492,
    DISTRIBUTED_SCHED_TEST_TT_ID                     = 1493,
    DISTRIBUTED_SCHED_TEST_LISTEN_ID                 = 1494,
    DISTRIBUTED_SCHED_TEST_CONNECTION_ID             = 1495,
    DISTRIBUTED_SCHED_TEST_INCOMPLETE_ID             = 1496,
    DISTRIBUTED_SCHED_TEST_ONDEMAND_ID               = 1497,
    DISTRIBUTED_SCHED_TEST_AUDIO_ID                  = 1498,
    DISTRIBUTED_SCHED_TEST_MEDIA_ID                  = 1499,
    SUBSYS_DRIVERS_SYS_ABILITY_ID_BEGIN              = 1500,
    SUBSYS_GLOBAL_SYS_ABILITY_ID_BEGIN               = 1600,
    ENTERPRISE_DEVICE_MANAGER_SA_ID                  = 1601,
    I18N_SA_ID                                       = 1602,
    SUBSYS_GRAPHIC_SYS_ABILITY_ID_BEGIN              = 1700,
    SUBSYS_HBS_SYS_ABILITY_ID_BEGIN                  = 1800,
    SUBSYS_IAWARE_SYS_ABILITY_ID_BEGIN               = 1900,
    RES_SCHED_SYS_ABILITY_ID                         = 1901,
    RESSCHEDD_SA_ID                                  = 1902,
    BACKGROUND_TASK_MANAGER_SERVICE_ID               = 1903,
    WORK_SCHEDULE_SERVICE_ID                         = 1904,
    COMPONENT_SCHEDULE_SERVICE_ID                    = 1905,
    SOC_PERF_SERVICE_SA_ID                           = 1906,
    DEVICE_USAGE_STATISTICS_SYS_ABILITY_ID           = 1907,
    MEMORY_MANAGER_SA_ID                             = 1909,
    SUSPEND_MANAGER_SYSTEM_ABILITY_ID                = 1910,
    ABNORMAL_EFFICIENCY_MGR_SYSTEM_ABILITY_ID        = 1911,
    CONCURRENT_TASK_SERVICE_ID                       = 1912,
    RESOURCE_QUOTA_CONTROL_SYSTEM_ABILITY_ID         = 1913,
    DEVICE_STANDBY_SERVICE_SYSTEM_ABILITY_ID         = 1914,
    TASK_HEARTBEAT_MGR_SYSTEM_ABILITY_ID             = 1915,
    APP_NAP_SYSTEM_ABILITY_ID                        = 1916,
    RES_SCHED_EXE_ABILITY_ID                         = 1918,
    SUBSYS_IDE_SYS_ABILITY_ID_BEGIN                  = 2000,
    SUBSYS_INTELLIACCESSORIES_SYS_ABILITY_ID_BEGIN   = 2100,
    SUBSYS_INTELLISPEAKER_SYS_ABILITY_ID_BEGIN       = 2200,
    SUBSYS_INTELLITV_SYS_ABILITY_ID_BEGIN            = 2300,
    SUBSYS_IOT_SYS_ABILITY_ID_BEGIN                  = 2400,
    SUBSYS_IOTHARDWARE_SYS_ABILITY_ID_BEGIN          = 2500,
    SUBSYS_IVIHARDWARE_SYS_ABILITY_ID_BEGIN          = 2600,
    IVIHARDWARE_MANAGER_SA_ID                        = 2601,
    IVIHARDWARE_ADAS_SA_ID                           = 2602,
    IVIHARDWARE_TBOX_SA_ID                           = 2603,
    IVIHARDWARE_CLUSTER_SA_ID                        = 2604,
    SUBSYS_KERNEL_SYS_ABILITY_ID_BEGIN               = 2700,
    SUBSYS_LOCATION_SYS_ABILITY_ID_BEGIN             = 2800,
    LOCATION_GEO_CONVERT_SA_ID                       = 2801,
    LOCATION_LOCATOR_SA_ID                           = 2802,
    LOCATION_GNSS_SA_ID                              = 2803,
    LOCATION_NETWORK_LOCATING_SA_ID                  = 2804,
    LOCATION_NOPOWER_LOCATING_SA_ID                  = 2805,
    LOCATION_NOTIFICATION_SA_ID                      = 2806,
    SUBSYS_MSDP_SYS_ABILITY_ID_BEGIN                 = 2900,
    MSDP_MOTION_SERVICE_ID                           = 2901,
    MSDP_DEVICESTATUS_SERVICE_ID                     = 2902,
    MSDP_MOVEMENT_SERVICE_ID                         = 2903,
    MSDP_SPATIAL_AWARENESS_SERVICE_ID                = 2904,
    MSDP_GEOFENCE_SERVICE_ID                         = 2905,
    MSDP_TIMELINE_SERVICE_ID                         = 2906,
    MSDP_USER_STATUS_SERVICE_ID                      = 2907,
    MSDP_MULTIMODAL_AWARENESS_SERVICE_ID             = 2908,
    SUBSYS_MULTIMEDIA_SYS_ABILITY_ID_BEGIN           = 3000,
    AUDIO_DISTRIBUTED_SERVICE_ID                     = 3001,
    PLAYER_DISTRIBUTED_SERVICE_ID                    = 3002,
    RADIO_IVI_SERVICE_ID                             = 3003,
    AUDIO_SWIFT_SERVICE_ID                           = 3004,
    UPDATE_DISTRIBUTED_SERVICE_ID                    = 3006,
    MEDIA_LIBRARY_SERVICE_ID                         = 3007,
    CAMERA_SERVICE_ID                                = 3008,
    AUDIO_POLICY_SERVICE_ID                          = 3009,
    AVSESSION_SERVICE_ID                             = 3010,
    AV_CODEC_SERVICE_ID                              = 3011,
    MEDIA_KEY_SYSTEM_SERVICE_ID                      = 3012,
    SUBSYS_MULTIMODAINPUT_SYS_ABILITY_ID_BEGIN       = 3100,
    MULTIMODAL_INPUT_SERVICE_ID                      = 3101,
    MULTIMODAL_CHANNEL_SERVICE_ID                    = 3102,
    SUBSYS_NOTIFICATION_SYS_ABILITY_ID_BEGIN         = 3200,
    EVENT_MANAGER_SERVICE_ID                         = 3201,
    COMMON_EVENT_SERVICE_ABILITY_ID                  = 3202,
    ADVANCED_NOTIFICATION_SERVICE_ABILITY_ID         = 3203,
    COMMON_EVENT_SERVICE_ID                          = 3299,
    SUBSYS_POWERMNG_SYS_ABILITY_ID_BEGIN             = 3300,
    POWER_MANAGER_SERVICE_ID                         = 3301,
    POWER_MANAGER_BATT_SERVICE_ID                    = 3302,
    POWER_MANAGER_THERMAL_SERVICE_ID                 = 3303,
    POWER_MANAGER_BATT_STATS_SERVICE_ID              = 3304,
    DISPLAY_MANAGER_SERVICE_ID                       = 3308,
    IVIPOWER_ENHANCED_SERVICE_ID                     = 3309,
    SUBSYS_ROUTER_SYS_ABILITY_ID_BEGIN               = 3400,
    SUBSYS_SECURITY_SYS_ABILITY_ID_BEGIN             = 3500,
    ACCESS_TOKEN_MANAGER_SERVICE_ID                  = 3503,
    TOKEN_SYNC_MANAGER_SERVICE_ID                    = 3504,
    PRIVACY_MANAGER_SERVICE_ID                       = 3505,
    SECURITY_COMPONENT_SERVICE_ID                    = 3506,
    LOCAL_CODE_SIGN_SERVICE_ID                       = 3507,
    SANDBOX_MANAGER_SERVICE_ID                       = 3508,
    DEVICE_SECURITY_LEVEL_MANAGER_SA_ID              = 3511,
    CERT_MANAGER_SERVICE_SA_ID                       = 3512,
    DEVICE_THREAT_DETECTION_SERVICE_SA_ID            = 3513,
    DLP_PERMISSION_SERVICE_ID                        = 3521,
    RISK_ANALYSIS_MANAGER_SA_ID                      = 3523,
    DATA_COLLECT_MANAGER_SA_ID                       = 3524,
    DLP_CREDENTIAL_SERVICE_ID                        = 3553,
    SUBSYS_SENSORS_SYS_ABILITY_ID_BEGIN              = 3600,
    SENSOR_SERVICE_ABILITY_ID                        = 3601,
    MISCDEVICE_SERVICE_ABILITY_ID                    = 3602,
    EXTSHB_SERVICE_ABILITY_ID                        = 3603,
    MEDICAL_SENSOR_SERVICE_ABILITY_ID                = 3605,
    SUBSYS_SMALLSERVICES_SYS_ABILITY_ID_BEGIN        = 3700,
    PASTEBOARD_SERVICE_ID                            = 3701,
    TIME_SERVICE_ID                                  = 3702,
    INPUT_METHOD_SYSTEM_ABILITY_ID                   = 3703,
    SCREENLOCK_SERVICE_ID                            = 3704,
    WALLPAPER_MANAGER_SERVICE_ID                     = 3705,
    DOWNLOAD_SERVICE_ID                              = 3706,
    PRINT_SERVICE_ID                                 = 3707,
    SCAN_SERVICE_ID                                  = 3708,
    SUBSYS_SOURCECODETRANSFORMER_SYS_ABILITY_ID_BEGIN = 3800,
    SUBSYS_STARTUP_SYS_ABILITY_ID_BEGIN              = 3900,
    PARAM_WATCHER_DISTRIBUTED_SERVICE_ID             = 3901,
    SYSPARAM_DEVICE_SERVICE_ID                       = 3902,
    SUBSYS_TELEPONY_SYS_ABILITY_ID_BEGIN             = 4000,
    TELEPHONY_SYS_ABILITY_ID                         = 4001,
    DCALL_SYS_ABILITY_ID                             = 4002,
    TELEPHONY_CALL_MANAGER_SYS_ABILITY_ID            = 4005,
    TELEPHONY_CELLULAR_CALL_SYS_ABILITY_ID           = 4006,
    TELEPHONY_CELLULAR_DATA_SYS_ABILITY_ID           = 4007,
    TELEPHONY_SMS_MMS_SYS_ABILITY_ID                 = 4008,
    TELEPHONY_STATE_REGISTRY_SYS_ABILITY_ID          = 4009,
    TELEPHONY_CORE_SERVICE_SYS_ABILITY_ID            = 4010,
    TELEPHONY_DATA_STORAGE_SYS_ABILITY_ID            = 4012,
    TELEPHONY_IMS_SYS_ABILITY_ID                     = 4014,
    SUBSYS_UPDATE_SYS_ABILITY_ID_BEGIN               = 4100,
    SYS_INSTALLER_DISTRIBUTED_SERVICE_ID             = 4101,
    QUICKFIX_ENGINE_SERVICE_ID                       = 4102,
    MODULE_UPDATE_SERVICE_ID                         = 4103,
    SUBSYS_USB_SYS_ABILITY_ID_BEGIN                  = 4200,
    USB_SYSTEM_ABILITY_ID                            = 4201,
    SUBSYS_WEARABLE_SYS_ABILITY_ID_BEGIN             = 4300,
    SUBSYS_WEARABLEHARDWARE_SYS_ABILITY_ID_BEGIN     = 4400,
    SUBSYS_IVI_SYS_ABILITY_ID_BEGIN                  = 4500,
    IVI_DRIVINGSAFETY_SA_ID                          = 4501,
    IVI_CONFIGMANAGER_SA_ID                          = 4502,
    IVI_COCKPITMONITOR_SA_ID                         = 4503,
    WINDOW_MANAGER_ID                                = 4600,
    VSYNC_MANAGER_ID                                 = 4601,
    VSYNC_MANAGER_TEST_ID                            = 4602,
    GRAPHIC_DUMPER_SERVICE_SA_ID                     = 4603,
    GRAPHIC_DUMPER_COMMAND_SA_ID                     = 4604,
    ANIMATION_SERVER_SA_ID                           = 4605,
    WINDOW_MANAGER_SERVICE_ID                        = 4606,
    DISPLAY_MANAGER_SERVICE_SA_ID                    = 4607,
    SOFTBUS_SERVER_SA_ID                             = 4700,
    DEVICE_AUTH_SERVICE_ID                           = 4701,
    SUBSYS_DISTRIBUTED_HARDWARE_SYS_ABILITY_ID_BEGIN = 4800,
    DISTRIBUTED_HARDWARE_SA_ID                       = 4801,
    DISTRIBUTED_HARDWARE_DEVICEMANAGER_SA_ID         = 4802,
    DISTRIBUTED_HARDWARE_CAMERA_SOURCE_SA_ID         = 4803,
    DISTRIBUTED_HARDWARE_CAMERA_SINK_SA_ID           = 4804,
    DISTRIBUTED_HARDWARE_AUDIO_SOURCE_SA_ID          = 4805,
    DISTRIBUTED_HARDWARE_AUDIO_SINK_SA_ID            = 4806,
    DISTRIBUTED_HARDWARE_SCREEN_SOURCE_SA_ID         = 4807,
    DISTRIBUTED_HARDWARE_SCREEN_SINK_SA_ID           = 4808,
    DISTRIBUTED_HARDWARE_INPUT_SOURCE_SA_ID          = 4809,
    DISTRIBUTED_HARDWARE_INPUT_SINK_SA_ID            = 4810,
    DEVICE_STORAGE_MANAGER_SERVICE_ID                = 5000,
    STORAGE_SERVICE_ID                               = 5001,
    STORAGE_MANAGER_DAEMON_ID                        = 5002,
    STORAGE_MANAGER_MANAGER_ID                       = 5003,
    FILE_ACCESS_SERVICE_ID                           = 5010,
    DEVICE_SERVICE_MANAGER_SA_ID                     = 5100,
    HDF_EXTERNAL_DEVICE_MANAGER_SA_ID                = 5110,
    SUBSYS_FILEMANAGEMENT_SYS_ABILITY_ID_BEGIN       = 5200,
    FILEMANAGEMENT_DISTRIBUTED_FILE_DAEMON_SA_ID     = 5201,
    FILEMANAGEMENT_DISTRIBUTED_FILE_SERVICE_SA_ID    = 5202,
    FILEMANAGEMENT_BACKUP_SERVICE_SA_ID              = 5203,
    FILEMANAGEMENT_CLOUD_SYNC_SERVICE_SA_ID          = 5204,
    FILEMANAGEMENT_CLOUD_DAEMON_SERVICE_SA_ID        = 5205,
    FILEMANAGEMENT_CLOUD_BACKUP_SERVICE_SA_ID        = 5206,
    AOT_COMPILER_SERVICE_ID                          = 5300,
    SUBSYS_TESTPLATFORM_SYS_ABILITY_ID_BEGIN         = 5500,
    DEVICE_ATTEST_PROFILE_SA_ID                      = 5501,
    TEST_SERVER_SA_ID                                = 5502,
    DISTRIBUTED_DEVICE_PROFILE_SA_ID                 = 6001,
    ADVERTISING_SA_ID                                = 6104,
    ECOLOGICAL_RULE_MANAGER_SA_ID                    = 6105,
    APP_DOMAIN_VERIFY_MANAGER_SA_ID                  = 6200,
    APP_DOMAIN_VERIFY_AGENT_SA_ID                    = 6201,
    SUBSYS_ACE_SYS_ABILITY_ID_BEGIN                  = 7001,
    ARKUI_UI_APPEARANCE_SERVICE_ID                   = 7002,
    CA_DAEMON_ID                                     = 8001,
    ASSET_SERVICE_ID                                 = 8100,
    EL5_FILEKEY_MANAGER_SERVICE_ID                   = 8250,

    // reserved for vendor
    VENDOR_SYS_ABILITY_ID_BEGIN                      = 0x00010000,  // 65536
    MEDIA_MONITOR_SERVICE_ID                         = 66160,
    VENDOR_SYS_ABILITY_ID_END                        = 0x00020000,  // 131072
    LAST_SYS_ABILITY_ID                              = 0x00ffffff,  // 16777215
};
} // namespace OHOS

#endif
