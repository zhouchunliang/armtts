#include "searchPattern.hpp"

#include <regex>
#include <string>
#include <cassert>

namespace VARAGAVAN {
namespace ARMTTS {

bool SearchPattern::checkMatch(const std::wstring& s) const noexcept {
    return std::regex_search(s, _search);
}

SearchPattern::SearchPattern(const std::wstring& s)
    : _search(s)
{
    assert(! s.empty());
}

}}