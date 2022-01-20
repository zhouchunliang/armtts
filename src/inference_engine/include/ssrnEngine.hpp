/**
 * @file ssrnEngine.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::SSRNEngine
 *
 *  Author: Albert Grigoryan
 *  E-mail: albert_grigoryan@yahoo.com
 *  109/26 Vardanants St.,
 *  Yerevan, 0050, Armenia
 *  Tel:  +374-93-635-380
 *
 *  Copyright © 2022 Albert Grigoryan. All rights reserved.
 */

#pragma once

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include <tensorflow/lite/c/c_api.h>

namespace VARAGAVAN {
namespace ARMTTS {

class SSRNEngine {
    friend class InferenceEngineTests_runInference_Test;

public:
    /*
     * @brief The only available default constructor.
     * @param model - SSRN TFL model.
     */
    SSRNEngine(const std::string& model);

    /*
     * @brief Check initialization of inputs&outputs.
     */
    bool isInitialized() noexcept;

    /*
     * @brief Print model details.
     */
    void printDetails() noexcept;

    /*
     * @brief Run the inferrence and return the mel spectograms.
     * @param y - mel spectograms comming from TextToMel engine.
     * @return Process SSRN and return spectrogramm predictions.
     */
    std::vector<std::vector<float>> runInference(std::vector<float> y);

    static void printPredictions(const std::vector<std::vector<float>>& z);

private:
    void initializeInputBuffers() noexcept;

    void initializeOutputBuffers() noexcept;

    void setY(std::vector<float>& y) noexcept;

    void setZ(std::vector<std::vector<float>>& z) noexcept;

public:
    static constexpr int Z_ROWS = 840;
    static constexpr int Z_COLUMNS = 1025;
    static constexpr int MAX_T = 210;
    static constexpr int N_MELS = 80;

private:
    std::unique_ptr<tflite::FlatBufferModel> _model;
    tflite::ops::builtin::BuiltinOpResolver _resolver;
    tflite::InterpreterBuilder _builder;
    std::unique_ptr<tflite::Interpreter> _interpreter;
    float* _y_input_ptr;
    float* _z_output_ptr;
};

}
}
