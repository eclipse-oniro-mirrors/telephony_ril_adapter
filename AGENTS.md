# telephony_ril_adapter 领域知识

## 项目定位

本仓库为 OpenHarmony 电话子系统（telephony）的 RIL Adapter 部件（part: `ril_adapter`）。职责：加载 modem 厂商库、实现 RIL 业务接口、事件调度管理，屏蔽不同厂商 modem 差异，通过 HDF 服务与上层电话服务通讯。优先按这些目录定位问题：

- `services/hril/`：核心业务逻辑层（C++）。HRilManager 单例（请求调度/上报分发/运行锁）、HRilBase 基类（事件号→处理函数映射）、7 个业务域类、select 事件循环与定时器。
- `services/hril_hdf/`：HDF 驱动服务（C），dlopen 加载厂商库（`RilInitOps`）、读系统参数、创建事件循环线程。
- `services/vendor/`：参考 AT 厂商库（C），AT 命令收发与响应解析、URC 主动上报。
- `interfaces/innerkits/`：对外 C 接口头文件（hril.h 契约、HREQ_*/HNOTI_* 枚举、业务数据结构）。
- `utils/native/`：hilog 日志封装（C/C++）。
- `test/unittest/`、`test/fuzztest/`：单元测试与 fuzz 测试。

## 任务到路径映射

| 任务类型 | 先看这里 | 原因 |
|---|---|---|
| 呼叫/数据/网络/SIM/短信/Modem/紧急救援业务修改 | 见下方「业务域文件对照表」 | 每个业务域 hril 层与 vendor 层各一对文件：hril 层实现接口与转换，vendor 层实现 AT 指令，请求统一经 hril_manager.cpp 分发 |

**业务域文件对照表**（hril 层 ↔ vendor 层配对文件，请求经 `HRilManager::TaskSchedule` 按 `MODULE_HRIL_*` + slotId 分发）：

| 业务域 | hril 层（接口/转换） | vendor 层（AT 指令） | 典型请求/备注 |
|---|---|---|---|
| 呼叫 | `services/hril/src/hril_call.cpp` | `services/vendor/src/at_call.c` | Dial/Reject/Hangup/会议/DTMF（Dial→hril_manager.cpp:444） |
| 数据 | `services/hril/src/hril_data.cpp` | `services/vendor/src/at_data.c` | ActivatePdpContext/GetPdpContextList/带宽/切片 |
| 网络 | `services/hril/src/hril_network.cpp` | `services/vendor/src/at_network.c` | 搜网/注册/信号（GetSignalStrength→AT^HCSQ?，at_network.c:464；最大源文件） |
| SIM | `services/hril/src/hril_sim.cpp` | `services/vendor/src/at_sim.c` | SIM IO/锁/STK/逻辑通道 |
| 短信 | `services/hril/src/hril_sms.cpp` | `services/vendor/src/at_sms.c` | SMS 收发/NewSMS 主动上报 |
| Modem | `services/hril/src/hril_modem.cpp` | `services/vendor/src/at_modem.c` | 射频状态/IMEI/基带版本 |
| 紧急救援 | `services/hril/src/hril_emc_rescue.cpp` | 无 `at_emc_rescue.c` | hril 独有域，经 emcRescueFuncs_ 走 RequestVendor；会劫持 Call 部分路由（GetCallList/Dial 等） |
| 请求调度/上报分发/运行锁 | `services/hril/src/hril_manager.cpp` | 全局唯一调度中枢（TaskSchedule/OnXxxReport/CreateHRilRequest） |
| 事件循环/定时器 | `services/hril/src/hril_event.cpp` + `hril_timer_callback.cpp` | select 事件循环与定时器链表 |
| 事件号→处理函数映射 | `services/hril/include/hril_base.h` | respMemberFuncMap_/notiMemberFuncMap_ |
| 请求号/通知号枚举 | `interfaces/innerkits/include/hril_request.h`（HREQ_*）、`hril_notification.h`（HNOTI_*） | 枚举定义处 |
| 厂商库加载/匹配 | `services/hril_hdf/src/hril_hdf.c` | LoadVendor/dlopen/RilInitOps |
| 厂商库契约 | `interfaces/innerkits/include/hril.h` | HRilReport/HRilOps/RilInitOps |
| HDI 版本数据路径（V1_1/V1_3） | `services/hril/src/hril_manager.cpp`（hrilOpsVersion_）+ 各域 `*Response*` 函数 | 版本分流在 manager，转换在各域 |
| slotId 语义/回退 | `services/vendor/src/vendor_util.c`（GetSlotId/GetSimSlotCount） | 槽位判定唯一入口 |
| 日志 | `utils/native/include/telephony_log_c.h`、`telephony_log_wrapper.h` | hilog 封装 |

