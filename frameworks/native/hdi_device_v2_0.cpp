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

#include "hdi_device_v2_0.h"

#include "hdf_base.h"
#include "mindir.h"

#include "hdi_prepared_model_v2_0.h"
#include "hdi_returncode_transform.h"
#include "memory_manager.h"
#include "transform.h"
#include "common/log.h"
#include "common/utils.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace {
OH_NN_DeviceType TransHDIDeviceV2_0Type(const V2_0::DeviceType& iDeviceType)
{
    switch (iDeviceType) {
        case V2_0::DeviceType::CPU:
            return OH_NN_CPU;
        case V2_0::DeviceType::GPU:
            return OH_NN_GPU;
        case V2_0::DeviceType::ACCELERATOR:
            return OH_NN_ACCELERATOR;
        default:
            return OH_NN_OTHERS;
    }
}

DeviceStatus TransHDIDeviceV2_0Status(const V2_0::DeviceStatus& iDeviceStatus)
{
    switch (iDeviceStatus) {
        case V2_0::DeviceStatus::AVAILABLE:
            return DeviceStatus::AVAILABLE;
        case V2_0::DeviceStatus::BUSY:
            return DeviceStatus::BUSY;
        case V2_0::DeviceStatus::OFFLINE:
            return DeviceStatus::OFFLINE;
        default:
            return DeviceStatus::UNKNOWN;
    }
}

V2_0::PerformanceMode TransPerformanceMode(const OH_NN_PerformanceMode& mode)
{
    switch (mode) {
        case OH_NN_PERFORMANCE_LOW:
            return V2_0::PerformanceMode::PERFORMANCE_LOW;
        case OH_NN_PERFORMANCE_MEDIUM:
            return V2_0::PerformanceMode::PERFORMANCE_MEDIUM;
        case OH_NN_PERFORMANCE_HIGH:
            return V2_0::PerformanceMode::PERFORMANCE_HIGH;
        case OH_NN_PERFORMANCE_EXTREME:
            return V2_0::PerformanceMode::PERFORMANCE_EXTREME;
        default:
            return V2_0::PerformanceMode::PERFORMANCE_NONE;
    }
}

V2_0::Priority TransPriority(const OH_NN_Priority& priority)
{
    switch (priority) {
        case OH_NN_PRIORITY_LOW:
            return V2_0::Priority::PRIORITY_LOW;
        case OH_NN_PRIORITY_MEDIUM:
            return V2_0::Priority::PRIORITY_MEDIUM;
        case OH_NN_PRIORITY_HIGH:
            return V2_0::Priority::PRIORITY_HIGH;
        default:
            return V2_0::Priority::PRIORITY_NONE;
    }
}
}

HDIDeviceV2_0::HDIDeviceV2_0(OHOS::sptr<V2_0::INnrtDevice> device) : m_iDevice(device)
{
    device->GetVersion(m_hdiVersion.first, m_hdiVersion.second);
}

