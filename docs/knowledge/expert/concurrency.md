# 并发与运行锁（expert）

## 线程模型总览

3 线程（详见 data-flow.md）：

1. **hril 事件循环线程**（hril_event.cpp `EventMessageLoop`）：select 监听 fd（固定 8 槽 `listenEventTable_`，满则静默丢弃）+ 定时器链表；pipe 写 1 字节唤醒。
2. **vendor 读线程**（at_support.c `ReaderLoop`）：读串口分帧、响应分类；与发送线程共享全局 `g_response`/`g_prefix`/`g_smsPdu`，靠 `g_commandmutex`/`g_commandcond` 同步；超时 `ClearCurCommand` 与读线程存在竞态。
3. **vendor 监听线程**（vendor_adapter.c `EventListeners`）：串口打开/初始化/断线重连。

## 关键锁与同步

| 锁/机制 | 位置 | 语义 |
|---|---|---|
| `dispatchMutex` | hril_manager.cpp | **全局串行化所有请求**：一个请求的处理（含 vendor 函数调用）全程持锁。吞吐瓶颈 + 持锁回调风险 |
| `requestListLock_` | hril_manager.cpp | 保护 requestList_ 增删 |
| callback_ 独立 mutex | hril_base.h | 保护 sptr<IRilCallback> |
| `g_commandmutex`/`g_commandcond` | at_support.c | AT 命令同步等待（超时竞态：ClearCurCommand vs ProcessResponse） |
| RunningLock | hril_manager.cpp + hril_notification_map.h | NEED_LOCK 通知上报前 ApplyRunningLock（power HDI V1_2）防进程被杀；200ms 定时器自动释放 / SendRilAck 显式释放 |

## 禁止事项

- **禁止** 改 3 线程模型的同步方式而不评估 RunningLock 与 dispatchMutex 影响（如把请求处理移出 dispatchMutex 会破坏 vendor 层的串行假设）。
- **禁止** 在 dispatchMutex 持锁路径上调用阻塞/耗时操作（vendor 回调、IO、sleep）。
- **禁止** 在通知上报路径（OnXxxReport）里做耗时转换——NEED_LOCK 通知依赖 RunningLock 保活，超时释放后进程可能被杀。
- 修改事件循环（hril_event.cpp）注意：`listenEventTable_` 固定 8 槽，槽满静默丢弃 fd 事件；`nfds_` 递减计算；fd_set 全局拷贝——并发注册 fd 时存在丢失风险。

## 已知事故

- **持锁异常**（c8d7dd3，DTS2026051353486）：容量常量 ITEMNUM_MAX=20 过小，上报堆积时处理不完导致锁长时间不释放 → 业务容量常量按并发峰值评估。
- **RunningLock 超时释放**：通知处理慢于 200ms 定时器时锁提前释放，进程可能被杀——长耗时转换不要放在通知路径。
