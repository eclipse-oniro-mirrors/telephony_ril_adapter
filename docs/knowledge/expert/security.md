# 安全加固与参数校验（expert）

## 编译期安全机制

| 机制 | 位置 | 说明 |
|---|---|---|
| CFI（前向） | services/hril、services/vendor BUILD.gn | 跨 DSO 函数指针调用豁免维护 cfi_blocklist.txt |
| PAC（后向） | services/hril BUILD.gn | `branch_protector_ret = "pac_ret"`（commit 410e404） |
| 栈保护 | 历史：删除 -fstack-protector-strong 改为 PAC 组合 | 见 commit 410e404 |

## 参数校验原则

### 必须校验
- HDI 入参：数组大小上限（如 `DataLinkBandwidthReportingRule` 的 maximumUplinkKbpsSize/maximumDownlinkKbpsSize ≤ MAX_LINK_KBPS_SIZE）、指针非空。
- 上报 response：`responseLen == 0`、`responseLen % sizeof(T) == 0`、`responseLen < sizeof(T)` 均拒绝（defect-patterns.md 模式 2）。
- vendor 解析：每个字段解析返回值 < 0 即失败返回，不静默填充（at-protocol.md 修改规则）。
- 跨层复制：外部输入长度 ≥ 目标数组容量时拒绝（模式 4）。

### 校验不过度（revert 教训）
- 案例：94f3565"修复参数未校验问题"（DTS2026010843994 系列）被 76207e0（DTS2026011910213）revert——过度校验导致"长时间无小区激活"。
- 原则：**只拒绝物理不可能的值，不为防御过度收缩合法协议范围**。modem 厂商行为差异大，合法范围判断要留余量；改校验逻辑前后对照既有厂商库/报文样本。

## 路径与输入安全

- 厂商库加载路径：`realpath` 后必须落在 `/vendor/lib64/` 或 `/vendor/modem/modem_vendor/lib64/` 前缀（hril_hdf.c LoadVendor），防任意 dlopen。
- 系统参数读取：`GetParameter` 有长度上限（PARAMETER_SIZE/SYSPARA_SIZE），参数名使用 const/persist 前缀约定。
- 日志：敏感信息（IMSI/IMEI/通话号码）使用 `%{private}d` 等 privacy 格式符，防日志泄露。

## 安全类问题单参考

- 473c58b（DTS2026051248748）：at_sim.c 安全问题修改（103 行重构，删除 at_sim.h 中 30 行接口声明）。
- 260b569（DTS2026051104718）：security problem。
- 高频主题：越界（读/写）、未校验入参、空指针、日志超限（f46649a log overlimit rectification）。
