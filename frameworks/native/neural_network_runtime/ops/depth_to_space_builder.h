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

#ifndef NEURAL_NETWORK_RUNTIME_DEPTH_TO_SPACE_BUILDER_H
#define NEURAL_NETWORK_RUNTIME_DEPTH_TO_SPACE_BUILDER_H

#include "mindir.h"

#include "ops_builder.h"
#include "ops_registry.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace Ops {
class DepthToSpaceBuilder : public OpsBuilder {
public:
    typedef OH_NN_ReturnCode (DepthToSpaceBuilder::*FuncPtr)(const std::shared_ptr<NNTensor>&);

    DepthToSpaceBuilder();
    ~DepthToSpaceBuilder() override;
    OH_NN_ReturnCode Build(const std::vector<uint32_t>& paramsIndex,
                           const std::vector<uint32_t>& inputsIndex,
                           const std::vector<uint32_t>& outputsIndex,
                           const std::vector<std::shared_ptr<NNTensor>>& allTensors) override;

    LiteGraphPrimitvePtr GetPrimitive() override;

private:
    OH_NN_ReturnCode SetBlockSize(const std::shared_ptr<NNTensor>& tensor);
    OH_NN_ReturnCode SetMode(const std::shared_ptr<NNTensor>& tensor);

private:
    int64_t m_blockSize {0};
    std::string m_mode;
    std::unordered_map<OH_NN_TensorType, FuncPtr> m_paramMap = {
        {OH_NN_DEPTH_TO_SPACE_BLOCK_SIZE, &DepthToSpaceBuilder::SetBlockSize},
        {OH_NN_DEPTH_TO_SPACE_MODE, &DepthToSpaceBuilder::SetMode}
    };
};
} // namespace Ops
} // namespace NeuralNetworkRuntime
} // namespace OHOS

#endif // NEURAL_NETWORK_RUNTIME_DEPTH_TO_SPACE_BUILDER_H
