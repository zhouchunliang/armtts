#include "phoneticEngine.hpp"
#include "utilities.hpp"
#include "logger.hpp"

#include <vector>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>

namespace VARAGAVAN {
namespace ARMTTS {


PhoneticEngine::PhoneticEngine() {
    loadConfiguration();
    std::setlocale(LC_ALL, "hy_AM.UTF-8");
}

void PhoneticEngine::toLoverCase(std::wstring& s) noexcept {
    const static std::map<wchar_t, wchar_t> letters = {
            {L'Ա', L'ա'},
            {L'Բ', L'բ'},
            {L'Գ', L'գ'},
            {L'Դ', L'դ'},
            {L'Ե', L'ե'},
            {L'Զ', L'զ'},
            {L'Է', L'է'},
            {L'Ը', L'ը'},
            {L'Թ', L'թ'},
            {L'Ժ', L'ժ'},
            {L'Ի', L'ի'},
            {L'Լ', L'լ'},
            {L'Խ', L'խ'},
            {L'Ծ', L'ծ'},
            {L'Կ', L'կ'},
            {L'Հ', L'հ'},
            {L'Ձ', L'ձ'},
            {L'Ղ', L'ղ'},
            {L'Ճ', L'ճ'},
            {L'Մ', L'մ'},
            {L'Յ', L'յ'},
            {L'Ն', L'ն'},
            {L'Շ', L'շ'},
            {L'Ո', L'ո'},
            {L'Չ', L'չ'},
            {L'Պ', L'պ'},
            {L'Ջ', L'ջ'},
            {L'Ռ', L'ռ'},
            {L'Ս', L'ս'},
            {L'Վ', L'վ'},
            {L'Տ', L'տ'},
            {L'Ր', L'ր'},
            {L'Ց', L'ց'},
            {L'Ւ', L'ւ'},
            {L'Փ', L'փ'},
            {L'Ք', L'ք'},
            {L'Օ', L'օ'},
            {L'Ֆ', L'ֆ'}
    };
    for (auto& c : s) {
        auto it = letters.find(c);
        if (it != letters.end()) {
            c = it->second;
        }
    }
}

std::wstring PhoneticEngine::processInitialEngine(const std::wstring& s) noexcept {
    std::vector<std::wstring> words = UTILITIES::splitStringIntoWords(s);
    std::wostringstream result(L"");
    for (auto& word : words) {
        result << _initial_engine.process(word) << L' ';
    }
    const std::wstring& ws = result.str();
    return ws.substr(0, ws.length() - 1);
}

std::wstring PhoneticEngine::processCaseSensitiveEngines(const std::wstring &s) noexcept {
    std::vector<std::wstring> words = UTILITIES::splitStringIntoWords(s);
    std::wostringstream result(L"");
    for (auto& word : words) {
        for (const auto& engine : _case_sensitive_engines) {
            word = engine.process(word);
        }
        result << word << L' ';
    }
    const std::wstring& ws = result.str();
    return ws.substr(0, ws.length() - 1);
}

std::wstring PhoneticEngine::processContextSensitiveEngines(const std::wstring &s) noexcept {
    std::wstring result = s;
    for(auto& engine: _context_sensitive_engines) {
        result = engine.process(result);
    }
    return result;
}

std::wstring PhoneticEngine::processEnginesWithMergedExceptions(const std::wstring &s) noexcept {
    std::vector<std::wstring> words = UTILITIES::splitStringIntoWords(s);
    std::wostringstream result(L"");
    for (auto& word : words) {
        std::vector<std::wstring> diff = UTILITIES::getDiff(word, word);
        for (const auto& engine : _engines_with_merged_exceptions) {
            std::wstring processedWord = engine.process(word);
            const auto& currentDiff = UTILITIES::getDiff(word, processedWord);
            diff = UTILITIES::mergeDiffs(diff, currentDiff);
        }
        result << UTILITIES::applyDiff(word, diff) << L' ';
    }
    const std::wstring& ws = result.str();
    return ws.substr(0, ws.length() - 1);
}

std::wstring PhoneticEngine::processEngines(const std::wstring &s) noexcept {
    std::vector<std::wstring> words = UTILITIES::splitStringIntoWords(s);
    std::wostringstream result(L"");
    for (auto& word : words) {
        for (const auto& engine : _engines) {
            word = engine.process(word);
        }
        result << word << L' ';
    }
    const std::wstring& ws = result.str();
    return ws.substr(0, ws.length() - 1);
}

std::wstring PhoneticEngine::processHiddenLettersEngine(const std::wstring& s) noexcept {
    std::vector<std::wstring> words = UTILITIES::splitStringIntoWords(s);
    std::wostringstream result(L"");
    for (auto& word : words) {
        result << _hidden_letters_engine.process(word) << L' ';
    }
    const std::wstring& ws = result.str();
    return ws.substr(0, ws.length() - 1);
}

std::wstring PhoneticEngine::process(const std::wstring& s) noexcept {
    std::wstring result = UTILITIES::removeDoubleSpaces(s);

    // Add a full stop if needed
    if (std::iswalpha(result.back())) {
        result += L'։';
    }

    // Remove non-supported characters.
    SearchReplacePattern p1(L"[^PE աբգդեզէըթժիլխծկհձղճմյնշոչպջռսվտրցւփքօֆևԱԲԳԴԵԶԷԸԹԺԻԼԽԾԿՀՁՂՃՄՅՆՇՈՉՊՋՌՍՎՏՐՑՒՓՔՕՖ\
    ։:;,.՝՞՜՝0123456789₽؋€£د.ج$֏ƒدب৳₹៛¥元]", L"", 0);
    result = p1.process(result);

    // Simplify punctuation marks.
    SearchReplacePattern p2(L"[։;:.]", L",", 0);
    result = p2.process(result);

    //  Process configuration engines
    result = processInitialEngine(result);
    result = processCaseSensitiveEngines(result);
    toLoverCase(result); // lover case
    result = processContextSensitiveEngines(result);
    result = processEnginesWithMergedExceptions(result);
    result = processEngines(result);
    result = processHiddenLettersEngine(result);

    return result;
}

void PhoneticEngine::initializeEngines(const nlohmann::json& j) noexcept {
    Logger::getLogger().info("Initializing phonetic engine...");
    std::string initial = "initial";
    std::string hidden_letters = "hidden_letters";
    std::string case_sensitive_engines = "case_sensitive_engines";
    std::string context_sensitive_engines = "context_sensitive_engines";
    std::string engines_with_merged_exceptions = "engines_with_merged_exceptions";
    std::string engines = "engines";

    // Check fields
    UTILITIES::checkFieldExistence(j, initial);
    UTILITIES::checkFieldExistence(j, hidden_letters);
    UTILITIES::checkFieldExistence(j, case_sensitive_engines);
    UTILITIES::checkFieldExistence(j, context_sensitive_engines);
    UTILITIES::checkFieldExistence(j, engines_with_merged_exceptions);
    UTILITIES::checkFieldExistence(j, engines);

    // Initialize
     _initial_engine = PhoneticProcessingEngine(j[initial], initial);
     _hidden_letters_engine = PhoneticProcessingHiddenLetters(j[hidden_letters], hidden_letters);
     for (const auto& c : j[case_sensitive_engines]) {
        const std::string engine_case = c;
        UTILITIES::checkFieldExistence(j, engine_case);
        _case_sensitive_engines.emplace_back(j[engine_case], engine_case);
     }
     for (const auto& c : j[context_sensitive_engines]) {
        const std::string engine_case = c;
        UTILITIES::checkFieldExistence(j, engine_case);
        _context_sensitive_engines.emplace_back(j[engine_case], engine_case);
     }
     for (const auto& c : j[engines_with_merged_exceptions]) {
        const std::string engine_case = c;
        UTILITIES::checkFieldExistence(j, engine_case);
        _engines_with_merged_exceptions.emplace_back(j[engine_case], engine_case);
    }
     for (const auto& c : j[engines]) {
        const std::string engine_case = c;
        UTILITIES::checkFieldExistence(j, engine_case);
        _engines.emplace_back(j[engine_case], engine_case);
    }
}

void PhoneticEngine::loadConfiguration() noexcept {
    Logger::getLogger().info("Loading phonetic engine configuration...");
    const std::string cfg_file = "cfg/phonetic_processing_configuration.json";
    std::ifstream f(cfg_file);
    nlohmann::json json_object = nlohmann::json::parse(f);
    initializeEngines(json_object);
}

}
}
