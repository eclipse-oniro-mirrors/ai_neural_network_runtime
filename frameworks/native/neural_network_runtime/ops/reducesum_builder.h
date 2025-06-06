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

#ifndef NEURAL_NETWORK_RUNTIME_REDUCESUM_BUILDER_H
#define NEURAL_NETWORK_RUNTIME_REDUCESUM_BUILDER_H

#include "ops_builder.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace Ops {
class ReduceSumBuilder : public OpsBuilder {
public:
    typedef OH_NN_ReturnCode (ReduceSumBuilder::*FuncPtr)(const std::shared_ptr<NNTensor>&);

    ReduceSumBuilder();
    ~ReduceSumBuilder() override;
    OH_NN_ReturnCode Build(const std::vector<uint32_t>& paramsIndex,
                           const std::vector<uint32_t>& inputsIndex,
                           const std::vector<uint32_t>& outputsIndex,
                           const std::vector<std::shared_ptr<NNTensor>>& allTensors) override;

    LiteGraphPrimitvePtr GetPrimitive() override;

private:
    OH_NN_ReturnCode SetCoeff(const std::shared_ptr<NNTensor>& tensor);
    OH_NN_ReturnCode SetReduceToEnd(const std::shared_ptr<NNTensor>& tensor);
    OH_NN_ReturnCode SetKeepDims(const std::shared_ptr<NNTensor>& tensor);

private:
    float m_coeff {0.0f};
    bool m_reduceToEnd {false};
    bool m_keepDims {false};
    std::unordered_map<OH_NN_TensorType, FuncPtr> m_paramMap = {
        {OH_NN_REDUCE_SUM_COEFF, &ReduceSumBuilder::SetCoeff},
        {OH_NN_REDUCE_SUM_REDUCE_TO_END, &ReduceSumBuilder::SetReduceToEnd},
        {OH_NN_REDUCE_SUM_KEEP_DIMS, &ReduceSumBuilder::SetKeepDims}
    };
};
} // namespace Ops
} // namespace NeuralNetworkRuntime
} // namespace OHOS

#endif // NEURAL_NETWORK_RUNTIME_REDUCESUM_BUILDER_H