## 知识索引

稳定背景知识放在 `docs/knowledge/`。改动前按场景读取对应文件：

| 场景 | 先读 |
| --- | --- |
| 修改请求/通知处理链路、上报数据结构 | `docs/knowledge/code_map/business-domains.md` |
| 追踪一次请求/上报的完整数据流与缓冲所有权 | `docs/knowledge/code_map/data-flow.md` |
| 修改多版本 HDI 数据转换 | `docs/knowledge/code_map/hdi-version-paths.md` |
| 涉及 slotId、双卡/虚拟卡 | `docs/knowledge/code_map/slot-model.md` |
| 修改 AT 解析、modem 报文相关 | `docs/knowledge/routing/at-protocol.md` |
| 修复越界/内存/错误码/解析缺陷 | `docs/knowledge/expert/defect-patterns.md` |
| 改动 hril.h 契约、厂商库接口 | `docs/knowledge/expert/abi-contracts.md` |
| 增删 HREQ_*/HNOTI_* 枚举 | `docs/knowledge/expert/enum-sync.md` |
| 涉及 ReqDataInfo/requestList_/回调内指针 | `docs/knowledge/expert/memory-rules.md` |
| 涉及多线程/锁/运行锁 | `docs/knowledge/expert/concurrency.md` |
| 涉及安全加固/参数校验 | `docs/knowledge/expert/security.md` |
| 构建/测试命令与覆盖边界 | `docs/knowledge/verify/build-and-test.md` |

## 高风险区域

改动这些位置前必读对应 expert/routing 文档，完成后按 defect-patterns.md 检查清单自检：

| 高风险位置 | 风险点 | 先读 |
|---|---|---|
| `services/vendor/src/at_network.c`（2069 行，最大源文件） | 各制式解析函数群、strsep 手工解析、字段错位 | `routing/at-protocol.md`、`expert/defect-patterns.md` |
| `services/hril/src/hril_manager.cpp` | 全局 dispatchMutex 串行化（吞吐瓶颈/持锁）、requestList_ 手写内存池、hrilOpsVersion_ 分支 | `expert/concurrency.md`、`expert/memory-rules.md` |
| `services/hril/src/hril_network.cpp` | 多 HDI 版本响应函数族、结构逐字段转换 | `code_map/hdi-version-paths.md` |
| `services/vendor/src/at_support.c` + `vendor_channel.c` | 读线程/发送线程共享全局响应状态、g_buffer 分帧、超时竞态 | `code_map/data-flow.md`、`expert/concurrency.md` |
| `services/vendor/src/vendor_report.c` | 30+ 分支前缀 if-else 链、mock 数据分支 | `routing/at-protocol.md` |
| `services/hril/src/hril_event.cpp` | listenEventTable_ 固定 8 槽（满则静默丢弃）、fd_set 全局拷贝 | `expert/concurrency.md` |
| `services/hril/src/hril_emc_rescue.cpp` | isEmcRescueMode_ 静态状态劫持 Call 路由（跨模块耦合） | `code_map/business-domains.md` |

## 词汇触发路由

