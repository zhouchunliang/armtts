/**
 * @file textToMelEngine.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::TextToMelEngine
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
#include <tensorflow/lite/delegates/gpu/delegate.h>

namespace VARAGAVAN {
namespace ARMTTS {

    class TextToMelEngine {

    public:
        /*
         * @brief The only available default constructor.
         * @param gpuDelegate - set true to enable GPU support (disabled by default)
         */
        TextToMelEngine(const std::string& model, bool gpuDelegate=false);

        /*
         * @brief Check initialization of inputs&outputs.
         */
        bool isInitialized() noexcept;

        /*
         * @brief Print model details.
         */
        void printDetails() noexcept;

        /*
         * @brief Run the inference and return the mel spectograms.
         * @param text - text to process
         * @return Process TextToMel and return Mel spectograms.
         */
        std::vector<float> runInference(std::vector<int> text);

        static void printMels(const std::vector<float>& y);

    private:
        void initializeInputBuffers() noexcept;

        void initializeOutputBuffers() noexcept;

        void setText(const std::vector<int>& text);

        void setCurrentY(std::vector<float>& y, int globalStep) noexcept;

        void setCurrentMaxAttention(int32_t& maxAttentions, int globalStep) noexcept;

        void setPreviousY(const std::vector<float>& y, int globalStep) noexcept;

        void setPreviousMaxAttention(int32_t maxAttention) noexcept;

    public:
        static constexpr int MAX_N = 180;
        static constexpr int N_MELS = 80;
        static constexpr int MAX_T = 210;
        static constexpr int Y_SIZE = MAX_T * N_MELS;

    private:
        std::unique_ptr<tflite::FlatBufferModel> _model;
        tflite::ops::builtin::BuiltinOpResolver _resolver;
        // GPU Support
        // TfLiteGpuDelegateOptionsV2 _gpu_delegate_options_v2;
        // TfLiteDelegate* _gpu_delegate;
        tflite::InterpreterBuilder _builder;
        std::unique_ptr<tflite::Interpreter> _interpreter;
        int32_t* _text_input_ptr;
        float* _y_input_ptr;
        int32_t* _prev_max_attentions_input_ptr;
        float* _y_output_ptr;
        int32_t* _global_step_output_ptr;
        int64_t* _max_attentions_output_ptr;
        float* _alignments_output_ptr;
    };

}
}
