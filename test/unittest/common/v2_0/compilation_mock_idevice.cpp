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

#include "common/utils.h"
#include "hdi_device_v2_0.h"
#include "nn_tensor.h"
#include "test/unittest/common/v2_0/mock_idevice.h"

OH_NN_ReturnCode OHOS::HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;

namespace OHOS {
namespace NeuralNetworkRuntime {
OH_NN_ReturnCode HDIDeviceV2_0::IsModelCacheSupported(bool& isSupported)
{
    // isSupported is false when expecting to return success
    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_SUCCESS) {
        // In order not to affect other use cases, set to the OH_NN_OPERATION_FORBIDDEN
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_SUCCESS;
    }

    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_FAILED) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_FAILED;
    }

    isSupported = true;
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::GetSupportedOperation(std::shared_ptr<const mindspore::lite::LiteGraph> model,
    std::vector<bool>& ops)
{
    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_INVALID_FILE) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        ops.emplace_back(true);
        return OH_NN_SUCCESS;
    }

    if (model == nullptr) {
        LOGE("HDIDeviceV2_0 mock GetSupportedOperation failed, Model is nullptr, cannot query supported operation.");
        return OH_NN_NULL_PTR;
    }

    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_SUCCESS) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        ops.emplace_back(false);
        return OH_NN_SUCCESS;
    }

    ops.emplace_back(true);
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsDynamicInputSupported(bool& isSupported)
{
    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_FAILED) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_FAILED;
    }

    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_INVALID_PATH) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_SUCCESS;
    }

    isSupported = true;
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsPerformanceModeSupported(bool& isSupported)
{
    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_FAILED) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_FAILED;
    }

    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_SUCCESS) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_SUCCESS;
    }

    isSupported = true;
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsPrioritySupported(bool& isSupported)
{
    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_INVALID_PARAMETER) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_INVALID_PARAMETER;
    }

    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_SUCCESS) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_SUCCESS;
    }

    isSupported = true;
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::IsFloat16PrecisionSupported(bool& isSupported)
{
    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_SUCCESS) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_SUCCESS;
    }

    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_MEMORY_ERROR) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        isSupported = false;
        return OH_NN_MEMORY_ERROR;
    }

    isSupported = true;
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::PrepareModel(std::shared_ptr<const mindspore::lite::LiteGraph> model,
    const ModelConfig& config, std::shared_ptr<PreparedModel>& preparedModel)
{
    if (model == nullptr) {
        LOGE("HDIDeviceV2_0 mock PrepareModel failed, the model is nullptr");
        return OH_NN_INVALID_PARAMETER;
    }

    if (config.enableFloat16 == false) {
        LOGE("HDIDeviceV2_0 mock PrepareModel failed, the enableFloat16 is false");
        return OH_NN_FAILED;
    }

    sptr<OHOS::HDI::Nnrt::V2_0::IPreparedModel> hdiPreparedModel = sptr<OHOS::HDI::Nnrt::V2_0
        ::MockIPreparedModel>(new (std::nothrow) OHOS::HDI::Nnrt::V2_0::MockIPreparedModel());
    if (hdiPreparedModel == nullptr) {
        LOGE("HDIDeviceV2_0 mock PrepareModel failed, error happened when new sptr");
        return OH_NN_NULL_PTR;
    }

    preparedModel = CreateSharedPtr<HDIPreparedModelV2_0>(hdiPreparedModel);
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIPreparedModelV2_0::ExportModelCache(std::vector<Buffer>& modelCache)
{
    if (!modelCache.empty()) {
        LOGE("HDIPreparedModelV2_0 mock ExportModelCache failed, the modelCache is not empty");
        return OH_NN_INVALID_PARAMETER;
    }

    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_FAILED) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        return OH_NN_FAILED;
    }

    int bufferSize = 13;
    Buffer buffer;
    std::string aBuffer = "mock_buffer_a";
    buffer.data = (void*)aBuffer.c_str();
    buffer.length = bufferSize;
    modelCache.emplace_back(buffer);

    Buffer buffer2;
    std::string bBuffer = "mock_buffer_b";
    buffer2.data = (void*)bBuffer.c_str();
    buffer2.length = bufferSize;
    modelCache.emplace_back(buffer2);

    return OH_NN_SUCCESS;
}

void* HDIDeviceV2_0::AllocateBuffer(size_t length)
{
    if (length == 0) {
        LOGE("HDIDeviceV2_0 mock AllocateBuffer failed, the length param is invalid");
        return nullptr;
    }

    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_NULL_PTR) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        return nullptr;
    }

    void* buffer = malloc(length);
    if (buffer == nullptr) {
        LOGE("HDIDeviceV2_0 mock AllocateBuffer failed, the buffer is nullptr");
        return nullptr;
    }
    return buffer;
}

OH_NN_ReturnCode HDIDeviceV2_0::ReleaseBuffer(const void* buffer)
{
    if (buffer == nullptr) {
        LOGE("HDIDeviceV2_0 mock ReleaseBuffer failed, the buffer is nullptr");
        return OH_NN_NULL_PTR;
    }

    free(const_cast<void *>(buffer));
    buffer = nullptr;
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode HDIDeviceV2_0::PrepareModelFromModelCache(const std::vector<Buffer>& modelCache,
    const ModelConfig& config, std::shared_ptr<PreparedModel>& preparedModel, bool& isUpdatable)
{
    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_FAILED) {
        HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode = OH_NN_OPERATION_FORBIDDEN;
        return OH_NN_FAILED;
    }

    if (modelCache.size() == 0 || config.enableFloat16 == false) {
        LOGE("HDIDeviceV2_0 mock PrepareModel failed, the modelCache size equals 0 or enableFloat16 is false");
        return OH_NN_FAILED;
    }

    sptr<OHOS::HDI::Nnrt::V2_0::IPreparedModel> hdiPreparedModel = sptr<OHOS::HDI::Nnrt::V2_0
        ::MockIPreparedModel>(new (std::nothrow) OHOS::HDI::Nnrt::V2_0::MockIPreparedModel());
    if (hdiPreparedModel == nullptr) {
        LOGE("HDIDeviceV2_0 mock PrepareModelFromModelCache failed, error happened when new sptr");
        return OH_NN_NULL_PTR;
    }

    preparedModel = CreateSharedPtr<HDIPreparedModelV2_0>(hdiPreparedModel);

    return OH_NN_SUCCESS;
}

bool NNTensor::IsDynamicShape() const
{
    if (HDI::Nnrt::V2_0::MockIPreparedModel::m_ExpectRetCode == OH_NN_FAILED) {
        return false;
    }

    return true;
}
} // namespace NeuralNetworkRuntime
} // namespace OHOS