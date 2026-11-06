# 编码铁律

本页提炼根AGENTS及当前实现中的领域约束。源码相对路径从仓根解析；发现文档与实现差异时分别说明，不以旧示例覆盖当前代码。

## 并发模型
- 注册表abilityMap_读使用shared_lock，写使用unique_lock；禁止绕过abilityMapLock_直接变更。其他表遵循各自锁，不能照搬单队列串行访问的插件模型。
- 服务目标在BUILD.gn定义SAMGR_USE_FFRT；按现有FFRTHandler封装投递工作，根AGENTS要求不新增std::thread。客户端DynamicCache本身使用std::mutex，不能把服务约束误读为现有全仓一律禁用std锁。
- 延迟/回调路径检查对象存活与停止时序；不能以取消排队任务推断已执行任务被终止。多用户manager生命周期与常驻单例的持有关系分别分析。

## 生命周期与资源
- 新增加载callback要同步考虑成功、超时、死亡及失败投递的释放路径。释放原调用PID的callbackCountMap_，不能使用异步线程当下身份。
- 订阅去重与额度检查顺序是业务语义；本地同said同对象先去重，再检查256上限。解除和死亡需匹配原关系并释放计数。
- SA死亡、listener死亡、callback死亡、进程对象死亡分别走对应recipient，不能用统一“删除SA”处理所有对象。
- 注册/移除存在晚期错误：例如scheduler空可能在表已写入/擦除之后返回。禁止未经核对就假定错误返回必然无副作用。

## IPC与错误处理
- 交易token是 `u"ohos.samgr.accessToken"`，不是接口descriptor `OHOS.ISystemAbilityManager`。统一检查和具体方法授权是不同层。
- 按根AGENTS保持既有SamgrInterfaceCode顺序，新码追加；接口变化同步两侧封包/解包、权限、测试和语言绑定。
- Proxy参数失败和Base参数失败可能使用不同错误码。异步ERR_OK仅表示当前阶段受理，禁止作为完成成功。
- 写状态的交易按项目要求核查AccessToken/相关权限；只凭存在统一token校验不能认为所有操作均已授权。

## 配置与平台
- 新增按需事件同时检查解析、事件枚举、采集插件映射、调度处理和清理；缺编译宏时profile事件未必有执行插件。
- 默认GN参数与实际产品参数分开。新增多实例行为覆盖普通调用、base调用、显式userId及停止中的manager。
- 不将参考仓的SA范围、超时、配额、队列和权限硬编码为本仓规则。当前公共ID校验包含0，根AGENTS术语写从1开始存在差异，具体操作仍可能附加限制。

## 标识与生成源
- 新增SA ID按根AGENTS检查本仓system_ability_definition.h、Rust definition.rs及hidumper跨仓映射同步关系；先识别真实源和派生产物，不直接改生成输出。
- 修改Rust接口检查manage/wrapper/C++桥接及cfg(multi_instance)，避免只更新C++签名。
- 根AGENTS禁止samgr直接include业务SA头；使用通用远程对象和said。

## DFX与构建
- 新增加载/卸载/错误路径按项目约束补DFX；事件名/字段与hisysevent.yaml一致，使用services/dfx的适配函数。
- 阻塞型IPC/调度变更按项目要求检查SamgrXCollie。其构建宏、回调及超时处理不能被示例默认值替代。
- 按目标BUILD.gn保持既有CFI等安全构建配置；豁免需有具体原因，不能为消除错误泛化关闭。
- 修改业务实现时运行受影响测试；本次没有业务改动，未执行这些运行验证。知识文档不宣称测试已通过。
