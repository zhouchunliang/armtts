/**
 * @file textPreprocessor.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::TextPreprocessor
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

#include "phoneticEngine.hpp"

#include <vector>

namespace VARAGAVAN {
namespace ARMTTS {

class TextPreprocessor {
public:
    /*
     * @brief The only available default constructor.
     */
    TextPreprocessor() = default;

    /*
     * @brief Preprocess the text and return encoded chunks with supported vocabulary.
     * @param text - text to preprocess
     * @return encoded chunks ready for TextToMel engine.
     */
    std::vector<std::vector<int>> preprocess(const std::wstring& text) noexcept;

    /*
     * @brief Find the character in the vocabulary and return the index, otherwise throw an error.
     * @param c - character to look up
     * @return index.
     */
    int getIndex(wchar_t character);

    /*
     * @brief Split the text with smaller MAX_N, chunks.
     * @param text - text to split.
     * @return Split text chunks.
     */
    std::vector<std::wstring> splitIntoChunks(const std::wstring& text);

private:
    static constexpr int MAX_N = 180;
    PhoneticEngine _phoneticEngine;

};

}}