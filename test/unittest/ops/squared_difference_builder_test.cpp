/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ops/squared_difference_builder.h"

#include <gtest/gtest.h>
#include "nn_tensor.h"
#include "ops_test.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS::NeuralNetworkRuntime::Ops;

namespace OHOS {
namespace NeuralNetworkRuntime {
namespace UnitTest {
class SquaredDifferenceBuilderTest : public OpsTest {
protected:
    void InitTensor(const std::vector<uint32_t>& inputsIndex,
        const std::vector<uint32_t>& outputsIndex) override;
    void CheckResult();

protected:
    SquaredDifferenceBuilder m_builder;
};

void SquaredDifferenceBuilderTest::InitTensor(const std::vector<uint32_t>& inputsIndex,
    const std::vector<uint32_t>& outputsIndex)
{
    std::vector<int32_t> inputDim = {3};
    std::vector<int32_t> OutputDim = {3};

    SaveInputTensor(inputsIndex, OH_NN_FLOAT32, inputDim, nullptr);
    SaveOutputTensor(outputsIndex, OH_NN_FLOAT32, OutputDim, nullptr);
}

void SquaredDifferenceBuilderTest::CheckResult()
{
    LiteGraphTensorPtr primitive = m_builder.GetPrimitive();
    LiteGraphTensorPtr expectPrimitive(nullptr, DestroyLiteGraphPrimitive);
    EXPECT_NE(primitive, expectPrimitive);
}

/**
 * @tc.name: squareddifference_build_001
 * @tc.desc: Provide normal input, output to verify the normal behavior of the Build function
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_build_001, TestSize.Level0)
{
    std::vector<uint32_t> inputsIndex = { 0, 1 };
    std::vector<uint32_t> outputsIndex = { 2 };
    std::vector<int32_t> inputDim = {3};
    std::vector<int32_t> OutputDim = {3};

    SaveInputTensor(inputsIndex, OH_NN_FLOAT32, inputDim, nullptr);
    SaveOutputTensor(outputsIndex, OH_NN_FLOAT32, OutputDim, nullptr);
    OH_NN_ReturnCode ret = m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_SUCCESS, ret);
}

/**
 * @tc.name: squareddifference_build_002
 * @tc.desc: Call Build func twice to verify the abnormal behavior of the Build function
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_build_002, TestSize.Level0)
{
    std::vector<uint32_t> inputsIndex = { 0, 1 };
    std::vector<uint32_t> outputsIndex = { 2 };
    std::vector<int32_t> inputDim = {3};
    std::vector<int32_t> OutputDim = {3};

    SaveInputTensor(inputsIndex, OH_NN_FLOAT32, inputDim, nullptr);
    SaveOutputTensor(outputsIndex, OH_NN_FLOAT32, OutputDim, nullptr);
    EXPECT_EQ(OH_NN_SUCCESS, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
    OH_NN_ReturnCode ret = m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_OPERATION_FORBIDDEN, ret);
}

/**
 * @tc.name: squareddifference_build_003
 * @tc.desc: rovide one more than normal input to verify the abnormal behavior of the Build function
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_build_003, TestSize.Level0)
{
    std::vector<uint32_t> inputsIndex = { 0, 1, 2 };
    std::vector<uint32_t> outputsIndex = { 3 };
    std::vector<int32_t> inputDim = {3};
    std::vector<int32_t> OutputDim = {3};

    SaveInputTensor(inputsIndex, OH_NN_FLOAT32, inputDim, nullptr);
    SaveOutputTensor(outputsIndex, OH_NN_FLOAT32, OutputDim, nullptr);
    OH_NN_ReturnCode ret = m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: squareddifference_build_004
 * @tc.desc: rovide one more than normal output to verify the abnormal behavior of the Build function
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_build_004, TestSize.Level0)
{
    std::vector<uint32_t> inputsIndex = { 0, 1 };
    std::vector<uint32_t> outputsIndex = { 2, 3 };
    std::vector<int32_t> inputDim = {3};
    std::vector<int32_t> OutputDim = {3};

    SaveInputTensor(inputsIndex, OH_NN_FLOAT32, inputDim, nullptr);
    SaveOutputTensor(outputsIndex, OH_NN_FLOAT32, OutputDim, nullptr);
    OH_NN_ReturnCode ret = m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: squareddifference_build_005
 * @tc.desc: Provide empty input, output, and parameters to verify the abnormal behavior of the Build function
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_build_005, TestSize.Level0)
{
    std::vector<uint32_t> inputsIndex = { 0, 1 };
    std::vector<uint32_t> outputsIndex = { 2 };
    std::vector<uint32_t> paramsIndex = {};
    OH_NN_ReturnCode ret = m_builder.Build(paramsIndex, inputsIndex, outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: squareddifference_build_006
 * @tc.desc: Provide empty output to verify the abnormal behavior of the Build function
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_build_006, TestSize.Level0)
{
    std::vector<uint32_t> inputsIndex = { 0, 1 };
    std::vector<uint32_t> outputsIndex = { 2 };
    std::vector<int32_t> inputDim = {3};

    SaveInputTensor(inputsIndex, OH_NN_FLOAT32, inputDim, nullptr);

    OH_NN_ReturnCode ret = m_builder.Build(m_paramsIndex, m_inputsIndex, outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: squareddifference_build_007
 * @tc.desc: Provide a param to verify the abnormal behavior of the Build function
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_build_007, TestSize.Level0)
{
    std::vector<uint32_t> inputsIndex = { 0, 1 };
    std::vector<uint32_t> outputsIndex = { 2 };
    std::vector<uint32_t> paramsIndex = { 4 };

    m_paramsIndex = paramsIndex;
    InitTensor(inputsIndex, outputsIndex);
    OH_NN_ReturnCode ret = m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors);
    EXPECT_EQ(OH_NN_INVALID_PARAMETER, ret);
}

/**
 * @tc.name: squareddifference_getprimitive_001
 * @tc.desc: Verify the GetPrimitive function return nullptr
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_getprimitive_001, TestSize.Level0)
{
    auto primitive = m_builder.GetPrimitive();
    LiteGraphPrimitvePtr expectPrimitive(nullptr, DestroyLiteGraphPrimitive);
    EXPECT_EQ(primitive, expectPrimitive);
}

/**
 * @tc.name: squareddifference_getprimitive_002
 * @tc.desc: Verify the normal return behavior of the getprimitive function
 * @tc.type: FUNC
 */
HWTEST_F(SquaredDifferenceBuilderTest, squareddifference_getprimitive_002, TestSize.Level0)
{
    std::vector<uint32_t> inputsIndex = { 0, 1 };
    std::vector<uint32_t> outputsIndex = { 2 };
    InitTensor(inputsIndex, outputsIndex);
    EXPECT_EQ(OH_NN_SUCCESS, m_builder.Build(m_paramsIndex, m_inputsIndex, m_outputsIndex, m_allTensors));
    CheckResult();
}
} // namespace UnitTest
} // namespace NeuralNetworkRuntime
} // namespace OHOS