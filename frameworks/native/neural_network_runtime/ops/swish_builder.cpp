/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "swish_builder.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace Ops {
static const int INPUT_NUM = 1;
static const int OUTPUT_NUM = 1;
static const int PARAM_NUM = 0;
static const std::string OP_NAME = "Swish";

SwishBuilder::SwishBuilder() {}

SwishBuilder::~SwishBuilder() {}

OH_NN_ReturnCode SwishBuilder::Build(const std::vector<uint32_t>& paramsIndex,
                                     const std::vector<uint32_t>& inputsIndex,
                                     const std::vector<uint32_t>& outputsIndex,
                                     const std::vector<std::shared_ptr<NNTensor>>& allTensors)
{
    if (m_isBuild) {
        LOGE("[Swish] Build failed, the swish operation has been build. cannot build again.");
        return OH_NN_OPERATION_FORBIDDEN;
    }

    auto ret = CheckIOIndex(inputsIndex, outputsIndex, allTensors, INPUT_NUM, OUTPUT_NUM);
    if (ret != OH_NN_SUCCESS) {
        LOGE("[Swish] Build failed, passed invalid input or output index.");
        return ret;
    }

    m_inputsIndex = inputsIndex;
    m_outputsIndex = outputsIndex;

    ret = CheckParamIndex(paramsIndex, allTensors, PARAM_NUM);
    if (ret != OH_NN_SUCCESS) {
        LOGE("[Swish] Build failed, passed invalid param index.");
        return ret;
    }

    m_name = OP_NAME;
    m_isBuild = true;
    return OH_NN_SUCCESS;
}

LiteGraphPrimitvePtr SwishBuilder::GetPrimitive()
{
    if (!m_isBuild) {
        LOGE("[Swish] GetPrimitive failed, cannot get primitive before call build.");
        return {nullptr, DestroyLiteGraphPrimitive};
    }

    float alpha {0.0f};
    float minVal {0.0f};
    float maxVal {0.0f};
    bool approximate {false};
    mindspore::lite::ActivationType activationType {mindspore::lite::ACTIVATION_TYPE_SWISH};

    void* primitive = mindspore::lite::MindIR_Activation_CreatePrimitive(activationType, alpha,
                                                                         minVal, maxVal, approximate);
    LiteGraphPrimitvePtr graphPrimitivePtr(primitive, DestroyLiteGraphPrimitive) ;
    return graphPrimitivePtr;
}

REGISTER_OPS(SwishBuilder, OH_NN_OPS_SWISH);
} // namespace Ops
} // namespace NeuralNetworkRuntime
} // namespace OHOS
