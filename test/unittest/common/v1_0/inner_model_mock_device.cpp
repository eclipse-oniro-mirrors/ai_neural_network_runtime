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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "utils.h"
#include "inner_model.h"
#include "hdi_device_v1_0.h"
#include "ops/div_builder.h"
#include "mock_idevice.h"

namespace OHOS {
namespace NeuralNetworkRuntime {

// Mock the palce where the operator GetPrimitive is called in inner_model build function.
Ops::LiteGraphPrimitvePtr Ops::DivBuilder::GetPrimitive()
{
    Ops::LiteGraphPrimitvePtr primitive = {nullptr, DestroyLiteGraphTensor};
    return primitive;
}

// Mock the palce where the device GetSupportedOperation is called in inner_model build function.
OH_NN_ReturnCode HDIDeviceV1_0::GetSupportedOperation(std::shared_ptr<const mindspore::lite::LiteGraph> model,
    std::vector<bool>& supportedOperations)
{
    supportedOperations = {true, true, true};

    if (model->name_ == "Loaded_NNR_Model") {
        return OH_NN_UNAVAILABLE_DEVICE;
    } else {
        return OH_NN_SUCCESS;
    }
}
} // NeuralNetworkRuntime
} // OHOS
