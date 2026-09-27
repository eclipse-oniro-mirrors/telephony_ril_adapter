# HDI 版本数据路径（code_map）

## 背景

本仓通过外部依赖 `drivers_interface_ril` 提供的 `libril_proxy_1.1~1.5` 与上层电话服务通讯（IRil/IRilCallback）。上层可能以不同 HDI 版本调用，hril 层需要分流：

- 版本分支决策点在 `services/hril/src/hril_manager.cpp` 的 `hrilOpsVersion_`（启动时探测得到）。
- 数据转换在各业务域的 `*Response*` / `*Updated*` 函数内按版本分别实现（如 `GetCurrentCellInfoResponse` / `GetCurrentCellInfoResponse_1_1` / `GetCurrentCellInfoResponse_1_2`）。

## 修改数据转换时必查

1. **确认走的版本分支**：grep `hrilOpsVersion_`，确认目标请求/通知在 V1_1 与 V1_3 路径上都有对应处理，不要只改其中一条。
2. **结构字段对齐**：HRil 结构（hril_vendor_*_defs.h）与 HDI 结构字段逐一对齐，字段错位是高频缺陷（见 defect-patterns.md）。
3. **回调签名差异**：不同版本 IRilCallback 的回调函数签名不同（V1_1 与 V1_3 参数类型可能不同），在 `Response`/`Notify` 模板调用点按版本选择。
4. **长度与对齐**：跨版本转换时对 `responseLen` 的 `% sizeof()` 校验不能丢（越界缺陷源）。

## 典型位置

| 内容 | 位置 |
|---|---|
| 版本探测/存储 | `services/hril/src/hril_manager.cpp`（hrilOpsVersion_，HRilInit/HRilRegOps） |
| 网络域多版本响应 | `services/hril/src/hril_network.cpp`（GetCurrentCellInfoResponse/_1_1/_1_2、GetNeighboringCellInfoListResponse/_1_2） |
| 数据域版本转换 | `services/hril/src/hril_data.cpp`（V1_1 结构转换入口） |
| 其余各域 | 各 `hril_<域>.cpp` 的 `*Response` 函数族 |