OH_NN_ReturnCode HDIDeviceV2_0::GetDeviceName(std::string& name)
{
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->GetDeviceName(name, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Get HDI device name failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::GetVendorName(std::string& name)
{
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->GetVendorName(name, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Get HDI device vendor name failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::GetVersion(std::string& version)
{
    version = 'v' + std::to_string(m_hdiVersion.first) + '_' + std::to_string(m_hdiVersion.second);
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::GetDeviceType(OH_NN_DeviceType& deviceType)
{
    V2_0::DeviceType iDeviceType;
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->GetDeviceType(iDeviceType, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Get HDI device type failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }

    deviceType = TransHDIDeviceV2_0Type(iDeviceType);
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::GetDeviceStatus(DeviceStatus& status)
{
    V2_0::DeviceStatus iDeviceStatus;
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->GetDeviceStatus(iDeviceStatus, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Get HDI device status failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    status = TransHDIDeviceV2_0Status(iDeviceStatus);
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::GetSupportedOperation(std::shared_ptr<const mindspore::lite::LiteGraph> model,
                                                  std::vector<bool>& ops)
{
    if (model == nullptr) {
        LOGE("Model is nullptr, cannot query supported operation.");
        return OH_NN_NULL_PTR;
    }

    OHOS::HDI::Nnrt::V2_0::SharedBuffer tensorBuffer {INVALID_FD, 0, 0, 0};
    size_t tensorSize = mindspore::lite::MindIR_LiteGraph_GetConstTensorSize(model.get());
    int32_t hdiRet {0};
    if (tensorSize > 0) {
        V2_0::NNRT_ReturnCode returnCode;
        hdiRet = m_iDevice->AllocateBuffer(tensorSize, tensorBuffer, returnCode);
        if (hdiRet != HDF_SUCCESS || tensorBuffer.fd == INVALID_FD) {
            LOGE("Allocate tensor buffer error when get supported operation. ErrorCode: %{public}d, \
                innerHDIRet=%{public}s", hdiRet, ConverterRetToString(returnCode).c_str());
            return OH_NN_FAILED;
        }
    }

    auto iModel = mindspore::lite::MindIR_LiteGraph_To_Model_V2_0(model.get(), tensorBuffer);
    if (iModel == nullptr) {
        LOGE("Parse litegraph to hdi model failed.");
        ReleaseSharedBuffer(tensorBuffer);
        return OH_NN_FAILED;
    }

    V2_0::NNRT_ReturnCode returnCode;
    hdiRet = m_iDevice->GetSupportedOperation(*iModel, ops, returnCode);

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    auto ret = ReleaseSharedBuffer(tensorBuffer);
    if (ret != OH_NN_SUCCESS) {
        LOGE("Release tensorBuffer failed.");
        return OH_NN_FAILED;
    }
    if (hdiRet != HDF_SUCCESS) {
        LOGE("Get supported operation failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            hdiRet, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsFloat16PrecisionSupported(bool& isSupported)
{
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->IsFloat16PrecisionSupported(isSupported, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Query fp16 precision supported failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsPerformanceModeSupported(bool& isSupported)
{
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->IsPerformanceModeSupported(isSupported, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Query performance mode supported failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsPrioritySupported(bool& isSupported)
{
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->IsPrioritySupported(isSupported, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Query priority supported failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsDynamicInputSupported(bool& isSupported)
{
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->IsDynamicInputSupported(isSupported, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Query dynamic input supported failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsModelCacheSupported(bool& isSupported)
{
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->IsModelCacheSupported(isSupported, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Query cache model supported failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::PrepareModel(std::shared_ptr<const mindspore::lite::LiteGraph> model,
                                         const ModelConfig& config,
                                         std::shared_ptr<PreparedModel>& preparedModel)
{
    if (model == nullptr) {
        LOGE("Model is nullptr, cannot prepare model.");
        return OH_NN_INVALID_PARAMETER;
    }

    OHOS::HDI::Nnrt::V2_0::SharedBuffer tensorBuffer {INVALID_FD, 0, 0, 0};
    size_t tensorSize = mindspore::lite::MindIR_LiteGraph_GetConstTensorSize(model.get());
    int32_t hdiRet {0};
    if (tensorSize > 0) {
        V2_0::NNRT_ReturnCode returnCode;
        hdiRet = m_iDevice->AllocateBuffer(tensorSize, tensorBuffer, returnCode);
        if (hdiRet != HDF_SUCCESS || tensorBuffer.fd == INVALID_FD) {
            LOGE("Allocate tensor buffer error when prepare model. ErrorCode: %{public}d, innerHDIRet=%{public}s",
                hdiRet, ConverterRetToString(returnCode).c_str());
            return OH_NN_FAILED;
        }
    }

    V2_0::Model* iModel = mindspore::lite::MindIR_LiteGraph_To_Model_V2_0(model.get(), tensorBuffer);
    if (iModel == nullptr) {
        LOGE("Parse litegraph to hdi model failed.");
        ReleaseSharedBuffer(tensorBuffer);
        return OH_NN_FAILED;
    }

    V2_0::ModelConfig iModelConfig;
    iModelConfig.enableFloat16 = config.enableFloat16;
    iModelConfig.mode = TransPerformanceMode(config.mode);
    iModelConfig.priority = TransPriority(config.priority);
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;

    V2_0::NNRT_ReturnCode returnCode;
    auto preparedRet = m_iDevice->PrepareModel(*iModel, iModelConfig, iPreparedModel, returnCode);

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    auto ret = ReleaseSharedBuffer(tensorBuffer);
    if (ret != OH_NN_SUCCESS) {
        LOGE("Release tensorBuffer failed.");
        return OH_NN_FAILED;
    }
    if (preparedRet != HDF_SUCCESS || iPreparedModel == nullptr) {
        LOGE("Prepare model failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            preparedRet, ConverterRetToString(returnCode).c_str());
        return OH_NN_FAILED;
    }

    preparedModel = CreateSharedPtr<HDIPreparedModelV2_0>(iPreparedModel);
    if (preparedModel == nullptr) {
        LOGE("Prepare model failed, because fail to create preparedModel instance.");
        return OH_NN_MEMORY_ERROR;
    }

    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::PrepareModelFromModelCache(const std::vector<ModelBuffer>& modelCache,
                                                       const ModelConfig& config,
                                                       std::shared_ptr<PreparedModel>& preparedModel)
{
    std::vector<V2_0::SharedBuffer> iBuffers;
    auto memManager = MemoryManager::GetInstance();
    Memory memory;
    OH_NN_ReturnCode ret;
    size_t modelCacheSize = modelCache.size();
    for (size_t i = 0; i < modelCacheSize; i++) {
        ret = memManager->GetMemory(modelCache[i].buffer, memory);
        if (ret != OH_NN_SUCCESS) {
            LOGE("The %{public}zuth model cache is invalid. Please put valid model cache.", i + 1);
            return ret;
        }
        iBuffers.emplace_back(V2_0::SharedBuffer {memory.fd, memory.length, 0, memory.length});
    }

    V2_0::ModelConfig iModelConfig;
    iModelConfig.enableFloat16 = config.enableFloat16;
    iModelConfig.mode = TransPerformanceMode(config.mode);
    iModelConfig.priority = TransPriority(config.priority);

    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;
    V2_0::NNRT_ReturnCode returnCode;
    auto hdiRet = m_iDevice->PrepareModelFromModelCache(iBuffers, iModelConfig, iPreparedModel, returnCode);
    if (hdiRet != HDF_SUCCESS) {
        LOGE("Prepare model from cache failed. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            hdiRet, ConverterRetToString(returnCode).c_str());
        return OH_NN_UNAVALIDABLE_DEVICE;
    }

    preparedModel = CreateSharedPtr<HDIPreparedModelV2_0>(iPreparedModel);
    if (preparedModel == nullptr) {
        LOGE("Prepare model from model cache failed, because fail to create preparedModel instance.");
        return OH_NN_MEMORY_ERROR;
    }
    return OH_NN_SUCCESS;
}

void* HDIDeviceV2_0::AllocateBuffer(size_t length)
{
    if (length == 0) {
        LOGE("The length param is invalid, length=0");
        return nullptr;
    }

    V2_0::SharedBuffer buffer;
    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->AllocateBuffer(length, buffer, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Allocate buffer error. ErrorCode: %{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return nullptr;
    }

    auto memManager = MemoryManager::GetInstance();
    auto addr = memManager->MapMemory(buffer.fd, length);
    if (addr == nullptr) {
        LOGE("Map fd to address failed.");
    }
    return addr;
}

OH_NN_ReturnCode HDIDeviceV2_0::ReleaseBuffer(const void* buffer)
{
    if (buffer == nullptr) {
        LOGE("Buffer is nullptr, no need to release.");
        return OH_NN_INVALID_PARAMETER;
    }

    auto memManager = MemoryManager::GetInstance();
    Memory memory;
    auto ret = memManager->GetMemory(buffer, memory);
    if (ret != OH_NN_SUCCESS) {
        LOGE("Invalid Buffer, it is not NNRt buffer.");
        return ret;
    }

    V2_0::SharedBuffer hdiBuffer {memory.fd, memory.length, 0, memory.length};
    V2_0::NNRT_ReturnCode returnCode;
    auto deviceResult = m_iDevice->ReleaseBuffer(hdiBuffer, returnCode);
    if (deviceResult != HDF_SUCCESS) {
        LOGE("Device release buffer error. ErrorCode: %{public}d, innerHDIRet=%{public}s",
            deviceResult, ConverterRetToString(returnCode).c_str());
        return OH_NN_FAILED;
    }

    ret = memManager->UnMapMemory(buffer);
    if (ret != OH_NN_SUCCESS) {
        LOGE("Unmap memory failed.");
        return ret;
    }

    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::ReleaseSharedBuffer(const V2_0::SharedBuffer& buffer)
{
    if (buffer.fd == INVALID_FD) {
        LOGI("No need to release. fd=%{public}d", INVALID_FD);
        return OH_NN_SUCCESS;
    }

    V2_0::NNRT_ReturnCode returnCode;
    auto ret = m_iDevice->ReleaseBuffer(buffer, returnCode);
    if (ret != HDF_SUCCESS) {
        LOGE("Device release buffer error. ErrorCode=%{public}d, innerHDIRet=%{public}s",
            ret, ConverterRetToString(returnCode).c_str());
        return OH_NN_FAILED;
    }
    return OH_NN_SUCCESS;
}
} // namespace NeuralNetworkRuntime
} // namespace OHOS