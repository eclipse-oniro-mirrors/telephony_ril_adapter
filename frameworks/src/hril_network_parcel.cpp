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
    if (!ReadBaseString(parcel, longName)) {
        return false;
    }
    if (!ReadBaseString(parcel, shortName)) {
        return false;
    }
    if (!ReadBaseString(parcel, numeric)) {
        return false;
    }
    return true;
}

bool OperatorInfoResult::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseString(parcel, longName)) {
        return false;
    }
    if (!WriteBaseString(parcel, shortName)) {
        return false;
    }
    if (!WriteBaseString(parcel, numeric)) {
        return false;
    }
    return true;
}

bool CsRegStatusInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, notifyType)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, regStatus)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, lacCode)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, cellId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, radioTechnology)) {
        return false;
    }
    return true;
}

bool CsRegStatusInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, notifyType)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, regStatus)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, lacCode)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, cellId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, radioTechnology)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, notifyType)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, regStatus)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, lacCode)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, cellId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, radioTechnology)) {
        return false;
    }
    return true;
}

bool PsRegStatusResultInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, notifyType)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, regStatus)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, lacCode)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, cellId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, radioTechnology)) {
        return false;
    }
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
    if (!ReadBaseString(parcel, longName)) {
        return false;
    }
    if (!ReadBaseString(parcel, shortName)) {
        return false;
    }
    if (!ReadBaseString(parcel, numeric)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, status)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, rat)) {
        return false;
    }
    return true;
}

bool AvailableNetworkInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseString(parcel, longName)) {
        return false;
    }
    if (!WriteBaseString(parcel, shortName)) {
        return false;
    }
    if (!WriteBaseString(parcel, numeric)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, status)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, rat)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, selectMode)) {
        return false;
    }
    if (!ReadBaseString(parcel, oper)) {
        return false;
    }
    if (!ReadBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

bool SetNetworkModeInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, selectMode)) {
        return false;
    }
    if (!WriteBaseString(parcel, oper)) {
        return false;
    }
    if (!WriteBaseInt64(parcel, flag)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, itemNum)) {
        return false;
    }
    availableNetworkInfo.resize(itemNum);
    for (int32_t i = 0; i < itemNum; i++) {
        availableNetworkInfo[i].ReadFromParcel(parcel);
    }
    if (!ReadBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

bool AvailableNetworkList::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, itemNum)) {
        return false;
    }
    for (int32_t i = 0; i < itemNum; i++) {
        availableNetworkInfo[i].Marshalling(parcel);
    }
    if (!WriteBaseInt64(parcel, flag)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.band)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.arfcn)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.bsic)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.cellId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.lac)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.rxlev)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.rxQuality)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.ta)) {
        return false;
    }
    return true;
}

bool CurrentCellInfo::ReadRayTypeLteParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.arfcn)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.cellId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.pci)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.tac)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.rsrp)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.rsrq)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.rssi)) {
        return false;
    }
    return true;
}

bool CurrentCellInfo::ReadRayTypeWcdmaParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.arfcn)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.psc)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.cellId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.lac)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.rscp)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.rxlev)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.ecno)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.drx)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.ura)) {
        return false;
    }
    return true;
}

bool CurrentCellInfo::WriteRayTypeGsmParcel(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.band)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.arfcn)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.bsic)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.cellId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.lac)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.rxlev)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.rxQuality)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.ta)) {
        return false;
    }
    return true;
}

bool CurrentCellInfo::WriteRayTypeLteParcel(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.arfcn)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.cellId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.pci)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.tac)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.rsrp)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.rsrq)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.rssi)) {
        return false;
    }
    return true;
}

bool CurrentCellInfo::WriteRayTypeWcdmaParcel(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.arfcn)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.psc)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.cellId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.lac)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.rscp)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.rxlev)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.ecno)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.drx)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.ura)) {
        return false;
    }
    return true;
}

bool CurrentCellInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, ratType)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mcc)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mnc)) {
        return false;
    }

    switch (ratType) {
        case NETWORK_TYPE_GSM:
            return ReadRayTypeGsmParcel(parcel);
        case NETWORK_TYPE_LTE:
            return ReadRayTypeLteParcel(parcel);
        case NETWORK_TYPE_WCDMA:
            return ReadRayTypeWcdmaParcel(parcel);
        case NETWORK_TYPE_UNKNOWN:
            return ReadRayTypeWcdmaParcel(parcel);
        default:
            return false;
    }

    return true;
}

