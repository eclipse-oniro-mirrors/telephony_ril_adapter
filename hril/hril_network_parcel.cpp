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

#include "hril_network_parcel.h"
#include <memory>
#include <string>
#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
std::shared_ptr<OperatorInfoResult> OperatorInfoResult::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<OperatorInfoResult> param = std::make_shared<OperatorInfoResult>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool OperatorInfoResult::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, longName);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, shortName);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, numeric);
    return true;
}

bool OperatorInfoResult::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, longName);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, shortName);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, numeric);
    return true;
}

bool CsRegStatusInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, regStatus);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, radioTechnology);
    return true;
}

bool CsRegStatusInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, regStatus);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, radioTechnology);
    return true;
}

std::shared_ptr<CsRegStatusInfo> CsRegStatusInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CsRegStatusInfo> param = std::make_shared<CsRegStatusInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool PsRegStatusResultInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, regStatus);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, radioTechnology);
    return true;
}

bool PsRegStatusResultInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, regStatus);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, radioTechnology);
    return true;
}

std::shared_ptr<PsRegStatusResultInfo> PsRegStatusResultInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<PsRegStatusResultInfo> param = std::make_shared<PsRegStatusResultInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool AvailableNetworkInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, longName);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, shortName);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, numeric);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, rat);
    return true;
}

bool AvailableNetworkInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, longName);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, shortName);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, numeric);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, rat);
    return true;
}

std::shared_ptr<AvailableNetworkInfo> AvailableNetworkInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<AvailableNetworkInfo> param = std::make_shared<AvailableNetworkInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SetNetworkModeInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, selectMode);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, oper);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, flag);
    return true;
}

bool SetNetworkModeInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, selectMode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, oper);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, flag);
    return true;
}

std::shared_ptr<SetNetworkModeInfo> SetNetworkModeInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SetNetworkModeInfo> param = std::make_shared<SetNetworkModeInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool AvailableNetworkList::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, itemNum);
    availableNetworkInfo.resize(itemNum);
    for (int32_t i = 0; i < itemNum; i++) {
        availableNetworkInfo[i].ReadFromParcel(parcel);
    }
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, flag);
    return true;
}

bool AvailableNetworkList::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, itemNum);
    for (int32_t i = 0; i < itemNum; i++) {
        availableNetworkInfo[i].Marshalling(parcel);
    }
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, flag);
    return true;
}

std::shared_ptr<AvailableNetworkList> AvailableNetworkList::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<AvailableNetworkList> param = std::make_shared<AvailableNetworkList>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CurrentCellInfo::ReadRayTypeGsmParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.band);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.arfcn);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.bsic);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.cellId);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.lac);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.rxlev);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.rxQuality);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.ta);
    return true;
}

bool CurrentCellInfo::ReadRayTypeLteParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.arfcn);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.cellId);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.pci);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.tac);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rsrp);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rsrq);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rssi);
    return true;
}

bool CurrentCellInfo::WriteRayTypeGsmParcel(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.band);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.arfcn);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.bsic);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.cellId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.lac);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.rxlev);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.rxQuality);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.ta);
    return true;
}

bool CurrentCellInfo::WriteRayTypeLteParcel(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.arfcn);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.cellId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.pci);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.tac);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rsrp);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rsrq);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rssi);
    return true;
}

bool CurrentCellInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ratType);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mcc);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mnc);

    switch (ratType) {
        case RAT_TYPE_GSM:
            return ReadRayTypeGsmParcel(parcel);
        case RAT_TYPE_LTE:
            return ReadRayTypeLteParcel(parcel);
        default:
            return false;
    }

    return true;
}

bool CurrentCellInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ratType);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mcc);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mnc);

    switch (ratType) {
        case RAT_TYPE_GSM:
            WriteRayTypeGsmParcel(parcel);
            break;
        case RAT_TYPE_LTE:
            WriteRayTypeLteParcel(parcel);
            break;
        default:
            return false;
    }
    return true;
}

std::shared_ptr<CurrentCellInfo> CurrentCellInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CurrentCellInfo> param = std::make_shared<CurrentCellInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool LocationUpdateResultInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, regState);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, lac);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, ci);
    return true;
}

bool LocationUpdateResultInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, regState);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, lac);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, ci);
    return true;
}

std::shared_ptr<LocationUpdateResultInfo> LocationUpdateResultInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<LocationUpdateResultInfo> param = std::make_shared<LocationUpdateResultInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CellListNearbyInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, itemNum);
    cellNearbyInfo.resize(itemNum);
    for (int32_t i = 0; i < itemNum; i++) {
        cellNearbyInfo[i].ReadFromParcel(parcel);
    }
    return true;
}

bool CellListNearbyInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, itemNum);
    for (int32_t i = 0; i < itemNum; i++) {
        cellNearbyInfo[i].Marshalling(parcel);
    }
    return true;
}

std::shared_ptr<CellListNearbyInfo> CellListNearbyInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CellListNearbyInfo> param = std::make_shared<CellListNearbyInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CellNearbyInfo::ReadRayTypeGsmListParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.band);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.arfcn);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.bsic);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.cellId);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.lac);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.rxlev);
    return true;
}

bool CellNearbyInfo::ReadRayTypeLteListParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.arfcn);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.pci);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rsrp);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rsrq);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rxlev);
    return true;
}

bool CellNearbyInfo::WriteRayTypeGsmListParcel(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.band);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.arfcn);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.bsic);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.cellId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.lac);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.gsm.rxlev);
    return true;
}

bool CellNearbyInfo::WriteRayTypeLteListParcel(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.arfcn);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.pci);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rsrp);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rsrq);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ServiceCellParas.lte.rxlev);
    return true;
}

bool CellNearbyInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ratType);

    switch (ratType) {
        case RAT_TYPE_GSM:
            return ReadRayTypeGsmListParcel(parcel);
        case RAT_TYPE_LTE:
            return ReadRayTypeLteListParcel(parcel);
        default:
            return false;
    }
    return true;
}

bool CellNearbyInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, ratType);

    switch (ratType) {
        case RAT_TYPE_GSM:
            WriteRayTypeGsmListParcel(parcel);
            break;
        case RAT_TYPE_LTE:
            WriteRayTypeLteListParcel(parcel);
            break;
        default:
            return false;
    }
    return true;
}

std::shared_ptr<CellNearbyInfo> CellNearbyInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CellNearbyInfo> param = std::make_shared<CellNearbyInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}
} // namespace Telephony
} // namespace OHOS
