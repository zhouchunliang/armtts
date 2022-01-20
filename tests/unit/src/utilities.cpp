#include "utilities.hpp"

#include <gtest/gtest.h>

#include <string>
#include <filesystem>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

// Unit Tests

TEST(UtilitiesTests, splitStringIntoWords) {
    std::vector<std::wstring> result = UTILITIES::splitStringIntoWords(L"աա բբ գգ");
    ASSERT_EQ(3, result.size());
    ASSERT_STREQ(L"աա", result.at(0).c_str());
    ASSERT_STREQ(L"բբ", result.at(1).c_str());
    ASSERT_STREQ(L"գգ", result.at(2).c_str());
}

TEST(UtilitiesTests, join) {
    std::vector<std::wstring> w = {L"աա", L"բբ", L"գգ"};
    std::wstring result = UTILITIES::join(w);
    ASSERT_STREQ(L"աա բբ գգ", result.c_str());
}

TEST(UtilitiesTests, removeDoubleSpaces) {
    std::wstring t(L"աա  բբ     գ");
    std::wstring result = UTILITIES::removeDoubleSpaces(t);
    ASSERT_STREQ(L"աա բբ գ", result.c_str());
}

TEST(UtilitiesTests, getDiff) {
    // case 1
    const std::vector<std::wstring>& result1 = UTILITIES::getDiff(L"աբգ", L"աբգ");
    ASSERT_EQ(3, result1.size());
    ASSERT_STREQ(L"", result1.at(0).c_str());
    ASSERT_STREQ(L"", result1.at(1).c_str());
    ASSERT_STREQ(L"", result1.at(2).c_str());
    // case 2
    const std::vector<std::wstring>& result2 = UTILITIES::getDiff(L"աբգ", L"աբդ");
    ASSERT_EQ(3, result2.size());
    ASSERT_STREQ(L"", result2.at(0).c_str());
    ASSERT_STREQ(L"", result2.at(1).c_str());
    ASSERT_STREQ(L"դ", result2.at(2).c_str());
    // case 3
    const std::vector<std::wstring>& result3 = UTILITIES::getDiff(L"աբգ", L"աբ");
    ASSERT_EQ(3, result3.size());
    ASSERT_STREQ(L"", result3.at(0).c_str());
    ASSERT_STREQ(L"", result3.at(1).c_str());
    ASSERT_STREQ(L"-", result3.at(2).c_str());
    // case 4
    const std::vector<std::wstring>& result4 = UTILITIES::getDiff(L"աբգ", L"աբդգ");
    ASSERT_EQ(3, result4.size());
    ASSERT_STREQ(L"", result4.at(0).c_str());
    ASSERT_STREQ(L"բդ", result4.at(1).c_str());
    ASSERT_STREQ(L"", result4.at(2).c_str());
}

TEST(UtilitiesTests, mergeDiffs) {
    std::vector<std::wstring> diff1 = {L"ա", L"", L""};
    std::vector<std::wstring> diff2 = {L"", L"", L"բ"};
    std::vector<std::wstring> mergedDiff = UTILITIES::mergeDiffs(diff1, diff2);
    ASSERT_EQ(3, mergedDiff.size());
    ASSERT_STREQ(L"ա", mergedDiff.at(0).c_str());
    ASSERT_STREQ(L"", mergedDiff.at(1).c_str());
    ASSERT_STREQ(L"բ", mergedDiff.at(2).c_str());
}

TEST(UtilitiesTests, isVowel) {
    ASSERT_TRUE(UTILITIES::isVowel(L'ա'));
    ASSERT_FALSE(UTILITIES::isVowel(L'բ'));
}

TEST(UtilitiesTests, hasVowel) {
    ASSERT_TRUE(UTILITIES::hasVowel(L"արտեր"));
    ASSERT_FALSE(UTILITIES::hasVowel(L"դտթ"));
}

TEST(UtilitiesTests, isConsonant) {
    ASSERT_FALSE(UTILITIES::isConsonant(L'ա'));
    ASSERT_TRUE(UTILITIES::isConsonant(L'բ'));
}

TEST(UtilitiesTests, getVowelFlagsForWord) {
    std::vector<bool> result = UTILITIES::getVowelFlagsForWord(L"օրինակ");
    ASSERT_EQ(6, result.size());
    ASSERT_EQ(true, result[0]);
    ASSERT_EQ(false, result[1]);
    ASSERT_EQ(true, result[2]);
    ASSERT_EQ(false, result[3]);
    ASSERT_EQ(true, result[4]);
    ASSERT_EQ(false, result[5]);
}

