# 业务域代码地图（code_map）

## 两层业务域文件对

每个业务域在 hril 层（C++，接口与数据转换）与 vendor 层（C，AT 指令）各有一对文件：

| 业务域 | hril 层（接口/转换） | vendor 层（AT 实现） | 职责范围 |
|---|---|---|---|
| Call | `services/hril/src/hril_call.cpp` + `include/hril_call.h` | `services/vendor/src/at_call.c` | 拨号/挂断/拒接/应答、会议电话、DTMF、USSD、呼叫转移/限制/等待、CLIP/CLIR、Vonr 开关、紧急号码列表 |
| Data | `services/hril/src/hril_data.cpp` + `include/hril_data.h` | `services/vendor/src/at_data.c` | PDP context 激活/去激活、APN、带宽上报规则、链路能力、网络切片（URSP/NSSAI/EHPLMN） |
| Modem | `services/hril/src/hril_modem.cpp` + `include/hril_modem.h` | `services/vendor/src/at_modem.c` | Radio 开关状态（含 lastRadioState 缓存）、IMEI/IMEISV/MEID、基带版本、关机 |
| Network | `services/hril/src/hril_network.cpp` + `include/hril_network.h` | `services/vendor/src/at_network.c` | 信号强度、CS/PS 注册状态、运营商/选网、小区信息、物理信道、NR 参数 |
| SIM | `services/hril/src/hril_sim.cpp` + `include/hril_sim.h` | `services/vendor/src/at_sim.c` | SIM 状态/IMSI、PIN/PUK 解锁、STK、APDU 逻辑通道、RadioProtocol、主卡设置 |
| SMS | `services/hril/src/hril_sms.cpp` + `include/hril_sms.h` | `services/vendor/src/at_sms.c` | GSM/CDMA 短信收发、SIM 卡短信读写、SMSC、小区广播 CB 配置 |
| EMC Rescue | `services/hril/src/hril_emc_rescue.cpp` + `include/hril_emc_rescue.h` | （并入 `at_support.c`/`vendor_report.c` 分支） | 救援模式开关、救援网络搜索、救援呼叫/短信；`IsInEmcRescueMode()` 劫持 GetCallList/Hangup/Answer 路由 |

## 基础设施文件

| 文件 | 职责 |
|---|---|
| `services/hril/src/hril_manager.cpp` | HRilManager 单例：按 slot 实例化 7 个业务域对象、TaskSchedule 全局调度、HRilRegOps 注册 vendor 函数表、OnXxxReport 上报分发、RunningLock 管理、CreateHRilRequest/ReleaseHRilRequest |
| `services/hril/src/hril_base.cpp` + `include/hril_base.h` | HRilBase 基类：respMemberFuncMap_/notiMemberFuncMap_ 双 map 事件分发、Response/Notify 调 HDI IRilCallback、hex/string 转换工具 |
| `services/hril/src/hril_event.cpp` + `include/hril_event.h` | select() 事件循环：fd 监听表（固定 8 槽）、定时器链表、待处理队列 |
| `services/hril/src/hril_timer_callback.cpp` | 定时器回调实现（RunningLock 超时、网络搜索超时等） |
| `services/hril_hdf/src/hril_hdf.c` | HDF 驱动入口：LoadVendor（dlopen/dlsym RilInitOps）、系统参数读取、UDEV USB modem 匹配、创建事件循环线程 |
| `services/vendor/src/vendor_adapter.c` | 厂商库入口 RilInitOps：定义 6 个业务 req ops 表、串口打开/初始化/重连监听线程 |
| `services/vendor/src/at_support.c` | AT 通道核心：ReaderLoop 读线程、ProcessResponse 响应分类、SendCommandLock 同步等待 |
| `services/vendor/src/vendor_channel.c` | 串口 read/write、g_buffer 行缓冲分帧 |
| `services/vendor/src/vendor_report.c` | URC 主动上报分发：按前缀 if-else 链解析 +Creg/+CMT/^HCSQ/^MONSC 等 |
| `services/vendor/src/vendor_util.c` | 字符串解析工具库：NextInt/NextStr/SkipATPrefix/IsResponseSuccess/GetSlotId/GetSimSlotCount |

## 接口与枚举位置

| 内容 | 位置 |
|---|---|
| HRilReport/HRilOps/RilInitOps 契约 | `interfaces/innerkits/include/hril.h` |
| 请求号枚举 HREQ_* | `interfaces/innerkits/include/hril_request.h` |
| 通知号枚举 HNOTI_* | `interfaces/innerkits/include/hril_notification.h` |
| 业务数据结构 | `interfaces/innerkits/include/hril_vendor_{call,data,modem,network,sim,sms,emc_rescue}_defs.h` |
| 公共结构 ReqDataInfo/ReportInfo | `interfaces/innerkits/include/hril_public_struct.h`、`hril_types.h` |
| 枚举（HRilRadioState 等） | `interfaces/innerkits/include/hril_enum.h` |
| HDI 接口（IRil/IRilCallback V1_1~V1_5） | 外部依赖 `drivers_interface_ril`，不在本仓 |
