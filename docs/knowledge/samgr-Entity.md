# 业务实体知识

samgr的主要业务实体，按“实体概念、上下文与场景、规格与约束、知识关联、代码与符号”组织。路径相对仓根；[技术机制](samgr-Technology.md)解释这些实体如何参与运行。

## 系统能力登记记录（SAInfo）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 可查询的服务对象记录，以当前manager内的said为索引。 |
| 核心特征 | remoteObj保存IRemoteObject；isDistributed标识分布式属性。当前结构不含根README旧示例中的capability/permission字段。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | AddSystemAbility校验后写abilityMap_；查询读记录；主动移除或对象死亡清除。覆盖登记会触发缓存失效及相关通知。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 存在记录表示此时可查对象，不保证后续远端一直存活；注册后续步骤失败可能已改变记录。 |
| 并发与生命周期约束 | 读共享锁、写独占锁；同said的含义还受所属manager/用户影响。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | SystemAbilityManager按用户选择Base或Multi manager。 |
| 下游影响 | 查询结果、监听通知、加载完成和缓存。 |
| 平级关联 | SaProfile描述配置，SystemAbilityContext描述调度；三者不等价。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `services/samgr/native/include/base_system_ability_manager.h` 中的 `SAInfo` |
| 关键方法 | `services/samgr/native/source/base_system_ability_manager.cpp` 中的 `AddSystemAbility` |

## 能力配置与派生视图（SaProfile）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 描述能力如何部署和调度的静态配置，不是已经加载的对象。 |
| 核心特征 | process、saId、libPath、dependSa、bootPhase、runOnCreate、distributed、start/stopOnDemand、autoRestart、extension等。CommonSaProfile保留manager所需字段；CollMgrSaProfile保留采集事件。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | ParseUtil解析JSON，再由manager、采集器、调度器按用途组织模型。配置缺失可使加载在受理前返回PROFILE_NOT_EXIST。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 配置声明、进程启动、库加载和能力注册是不同阶段；解析失败不能用空默认模型当有效能力。 |
| 并发与生命周期约束 | 修改策略需核对allowUpdate等开关；结构默认值不代表每个产品实际取值。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | 产品profile及ParseUtil。 |
| 下游影响 | 按需触发、进程归属、回收策略、分布式和扩展筛选。 |
| 平级关联 | CommonSaProfile与CollMgrSaProfile为同能力的不同用途投影。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `interfaces/innerkits/common/include/sa_profiles.h` 中的 `SaProfile` |
| 关键方法 | `services/common/src/parse_util.cpp` 中的 `ParseSaProfiles` |

## 能力状态监听关系（SAListener）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 消费者订阅某said变化时保存的监听对象及原始调用身份。 |
| 核心特征 | listener、callingPid、state；ListenerState为INIT/NOTIFIED；listenerMap_以said聚合，subscribeCountMap_按PID累计。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 订阅校验→同对象去重→检查配额→保存及挂死亡通知→按现有服务状态通知；主动解除或listener死亡回收。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 相同said同AsObject重复订阅是幂等成功；本地超限返回ERR_PERMISSION_DENIED，不应等同权限配置失败。 |
| 并发与生命周期约束 | listenerMapLock_保护关系与计数；解除必须匹配实际对象并递减原PID计数。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | SubscribeSystemAbility及多用户全局订阅路径。 |
| 下游影响 | 添加/移除回调、订阅额度及死亡清理。 |
| 平级关联 | 进程状态listener使用另一接口；SA死亡不等于listener死亡。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `services/samgr/native/include/base_system_ability_manager.h` 中的 `SAListener` |
| 关键方法 | `services/samgr/native/source/base_system_ability_manager.cpp` 中的 `SubscribeSystemAbilityInner` |

## 加载期间记录（AbilityItem）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 加载尚未完成期间聚合said对应的触发上下文和等待回调。 |
| 核心特征 | state为内部INIT/STARTING/STARTED；callbackMap按设备分组，每项保存callback与callingPid；event保存触发事件。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | DoLoad对象已存在时直接安排通知；否则按对象去重并保存，随后启动进程/能力、安排检查；完成、超时和死亡分别处理回调。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | callbackCountMap_在manager内按PID计数，不是每个said单独额度；同对象去重先于上限检查。 |
| 并发与生命周期约束 | 保存/相关清理路径使用onDemandLock_；释放用记录的原PID；不得把内部AbilityState混同调度器SystemAbilityState。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | BaseSystemAbilityManager::startingAbilityMap_。 |
| 下游影响 | 异步通知、延迟检查、配额和回调死亡。 |
| 平级关联 | LoadRequestInfo是调度请求，AbilityItem是manager等待记录；二者身份不同。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `services/samgr/native/include/base_system_ability_manager.h` 中的 `AbilityItem` |
| 关键方法 | `services/samgr/native/source/base_system_ability_manager.cpp` 中的 `DoLoadSystemAbility` |

