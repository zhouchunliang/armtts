#include "engine.hpp"
#include "utilities.hpp"

#include <libgen.h>
#include <boost/filesystem.hpp>

#include <sys/stat.h>
#include <cmath>
#include <vector>

namespace VARAGAVAN {
namespace ARMTTS {

Engine::Engine()
    : _inferenceEngine(TEXT_TO_MEL_MODEL, SSRN_MODEL)
    , _textPreprocessor()
{
}

// TODO: create files in the results/tmp directory.
std::string Engine::convertMelToWave(const std::vector<std::vector<float>>& mels) noexcept {
    const int max_db = 100;
    const int ref_db = 20;
    const int sample_rate = 22050;
    std::vector<std::vector<float>> tmp = mels;
    for (auto& i : tmp) {
        std::for_each(i.begin(), i.end(), [](float& i){
            // Denoramlize
            i = (std::min<float>(std::max<float>(0.0f, i), 1.0f) * max_db) - max_db + ref_db;
            // Amplitude
            i = pow(10, i * 0.05);
            i = pow(i, 1.5);
        });
    }
    std::vector<std::vector<std::complex<float>>> c(tmp.size(), std::vector<std::complex<float>>(tmp[0].size()));
    for (int i = 0; i < tmp.size(); ++i) {
        for (int j = 0; j < tmp[0].size(); ++j) {
            c[i][j] = std::complex<float>(tmp[i][j], 0);
        }
    }
    std::vector<float> r = UTILITIES::griffinLim(c);

    boost::filesystem::path temp = boost::filesystem::unique_path();
    const std::string resultFileName =
        std::filesystem::temp_directory_path().generic_string() + temp.native() + ".wav";
    UTILITIES::writeWavFile(resultFileName, r, sample_rate);
    return resultFileName;
}

std::vector<std::string> Engine::process(const std::wstring& s) noexcept {
    const std::vector<std::vector<int>> texts = _textPreprocessor.preprocess(s);
    std::vector<std::string> results;
    results.reserve(texts.size());
    for (const auto& t : texts) {
        const std::vector<std::vector<float>> mel = _inferenceEngine.runInference(t);
        results.push_back(convertMelToWave(mel));
    }
    return results;
}


}}
