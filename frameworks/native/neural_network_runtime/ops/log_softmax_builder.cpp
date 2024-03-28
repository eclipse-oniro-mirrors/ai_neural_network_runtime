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

#include "log_softmax_builder.h"

#include "mindir.h"
#include "ops_registry.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace Ops {
static const int INPUT_NUM = 1;
static const int OUTPUT_NUM = 1;
static const int PARAM_MAX_NUM = 1;
static const int SCALAR_LENGTH = 1;
static const std::string OP_NAME = "LogSoftmax";

LogSoftmaxBuilder::LogSoftmaxBuilder() {}

LogSoftmaxBuilder::~LogSoftmaxBuilder() {}

OH_NN_ReturnCode LogSoftmaxBuilder::SetAxis(std::shared_ptr<NNTensor>& tensor)
{
    if (tensor->GetDataType() != OH_NN_INT64) {
        LOGE("[LogSoftmax] The axis should be type OH_NN_INT64.");
        return OH_NN_INVALID_PARAMETER;
    }

    if (tensor->GetElementCount() != SCALAR_LENGTH) {
        LOGE("[LogSoftmax] The axis should be scalar.");
        return OH_NN_INVALID_PARAMETER;
    }

    void* buffer = tensor->GetBuffer();
    if (buffer == nullptr) {
        LOGE("[LogSoftmax] Tensor buffer is nullptr.");
        return OH_NN_INVALID_PARAMETER;
    }
    m_axis = *(static_cast<const int64_t*>(buffer));

    return OH_NN_SUCCESS;
}

OH_NN_ReturnCode LogSoftmaxBuilder::Build(const std::vector<uint32_t>& paramsIndex,
                                          const std::vector<uint32_t>& inputsIndex,
                                          const std::vector<uint32_t>& outputsIndex,
                                          const std::vector<std::shared_ptr<NNTensor>>& allTensors)
{
    if (m_isBuild) {
        LOGE("[LogSoftmax] Build failed, the logSoftmax operation has been build. cannot build again.");
        return OH_NN_OPERATION_FORBIDDEN;
    }

    auto ret = CheckIOIndex(inputsIndex, outputsIndex, allTensors, INPUT_NUM, OUTPUT_NUM);
    if (ret != OH_NN_SUCCESS) {
        LOGE("[LogSoftmax] Build failed, passed invalid input or output index.");
        return ret;
    }
    
    m_inputsIndex = inputsIndex;
    m_outputsIndex = outputsIndex;

    ret = CheckParamIndex(paramsIndex, allTensors, PARAM_MAX_NUM);
    if (ret != OH_NN_SUCCESS) {
        LOGE("[LogSoftmax] Build failed, passed invalid param index.");
        return ret;
    }

    for (int i : paramsIndex) {
        std::shared_ptr<NNTensor> tensor = allTensors[i];
        tensor->IdentifyOpParameter();
        switch (tensor->GetType()) {
            case OH_NN_LOG_SOFTMAX_AXIS:
                ret = SetAxis(tensor);
                break;
            default:
                LOGE("[LogSoftmax] Build failed, param invalid, type=%d", tensor->GetType());
                return OH_NN_INVALID_PARAMETER;
        }

        if (ret != OH_NN_SUCCESS) {
            LOGE("[LogSoftmax] Build failed, passed invalid param.");
            return ret;
        }
    }

    m_name = OP_NAME;
    m_isBuild = true;
    return OH_NN_SUCCESS;
}

LiteGraphPrimitvePtr LogSoftmaxBuilder::GetPrimitive()
{
    if (!m_isBuild) {
        LOGE("[LogSoftmax] GetPrimitive failed, cannot get primitive before call build.");
        return {nullptr, DestroyLiteGraphPrimitive};
    }

    void* primitive = mindspore::lite::MindIR_LogSoftmax_CreatePrimitive(m_axis);
    LiteGraphPrimitvePtr graphPrimitivePtr(primitive, DestroyLiteGraphPrimitive) ;
    return graphPrimitivePtr;
}

REGISTER_OPS(LogSoftmaxBuilder, OH_NN_OPS_LOG_SOFTMAX);
} // namespace Ops
} // namespace NeuralNetworkRuntime
} // namespace OHOS
