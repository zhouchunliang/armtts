#include "textToMelEngine.hpp"
#include "logger.hpp"
#include "utilities.hpp"

#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include <tensorflow/lite/c/c_api.h>

#include <cstdio>
#include <string>
#include <cassert>
#include <iostream>
#include <thread>

namespace VARAGAVAN {
namespace ARMTTS {

void TextToMelEngine::printMels(const std::vector<float>& y) {
    CHECK(MAX_T * N_MELS == y.size(), "TextToMel: Incorrect input size.");
    std::cout << "Y(mels):" << std::endl;
    std::cout << '[' << std::endl;
    for (int i = 0; i < MAX_T; ++i) {
        std::cout << '[';
        for (int j = 0; j < N_MELS; ++j) {
            const int index = (i * N_MELS) + j;
            std::cout << y[index];
            if (j != N_MELS - 1) {
                std::cout << ", ";
            }
        }
        if (i != MAX_T - 1) {
            std::cout << "], " << std::endl;
        } else {
            std::cout << ']' << std::endl;
        }
    }
    std::cout << ']' << std::endl;
}

TextToMelEngine::TextToMelEngine(const std::string& model, bool gpuDelegate)
    : _model(tflite::FlatBufferModel::BuildFromFile(model.c_str()))
    , _resolver()
    // GPU Support
    // , _gpu_delegate_options_v2()
    // , _gpu_delegate(nullptr)
    , _builder(*_model, _resolver)
    , _interpreter(nullptr)
    , _text_input_ptr(nullptr)
    , _y_input_ptr(nullptr)
    , _prev_max_attentions_input_ptr(nullptr)
    , _y_output_ptr(nullptr)
    , _global_step_output_ptr(nullptr)
    , _max_attentions_output_ptr(nullptr)
    , _alignments_output_ptr(nullptr)
{
    Logger::getLogger().info("Initializing TextToMel engine...");
    CHECK(_model != nullptr, "TextToMel model failed to load");

    if (gpuDelegate) {
        // GPU support
        // // Initialize interpreter with GPU delegate
        // _gpu_delegate_options_v2 = TfLiteGpuDelegateOptionsV2Default();
        // _gpu_delegate = TfLiteGpuDelegateV2Create(&_gpu_delegate_options_v2);
        // _builder.AddDelegate(_gpu_delegate);
    } else {
        // Set the number of CPU threads to be used.
        const int cpuCount = std::thread::hardware_concurrency();
        if (0 != cpuCount) {
            _builder.SetNumThreads(cpuCount);
        }
    }

    _builder(&_interpreter);
    MINIMAL_CHECK(_interpreter != nullptr);
    // Allocate tensor buffers.
    MINIMAL_CHECK(_interpreter->AllocateTensors() == kTfLiteOk);
    initializeInputBuffers();
    initializeOutputBuffers();
}

bool TextToMelEngine::isInitialized() noexcept {
    Logger::getLogger().info("Checking TextToMel processing engine...");
    bool isInitialized = true;
    if (nullptr == _text_input_ptr ) {
        Logger::getLogger().error("_text_input_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_text_input_ptr is defined");
    }
    if (nullptr == _y_input_ptr) {
        Logger::getLogger().error("_y_input_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_y_input_ptr is defined");
    }
    if (nullptr == _prev_max_attentions_input_ptr) {
        Logger::getLogger().error("_prev_max_attentions_input_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_prev_max_attentions_input_ptr is defined");
    }
    if (nullptr == _y_output_ptr) {
        Logger::getLogger().error("_y_output_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_y_output_ptr is defined");
    }
    if (nullptr == _global_step_output_ptr) {
        Logger::getLogger().error("_global_step_output_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_global_step_output_ptr is defined");
    }
    if (nullptr == _max_attentions_output_ptr) {
        Logger::getLogger().error("_max_attentions_output_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_max_attentions_output_ptr is defined");
    }
    if (nullptr == _alignments_output_ptr) {
        Logger::getLogger().error("_alignments_output_ptr is not defined");
        isInitialized = false;
    } else {
        Logger::getLogger().info("_alignments_output_ptr is defined");
    }
    return isInitialized;
}

void TextToMelEngine::printDetails() noexcept {
    tflite::PrintInterpreterState(_interpreter.get());
}

void TextToMelEngine::initializeInputBuffers() noexcept {
    /**
     * Input               ID Name           Type            AllocType       Size (Bytes/MB) Shape      MemAddr-Offset
     * texts                0 Placeholder    kTfLiteInt32    kTfLiteArenaRw  720 / 0.00      [1,180]    [0, 720)
     * Y                    1 Placeholder_1  kTfLiteFloat32  kTfLiteArenaRw  67200 / 0.06    [1,210,80] [768, 67968)
     * prev_max_attentions  2 Placeholder_2  kTfLiteInt32    kTfLiteArenaRw  4 / 0.00        [1]        [67968, 67972)
     */
    _text_input_ptr = _interpreter->typed_input_tensor<int>(0);
    _y_input_ptr = _interpreter->typed_input_tensor<float>(1);
    _prev_max_attentions_input_ptr = _interpreter->typed_input_tensor<int>(2);
}

void TextToMelEngine::initializeOutputBuffers() noexcept {
    /**
     * Output           ID Name                      Type            AllocType       Size (Bytes/MB) Shape       MemAddr-Offset
     * global_step       3 gs/global_step            kTfLiteInt32    kTfLiteMmapRo   4 / 0.00        (null)      [96067508, 96067512)
     * Y              1329 Text2Mel/AudioDec/Sigmoid kTfLiteFloat32  kTfLiteArenaRw  67200 / 0.06    [1,210,80]  [68032, 135232)
     * max_attentions 1060 Text2Mel/Attention/ArgMax kTfLiteInt64    kTfLiteArenaRw  1680 / 0.00     [1,210]     [1726592, 1728272)
     * alignments     1062 Text2Mel/Attention/tra... kTfLiteFloat32  kTfLiteArenaRw  151200 / 0.14   [1,180,210] [1575360, 1726560)
     */
    _global_step_output_ptr = _interpreter->typed_output_tensor<int32_t>(0);
    _y_output_ptr = _interpreter->typed_output_tensor<float>(1);
    _max_attentions_output_ptr = _interpreter->typed_output_tensor<int64_t>(2);
    _alignments_output_ptr = _interpreter->typed_output_tensor<float>(3);
}

void TextToMelEngine::setPreviousY(const std::vector<float>& y, int globalStep) noexcept {
    assert(nullptr != _y_input_ptr);
    assert(MAX_T * N_MELS == y.size());
    assert(globalStep < MAX_T);
    for (int i = 0; i < N_MELS; ++i) {
        const int index = (N_MELS * globalStep) + i;
            _y_input_ptr[index] = y[index];
    }
}

void TextToMelEngine::setPreviousMaxAttention(int32_t maxAttention) noexcept {
    assert(nullptr != _max_attentions_output_ptr);
    *_prev_max_attentions_input_ptr = maxAttention;
}

void TextToMelEngine::setCurrentY(std::vector<float>& y, int globalStep) noexcept {
    assert(nullptr != _y_input_ptr);
    assert(MAX_T * N_MELS == y.size());
    assert(globalStep < MAX_T);
    for (int i = 0; i < N_MELS; ++i) {
        const int index = (N_MELS * globalStep) + i;
        y[index] = _y_output_ptr[index];
    }
}

void TextToMelEngine::setCurrentMaxAttention(int32_t& maxAttention, int globalStep) noexcept {
    assert(nullptr != _max_attentions_output_ptr);
    assert(globalStep < MAX_T);
    maxAttention = static_cast<int32_t>(_max_attentions_output_ptr[globalStep]);
}

void TextToMelEngine::setText(const std::vector<int> &text) {
    CHECK(text.size() <= MAX_N, "TextToMel: Text input too long.");
    for (int i = 0; i < text.size(); ++i) {
        _text_input_ptr[i] = text[i];
    }
    for (int i = text.size(); i < MAX_N; ++i) {
        _text_input_ptr[i] = 0;
    }
}

std::vector<float> TextToMelEngine::runInference(std::vector<int> text) {
    Logger::getLogger().info("Running TextToMel inference...");
    int32_t maxAttention = 0;
    std::vector<float> y(MAX_T * N_MELS, 0);

    // Prepare inputs
    setText(text);
    for (int i = 0; i < MAX_T; ++i) {
        setPreviousY(y, i);
    }
    setPreviousMaxAttention(0);

    // Run inference
    for (int i = 0; i < MAX_T; ++i) {
        CHECK(_interpreter->Invoke() == kTfLiteOk, "TextToMel: Inference failed.");

        // Get outputs
        setCurrentY(y, i);
        setCurrentMaxAttention(maxAttention, i);

        // Set inputs
        setPreviousY(y, i);
        setPreviousMaxAttention(maxAttention);
    }
    return y;
}

}}