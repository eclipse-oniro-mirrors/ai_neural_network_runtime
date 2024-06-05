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

#include "ops/avgpool_builder.h"

#include "ops_test.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS::NeuralNetworkRuntime::Ops;

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace UnitTest {
class AvgPoolPadBuilderTest : public OpsTest {
public:
    void SetUp() override;
    void TearDown() override;

    void SetPad(OH_NN_DataType dataType,
        const std::vector<int32_t> &dim,  const OH_NN_QuantParam* quantParam, OH_NN_TensorType type);
    void SetRoundMode(OH_NN_DataType dataType,
        const std::vector<int32_t> &dim,  const OH_NN_QuantParam* quantParam, OH_NN_TensorType type);
    void SetGlobal(OH_NN_DataType dataType,
        const std::vector<int32_t> &dim,  const OH_NN_QuantParam* quantParam, OH_NN_TensorType type);
    void SetPadParams();

public:
    AvgPoolBuilder m_builder;
    std::vector<int32_t> m_input_dim{1, 3, 3, 1};
    std::vector<int32_t> m_output_dim{1, 2, 2, 1};
    std::vector<int32_t> m_kenelsize_dim{2};
    std::vector<int32_t> m_stride_dim{2};
    std::vector<int32_t> m_pad_dim{4};
    std::vector<int32_t> m_param_dim{};
    std::vector<uint32_t> m_inputs{0};
    std::vector<uint32_t> m_outputs{1};
    std::vector<uint32_t> m_params{2, 3, 4, 5, 6, 7};
};

void AvgPoolPadBuilderTest::SetUp() {}

void AvgPoolPadBuilderTest::TearDown() {}

void AvgPoolPadBuilderTest::SetRoundMode(OH_NN_DataType dataType,
    const std::vector<int32_t> &dim,  const OH_NN_QuantParam* quantParam, OH_NN_TensorType type)
{
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(dataType, dim, quantParam, type);
    int32_t* roundModeValue = new (std::nothrow) int32_t(0);
    EXPECT_NE(nullptr, roundModeValue);
    tensor->SetBuffer(roundModeValue, sizeof(int32_t));
    m_allTensors.emplace_back(tensor);
}

void AvgPoolPadBuilderTest::SetGlobal(OH_NN_DataType dataType,
    const std::vector<int32_t> &dim,  const OH_NN_QuantParam* quantParam, OH_NN_TensorType type)
{
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(dataType, dim, quantParam, type);
    bool* globalValue = new (std::nothrow) bool(false);
    EXPECT_NE(nullptr, globalValue);
    tensor->SetBuffer(globalValue, sizeof(bool));
    m_allTensors.emplace_back(tensor);
}

void AvgPoolPadBuilderTest::SetPad(OH_NN_DataType dataType,
    const std::vector<int32_t> &dim,  const OH_NN_QuantParam* quantParam, OH_NN_TensorType type)
{
    int32_t padNum{4};
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(dataType, dim, quantParam, type);
    int64_t* padValue = new (std::nothrow) int64_t[padNum] {0, 0, 0, 0};
    EXPECT_NE(nullptr, padValue);
    tensor->SetBuffer(padValue, sizeof(int64_t) * padNum);
    m_allTensors.emplace_back(tensor);
}

void AvgPoolPadBuilderTest::SetPadParams()
{
    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
}

/**
 * @tc.name: avgpool_build_pad_001
 * @tc.desc: Verify the success of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_001, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);
    SetPadParams();
    EXPECT_EQ(OH_NN_SUCCESS, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_002
 * @tc.desc: Verify the forbidden of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_002, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);
    SetPadParams();
    EXPECT_EQ(OH_NN_SUCCESS, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
    EXPECT_EQ(OH_NN_OPERATION_FORBIDDEN, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_003
 * @tc.desc: Verify the missing input of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_003, TestSize.Level1)
{
    m_inputs = {};
    m_outputs = {0};
    m_params = {1, 2, 3, 4, 5, 6};
    m_paramsIndex = m_params;

    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetPadParams();
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_004
 * @tc.desc: Verify the missing output of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_004, TestSize.Level1)
{
    m_inputs = {0};
    m_outputs = {};
    m_params = {1, 2, 3, 4, 5, 6};
    m_paramsIndex = m_params;

    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetPadParams();
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_005
 * @tc.desc: Verify the inputIndex out of bounds of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_005, TestSize.Level1)
{
    m_inputs = {8};
    m_outputs = {1};
    m_params = {2, 3, 4, 5, 6, 7};
    m_paramsIndex = m_params;

    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetPadParams();
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_006
 * @tc.desc: Verify the outputIndex out of bounds of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_006, TestSize.Level1)
{
    m_inputs = {0};
    m_outputs = {8};
    m_params = {2, 3, 4, 5, 6, 7};
    m_paramsIndex = m_params;

    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetPadParams();
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_007
 * @tc.desc: Verify the invalid kernelSize of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_007, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    int32_t numKernels{2};
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT32, m_kenelsize_dim, nullptr,
        OH_NN_AVG_POOL_KERNEL_SIZE);
    int32_t* kernelSizeValue = new (std::nothrow) int32_t[numKernels]{1, 1};
    EXPECT_NE(nullptr, kernelSizeValue);
    tensor->SetBuffer(kernelSizeValue, sizeof(int32_t) * numKernels);
    m_allTensors.emplace_back(tensor);

    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_008
 * @tc.desc: Verify the invalid stride of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_008, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);
    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);

    int32_t numStride{2};
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT32, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    int32_t* strideValue = new (std::nothrow) int32_t[numStride]{1, 1};
    EXPECT_NE(nullptr, strideValue);
    tensor->SetBuffer(strideValue, sizeof(int32_t) * numStride);
    m_allTensors.emplace_back(tensor);

    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_009
 * @tc.desc: Verify the invalid pad of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_009, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    int32_t padNum{4};
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT32, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    int32_t* padValue = new (std::nothrow) int32_t[padNum]{0, 0, 0, 0};
    EXPECT_NE(nullptr, padValue);

    tensor->SetBuffer(padValue, sizeof(int32_t) * padNum);
    m_allTensors.emplace_back(tensor);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_010
 * @tc.desc: Verify the invalid activation of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_010, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT32, m_param_dim, nullptr,
        OH_NN_AVG_POOL_ACTIVATION_TYPE);
    int32_t* activationValue = new (std::nothrow) int32_t(0);
    EXPECT_NE(nullptr, activationValue);

    tensor->SetBuffer(activationValue, sizeof(int32_t));
    m_allTensors.emplace_back(tensor);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_011
 * @tc.desc: Verify the invalid roundMode of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_011, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT64, m_param_dim, nullptr,
        OH_NN_AVG_POOL_ROUND_MODE);
    int64_t* roundModeValue = new (std::nothrow) int64_t(0);
    EXPECT_NE(nullptr, roundModeValue);

    tensor->SetBuffer(roundModeValue, sizeof(int64_t));
    m_allTensors.emplace_back(tensor);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_012
 * @tc.desc: Verify the invalid activation of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_012, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT32, m_param_dim, nullptr,
        OH_NN_AVG_POOL_GLOBAL);
    int32_t* globalValue = new (std::nothrow) int32_t(0);
    EXPECT_NE(nullptr, globalValue);

    tensor->SetBuffer(globalValue, sizeof(int32_t));
    m_allTensors.emplace_back(tensor);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_013
 * @tc.desc: Verify the activation scalar length of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_013, TestSize.Level1)
{
    m_param_dim = {2};
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    int8_t* activationValue = new (std::nothrow) int8_t[2]{1, 2};
    EXPECT_NE(nullptr, activationValue);

    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT8, m_param_dim, nullptr,
        OH_NN_AVG_POOL_ACTIVATION_TYPE);
    tensor->SetBuffer(activationValue, 2 * sizeof(int8_t));
    m_allTensors.emplace_back(tensor);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_014
 * @tc.desc: Verify the avgpool without set kernelsize of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_014, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT64, m_kenelsize_dim, nullptr,
        OH_NN_AVG_POOL_KERNEL_SIZE);
    m_allTensors.emplace_back(tensor);

    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_015
 * @tc.desc: Verify the avgpool without set stride of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_015, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    m_allTensors.emplace_back(tensor);

    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_016
 * @tc.desc: Verify the avgpool without set pad of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_016, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT32, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    m_allTensors.emplace_back(tensor);

    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_017
 * @tc.desc: Verify the avgpool without set activation of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_017, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);

    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT8, m_param_dim, nullptr,
        OH_NN_AVG_POOL_ACTIVATION_TYPE);
    m_allTensors.emplace_back(tensor);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_018
 * @tc.desc: Verify the avgpool without set activation of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_018, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);

    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT64, m_param_dim, nullptr,
        OH_NN_AVG_POOL_ROUND_MODE);
    m_allTensors.emplace_back(tensor);
    SetGlobal(OH_NN_BOOL, m_param_dim, nullptr, OH_NN_AVG_POOL_GLOBAL);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_build_pad_019
 * @tc.desc: Verify the avgpool without set activation of the build function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_build_pad_019, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetKernelSize(OH_NN_INT64, m_kenelsize_dim, nullptr, OH_NN_AVG_POOL_KERNEL_SIZE);
    SetStride(OH_NN_INT64, m_stride_dim, nullptr, OH_NN_AVG_POOL_STRIDE);
    SetPad(OH_NN_INT64, m_pad_dim, nullptr, OH_NN_AVG_POOL_PAD);
    SetActivation(OH_NN_INT8, m_param_dim, nullptr, OH_NN_AVG_POOL_ACTIVATION_TYPE);
    SetRoundMode(OH_NN_INT32, m_param_dim, nullptr, OH_NN_AVG_POOL_ROUND_MODE);

    std::shared_ptr<NNTensor> tensor = TransToNNTensor(OH_NN_INT8, m_param_dim, nullptr,
        OH_NN_AVG_POOL_GLOBAL);
    m_allTensors.emplace_back(tensor);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
}

/**
 * @tc.name: avgpool_getprimitive_pad_001
 * @tc.desc: Verify the behavior of the GetPrimitive function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_getprimitive_pad_001, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetPadParams();
    EXPECT_EQ(OH_NN_SUCCESS, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
    LiteGraphTensorPtr primitive = m_builder.GetPrimitive();
    LiteGraphTensorPtr expectPrimitive = {nullptr, DestroyLiteGraphPrimitive};
    EXPECT_NE(expectPrimitive, primitive);

    std::vector<int64_t> expetctKernelSize = mindspore::lite::MindIR_AvgPoolFusion_GetKernelSize(primitive.get());
    std::vector<int64_t> kernelSizeValueTest{1, 1};
    EXPECT_EQ(kernelSizeValueTest, expetctKernelSize);
    std::vector<int64_t> expetctStrides = mindspore::lite::MindIR_AvgPoolFusion_GetStrides(primitive.get());
    std::vector<int64_t> strideValueTest{1, 1};
    std::vector<int64_t> expetctPadValue  = mindspore::lite::MindIR_AvgPoolFusion_GetPad(primitive.get());
    std::vector<int64_t> padValueValueTest{0, 0, 0, 0};
    EXPECT_EQ(padValueValueTest, expetctPadValue);

    int8_t activationValue = 0;
    int expectActivation = mindspore::lite::MindIR_AvgPoolFusion_GetActivationType(primitive.get());
    EXPECT_EQ(activationValue, expectActivation);
    mindspore::lite::RoundMode roundModeValue = mindspore::lite::ROUND_MODE_FLOOR;
    auto expectRoundMode = mindspore::lite::MindIR_AvgPoolFusion_GetRoundMode(primitive.get());
    EXPECT_EQ(roundModeValue, expectRoundMode);
    bool globalValue = false;
    bool expectGlobal = mindspore::lite::MindIR_AvgPoolFusion_GetGlobal(primitive.get());
    EXPECT_EQ(globalValue, expectGlobal);
}

/**
 * @tc.name: avgpool_getprimitive_pad_002
 * @tc.desc: Verify the behavior of the GetPrimitive function
 * @tc.type: FUNC
 */
HWTEST_F(AvgPoolPadBuilderTest, avgpool_getprimitive_pad_002, TestSize.Level1)
{
    m_paramsIndex = m_params;
    SaveInputTensor(m_inputs, OH_NN_FLOAT32, m_input_dim, nullptr);
    SaveOutputTensor(m_outputs, OH_NN_FLOAT32, m_output_dim, nullptr);

    SetPadParams();
    LiteGraphTensorPtr primitive = m_builder.GetPrimitive();
    LiteGraphTensorPtr expectPrimitive = {nullptr, DestroyLiteGraphPrimitive};
    EXPECT_EQ(expectPrimitive, primitive);
}
} // namespace UnitTest
} // namespace NeuralNetworkRuntime
} // namespace OHOS