## 系统进程对象与启动记录（StartingProcessInfo）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 以进程名组织的管理对象与启动期间观测，用于协调同进程多个SA。 |
| 核心特征 | systemProcessMap_存LocalAbilityManager远程对象；StartingProcessInfo存procName、调用方/被调方身份、calleeSaId和开始时间。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | AddSystemProcess登记进程对象；生命周期动作查进程代理；进程死亡RemoveSystemProcess；启动记录用于过程管理与耗时。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 能找到进程对象不代表其中所有SA已注册；一个进程内可有多个SA，移除单能力不能默认杀整个进程。 |
| 并发与生命周期约束 | systemProcessMapLock_与startingProcessMapLock_分别保护各自表，不能只拿abilityMapLock_处理。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | Base的进程登记接口与启动协调。 |
| 下游影响 | LocalAbilityManager操作、进程状态和耗时DFX。 |
| 平级关联 | SystemProcessContext用于调度状态；StartingProcessInfo用于启动过程信息。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `services/samgr/native/include/base_system_ability_manager.h` 中的 `StartingProcessInfo` |
| 关键方法 | `services/samgr/native/source/base_system_ability_manager.cpp` 中的 `AddSystemProcess` |

## 能力调度上下文（SystemAbilityContext）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 调度器中单个SA的动态状态和待处理请求。 |
| 核心特征 | systemAbilityId、state、ownProcessContext、pendingLoadEventList/CountMap、pendingUnloadEvent、unloadRequest、delayUnloadTime及时间字段。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 初始化profile产生上下文；加载/卸载/取消/死亡事件进入调度器，再经事件处理和状态机守卫推进。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | NOT_LOADED/LOADING/LOADED/UNLOADABLE/UNLOADING；合法迁移由各handler.CanEnter定义，不是枚举顺序即迁移顺序。 |
| 并发与生命周期约束 | 迁移前更新所属进程的状态计数，成功后写state并OnEnter；不能只改state绕过守卫和计数。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | SystemAbilityStateScheduler::abilityContextMap_。 |
| 下游影响 | 实际加载/空闲/卸载动作、等待请求及超时恢复。 |
| 平级关联 | 所属SystemProcessContext共同决定进程级操作。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `services/samgr/native/include/schedule/system_ability_state_context.h` 中的 `SystemAbilityContext` |
| 关键方法 | `services/samgr/native/source/schedule/system_ability_state_machine.cpp` 中的 `AbilityStateTransitionLocked` |

## 进程调度上下文（SystemProcessContext）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 调度器按进程聚合SA、状态数量和重启控制的动态记录。 |
| 核心特征 | processName、saList、abilityStateCountMap、pid/uid、state、restartCountsCtrl、enableRestart、lastStopTime等。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 调度初始化建立SA与进程关联；SA迁移改变计数；启动/停止/死亡事件推进进程状态。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 进程状态NOT_STARTED/STARTED/STOPPING；进程已STARTED与SA已LOADED不等价。 |
| 并发与生命周期约束 | stateCountLock保护状态数量；进程锁和调度器映射锁负责其他范围，不能从一个计数锁推断整个对象无竞争。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | SystemAbilityStateScheduler::processContextMap_。 |
| 下游影响 | 进程启动/停止、空闲回收和重启策略。 |
| 平级关联 | 一进程关联多个SystemAbilityContext，计数反映其中各SA状态。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `services/samgr/native/include/schedule/system_ability_state_context.h` 中的 `SystemProcessContext` |
| 关键方法 | `services/samgr/native/source/schedule/system_ability_state_machine.cpp` 中的 `UpdateStateCount` |

## 生命周期请求（LoadRequestInfo / UnloadRequestInfo）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 调度器接收的操作消息，保留请求身份与事件原因。 |
| 核心特征 | LoadRequestInfo含deviceId、callback、systemAbilityId、callingPid、loadEvent；卸载请求含unloadEvent、said和PID。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | Base入口校验profile/调用方后构建请求，交调度器；当前状态可能导致执行、等待或拒绝。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 请求获受理与操作完成分开；卸载ERR_OK不能当对象已经移除。 |
| 并发与生命周期约束 | 待处理请求保存在调度上下文，替换或取消必须考虑原请求及迟到事件。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | Base接口、采集触发及调度器。 |
| 下游影响 | 状态转移、下游操作与结果通知。 |
| 平级关联 | OnDemandEvent表达原因；AbilityItem聚合等待回调，两者不能互相替代。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `services/samgr/native/include/schedule/system_ability_state_context.h` 中的 `LoadRequestInfo` |
| 关键方法 | `services/samgr/native/source/base_system_ability_manager.cpp` 中的 `UnloadSystemAbility` |

