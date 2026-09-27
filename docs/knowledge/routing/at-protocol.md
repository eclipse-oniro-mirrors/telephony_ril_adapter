# AT 报文格式契约（routing）

## 触发场景

修改 vendor 层 AT 命令收发、响应/URC 解析、上报数据时，先按本节理解报文契约，再动手。

## 通道与解析基础

- 串口路径：默认 `/dev/ttyUSB0`，系统参数 `const.telephony.ril.attty.path` 可覆盖。
- 行缓冲：`services/vendor/src/vendor_channel.c` 全局 `g_buffer` 手工分帧，读线程独占。
- 响应分类：`at_support.c` `ProcessResponse` —— OK / ERROR / +CME ERROR: / +CMS ERROR: / 前缀匹配 / 主动上报。
- 解析原语（`vendor_util.c`）：`SkipATPrefix`（找第一个 `:` 并跳过后继续）、`NextInt`/`NextInt64`/`NextIntFromHex`/`NextULongFromHex`/`NextStr`/`NextTxtStr`（全部基于 `strsep` + `strtol`，缺字段/空串返回错误）、`SkipSpace`/`SkipNextComma`、`IsResponseSuccess`/`IsResponseError`/`IsSmsNotify`。

## 关键报文格式（at_network.c 示例）

| 报文 | 格式 | 解析路径 |
|---|---|---|
| 邻居小区列表 | `AT^MONNC` → `^MONNC: GSM <band>,<arfcn>,<bsic>,<cellId hex>,<lac hex>,<rxlev>`（每制式字段序不同：LTE=arfcn,pci,rsrp,rsrq,rxlev；NR=arfcn,pci,tac,nci 等） | `ReqGetNeighboringCellInfoList` → `ParseCellInfos` → `ParseCellInfo{Gsm,Lte,Wcdma,Cdma,Tdscdma,Nr}` |
| 当前小区 | `AT^MONSC` → `^MONSC: <ratType>,<mcc>,<mnc>,<字段...>`（GSM 8 字段、LTE 7 字段、CDMA 9 字段…） | `ReqGetCurrentCellInfo` → `ParseGetCurrentCellInfoResponseLine` → `ParseGet*CellInfoLine` |
| 当前小区 URC | `^MONSC:` 主动上报，多小区用 `;` 分隔 | `vendor_report.c` → `ProcessCurrentCellList` |
| 常驻网络 URC | `^XRESIDENT:<mcc>,<mnc>` | `vendor_report.c` → `ResidentNetworkUpdated` |

## 已知陷阱（真实缺陷）

1. **`;` 切分后段内无 `:` 前缀**：`ProcessCurrentCellList` 按 `;` 切分，`SkipATPrefix` 需要 `:`，导致只有首个含前缀的段被解析、多小区只上报第一个（at_network.c:1451-1457）。改此逻辑须保留首段前缀语义。
2. **字段缺失不报错**：`ParseGet*CellInfoLine`（^MONSC 路径）忽略所有 `NextInt` 返回值，缺字段时按 strtol("")=0 填充，产生脏数据而非失败——字段序与真实 modem 输出不一致时逐个错位消费。
3. **制式匹配**：`ParseCellInfos` 用 `ReportStrWith(pStr, "^MONNC: GSM")` 前缀匹配，未知制式进 else 分支失败——新增制式必须显式加分支。
4. **引号语义**：`NextStr`/`NextIntFromHex` 对 `"..."` 引号内的值有专门处理（`strsep(s, "\"")`），畸形引号（未闭合）会返回空串——解析返回 < 0 时必须走失败分支，不能继续。

## 修改规则

- 新增/修改 AT 解析时，每个字段解析都要校验返回值，失败即返回错误码，不要静默填充 0。
- 上报数据长度必须以实际解析结果为准，不要用固定长度。
- 涉及 modem 行为契约（字段序）变更时，先在 `test/fuzztest` 与 `zero_branch_test` 补非法报文用例（hril 层），并确认 vendor 层解析在异常输入下不崩溃（vendor 层无法 host 单测，靠评审与真机日志验证）。
