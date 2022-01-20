#include "searchReplacePattern.hpp"

#include <regex>
#include <string>
#include <cassert>

namespace VARAGAVAN {
namespace ARMTTS {

bool SearchReplacePattern::checkMatch(const std::wstring& s) const noexcept {
    return std::regex_search(s, _search, std::regex_constants::match_any);
}

std::wstring SearchReplacePattern::process(const std::wstring& s) const noexcept {
    if (0 == _count) {
        return std::regex_replace(s, _search, _replace, std::regex_constants::match_any);
    }
    int matchesCount = 0;
    std::wstring previousResult = s;
    std::wstring currentResult(L"");
    while (matchesCount < _count) {
        currentResult = std::regex_replace(previousResult, _search, _replace, std::regex_constants::format_first_only);
        if (currentResult == previousResult) {
            break;
        }
        previousResult = currentResult;
        ++matchesCount;
    }
    return currentResult;
}

SearchReplacePattern::SearchReplacePattern(const std::wstring& s,
        const std::wstring& r, int c)
        : _search(s)
        , _replace(
                std::regex_replace(r,
                                   std::wregex(L"\\\\([0-9]+)"),
                                   L"$$$1",
                                   std::regex_constants::match_any))
        , _count(c)
{
    assert(! s.empty());
}

}}