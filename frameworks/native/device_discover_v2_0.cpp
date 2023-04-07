/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "device_discover.h"
#include "hdi_device_v2_0.h"
#include "hdi_returncode_transform.h"
#include "common/log.h"
#include "common/utils.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
std::shared_ptr<Device> DiscoverHDIDevicesV2_0(std::string& deviceName, std::string& vendorName, std::string& version)
{
    // only one device from HDI now.
    OHOS::sptr<V2_0::INnrtDevice> iDevice = V2_0::INnrtDevice::Get();
    if (iDevice == nullptr) {
        LOGW("Get HDI device failed.");
        return nullptr;
    }

    V2_0::NNRT_ReturnCode returnCode;
    auto hdiRet = iDevice->GetDeviceName(deviceName, returnCode);
    if (hdiRet != HDF_SUCCESS) {
        LOGW("Get device name failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            hdiRet, ConverterRetToString(returnCode).c_str());
        return nullptr;
    }
    hdiRet = iDevice->GetVendorName(vendorName, returnCode);
    if (hdiRet != HDF_SUCCESS) {
        LOGW("Get vendor name failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            hdiRet, ConverterRetToString(returnCode).c_str());
        return nullptr;
    }
    std::pair<uint32_t, uint32_t> hdiVersion;
    hdiRet = iDevice->GetVersion(hdiVersion.first, hdiVersion.second);
    if (hdiRet != HDF_SUCCESS) {
        LOGW("Get version failed. ErrorCode=%{public}d", hdiRet);
        return nullptr;
    }
    version = 'v' + std::to_string(hdiVersion.first) + '_' + std::to_string(hdiVersion.second);

    std::shared_ptr<Device> device = CreateSharedPtr<HDIDeviceV2_0>(iDevice);
    if (device == nullptr) {
        LOGW("Failed to register device, because fail to create device instance.");
    }
    return device;
}
} // namespace NeuralNetworkRuntime
} // namespace OHOS