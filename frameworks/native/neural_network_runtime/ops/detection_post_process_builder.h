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

#ifndef NEURAL_NETWORK_RUNTIME_DETECTION_POST_PROCESS_BUILDER_H
#define NEURAL_NETWORK_RUNTIME_DETECTION_POST_PROCESS_BUILDER_H

#include "ops_builder.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace Ops {
class DetectionPostProcessBuilder : public OpsBuilder {
public:
    DetectionPostProcessBuilder();
    ~DetectionPostProcessBuilder() override;
    OH_NN_ReturnCode Build(const std::vector<uint32_t>& paramsIndex,
                           const std::vector<uint32_t>& inputsIndex,
                           const std::vector<uint32_t>& outputsIndex,
                           const std::vector<std::shared_ptr<NNTensor>>& allTensors) override;

    LiteGraphPrimitvePtr GetPrimitive() override;

private:
    OH_NN_ReturnCode SetInputSize(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetScale(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetNmsIoUThreshold(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetNmsScoreThreshold(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetMaxDetections(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetDetectionsPerClass(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetMaxClassesPerDetection(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetNumClasses(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetUseRegularNms(std::shared_ptr<NNTensor> tensor);
    OH_NN_ReturnCode SetOutQuantized(std::shared_ptr<NNTensor> tensor);

private:
    int64_t m_inputSize {0};
    std::vector<float> m_scale;
    float m_nmsIoUThreshold {0.0f};
    float m_nmsScoreThreshold {0.0f};
    int64_t m_maxDetections {0};
    int64_t m_detectionsPerClass {0};
    int64_t m_maxClassesPerDetection {0};
    int64_t m_numClasses {0};
    bool m_useRegularNms {false};
    bool m_outQuantized {false};
};
} // namespace Ops
} // namespace NeuralNetworkRuntime
} // namespace OHOS

#endif // NEURAL_NETWORK_RUNTIME_DETECTION_POST_PROCESS_BUILDER_H
