#include "phoneticProcessingHiddenLetters.hpp"
#include "utilities.hpp"

#include <codecvt>
#include <regex>

namespace VARAGAVAN {
namespace ARMTTS {

// TODO: Resolve the recursion.
std::wstring PhoneticProcessingHiddenLetters::processHiddenLettersComplmentaryFunction(const std::wstring& s) const
    noexcept {
    static const std::wregex match(L"([բգդզթժլխծկհձղմյնչպջռսվտրցփքֆ])([բգդզթժլխծկհձղմյնչպջռսվտրցփքֆ])");
    static const std::wstring replace(L"$1ը$2");
    std::vector<std::wstring> syllables({});
    UTILITIES::setSyllables(s, syllables);
    std::wostringstream word(L"");
    for (const auto& syllable : syllables) {
        if (! UTILITIES::hasVowel(syllable)) {
            if (1 == syllable.length()) {
                word << syllable << L"ը";
            } else {
                word << std::regex_replace(syllable, match, replace);
            }
        } else {
            word << syllable;
        }
    }
    return word.str();
}


std::wstring PhoneticProcessingHiddenLetters::process(const std::wstring &s) const noexcept {
    // Excluding exception handling: return current if matches
    if (_excluding_pattern.checkMatch(s)) {
        return s;
    }
    // Exception case handling: apply one case and exit
    for (const auto& exception : _exceptions) {
        if (exception.checkMatch(s)) {
            return exception.process(s);
        }
    }
    // General pattern handling: apply all cases
    std::wstring tmp(s);
    for (const auto& pattern : _general_patterns) {
        tmp = pattern.process(tmp);
    }
    // Algorithmic hidden letters handling
    tmp = processHiddenLettersComplmentaryFunction(tmp);
    return tmp;
}

PhoneticProcessingHiddenLetters::PhoneticProcessingHiddenLetters(const nlohmann::json& j,
    const std::string& name) noexcept : PhoneticProcessingEngine(j, name)
{
}

}}
