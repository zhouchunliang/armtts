/**
 * @file phoneticEngine.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::PhoneticEngine
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
#include "phoneticProcessingHiddenLetters.hpp"

#include <json.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>


namespace VARAGAVAN {
namespace ARMTTS {


class PhoneticEngine {
public:
    /**
     * @brief Default constructor.
     **/
    PhoneticEngine();

    void toLoverCase(std::wstring& s) noexcept;

    std::wstring processInitialEngine(const std::wstring& s) noexcept;

    std::wstring processCaseSensitiveEngines(const std::wstring& s) noexcept;

    std::wstring processContextSensitiveEngines(const std::wstring& s) noexcept;

    std::wstring processEnginesWithMergedExceptions(const std::wstring& s) noexcept;

    std::wstring processEngines(const std::wstring& s) noexcept;

    std::wstring processHiddenLettersEngine(const std::wstring& s) noexcept;

    /**
     * @brief Process specified string.
     * @param s - string to process
     * @return processed string.
     **/
    std::wstring process(const std::wstring& s) noexcept;

private:

    void initializeEngines(const nlohmann::json& j) noexcept;

    void loadConfiguration() noexcept;

private:
    PhoneticProcessingEngine _initial_engine;
    PhoneticProcessingHiddenLetters _hidden_letters_engine;
    std::vector<PhoneticProcessingEngine> _case_sensitive_engines;
    std::vector<PhoneticProcessingEngine> _context_sensitive_engines;
    std::vector<PhoneticProcessingEngine> _engines_with_merged_exceptions;
    std::vector<PhoneticProcessingEngine> _engines;
};

}
}
