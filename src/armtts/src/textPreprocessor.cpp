#include "textPreprocessor.hpp"
#include "utilities.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>

namespace VARAGAVAN {
namespace ARMTTS {

int TextPreprocessor::getIndex(wchar_t character) {
    static const std::vector<wchar_t> vocabulary = {
            L'P', L'E', L' ', L'ա', L'բ', L'գ', L'դ', L'զ', L'է', L'ը', L'թ',
            L'ժ', L'ի', L'լ', L'խ', L'ծ', L'կ', L'հ', L'ձ', L'ղ', L'ճ', L'մ',
            L'յ', L'ն', L'շ', L'չ', L'պ', L'ջ', L'ռ', L'ս', L'վ', L'տ', L'ր',
            L'ց', L'ւ', L'փ', L'ք', L'օ', L'ֆ', L',', L'՞', L'՜', L'՝'};
    auto i = std::find(vocabulary.begin(), vocabulary.end(), character);
    CHECK(i != vocabulary.end(), "Error: A character was not found in the vocabulary.");
    return std::distance(vocabulary.begin(), i);
}

std::vector<std::vector<int>> TextPreprocessor::preprocess(const std::wstring& text) noexcept {
    std::wstring t = _phoneticEngine.process(text) + L'E';
    const std::vector<std::wstring> textChunks = splitIntoChunks(t);
    std::vector<std::vector<int>> result;
    for (const auto& c : textChunks) {
        std::vector<int> processedText(c.size(), 0);
        for (int i = 0; i < c.size(); ++i) {
            processedText[i] = getIndex(c[i]);
        }
        result.push_back(std::move(processedText));
    }
    return result;
}

std::vector<std::wstring> TextPreprocessor::splitIntoChunks(const std::wstring& text) {
    std::vector<std::wstring> result;
    if (text.length() <= MAX_N) {
        result.push_back(text);
        return result;
    }
    std::wstring textToSplit = text;
    auto startReverseIterator = text.rend();
    while (std::distance(text.rbegin(), startReverseIterator) > MAX_N) {
        auto endIterator = std::find(startReverseIterator - MAX_N, text.rend(), L' ');
        int startPosition = std::distance(text.begin(), startReverseIterator.base());
        if (endIterator == text.rend()) {
            // Take the maximum if no space ' ', character was found.
            endIterator = startReverseIterator - MAX_N;
        }
        int length = std::distance(startReverseIterator.base(), endIterator.base());
        // Skip ending and leading spaces.
        if (text[startPosition + length - 1] == L' ') { --length; }
        if (text[startPosition] == L' ') { ++startPosition; }
        result.push_back(text.substr(startPosition, length));
        // Update the reverse end iterator
        startReverseIterator = endIterator;
    }
    // Add the remaining part.
    int startPosition = std::distance(text.begin(), startReverseIterator.base());
    int length = std::distance(startReverseIterator.base(), text.end());
    // Skip ending and leading spaces.
    if (text[startPosition + length - 1] == L' ') { --length; }
    if (text[startPosition] == L' ') { ++startPosition; }
    result.push_back(text.substr(startPosition, length));
    return result;
}

}
}