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

#include "lessequal_builder.h"

#include "mindir.h"
#include "ops_registry.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace Ops {
static const int INPUT_NUMS = 2;
static const int OUTPUT_NUMS = 1;
static const int PARAM_NUM = 0;
static const std::string OP_NAME = "LessEqual";

LessEqualBuilder::LessEqualBuilder() {}

LessEqualBuilder::~LessEqualBuilder() {}

OH_NN_ReturnCode LessEqualBuilder::Build(const std::vector<uint32_t>& paramsIndex,
                                         const std::vector<uint32_t>& inputsIndex,
                                         const std::vector<uint32_t>& outputsIndex,
                                         const std::vector<std::shared_ptr<NNTensor>>& allTensors)
{
    if (m_isBuild) {
        LOGE("[LessEqual] Build failded, operation has been build, cannot build again.");
        return OH_NN_OPERATION_FORBIDDEN;
    }

    OH_NN_ReturnCode returnCode = CheckIOIndex(inputsIndex, outputsIndex, allTensors, INPUT_NUMS, OUTPUT_NUMS);
    if (returnCode != OH_NN_SUCCESS) {
        LOGE("[LessEqual] Build failded, Passed invalid input or output indices.");
        return returnCode;
    }

    returnCode = CheckParamIndex(paramsIndex, allTensors, PARAM_NUM);
    if (returnCode != OH_NN_SUCCESS) {
        LOGE("[LessEqual] Build failded, Passed invalid param indices.");
        return returnCode;
    }

    m_inputsIndex = inputsIndex;
    m_outputsIndex = outputsIndex;

    // The quantization type of the first output determinies that of the operator.
    SetQuantType(outputsIndex, allTensors);

    m_name = OP_NAME;
    m_isBuild = true;
    return OH_NN_SUCCESS;
}

LiteGraphPrimitvePtr LessEqualBuilder::GetPrimitive()
{
    if (!m_isBuild) {
        LOGE("[LessEqual] GetPrimitive failed, cannot get primitive before call build.");
        return {nullptr, DestroyLiteGraphPrimitive};
    }

    void* primitive = mindspore::lite::MindIR_LessEqual_CreatePrimitive();
    LiteGraphPrimitvePtr graphPrimitivePtr(primitive, DestroyLiteGraphPrimitive);
    return graphPrimitivePtr;
}

REGISTER_OPS(LessEqualBuilder, OH_NN_OPS_LESS_EQUAL);
} // namespace Ops
} // namespace NeuralNetworkRuntime
} // namespace OHOS