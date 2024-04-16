/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "reduceL2_builder.h"

#include "mindir.h"
#include "ops_registry.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace Ops {
static const int INPUT_NUM = 2;
static const int OUTPUT_NUM = 1;
static const int PARAM_MAX_NUM = 3;
static const int SCALE_LENGTH = 1;
static const std::string OP_NAME = "ReduceL2";

ReduceL2Builder::ReduceL2Builder() {}

ReduceL2Builder::~ReduceL2Builder() {}

OH_NN_ReturnCode ReduceL2Builder::SetCoeff(std::shared_ptr<NNTensor> tensor)
{
    if (tensor->GetDataType() != OH_NN_FLOAT32) {
        LOGE("[ReduceL2] The coeff should be type OH_NN_FLOAT32.");
        return OH_NN_INVALID_PARAMETER;
    }

    if (tensor->GetElementCount() != SCALE_LENGTH) {
        LOGE("[ReduceL2] The coeff should be scalar.");
        return OH_NN_INVALID_PARAMETER;
    }

    void* buffer = tensor->GetBuffer();
    if (buffer == nullptr) {
        LOGE("[ReduceL2] Tensor buffer is nullptr.");
        return OH_NN_INVALID_PARAMETER;
    }
    m_coeff = *(static_cast<const float*>(buffer));

    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode ReduceL2Builder::SetReduceToEnd(std::shared_ptr<NNTensor> tensor)
{
    if (tensor->GetDataType() != OH_NN_BOOL) {
        LOGE("[ReduceL2] SetReduceToEnd failed, the reduceToEnd should be type OH_NN_BOOL.");
        return OH_NN_INVALID_PARAMETER;
    }

    tensor->IdentifyOpParameter();
    if (tensor->GetElementCount() != SCALE_LENGTH) {
        LOGE("[ReduceL2] SetReduceToEnd failed, the reduceToEnd dimensions should be scalar.");
        return OH_NN_INVALID_PARAMETER;
    }

    void* buffer = tensor->GetBuffer();
    if (buffer == nullptr) {
        LOGE("[ReduceL2] SetReduceToEnd failed, the reduceToEnd passed buffer is empty.");
        return OH_NN_INVALID_PARAMETER;
    }

    m_reduceToEnd = *(static_cast<bool*>(buffer));
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode ReduceL2Builder::SetKeepDims(std::shared_ptr<NNTensor> tensor)
{
    if (tensor->GetDataType() != OH_NN_BOOL) {
        LOGE("[ReduceL2] SetKeepDims failed, the keep_dims should be type OH_NN_BOOL.");
        return OH_NN_INVALID_PARAMETER;
    }

    tensor->IdentifyOpParameter();
    if (tensor->GetElementCount() != SCALE_LENGTH) {
        LOGE("[ReduceL2] SetKeepDims failed, the keep_dims dimensions should be scalar.");
        return OH_NN_INVALID_PARAMETER;
    }

    void* buffer = tensor->GetBuffer();
    if (buffer == nullptr) {
        LOGE("[ReduceL2] SetKeepDims failed, the keep_dims passed buffer is empty.");
        return OH_NN_INVALID_PARAMETER;
    }

    m_keepDims = *(static_cast<bool*>(buffer));
    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode ReduceL2Builder::Build(const std::vector<uint32_t>& paramsIndex,
                                        const std::vector<uint32_t>& inputsIndex,
                                        const std::vector<uint32_t>& outputsIndex,
                                        const std::vector<std::shared_ptr<NNTensor>>& allTensors)
{
    if (m_isBuild) {
        LOGE("[ReduceL2] Build failed, the ReduceL2 operation has been build, cannot build again.");
        return OH_NN_OPERATION_FORBIDDEN;
    }

    OH_NN_ReturnCode returnCode = CheckIOIndex(inputsIndex, outputsIndex, allTensors, INPUT_NUM, OUTPUT_NUM);
    if (returnCode != OH_NN_SUCCESS) {
        LOGE("[ReduceL2] Build failed, passed invalid input or output index of ReduceL2 operation index.");
        return returnCode;
    }

    m_inputsIndex = inputsIndex;
    m_outputsIndex = outputsIndex;

    returnCode = CheckParamIndex(paramsIndex, allTensors, PARAM_MAX_NUM);
    if (returnCode != OH_NN_SUCCESS) {
        LOGE("[ReduceL2] Build failed, passed invalid param index of ReduceL2 operation index.");
        return returnCode;
    }

    for (uint32_t i : paramsIndex) {
        std::shared_ptr<NNTensor> tensor = allTensors[i];
        if (m_paramMap.find(tensor->GetType()) != m_paramMap.end()) {
            returnCode = (this->*(m_paramMap[tensor->GetType()]))(tensor);
        } else {
            LOGE("[ReduceL2] Build failed, param invalid, type=%d", tensor->GetType());
            return OH_NN_INVALID_PARAMETER;
        }

        if (returnCode != OH_NN_SUCCESS) {
            LOGE("[ReduceL2] Build failed, passed invalid param.");
            return returnCode;
        }
    }

    m_name = OP_NAME;
    m_isBuild = true;
    return OH_NN_SUCCESS;
}

LiteGraphPrimitvePtr ReduceL2Builder::GetPrimitive()
{
    if (!m_isBuild) {
        LOGE("[ReduceL2] GetPrimitive failed, cannot get primitive before call build.");
        return {nullptr, DestroyLiteGraphPrimitive};
    }

    mindspore::lite::ReduceMode mode {mindspore::lite::REDUCE_MODE_L2};

    void* primitive = mindspore::lite::MindIR_ReduceFusion_CreatePrimitive(m_keepDims, mode, m_reduceToEnd, m_coeff);
    LiteGraphPrimitvePtr graphPrimitivePtr(primitive, DestroyLiteGraphPrimitive);
    return graphPrimitivePtr;
}

REGISTER_OPS(ReduceL2Builder, OH_NN_OPS_REDUCE_L2);
} // namespace Ops
} // namespace NeuralNetworkRuntime
} // namespace OHOS