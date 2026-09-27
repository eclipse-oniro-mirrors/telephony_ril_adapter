# 构建与测试（verify）

## 环境前提

- 本仓是 OpenHarmony 部件，**不能独立 gn 构建**；需置于源码树 `base/telephony/ril_adapter/` 下，从源码树根执行 `hb build`（hb 工具驱动 gn+ninja）。
- 外部依赖：`drivers_interface_ril`（HDI 接口 libril_proxy_1.1~1.5）、power HDI V1_2（RunningLock）。

## 构建命令

```sh
hb set                                   # 选择产品
hb build ril_adapter                     # 部件全量（4 个目标：hril_innerkits/hril/hril_hdf/ril_vendor）
hb build -T //base/telephony/ril_adapter/services/hril:hril                    # hril 核心库（CFI 开启）
hb build -T //base/telephony/ril_adapter/interfaces/innerkits:hril_innerkits   # 接口库
hb build -T //base/telephony/ril_adapter/services/hril_hdf:hril_hdf            # HDF 驱动库
hb build -T //base/telephony/ril_adapter/services/vendor:ril_vendor            # 参考厂商库
```

## 测试命令

```sh
# 主单测套件（按业务域一域一文件，含 mock 回调与分支覆盖）
hb build -T //base/telephony/ril_adapter/test/unittest/ril_adapter_gtest:tel_ril_adapter_gtest
# 旧式接口测试（直调 HRilManager，需真机/模拟 modem）
hb build -T //base/telephony/ril_adapter/test/unittest:ril_adapter_test
# fuzz（7 个 fuzzer，单个示例；顶层聚合 test/fuzztest:fuzztest）
hb build -T //base/telephony/ril_adapter/test/fuzztest:newsmsnotify_fuzzer:fuzztest
```

## 测试覆盖边界（重要）

| 测试 | 覆盖 | 不覆盖 |
|---|---|---|
| tel_ril_adapter_gtest（7 个业务域 + callback + zero_branch） | hril 层 C++：请求/响应转换、错误分支、非法参数（`#define private public` 侵入式） | vendor 层 C 解析器、真实 modem 交互 |
| ril_adapter_test（旧式） | 通过 sptr<IRil V1_5> 连真实 HDI 服务（集成风格，`_0100` 正常/`_0200` 错误成对） | 无 modem 时不可跑 |
| fuzztest（7 个 fuzzer） | hril 层通知上报解析路径（OnXxxReport 分发链，FuzzedDataProvider 生成随机 slotId/错误码/字段） | vendor 层 AT 解析 |

**结论：vendor 层（C，AT 解析）无 host 侧测试命令**。修改 `services/vendor/` 的解析代码后，验证手段 = 代码评审（对照 defect-patterns.md / at-protocol.md）+ 真机 modem 日志，不能期望跑 gtest。若需加固 vendor 解析，可先在 fuzztest/zero_branch 补充 hril 层触发用例间接覆盖。

## 任务类型 → 验证命令矩阵

| 任务类型 | 编译目标 | 测试命令 |
|---|---|---|
| 业务域逻辑修改（call/data/modem/network/sim/sms/emc_rescue） | `-T .../services/hril:hril`（改 hril 层时）+ `-T .../services/vendor:ril_vendor`（改 vendor 层时） | 对应域 gtest：`-T .../test/unittest/ril_adapter_gtest:tel_ril_adapter_gtest`（跑对应域用例，如 ril_network_test） |
| 接口/枚举变更（HREQ_*/HNOTI_*/hril.h） | `-T .../interfaces/innerkits:hril_innerkits` + 全量 `hb build ril_adapter` | gtest 全量 + fuzz：`-T .../test/fuzztest:fuzztest`（通知路径变更时） |
| hril_hdf 厂商库加载逻辑 | `-T .../services/hril_hdf:hril_hdf` | 无单测，真机验证加载日志（dlopen/RilInitOps 注册） |
| vendor 层 AT 解析修改 | `-T .../services/vendor:ril_vendor` | **无 host 测试命令**（见覆盖边界）；回退：代码评审对照 defect-patterns.md + 真机 modem 日志；如需间接覆盖，先在 zero_branch_test/fuzz 补 hril 层触发用例 |
| 缺陷修复（越界/内存/错误码） | 对应层目标 | 修复点对应域 gtest + 检查清单自检（defect-patterns.md 模式 1-9） |

## 格式与静态检查

- 代码风格：OpenHarmony C/C++（4 空格缩进，`_s` 安全函数，错误码使用 HRIL_ERR_*）。
- 提交消息：涉及 DTS 问题单时遵循仓内格式（TicketNo/Description/Team/... AUTO-GENERATED 模板）。

## Done 定义

1. [ ] 所有修改的文件 LSP 诊断无新增 error
2. [ ] 编译成功（`hb build` 对应目标）
3. [ ] 代码格式化已执行（OpenHarmony C/C++ 风格，缩进 4 空格）
4. [ ] 相关测试用例通过（gtest 目标或已说明覆盖边界）
5. [ ] 未引入公共 API/ABI 不兼容变更（hril.h / HDI 版本）
6. [ ] 未违反约束和边界中列出的禁止事项
