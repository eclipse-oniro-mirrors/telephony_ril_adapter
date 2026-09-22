# 高频缺陷模式（expert）

2025-2026 年 10+ 笔"越界/安全/参数校验"修复提交沉淀出的检查清单。修复缺陷或写新代码时逐条对照。

## Agent 常见失败模式（先读）

| Agent 失败模式 | 应对 |
|---|---|
| 看到 `(const char *)response` 就构造字符串，不看长度 | 模式 1/2：必须带 responseLen 并校验 |
| 改 vendor 层（C/AT 解析）后期望跑 gtest 验证 | 不可能：vendor 层无 host 测试（verify/build-and-test.md 覆盖边界），改用评审 + 真机日志 |
| 修改 HREQ_*/HNOTI_* 枚举但漏同步 map 文件 | enum-sync.md 6 步清单 |
| 为了防御加校验，把合法 modem 报文也拒了 | security.md"校验不过度"（有 revert 教训 76207e0） |
| 上报响应找不到 requestList_ 匹配项（serial 不一致）就绕过 | 先查 data-flow.md serial 透传链路，泄漏是根因信号不是可绕过点 |
| 改 slot 相关循环想当然用 `<` | slot-model.md：含 vSIM 时 `<=`（0a7b6ca crash 案例） |

## 模式 1：字符串无长度构造 → 越界读

- 表现：crash / ASAN 越界读报错。
- 反模式：`std::string((const char *)response)`（无长度）、`static_cast<const char *>(response)` 直接构造。
- 正解：带 `responseLen` 构造 + 上限截断（IMEI_MAX_LEN=15、IMSI_MAX_LEN=15）+ `responseLen` 校验。
- 案例：01a6c3b（DTS2026050909588，hril_sim.cpp/hril_modem.cpp GetImei/GetImsi/GetBasebandVersion/SimStk*Notify）。

## 模式 2：responseLen 未校验即解引用

- 反模式：`response != nullptr` 但 `responseLen == 0` 或 `responseLen % sizeof(T) != 0` 时仍读取。
- 正解：先校验 `responseLen == 0` 与 `% sizeof()` 对齐，不合法返回 `HRIL_ERR_INVALID_PARAMETER`。
- 案例：8c7c221（DTS2026010843994，hril_data.cpp ActivatePdpContextResponse/PdpContextListUpdated/DataLinkCapabilityUpdated）。

## 模式 3：解析循环行数未与数组上限对齐

- 反模式：`for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next)` 直接写目标数组，响应行数超上限即越界写。
- 正解：循环条件加目标数组上限（`pdnCount < MAX_PDP_NUM`）。
- 案例：cc5e256（DTS2026051105183，at_data.c QueryAllSupportPDNInfos）。

## 模式 4：跨层入参长度未校验直接复制

- 反模式：把外部输入（std::string / 指针+长度）逐字节拷入定长数组，未先比长度。
- 正解：`input.size() >= MAX_LEN` 时拒绝（`HRIL_ERR_MEMORY_FULL`/`HRIL_ERR_INVALID_PARAMETER`），再复制。
- 案例：c948cd4（DTS2025093015213，hril_emc_rescue.cpp SendEmcRescueMessage smsStr[HRIL_EMC_RESCUE_SMS_STR_MAX_LEN]）。

## 模式 5：失败分支不回填错误码

- 反模式：解析/响应失败时只置空 data 或返回 0，上层拿到 NONE 错误码。
- 正解：失败时显式置 `RIL_ERR_INVALID_RESPONSE` / `HRIL_ERR_INVALID_PARAMETER` 等。
- 案例：f56daa1（DTS2025092919151，hril_modem.cpp GetVoiceRadioTechnologyResponse responseLen < sizeof() 时置错误码）。

## 模式 6：循环边界与数组/容量上限错位

- 反模式：`<`/`<=` 边界理解错误（含 vSIM 时少循环一次 → 漏初始化 → crash）；容量常量过小（并发上报堆积）。
- 正解：slot 循环含虚拟卡用 `<= hrilSimSlotCount_`；业务容量常量按并发峰值评估。
- 案例：0a7b6ca（DTS2026080119558，hril_manager.cpp slot 循环 `<`→`<=`）、c8d7dd3（DTS2026051353486，hril_network.cpp ITEMNUM_MAX 20→100 持锁异常）。

## 模式 7：异常分支空指针

- 反模式：错误路径上使用未判空/未初始化的指针。
- 正解：每个失败分支独立判空返回；用 `zero_branch_test` 覆盖。
- 案例：9e9923b（DTS2025100933236，hril_modem.cpp）。

## 模式 8：malloc/free 配对遗漏

- 反模式：`calloc` 后中途 return 不 free；错误 handler 里 free 了未分配的指针。
- 正解：分配后所有出口统一释放；错误 handler 只释放已分配资源。
- 案例：at_network.c OperListErrorHandler / at_call.c pCallsTmp / at_data.c pDcrs 释放链。

## 模式 9：strsep 原地改写共享缓冲

- 反模式：对读线程共享的行缓冲直接 `strsep`（写 '\0'）或跨层持有段内指针。
- 正解：解析前确认缓冲为调用方私有（如 vendor_report.c 的 `strdup` 副本）；上报前完成所有段解析。
- 案例：at-protocol.md 陷阱 1（ProcessCurrentCellList 多小区只报首个）。
