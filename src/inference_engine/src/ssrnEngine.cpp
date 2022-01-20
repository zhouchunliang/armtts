#include "ssrnEngine.hpp"
#include "logger.hpp"
#include "utilities.hpp"

#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include <tensorflow/lite/c/c_api.h>

#include <cstdio>
#include <string>
#include <cassert>
#include <iostream>

namespace VARAGAVAN {
namespace ARMTTS {

void SSRNEngine::printPredictions(const std::vector<std::vector<float>> &z) {
    CHECK(Z_ROWS == z.size() && Z_COLUMNS == z.front().size(), "SSRN: Incorrect input size.");
    std::cout << "Z(mel predictions):" << std::endl;
    std::cout << '[' << std::endl;
    for (int i = 0; i < Z_ROWS; ++i) {
        std::cout << '[' << std::endl;
        for (int j = 0; j < Z_COLUMNS; ++j) {
            std::cout << z[i][j];
            if (j != Z_COLUMNS - 1) {
                std::cout << ", ";
            }
        }
        if (i == Z_ROWS - 1) {
            std::cout << "]" << std::endl;
        } else {
            std::cout << "]," << std::endl;
        }
    }
    std::cout << ']' << std::endl;
}

SSRNEngine::SSRNEngine(const std::string& model)
    : _model(tflite::FlatBufferModel::BuildFromFile(model.c_str()))
    , _resolver()
    , _builder(*_model, _resolver)
    , _interpreter(nullptr)
    , _y_input_ptr(nullptr)
    , _z_output_ptr(nullptr)
{
    Logger::getLogger().info("Initializing SSRN engine...");
    CHECK(_model != nullptr, "SSRN model failed to load");
    _builder(&_interpreter);
    MINIMAL_CHECK(_interpreter != nullptr);
    // Allocate tensor buffers.
    MINIMAL_CHECK(_interpreter->AllocateTensors() == kTfLiteOk);
    initializeInputBuffers();
    initializeOutputBuffers();
}

bool SSRNEngine::isInitialized() noexcept {
    Logger::getLogger().info("Checking SSRN engine...");
    bool isInitialized = true;
    if (nullptr == _y_input_ptr ) {
        Logger::getLogger().error("_y_input_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_y_input_ptr is defined");
    }
    if (nullptr == _z_output_ptr) {
        Logger::getLogger().error("_z_output_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_z_output_ptr is defined");
    }
    return isInitialized;
}

void SSRNEngine::printDetails() noexcept {
    tflite::PrintInterpreterState(_interpreter.get());
}

void SSRNEngine::initializeInputBuffers() noexcept {
    /**
     * Input ID  Name                       Type            AllocType       Size (Bytes/MB)  Shape       MemAddr-Offset
     * y     0   Text2Mel/AudioDec/Sigmoid  kTfLiteFloat32  kTfLiteArenaRw  67200 / 0.06     [1,210,80]  [0, 67200)
     */
    _y_input_ptr = _interpreter->typed_input_tensor<float>(0);
}

void SSRNEngine::initializeOutputBuffers() noexcept {
    /**
     * Output  ID  Name          Type            AllocType       Size (Bytes/MB) Shape         MemAddr-Offset
     * Z       466 SSRN/Sigmoid  kTfLiteFloat32  kTfLiteArenaRw  3444000 / 3.28  [1,840,1025]  [-1, -1)
     */
    _z_output_ptr = _interpreter->typed_output_tensor<float>(0);
}

void SSRNEngine::setY(std::vector<float>& y) noexcept {
    assert(nullptr != _y_input_ptr);
    assert(MAX_T * N_MELS == y.size());
    for (int i = 0; i < MAX_T; ++i) {
        for (int j = 0; j < N_MELS; ++j) {
            const int index = (N_MELS * i) + j;
            _y_input_ptr[index] = y[index];
        }
    }
}

void SSRNEngine::setZ(std::vector<std::vector<float>>& z) noexcept {
    assert(nullptr != _z_output_ptr);
    assert(Z_ROWS == z.size());
    assert(Z_COLUMNS == z.front().size());
    for (int i = 0; i < Z_ROWS; ++i) {
        for (int j = 0; j < Z_COLUMNS; ++j) {
            const int index = (i * Z_COLUMNS) + j;
            z[i][j] = _z_output_ptr[index];
        }
    }
}

std::vector<std::vector<float>> SSRNEngine::runInference(std::vector<float> y) {
    Logger::getLogger().info("Running SSRN inference...");
    // Prepare inputs
    setY(y);

    // Run inference
    CHECK(_interpreter->Invoke() == kTfLiteOk, "SSRN: Inference failed.");

    // Get outputs
    std::vector<std::vector<float>> z(Z_ROWS, std::vector<float>(Z_COLUMNS));
    setZ(z);

    return z;
}

}}