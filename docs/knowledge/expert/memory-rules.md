# 请求内存管理规则（expert）

## ReqDataInfo 生命周期（手写内存池）

```
HRilManager::CreateHRilRequest(slotId, serial, request)
  → malloc ReqDataInfo（interfaces/innerkits/include/hril_public_struct.h）
  → 存入 requestList_：unordered_map<requestId, vector<ReqDataInfo *>>，requestListLock_ 保护
  → vendor 处理后经 ReportInfo.requestInfo 回传
  → HRilManager 按 requestId/serial 匹配后 ReleaseHRilRequest 释放
```

## 硬性规则

1. **vendor 侧禁止跨回调持有 `ReportInfo.requestInfo` 指针**——仅回调期间有效，响应返回后会被 manager 释放。
2. `ReqDataInfo` 只由 `CreateHRilRequest`/`ReleaseHRilRequest` 创建/释放，**禁止**业务代码自行 free。
3. 上报 data 指针（`const uint8_t *`）同步拷贝/转换，不跨线程保留；vendor 层栈上结构（如 `CellInfoList`）必须在 On<域>Report 调用完成后才能释放。
4. `requestList_` 的查找依赖 requestId/serial 精确匹配，**请求号/上报号不匹配会导致内存泄漏**（找不到匹配项不释放）——新增请求时确认响应路径一定走 ReleaseHRilRequest。
5. 全局 `dispatchMutex` 持锁期间禁止做耗时操作（vendor 回调/IO），避免持锁异常（见 concurrency.md）。

## 相关缺陷模式

- malloc 后错误分支漏 free → 模式 8（defect-patterns.md）。
- 响应到达但 requestId 不匹配 → 泄漏 + 无响应，排查时先核对 serial 透传链路（data-flow.md）。
