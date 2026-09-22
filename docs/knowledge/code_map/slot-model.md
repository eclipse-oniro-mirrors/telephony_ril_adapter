# slotId 语义与回退（code_map）

## 唯一判定入口

| 函数 | 位置 | 语义 |
|---|---|---|
| `GetSlotId(requestInfo)` | `services/vendor/src/vendor_util.c` | 请求类：取 `requestInfo->slotId`；主动上报类（requestInfo==NULL）：读系统参数 `persist.sys.support.slotid`。若 `slotId >= GetSimSlotCount()` 回退 `HRIL_SIM_SLOT_0` 并记错误日志 |
| `GetSimSlotCount()` | `services/vendor/src/vendor_util.c` | 读 `const.telephony.slotCount`（默认 1）；若 `const.booster.virtual_modem_switch=true` 且 slotCount==0 则强制为 1 |

## 当前槽位模型

- 3 槽 = 2SIM + 1vSIM（虚拟卡，2026 起扩展：commit 13b1c9a"分布式卡槽数量设置到3"、a4bc699"扩展slotId"）。
- hril 层 `hrilSimSlotCount_` 的循环边界语义：**含虚拟卡时应 `<=`，不含时应 `<`**——0a7b6ca（ril crash 修复）就是把循环从 `<` 改为 `<=` 才覆盖第 3 槽。
- 双 modem 设备同样是 3 槽（2SIM + 1vSIM），由 `HRIL_VSIM_MODEM_COUNT_STR` 参数判断。

## 修改注意事项

1. 修改 slot 循环边界前先确认该循环处理的业务是否含 vSIM（呼叫/短信共享 vs 物理卡专属）。
2. vendor 侧 slotId 回退只发生在 `GetSlotId`，不要在 at_* 各文件里自行造 slot 判定逻辑。
3. 新增 slot 相关系统参数时注意参数名 `const.`/`persist.` 前缀语义（const 只读、persist 可持久化）。
4. 上报类 notify 无 requestInfo，slot 判定依赖 `persist.sys.support.slotid`——多卡时该参数缺失会导致全部上报落在卡 0。
