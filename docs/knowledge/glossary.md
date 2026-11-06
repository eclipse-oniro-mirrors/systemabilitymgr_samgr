# 术语表

业务、实体与机制的共同词汇。源码位置见[实体](samgr-Entity.md)和[技术机制](samgr-Technology.md)，术语采用当前仓语义。

## 系统能力（SA / System Ability）

通过said登记和发现的远程服务；不是业务进程本身，一个进程可有多个SA。对应SAInfo与SystemAbilityContext。

## 系统能力标识（said）

管理接口使用的整型ID。当前公共CheckInputSysAbilityId接受0至0x00ffffff；具体交易还有其他条件，根AGENTS从1开始的描述不等于当前公共实现。

## 系统管理器与进程内管理器（samgr / LSA）

samgr跨进程协调能力；LocalAbilityManager位于SA进程侧处理Start/Stop/Idle/Active等。代理由services/lsamgr实现。

## 配置声明与注册对象（SaProfile / SAInfo）

前者描述部署和策略，后者存可查询对象。配置存在不保证对象已注册。

## 按需加载与卸载（on-demand）

接口或系统事件触发生命周期操作；受理、状态推进、下游动作和完成通知是分开的阶段。

## 加载回调与状态监听（LoadCallback / StatusChange）

加载回调对应请求完成，状态listener对应能力添加/移除。两者的计数、死亡recipient与清理语义不同。

## 能力状态与进程状态

SA有NOT_LOADED/LOADING/LOADED/UNLOADABLE/UNLOADING；进程有NOT_STARTED/STARTED/STOPPING。状态枚举顺序不是合法迁移表。

## 内部启动记录状态（AbilityState）

AbilityItem中的INIT/STARTING/STARTED，不是调度器的SA五态。

## 空闲与可卸载（Idle / UNLOADABLE）

进入可卸载阶段不代表对象立即移除或进程已停止，受策略和后续事件影响。

## 按需事件与条件（OnDemandEvent / OnDemandCondition）

事件保存类型、name/value、条件和额外数据；当前相等比较只看eventId/name/value。

## 本地设备组与远端来源

加载等待按设备键分组，RPC回调还保存来源网络ID；不能只用said关联所有请求。

## 多实例manager与前台用户

多实例SA可按用户拥有独立manager；base调用者的隐式查询可能路由到前台用户，停止中manager已从可路由集合退出。

## 动态缓存水位（water line）

DynamicCache用参数字符串判断本地缓存是否仍适用，结合said与对象存活判断；不同于只按TTL缓存。

## FFRTHandler

samgr中的任务封装，用于工作、死亡、采集等任务；它的存在不代表全部共享状态都无锁串行。

## IPC descriptor与interface token

descriptor为OHOS.ISystemAbilityManager，交易token为ohos.samgr.accessToken；两者用途不同。

## DFX与SamgrXCollie

故障/耗时/事件/trace和阻塞监测基础设施；日志或事件缺失需要结合配置、阈值与采集完整性解释。

## 扩展属性（extension）

profile中的扩展标识用于筛选配置/运行对象，不是任意插件库加载的同义词。

## 启动阶段（BootPhase）

SaProfile.bootPhase表达启动优先级，ParseUtil映射相应字符串；与单SA状态机生命周期阶段不同。
