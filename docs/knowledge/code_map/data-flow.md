# 数据流与缓冲所有权（code_map）

## 请求链路（Request）

```
上层电话服务
  │ HDI 调用 IRil::<业务>Request（V1_1~V1_5 proxy，跨进程）
  ▼
HRilManager::TaskSchedule(serial, funcId, data, dataLen)
  │ 全局 dispatchMutex 串行化；按 funcId 分发到对应业务域对象（按 slotId 实例化）
  ▼
HRil<域>::RequestVendor<ReqFuncSet, FuncPointer>   （hril_<域>.cpp）
  │ CreateHRilRequest：malloc ReqDataInfo（slotId/serial/request）存入 requestList_（unordered_map，requestListLock_ 保护）
  │ 调 vendor 函数表指针（HRilOps 中该域的 req 函数）
  ▼
vendor 层 Req<业务>（at_<域>.c）
  │ SendCommandLock(cmd, prefix, timeout, &responseInfo)：AT 命令 + 同步等待
  │   - vendor 读线程（ReaderLoop）读串口 → vendor_channel.c 分帧 → at_support.c 分类（OK/ERROR/+CME/前缀）
  │   - g_response/g_prefix/g_smsPdu 全局状态 + g_commandmutex/g_commandcond 同步
  ▼
vendor 通过 g_reportOps->On<域>Report(slotId, reportInfo, data, dataLen) 上报
```

## 上报分发链路（Report）

```
vendor ReportInfo（type=HRIL_RESPONSE / HRIL_NOTIFICATION）
  ▼
HRilManager::On<域>Report（hril_manager.cpp 7 个入口）
  │ type 分流：
  │   RESPONSE → 匹配 requestList_ 中 serial 对应的 ReqDataInfo → ReleaseHRilRequest 释放
  │   NOTIFICATION → 查 notificationMap_ 判断 NEED_LOCK → ApplyRunningLock（power HDI V1_2）
  │                  200ms 定时器自动释放 / SendRilAck 显式释放
  ▼
HRil<域>::Response/Notify 模板（hril_base.h）
  │ respMemberFuncMap_/notiMemberFuncMap_：事件号 → 处理函数
  │ 数据转换（HRil 结构 ↔ HDI 结构，注意版本分支见 hdi-version-paths.md）
  ▼
callback_（sptr<IRilCallback V1_5>，独立 mutex）→ HDI 回调上层
```

## 缓冲所有权规则

| 缓冲 | 所有者 | 生命周期 |
|---|---|---|
| `ReqDataInfo` | HRilManager（requestList_） | CreateHRilRequest（malloc）→ 响应返回时 ReleaseHRilRequest（free）；vendor 回调内仅在 `ReportInfo.requestInfo` 期间有效 |
| `ReportInfo` | vendor 栈上构造 | 回调期间有效，不可跨回调持有其指针 |
| `ResponseInfo`/`Line` 链表 | vendor 层（at_support.c malloc） | SendCommandLock 返回后由解析函数处理，`FreeResponseInfo` 释放；`pLine->data` 指向 g_buffer 内部分帧（读线程独占） |
| 上报 data 指针 | 调用方（vendor 或 manager 栈上结构） | 同步传入 On<域>Report 并被拷贝/转换，不跨线程保留 |

## 3 线程模型

| 线程 | 创建处 | 职责 |
|---|---|---|
| hril 事件循环线程 | `hril_hdf.c`（HRilInit） | `HRilTimerCallback::EventLoop()` → `HRilEvent::EventMessageLoop()`：select 监听 fd + 定时器，超时执行 pending 回调 |
| vendor 读线程 | `at_support.c`（ReaderLoop） | 读串口、分帧、响应分类、唤醒等待中的 AT 命令 |
| vendor 监听线程 | `vendor_adapter.c`（EventListeners） | 打开串口 + ModemInit 初始化 + 断线重连循环 |

注意：定时器通过 `g_reportOps->OnTimerCallback` 注册 → HRilManager → HRilTimerCallback → pipe 写入唤醒 select。