| 术语/缩写 | 含义 | 指向 |
|-----------|------|------|
| **HRilManager** | hril 层单例，请求调度与上报分发中枢 | `services/hril/src/hril_manager.cpp` |
| **HRilBase** | 业务域基类，事件号→处理函数双 map | `services/hril/include/hril_base.h` |
| **HRilReport** | 框架给 vendor 的 8 个上报回调（7 业务 + 定时器） | `interfaces/innerkits/include/hril.h` |
| **HRilOps** | vendor 返回的 7 个业务函数表 + version | `interfaces/innerkits/include/hril.h` |
| **RilInitOps** | 厂商库导出入口函数（dlopen 后 dlsym） | `services/vendor/include/vendor_adapter.h` |
| **HREQ_\*** | 请求号枚举 | `interfaces/innerkits/include/hril_request.h` |
| **HNOTI_\*** | 通知号枚举 | `interfaces/innerkits/include/hril_notification.h` |
| **ReqDataInfo** | 请求上下文（slotId/serial/request），手写内存池管理 | `interfaces/innerkits/include/hril_public_struct.h` |
| **ReportInfo** | vendor→框架上报载体（type=RESPONSE/NOTIFICATION） | `interfaces/innerkits/include/hril_public_struct.h` |
| **NEED_LOCK** | 通知是否需要申请 RunningLock 保活 | `services/hril/include/hril_notification_map.h` |
| **dispatchMutex** | 全局串行化所有请求的 pthread 锁 | `services/hril/src/hril_manager.cpp` |
| **EMC Rescue** | 紧急救援业务域（2025 新增），会劫持 Call 的部分路由 | `services/hril/src/hril_emc_rescue.cpp` |
| **vSIM** | 虚拟卡（3 槽 = 2SIM + 1vSIM） | `services/vendor/src/vendor_util.c` GetSimSlotCount |

## 约束和边界

### 禁止事项
- **禁止** 修改 `interfaces/innerkits/include/hril.h` 中 `HRilReport`/`HRilOps` 字段的顺序、类型或删除字段（dlopen ABI 契约，破坏所有厂商库）。
- **禁止** 用 `std::string((const char *)response)` 无长度构造字符串；必须带 `responseLen` 并校验（见 defect-patterns.md）。
- **禁止** 解引用 response 指针前不校验 `responseLen == 0` 与 `% sizeof()` 对齐。
- **禁止** 增删 HREQ_*/HNOTI_* 枚举而不同步 `hril_event_map.h` / `hril_notification_map.h`。
- **禁止** vendor 侧跨回调持有 `ReportInfo.requestInfo` 指针（仅回调期间有效）。
- **禁止** 改 3 线程模型的核心结构（hril 事件循环线程 / vendor 读线程 / vendor 监听线程）的同步方式而不评估 RunningLock 与 dispatchMutex 的影响。
- **禁止** 为防御过度收缩合法协议范围（有 revert 教训，见 security.md）。
- **禁止** 假设 vendor 层（C，AT 解析）可跑 host 侧单测——gtest/fuzz 只覆盖 hril 层。
- **禁止** 修改 slot 循环边界语义而不考虑 vSIM（`<= hrilSimSlotCount_` 与 `<` 的区别）。

## 构建和验证

构建命令从 OpenHarmony 源码树根执行（本仓需位于 `base/telephony/ril_adapter/`，不能独立 gn 构建）：

```sh
hb set   # 选择产品
hb build ril_adapter                          # 部件全量
hb build -T //base/telephony/ril_adapter/services/hril:hril
hb build -T //base/telephony/ril_adapter/interfaces/innerkits:hril_innerkits
hb build -T //base/telephony/ril_adapter/services/hril_hdf:hril_hdf
hb build -T //base/telephony/ril_adapter/services/vendor:ril_vendor
hb build -T //base/telephony/ril_adapter/test/unittest/ril_adapter_gtest:tel_ril_adapter_gtest
hb build -T //base/telephony/ril_adapter/test/unittest:ril_adapter_test
hb build -T //base/telephony/ril_adapter/test/fuzztest:newsmsnotify_fuzzer:fuzztest
```

## 编辑前声明

动手修改前，先声明（在回复开头）：

```
任务类别：<业务域修改 / 接口变更 / 缺陷修复 / 测试 / 其他>
已读文档：<本次已读 docs/knowledge/ 下的文件列表>
已知约束：<约束和边界中与本任务相关的条目>
```

## Done 定义

1. [ ] 所有修改的文件 LSP 诊断无新增 error
2. [ ] 编译成功（`hb build` 对应目标）
3. [ ] 代码格式化已执行（OpenHarmony C/C++ 风格，缩进 4 空格）
4. [ ] 相关测试用例通过（gtest 目标或已说明覆盖边界）
5. [ ] 未引入公共 API/ABI 不兼容变更（hril.h / HDI 版本）
6. [ ] 未违反约束和边界中列出的禁止事项