TEST(UtilitiesTests, setSyllables) {
    // Case 1, միավանկ
    std::vector<std::wstring> syllables1({});
    UTILITIES::setSyllables(L"տտ", syllables1);
    ASSERT_EQ(1, syllables1.size());
    ASSERT_STREQ(L"տտ", syllables1[0].c_str());
    // Case 2, բազմավանկ
    std::vector<std::wstring> syllables2({});
    UTILITIES::setSyllables(L"կաթնասւն", syllables2);
    ASSERT_EQ(3, syllables2.size());
    ASSERT_STREQ(L"կաթ", syllables2[0].c_str());
    ASSERT_STREQ(L"նա", syllables2[1].c_str());
    ASSERT_STREQ(L"սւն", syllables2[2].c_str());
    // Case 3
    std::vector<std::wstring> syllables3({});
    UTILITIES::setSyllables(L"", syllables3);
    ASSERT_EQ(1, syllables3.size());
    ASSERT_STREQ(L"", syllables3[0].c_str());
}

TEST(UtilitiesTests, readWavFile) {
    std::string testFile = "tests/unit/samples/test.wav";
    std::pair<std::vector<float>, int> samples = UTILITIES::readWavFile(testFile);
    ASSERT_EQ(11025, samples.second);
    ASSERT_EQ(2029, samples.first.size());
    ASSERT_DOUBLE_EQ(0.0546875, samples.first[0]);
}

TEST(UtilitiesTests, writeWavFile) {
    std::string testFile = "tests/unit/samples/test.wav";
    std::string tmpPath = std::filesystem::temp_directory_path().string();
    std::string tmpFile = tmpPath + "/armtts_tmp.wav";

    std::pair<std::vector<float>, int> samples = UTILITIES::readWavFile(testFile);
    UTILITIES::writeWavFile(tmpFile, samples.first, samples.second);

    std::pair<std::vector<float>, int> testSamples = UTILITIES::readWavFile(tmpFile);
    ASSERT_EQ(samples.second, testSamples.second);
    ASSERT_EQ(samples.first.size(), testSamples.first.size());
    ASSERT_DOUBLE_EQ(samples.first[0], testSamples.first[0]);
    std::filesystem::remove(tmpFile);
}

TEST(UtilitiesTests, rfft) {
    std::vector<float> r = { 1, 2, 3, 4, 5, 6, 7, 8 };
    std::vector<std::complex<float>> c = UTILITIES::rfft(std::move(r));
    ASSERT_EQ(5, c.size());
    ASSERT_DOUBLE_EQ(36, c[0].real());
}

TEST(UtilitiesTests, irfft) {
    std::vector<float> r1 = { 1, 2, 3, 4, 5, 6, 7, 8 };
    std::vector<std::complex<float>> c = UTILITIES::rfft(std::move(r1));
    std::vector<float> r2 = UTILITIES::irfft(c);
    ASSERT_EQ(r1.size(), r2.size());
    for (int index = 0; index < r1.size(); ++index) {
        ASSERT_DOUBLE_EQ(r1[index], r2[index]);
    }
}

TEST(UtilitiesTests, stft) {
    std::string testFile = "tests/unit/samples/test.wav";
    std::pair<std::vector<float>, int> samples = UTILITIES::readWavFile(testFile);
    ASSERT_EQ(2029, samples.first.size());

    const int nnft = 2048;
    const int hopLength = 275;
    const int winLength = 1102;
    std::vector<std::vector<std::complex<float>>> result = UTILITIES::stft(samples.first, nnft, hopLength, winLength);

    ASSERT_EQ(8, result.size());
    ASSERT_EQ(1025, result[0].size());
    // Check some values
    ASSERT_DOUBLE_EQ(0.231882, int(result[0][50].real() * 1000000) / 1000000.0);
}

TEST(UtilitiesTests, getWindowSumsquare) {
    int n_fft = 2048;
    int win_length = n_fft;
    int n_frames = 8;
    int hop_length = win_length / 4;

    std::vector<float> windowSumsquare = UTILITIES::getWindowSumsquare(win_length, n_frames, n_fft, hop_length);
    ASSERT_EQ(5632, windowSumsquare.size());
    ASSERT_DOUBLE_EQ(0.109928, int(windowSumsquare[400] * 1000000) / 1000000.0);
}

TEST(UtilitiesTests, overlappingAdd) {
    int n_fft = 2048;
    int win_length = n_fft;
    int n_frames = 8;
    int hop_length = win_length / 4;

    std::vector<float> y(5632, 0);
    std::vector<std::vector<float>> y2(8, std::vector<float>(2024, 1));
    UTILITIES::overlappingAdd(y, y2, hop_length);
    ASSERT_EQ(5632, y.size());
    ASSERT_DOUBLE_EQ(1, y[100]);
    ASSERT_DOUBLE_EQ(2, y[520]);
}

TEST(UtilitiesTests, istft) {
    std::string testFile = "tests/unit/samples/test.wav";
    std::pair<std::vector<float>, int> samples = UTILITIES::readWavFile(testFile);
    ASSERT_EQ(2029, samples.first.size());

    const int nnft = 2048;
    const int hopLength = 275;
    const int winLength = 1102;
    std::vector<std::vector<std::complex<float>>> c = UTILITIES::stft(samples.first, nnft, hopLength, winLength);
    std::vector<float> y = UTILITIES::istft(c);
    ASSERT_DOUBLE_EQ(int(samples.first[50] * 10000) / 100000.0, int(y[50] * 10000) / 100000.0);
}


}}}