## 按需事件（OnDemandEvent）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 描述触发能力启动或停止的事件及附加条件。 |
| 核心特征 | eventId、name/value、extraDataId、persistence、conditions、enableOnce、loadPriority、extraMessages；类别包括接口、网络、开关、参数、公共事件、定时和引用。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | JSON解析生成策略，采集器接收外部变化并匹配能力；接口调用也构造事件作为原因。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 当前operator==只比较eventId/name/value，不能用相等结果证明所有条件和extraMessages完全一致。 |
| 并发与生命周期约束 | 特性宏控制插件是否存在；enableOnce、持久化及附加数据生命周期需按相应路径处理。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | StartOnDemand/StopOnDemand及运行中的采集器。 |
| 下游影响 | 加载/卸载请求、策略更新和事件原因传递。 |
| 平级关联 | OnDemandCondition为附加条件；SaControlInfo是匹配后的操作信息。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `interfaces/innerkits/common/include/sa_profiles.h` 中的 `OnDemandEvent` |
| 关键方法 | `services/samgr/native/source/collect/device_status_collect_manager.cpp` 中的 `FilterOnDemandSaProfiles` |

## 事件匹配控制信息（SaControlInfo）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 采集事件匹配后描述要对哪个SA实施何种按需控制。 |
| 核心特征 | ondemandId、saId、enableOnce、loadPriority、cacheCommonEvent。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | profile事件经采集manager组织和匹配，形成后续启动/停止控制列表；一次性事件和缓存数据要协调后续清理。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 不是profile全部字段副本，也不是远程服务对象；相同事件可关联多个能力。 |
| 并发与生命周期约束 | 按照采集manager和调度边界投递，插件不存在与事件条件不匹配需分别解释。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | DeviceStatusCollectManager与CollMgrSaProfile。 |
| 下游影响 | 按需事件调度、一次性策略及公共事件额外数据。 |
| 平级关联 | OnDemandEvent为输入语义，SaControlInfo为能力控制选择。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `interfaces/innerkits/common/include/sa_profiles.h` 中的 `SaControlInfo` |
| 关键方法 | `services/samgr/native/source/collect/device_status_collect_manager.cpp` 中的 `AddCollectEvents` |

## 用户manager及停止上下文（UserLifecycleManager）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 在多实例构建下，组织活动用户manager、前台身份和停止中对象。 |
| 核心特征 | multiUserManagers_、validUserIds_、userStateMap_、userStoppingContexts_及foregroundUserId_分别表示可路由、有效性、状态、停止持有与前台。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 激活构造Multi manager并初始化；切换调整前台；停止先移出可路由集合，再等待进程退出并Destroy。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | RouteForUser对非多实例said返回BASE_USER；多实例时base调用者走前台，其他调用者走自己。停止中找不到可路由manager可能是设计窗口。 |
| 并发与生命周期约束 | 按用户生命周期锁序列化操作；全局订阅快照、发布和停止清理有额外锁/身份检查，禁止绕过。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | SystemAbilityManager的用户路由与OnUserStateChanged。 |
| 下游影响 | 所有用户范围查询、加载、订阅及资源回收。 |
| 平级关联 | MultiSystemAbilityManager复用Base机制，但不等于共用同一份注册表。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `services/samgr/native/include/user_lifecycle_manager.h` 中的 `UserLifecycleManager` |
| 关键方法 | `services/samgr/native/source/user_lifecycle_manager.cpp` 中的 `HandleUserStopping` |

## 扩展查询结果（SaExtensionInfo）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体定义 | 按profile扩展标识关联运行SA及其所属进程对象的查询结果。 |
| 核心特征 | saId和processObj；接口还可单独返回扩展匹配ID和运行对象。 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | GetExtensionSaIds筛配置，GetExtensionRunningSaList/GetRunningSaExtensionInfoList结合运行状态形成结果。 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 配置匹配不代表能力正在运行；对象列表和ID列表的语义不能混同。 |
| 并发与生命周期约束 | 应按当前manager的表与锁读状态；不能把返回的processObj理解为业务SA对象。 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | ISystemAbilityManager扩展接口。 |
| 下游影响 | 扩展能力使用方及LocalAbilityManager扩展处理。 |
| 平级关联 | SaProfile.extension、SAInfo及进程管理对象共同决定结果。 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类/结构 | `interfaces/innerkits/samgr_proxy/include/if_system_ability_manager.h` 中的 `SaExtensionInfo` |
| 关键方法 | `services/samgr/native/source/base_system_ability_manager.cpp` 中的 `GetRunningSaExtensionInfoList` |
