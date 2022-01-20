/**
 * @file engine.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::Engine
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

#include "inferenceEngine.hpp"
#include "textPreprocessor.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

namespace VARAGAVAN {
namespace ARMTTS {

const std::string TEXT_TO_MEL_MODEL = "models/TextToMel.tflite";
const std::string SSRN_MODEL = "models/SSRN.tflite";
const std::string RESULT_FILE = "result/result.wav";

class Engine {
public:
    /**
     * @brief Default constructor.
     **/
    Engine();

    /**
     * @brief Process specified string.
     * @param s - text to process
     * @return vector of generated wav files.
     **/
    std::vector<std::string> process(const std::wstring& s) noexcept;

private:
    std::string convertMelToWave(const std::vector<std::vector<float>>& mels) noexcept;

private:
    InferenceEngine _inferenceEngine;
    TextPreprocessor _textPreprocessor;

};

}
}
