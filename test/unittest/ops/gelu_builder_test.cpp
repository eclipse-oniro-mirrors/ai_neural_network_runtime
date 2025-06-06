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

#include "ops/gelu_builder.h"

#include "ops_test.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS::NeuralNetworkRuntime::Ops;

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace UnitTest {
class GeluBuilderTest : public OpsTest {
public:
    void SetUp() override;
    void TearDown() override;

protected:
    void SetApproximate(OH_NN_DataType dataType,
        const std::vector<int32_t> &dim,  const OH_NN_QuantParam* quantParam, OH_NN_TensorType type);

protected:
    GeluBuilder m_gelu;
    std::vector<uint32_t> m_inputs {0};
    std::vector<uint32_t> m_outputs {1};
    std::vector<uint32_t> m_params {2};
    std::vector<int32_t> m_inputDim {1, 5, 1, 1};
    std::vector<int32_t> m_outputDim {1, 5, 1, 1};
    std::vector<int32_t> m_paramsDim {};
};

void GeluBuilderTest::SetUp() {}

void GeluBuilderTest::TearDown() {}

void GeluBuilderTest::SetApproximate(OH_NN_DataType dataType,
    const std::vector<int32_t> &dim, const OH_NN_QuantParam* quantParam, OH_NN_TensorType type)
{
    std::shared_ptr<NNTensor> outQuantizedTensor = TransToNNTensor(dataType, dim, quantParam, type);
    bool* outQuantizedValue = new (std::nothrow) bool(false);
    EXPECT_NE(nullptr, outQuantizedValue);
    outQuantizedTensor->SetBuffer(outQuantizedValue, sizeof(bool));
    m_allTensors.emplace_back(outQuantizedTensor);
}

/**
 * @tc.name: gelu_build_001
 * @tc.desc: Verify that the build function returns a successful message.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_001, TestSize.Level0)
{
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_outputDim, nullptr);
    SetApproximate(OH_NN_BOOL, m_paramsDim, nullptr, OH_NN_GELU_APPROXIMATE);

    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_SUCCESS, ret);
}

/**
 * @tc.name: gelu_build_002
 * @tc.desc: Verify that the build function returns a failed message with true m_isBuild.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_002, TestSize.Level0)
{
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_outputDim, nullptr);
    SetApproximate(OH_NN_BOOL, m_paramsDim, nullptr, OH_NN_GELU_APPROXIMATE);

    EXPECT_EQ(OH_NN_SUCCESS, m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors));
    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_OPERATION_FORBIDDEN, ret);
}

/**
 * @tc.name: gelu_build_003
 * @tc.desc: Verify that the build function returns a failed message with invalided input.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_003, TestSize.Level0)
{
    m_inputs = {0, 1};
    m_outputs = {2};
    m_params = {3};

    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_outputDim, nullptr);
    SetApproximate(OH_NN_BOOL, m_paramsDim, nullptr, OH_NN_GELU_APPROXIMATE);

    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: gelu_build_004
 * @tc.desc: Verify that the build function returns a failed message with invalided output.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_004, TestSize.Level0)
{
    std::vector<uint32_t> m_outputs = {1, 2};
    m_params = {3};

    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_outputDim, nullptr);
    SetApproximate(OH_NN_BOOL, m_paramsDim, nullptr, OH_NN_GELU_APPROXIMATE);

    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: gelu_build_005
 * @tc.desc: Verify that the build function returns a failed message with empty allTensor.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_005, TestSize.Level0)
{
    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputs, m_outputs, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: gelu_build_006
 * @tc.desc: Verify that the build function returns a failed message without output tensor.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_006, TestSize.Level0)
{
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);

    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputs, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: gelu_build_007
 * @tc.desc: Verify that the build function returns a failed message with a virtual parameter.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_007, TestSize.Level0)
{
    m_params = {2, 3};
    std::vector<int32_t> paramDim = {};

    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_outputDim, nullptr);
    std::shared_ptr<NNTensor> paramTensor;
    paramTensor = TransToNNTensor(OH_NN_INT32, paramDim, nullptr, OH_NN_TENSOR);
    m_allTensors.emplace_back(paramTensor);
    SetApproximate(OH_NN_BOOL, m_paramsDim, nullptr, OH_NN_GELU_APPROXIMATE);

    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: gelu_build_008
 * @tc.desc: Verify that the build function returns a failed message with invalid approximate's dataType.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_008, TestSize.Level0)
{
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_outputDim, nullptr);

    std::shared_ptr<NNTensor> approximateTensor = TransToNNTensor(OH_NN_INT64, m_paramsDim,
        nullptr, OH_NN_GELU_APPROXIMATE);
    int64_t* approximateValue = new (std::nothrow) int64_t[1] {0};
    EXPECT_NE(nullptr, approximateValue);
    approximateTensor->SetBuffer(approximateValue, sizeof(int64_t));
    m_allTensors.emplace_back(approximateTensor);

    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: gelu_build_009
 * @tc.desc: Verify that the build function returns a failed message with passing invalid approximate param.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_009, TestSize.Level0)
{
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_outputDim, nullptr);
    SetApproximate(OH_NN_BOOL, m_paramsDim, nullptr, OH_NN_MUL_ACTIVATION_TYPE);

    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: Gelu_build_011
 * @tc.desc: Verify that the build function returns a failed message without set buffer for approximate.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_build_011, TestSize.Level0)
{
    SaveInputTensor(m_inputs, OH_NN_INT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_INT32, m_outputDim, nullptr);

    std::shared_ptr<NNTensor> approximateTensor = TransToNNTensor(OH_NN_BOOL, m_paramsDim,
        nullptr, OH_NN_GELU_APPROXIMATE);
    m_allTensors.emplace_back(approximateTensor);

    OH_NN_ReturnCode ret = m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: gelu_getprimitive_001
 * @tc.desc: Verify that the getPrimitive function returns a successful message
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_getprimitive_001, TestSize.Level0)
{
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_inputDim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_outputDim, nullptr);
    SetApproximate(OH_NN_BOOL, m_paramsDim, nullptr, OH_NN_GELU_APPROXIMATE);

    bool approximateValue = false;
    EXPECT_EQ(OH_NN_SUCCESS, m_gelu.Build(m_params, m_inputsIndex, m_outputsIndex, m_allTensors));
    LiteGraphPrimitvePtr primitive = m_gelu.GetPrimitive();
    LiteGraphPrimitvePtr expectPrimitive(nullptr, DestroyLiteGraphPrimitive);
    EXPECT_NE(expectPrimitive, primitive);

    mindspore::lite::ActivationType activationType = mindspore::lite::ACTIVATION_TYPE_GELU;
    auto returnValue = mindspore::lite::MindIR_Activation_GetActivationType(primitive.get());
    EXPECT_EQ(returnValue, activationType);
    auto returnApproximateValue = mindspore::lite::MindIR_Activation_GetApproximate(primitive.get());
    EXPECT_EQ(returnApproximateValue, approximateValue);
}

/**
 * @tc.name: gelu_getprimitive_002
 * @tc.desc: Verify that the getPrimitive function returns a failed message without build.
 * @tc.type: FUNC
 */
HWTEST_F(GeluBuilderTest, gelu_getprimitive_002, TestSize.Level0)
{
    GeluBuilder gelu;
    LiteGraphPrimitvePtr primitive = m_gelu.GetPrimitive();
    LiteGraphPrimitvePtr expectPrimitive(nullptr, DestroyLiteGraphPrimitive);
    EXPECT_EQ(expectPrimitive, primitive);
}
}
}
}