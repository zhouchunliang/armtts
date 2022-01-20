#include "phoneticProcessingEngine.hpp"
#include "utilities.hpp"
#include "logger.hpp"

#include <codecvt>
#include <regex>
#include <cassert>

namespace VARAGAVAN {
namespace ARMTTS {

const std::string& PhoneticProcessingEngine::getName() const noexcept {
    return _name;
}

const SearchPattern&
PhoneticProcessingEngine::getExludingPattern() const noexcept {
    return _excluding_pattern;
}

const std::vector<SearchReplacePattern>&
PhoneticProcessingEngine::getExceptions() const noexcept {
    return _exceptions;
}

const std::vector<SearchReplacePattern>&
PhoneticProcessingEngine::getGeneralPatterns() const noexcept {
    return _general_patterns;
}

std::wstring PhoneticProcessingEngine::process(const std::wstring &s) const noexcept {
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
    return tmp;
}

void PhoneticProcessingEngine::
parseExcludingPattern(const nlohmann::json& j) noexcept {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    const std::string& s = j;
    const std::wstring ws = converter.from_bytes(s.data());
    _excluding_pattern = SearchPattern(ws);
}

void PhoneticProcessingEngine::
parseExceptions(const nlohmann::json& j) noexcept {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    for (const auto& e : j) {
        const std::string& s = e[0];
        const std::string& r = e[1];
        const std::wstring ws = converter.from_bytes(s.data());
        const std::wstring wr = converter.from_bytes(r.data());
        _exceptions.emplace_back(ws, wr, e[2]);
    }
}

void PhoneticProcessingEngine::
parseGeneralPatterns(const nlohmann::json& j) noexcept {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    for (const auto& p : j) {
        const std::string& s = p[0];
        const std::string& r = p[1];
        const std::wstring ws = converter.from_bytes(s.data());
        const std::wstring wr = converter.from_bytes(r.data());
        _general_patterns.emplace_back(ws, wr, p[2]);
    }
}

std::ostream&
operator<<(std::ostream& o, const PhoneticProcessingEngine& p) noexcept {
    o << "Engine: " << p.getName() << std::endl;
    if (!p.getExceptions().empty()) {
        o << "Exceptions: " << p.getExceptions().size() << std::endl;
    }
    if (!p.getGeneralPatterns().empty()) {
        o << "General Patterns: " << p.getGeneralPatterns().size() << std::endl;
    }
    o << std::endl;
    return o;
}

PhoneticProcessingEngine::PhoneticProcessingEngine(const nlohmann::json& j,
                                                   const std::string& name) noexcept : _name(name) {
    assert(! name.empty());
    Logger::getLogger().debug("Creating phonetic engine: %s", name.c_str());
    const std::string exclude = "excluding_pattern";
    const std::string exceptions = "exceptions";
    const std::string general_patterns = "general_patterns";
    // Check fields
    UTILITIES::checkFieldExistence(j, exclude);
    UTILITIES::checkFieldExistence(j, exceptions);
    UTILITIES::checkFieldExistence(j, general_patterns);
    // Parse patterns
    parseExcludingPattern(j[exclude]);
    parseExceptions(j[exceptions]);
    parseGeneralPatterns(j[general_patterns]);
}

}}
