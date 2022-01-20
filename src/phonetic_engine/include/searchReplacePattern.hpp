/**
 * @file searchReplacePattern.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::SearchReplacePattern
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

#include <regex>
#include <string>

namespace VARAGAVAN {
namespace ARMTTS {

/**
 * @brief Struct representing a search/replace pattern.
 **/
class SearchReplacePattern {
public:
    /**
     * @brief constructor.
     */
    SearchReplacePattern(const std::wstring& s, const std::wstring& r, int c);

    SearchReplacePattern(const SearchReplacePattern&) = default;

    /**
     * @brief check pattern match.
     * @param s - string to check
     * @return true in case of match, otherwise return false.
     */
    bool checkMatch(const std::wstring& s) const noexcept;

    /**
     * @brief Process a pattern.
     * @param s - string to process
     * @return the processed string.
     */
    std::wstring process(const std::wstring& s) const noexcept;

private:
    std::wregex _search;
    std::wstring _replace;
    int _count;
};

}}