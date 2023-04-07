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


#ifndef NEURAL_NETWORK_RUNTIME_HDI_RETURNCODE_TRANSFORM_H
#define NEURAL_NETWORK_RUNTIME_HDI_RETURNCODE_TRANSFORM_H

#include <v2_0/nnrt_types.h>

namespace V2_0 = OHOS::HDI::Nnrt::V2_0;

namespace OHOS {
namespace NeuralNetworkRuntime {
std::string ConverterRetToString(V2_0::NNRT_ReturnCode returnCode);
} // namespace NeuralNetworkRuntime
} // OHOS
#endif // NEURAL_NETWORK_RUNTIME_HDI_RETURNCODE_TRANSFORM_H