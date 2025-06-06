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

#ifndef NEURAL_NETWORK_RUNTIME_CPP_API_TYPE_H
#define NEURAL_NETWORK_RUNTIME_CPP_API_TYPE_H

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "neural_network_runtime/neural_network_runtime_type.h"

namespace OHOS {
namespace NeuralNetworkRuntime {
// ALLOCATE_BUFFER_LIMIT is 1 Gb
const size_t ALLOCATE_BUFFER_LIMIT = 1024 * 1024 * 1024;
enum DeviceStatus: int {
    UNKNOWN,
    AVAILABLE,
    BUSY,
    OFFLINE
};

enum class TuningStrategy {
    OFF = 0,
    ON_DEVICE_TUNING,
    ON_DEVICE_PREPROCESS_TUNING,
    ON_CLOUD_TUNING
};

struct Buffer {
    void* data = nullptr;
    size_t length = 0;
    int fd = -1;
};

struct ExtensionConfig {
    Buffer quantBuffer;
    std::string modelName;
    std::string isProfiling;
    std::map<std::string, std::string> opLayout;
    TuningStrategy tuningStrategy{TuningStrategy::OFF};
    // inputDims and dynamicDims are used in hiai adapter
    std::vector<std::vector<int32_t>> inputDims;
    std::vector<std::vector<int32_t>> dynamicDims;
    bool isNpuFmShared = false;
    bool isExceedRamLimit = false;
};

struct ModelConfig {
    bool enableFloat16;
    OH_NN_PerformanceMode mode;
    OH_NN_Priority priority;
    std::string cachePath;
    ExtensionConfig extensionConfig;
};

struct QuantParam {
    uint32_t numBits;
    double scale;
    int32_t zeroPoint;
};

struct IOTensor {
    std::string name;
    OH_NN_DataType dataType;
    OH_NN_Format format;
    std::vector<int> dimensions;
    void* data;
    size_t length;
};
} // NeuralNetworkRuntime
} // OHOS

#endif // NEURAL_NETWORK_RUNTIME_CPP_API_TYPE_H