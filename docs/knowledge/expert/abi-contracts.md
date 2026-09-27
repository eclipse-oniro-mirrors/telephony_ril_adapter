# 厂商库 ABI 契约（expert）

## 契约文件

`interfaces/innerkits/include/hril.h` 定义双向二进制契约：

| 符号 | 方向 | 内容 |
|---|---|---|
| `HRilReport` | 框架 → vendor | 8 个回调：OnCallReport / OnDataReport / OnModemReport / OnNetworkReport / OnSimReport / OnSmsReport / OnEmcRescueReport / OnTimerCallback |
| `HRilOps` | vendor → 框架 | 7 个业务函数表指针（callOps/simOps/smsOps/dataOps/networkOps/modemOps/emcRescueOps）+ version |
| `RilInitOps(const HRilReport *)` | 厂商库导出 | dlopen 后 dlsym 的入口函数，返回 `const HRilOps *` |

## 加载流程（hril_hdf.c LoadVendor）

1. 读系统参数 `const.sys.radio.vendorlib.path` 得到库路径；虚拟 modem 开关 `const.booster.virtual_modem_switch=true` 时用 `libril_msgtransfer.z.so`。
2. UDEV 枚举 USB tty 设备，按 idVendor/idProduct 匹配 `modem_adapter.h` 表（如 MEIG SLM790 → libril_vendor.z.so）。
3. `realpath` 安全校验：库必须位于 `/vendor/lib64/` 或 `/vendor/modem/modem_vendor/lib64/` 前缀下。
4. `dlopen(RTLD_NOW)` → `dlsym("RilInitOps")` → `HRilInit()` → `rilInitOps(&g_reportOps)` → `HRilRegOps(ops)`。

## 禁止事项

- **禁止** 修改 `HRilReport`/`HRilOps` 字段的顺序、类型或删除字段——所有已发布厂商库按 ABI 加载，改序/删字段即破坏所有厂商库。
- 需要扩展时只允许**在结构体尾部追加字段**，且旧厂商库必须忽略新字段（version 字段用于能力协商，hrilOpsVersion_ 据此分流）。
- **禁止** 修改 `RilInitOps` 签名（返回类型/参数）。
- **禁止** 假设所有厂商库都实现 7 个域——参考实现 services/vendor 是全量实现，第三方库可能只实现部分域；hril 层调用前需判空。
- 业务数据结构（hril_vendor_*_defs.h）同样属于跨 DSO 契约，字段增删影响厂商库；追加字段优先。

## 相关安全机制

- CFI（Control Flow Integrity）开启：跨 DSO 函数指针调用需要豁免时维护 `services/hril/cfi_blocklist.txt` / `services/vendor/cfi_blocklist.txt`。
- 后向 PAC：`branch_protector_ret = "pac_ret"`（commit 410e404）。
