# 事件号枚举同步规则（expert）

## 枚举定义位置

| 枚举 | 定义文件 | 配套映射 |
|---|---|---|
| HREQ_*（请求号） | `interfaces/innerkits/include/hril_request.h` | `services/hril/include/hril_event_map.h`（请求号→名称，仅日志） |
| HNOTI_*（通知号） | `interfaces/innerkits/include/hril_notification.h` | `services/hril/include/hril_notification_map.h`（通知号→NEED_LOCK/UNNEED_LOCK，运行锁策略） |

## 分发机制

- HRilBase 构造函数中通过 `AddBasicHandlerToMap` / `AddNotificationToMap` 把事件号注册到 `respMemberFuncMap_` / `notiMemberFuncMap_`（`hril_base.h`）。
- 请求到来/上报返回时按事件号查表分发——**枚举定义与 map 注册不一致 = 分发静默失败**。

## 变更清单（增删改 HREQ_*/HNOTI_* 必须同步）

1. `hril_request.h` / `hril_notification.h`：枚举本体。
2. `hril_event_map.h`：请求号名称映射（调试日志可读性）。
3. `hril_notification_map.h`：通知号锁策略——需要保活的通知（异步上报、随系统休眠）必须 `NEED_LOCK`，否则进程可能被杀。
4. 业务域类构造函数：`AddBasicHandlerToMap` / `AddNotificationToMap` 注册处理函数。
5. HDI 侧：IRilCallback 对应回调（外部 drivers_interface_ril，需跨仓同步）。
6. 业务数据结构（hril_vendor_*_defs.h）若随枚举扩展，遵守 abi-contracts.md 的追加规则。

## 检查方法

- 编译期：枚举与 map 均为运行时表，编译不报错——靠 grep 双向核对。
- 运行期：请求/通知丢失时查日志（事件号名称映射）与 `zero_branch_test` / fuzztest 用例覆盖。
