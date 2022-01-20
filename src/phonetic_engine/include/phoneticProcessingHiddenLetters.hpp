/**
 * @file phoneticProcessingHiddenLetters.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::PhoneticProcessingHiddenLetters
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

#include "phoneticProcessingEngine.hpp"

#include <json.hpp>

namespace VARAGAVAN {
namespace ARMTTS {

class PhoneticProcessingHiddenLetters : public PhoneticProcessingEngine {

public:
    /**
     * @brief Default constructor.
     **/
    PhoneticProcessingHiddenLetters() = default;

    /**
     * @brief Constructor.
     **/
    PhoneticProcessingHiddenLetters(const nlohmann::json& j, const std::string& name) noexcept;

    /**
     * @brief Process the engine applying specified excludong/general patterns and exceptions.
     * @param s - string to process
     * @return the processed string.
     */
    std::wstring process(const std::wstring& s) const noexcept;

    std::wstring processHiddenLettersComplmentaryFunction(const std::wstring& s) const noexcept;
};

}}