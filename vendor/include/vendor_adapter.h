/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_VENDOR_ADAPTER_H
#define OHOS_VENDOR_ADAPTER_H

#include <fcntl.h>
#include <termios.h>
#include "hril.h"
#include "at_call.h"
#include "at_data.h"
#include "at_modem.h"
#include "at_network.h"
#include "at_sim.h"
#include "at_sms.h"
#include "at_support.h"
#include "vendor_util.h"
#include "vendor_report.h"

#define SLEEP_TIME 10

// get radio state
HRilRadioState GetRadioState(void);
// set radio state
int SetRadioState(HRilRadioState newState, int rst);

#endif // OHOS_VENDOR_ADAPTER_H
