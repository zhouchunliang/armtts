/**
 * @file phoneticProcessingEngine.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::PhoneticProcessingEngine
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

#include "searchPattern.hpp"
#include "searchReplacePattern.hpp"

#include <json.hpp>

namespace VARAGAVAN {
namespace ARMTTS {

class PhoneticProcessingEngine {

public:
    /**
     * @brief Default constructor.
     **/
    PhoneticProcessingEngine() = default;

    /**
     * @brief Constructor.
     **/
    PhoneticProcessingEngine(const nlohmann::json& j,
                             const std::string& name) noexcept;

    /**
     * @brief Process the engine applying specified excludong/general patterns and exceptions.
     * @param s - string to process
     * @return the processed string.
     */
    std::wstring process(const std::wstring& s) const noexcept;

    friend std::ostream& operator<<(std::ostream& o, const PhoneticProcessingEngine& p) noexcept;

    /**
     * @brief get name.
     * @return name.
     **/
    const std::string& getName() const noexcept;

    /**
     * @brief get excluding pattern.
     * @return excluding pattern.
     **/
    const SearchPattern& getExludingPattern() const noexcept;

    /**
     * @brief get exceptions.
     * @return exceptions.
     **/
    const std::vector<SearchReplacePattern>& getExceptions() const noexcept;

    /**
     * @brief get general patterns.
     * @return general patterns.
     **/
    const std::vector<SearchReplacePattern>& getGeneralPatterns() const noexcept;

private:
    void parseExcludingPattern(const nlohmann::json& j) noexcept;

    void parseExceptions(const nlohmann::json& j) noexcept;

    void parseGeneralPatterns(const nlohmann::json& j) noexcept;

private:
    std::string _name;
    SearchPattern _excluding_pattern;
    std::vector<SearchReplacePattern> _general_patterns;
    std::vector<SearchReplacePattern> _exceptions;

    friend class PhoneticProcessingHiddenLetters;
};

}}