bool CurrentCellInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, ratType)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mcc)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mnc)) {
        return false;
    }

    switch (ratType) {
        case NETWORK_TYPE_GSM:
            WriteRayTypeGsmParcel(parcel);
            break;
        case NETWORK_TYPE_LTE:
            WriteRayTypeLteParcel(parcel);
            break;
        case NETWORK_TYPE_WCDMA:
            WriteRayTypeWcdmaParcel(parcel);
            break;
        case NETWORK_TYPE_UNKNOWN:
            WriteRayTypeWcdmaParcel(parcel);
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

bool CellListNearbyInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, itemNum)) {
        return false;
    }
    if (itemNum <= 0) {
        cellNearbyInfo.clear();
    } else {
        cellNearbyInfo.resize(itemNum);
        for (int32_t i = 0; i < itemNum; i++) {
            if (!cellNearbyInfo[i].ReadFromParcel(parcel)) {
                return false;
            }
        }
    }
    return true;
}

bool CellListNearbyInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, itemNum)) {
        return false;
    }
    for (int32_t i = 0; i < itemNum; i++) {
        if (!cellNearbyInfo[i].Marshalling(parcel)) {
            return false;
        }
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
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.band)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.arfcn)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.bsic)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.cellId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.lac)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.gsm.rxlev)) {
        return false;
    }
    return true;
}

bool CellNearbyInfo::ReadRayTypeLteListParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.arfcn)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.pci)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.rsrp)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.rsrq)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.lte.rxlev)) {
        return false;
    }
    return true;
}

bool CellNearbyInfo::ReadRayTypeWcdmaListParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.arfcn)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.psc)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.rscp)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ServiceCellParas.wcdma.ecno)) {
        return false;
    }
    return true;
}

bool CellNearbyInfo::WriteRayTypeGsmListParcel(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.band)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.arfcn)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.bsic)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.cellId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.lac)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.gsm.rxlev)) {
        return false;
    }
    return true;
}

bool CellNearbyInfo::WriteRayTypeLteListParcel(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.arfcn)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.pci)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.rsrp)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.rsrq)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.lte.rxlev)) {
        return false;
    }
    return true;
}

bool CellNearbyInfo::WriteRayTypeWcdmaListParcel(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.arfcn)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.psc)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.rscp)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ServiceCellParas.wcdma.ecno)) {
        return false;
    }
    return true;
}

bool CellNearbyInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, ratType)) {
        return false;
    }
    switch (ratType) {
        case NETWORK_TYPE_GSM:
            return ReadRayTypeGsmListParcel(parcel);
        case NETWORK_TYPE_LTE:
            return ReadRayTypeLteListParcel(parcel);
        case NETWORK_TYPE_WCDMA:
            return ReadRayTypeWcdmaListParcel(parcel);
        case NETWORK_TYPE_UNKNOWN:
            return ReadRayTypeGsmListParcel(parcel);
        default:
            return false;
    }
    return true;
}

bool CellNearbyInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, ratType)) {
        return false;
    }

    switch (ratType) {
        case NETWORK_TYPE_GSM:
            WriteRayTypeGsmListParcel(parcel);
            break;
        case NETWORK_TYPE_LTE:
            WriteRayTypeLteListParcel(parcel);
            break;
        case NETWORK_TYPE_WCDMA:
            WriteRayTypeWcdmaListParcel(parcel);
            break;
        case NETWORK_TYPE_UNKNOWN:
            WriteRayTypeGsmListParcel(parcel);
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

bool PreferredNetworkTypeInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, preferredNetworkType)) {
        return false;
    }
    if (!ReadBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

bool PreferredNetworkTypeInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, preferredNetworkType)) {
        return false;
    }
    if (!WriteBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

std::shared_ptr<PreferredNetworkTypeInfo> PreferredNetworkTypeInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<PreferredNetworkTypeInfo> param = std::make_shared<PreferredNetworkTypeInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool PsAttachStatusInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, psAttachStatus)) {
        return false;
    }
    if (!ReadBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

bool PsAttachStatusInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, psAttachStatus)) {
        return false;
    }
    if (!WriteBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

std::shared_ptr<PsAttachStatusInfo> PsAttachStatusInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<PsAttachStatusInfo> param = std::make_shared<PsAttachStatusInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool ImsRegStatusInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, notifyType)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, regInfo)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, extInfo)) {
        return false;
    }
    if (!ReadBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

bool ImsRegStatusInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, notifyType)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, regInfo)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, extInfo)) {
        return false;
    }
    if (!WriteBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

std::shared_ptr<ImsRegStatusInfo> ImsRegStatusInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<ImsRegStatusInfo> param = std::make_shared<ImsRegStatusInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}
} // namespace Telephony
} // namespace OHOS
