#include "inferenceEngine.hpp"
#include "logger.hpp"


namespace VARAGAVAN {
namespace ARMTTS {

InferenceEngine::InferenceEngine(const std::string& textToMelModel, const std::string& ssrnModel)
    : _textToMelEngine(textToMelModel)
    , _ssrnEngine(ssrnModel)
{
}

bool InferenceEngine::isInitialized() noexcept {
    return _textToMelEngine.isInitialized() && _ssrnEngine.isInitialized();
}


std::vector<std::vector<float>> InferenceEngine::runInference(const std::vector<int>& text) {
    Logger::getLogger().info("Running inference...");
    const std::vector<float>& y = _textToMelEngine.runInference(text);
    const std::vector<std::vector<float>> melPredictions = _ssrnEngine.runInference(y);
    return melPredictions;
}


}}