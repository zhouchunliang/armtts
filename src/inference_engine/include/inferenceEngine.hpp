/**
 * @file inferenceEngine.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::InferenceEngine
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

#include "textToMelEngine.hpp"
#include "ssrnEngine.hpp"

namespace VARAGAVAN {
namespace ARMTTS {

class InferenceEngine {

public:
    /*
     * @brief The only available default constructor.
     * @param textToMelModel - TFL TextToMel model.
     * @param ssrnModel - TFL SSRN model.
     */
    InferenceEngine(const std::string& textToMelModel, const std::string& ssrnModel);

    /*
     * @brief Check initialization of SSRN and TextToMel models..
     */
    bool isInitialized() noexcept;

    /*
     * @brief Run the inference and return the mel spectrogram.
     * @param text - preprocessed text.
     * @return Process TextToMel and SSRN and return mel spectrogram.
     */
    std::vector<std::vector<float>> runInference(const std::vector<int>& text);

private:
    TextToMelEngine _textToMelEngine;
    SSRNEngine _ssrnEngine;
};

}}