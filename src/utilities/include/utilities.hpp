/**
 * @file utilities.hpp
 *
 *  Author: Albert Grigoryan
 *  E-mail: albert_grigoryan@yahoo.com
 *  109/26 Vardanants St.,
 *  Yerevan, 0050, Armenia
 *  Tel:  +374-93-635-380
 *
 *  Copyright © 2022 Albert Grigoryan. All rights reserved.
 */

#include "logger.hpp"

#include "json.hpp"
#include "fftw3.h"

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <complex>

namespace VARAGAVAN {
namespace ARMTTS {
namespace UTILITIES {

#define MINIMAL_CHECK(x)                             \
if (!(x)) {                                          \
    std::ostringstream o;                            \
    o << "Error at " << __FILE__ << ':' << __LINE__; \
    VARAGAVAN::ARMTTS::Logger::getLogger().error(o.str().c_str()); \
    throw std::runtime_error(o.str());               \
}

#define CHECK(b, e)                           \
if (!(b)) {                                   \
    std::ostringstream o;                     \
    o << "Error: " << e;                      \
    VARAGAVAN::ARMTTS::Logger::getLogger().error(o.str().c_str()); \
    throw std::runtime_error(o.str());        \
}

enum class PaddingMode {
    CONSTANT,
    REFLECT
};

/**
 * @brief Split string into words.
 * @param s - string
 * @return An array of words.
 */
std::vector<std::wstring> splitStringIntoWords(const std::wstring& s) noexcept;

/**
 * @brief Join words
 * @param w - vector with words
 * @return joined result.
 */
std::wstring join(const std::vector<std::wstring>& w) noexcept;

/**
 * @brief Remove all float spaces from the string.
 * @param s - string to process
 * @return String with all float spaces removed.
 */
std::wstring removeDoubleSpaces(const std::wstring& s) noexcept;

/**
 * @brief Get diff of two strings.
 * @param a - 1st string
 * @param b - 2nd string
 * @return a diff array.
 */
std::vector<std::wstring> getDiff(const std::wstring& a, const std::wstring& b) noexcept;

/**
 * @brief Merge diffs.
 * @param a - diff 1
 * @param b - diff 2
 * @return merged diff array.
 */
std::vector<std::wstring> mergeDiffs(const std::vector<std::wstring>& a, const std::vector<std::wstring>& b);

/**
 * @brief Apply the diff.
 * @param s - string
 * @param d - diff
 * @return Modified string.
 */
std::wstring applyDiff(const std::wstring& s, const std::vector<std::wstring>& d);

/**
 * Check Json field existence and throw exception if it don't.
 * @param j - Json object
 * @param f - field to check
 */
void checkFieldExistence(const nlohmann::json& j, const std::string& f);

/**
 * @brief Check wether the character is vowel.
 * @param w - character to check
 * @return true if it is vowel, otherwise return false.
 */
bool isVowel(const wchar_t w) noexcept;

/**
 * @brief Check wether string has a vowel.
 * @param w - string to check
 * @return true if it is has, otherwise return false.
 */
bool hasVowel(const std::wstring& w) noexcept;

/**
 * @brief Check wether the character is consonant.
 * @param w - character to check
 * @return true if it is consonant, otherwise return false.
 */
bool isConsonant(const wchar_t w) noexcept;

/**
 * @Set syllables recursively (վանկատել)
 * @param word
 * @param syllables
 */
void setSyllables(const std::wstring& word, std::vector<std::wstring>& syllables) noexcept;

/**
 * @brief Get vowel flags for word.
 * @param w - wide string
 * @return vector of booleans indicating vowels.
 */
std::vector<bool> getVowelFlagsForWord(const std::wstring& w) noexcept;

/**
 * @brief Read wav file.
 * @param file - file to read.
 * @return pair of mels and sample rate.
 */
std::pair<std::vector<float>, int> readWavFile(const std::string& file);

/**
 * @brief Write wav file in Wave&Double format.
 * @param file - file to create/update.
 * @param mels - mel spectogramms.
 * @param sr - sample rate
 */
void writeWavFile(const std::string& file, std::vector<float>& samples, const int sr);

/**
 * @brief Compute the one-dimensional discrete Fourier Transform for real input.
 * @param r - real input.
 * @return complex result.
 */
 std::vector<std::complex<float>> rfft(std::vector<float>&& r) noexcept;

 /**
  * @brief Compute the inverse of the one-dimensional discrete Fourier Transform of real input computed by rfft.
  * @param c - complex input.
  * @return real result.
  */
 std::vector<float> irfft(const std::vector<std::complex<float>>& c) noexcept;

 /**
  * @brief Hann window function.
  * @param n - n-th point
  * @param size - window size
  * @return return the window value of the n-th point.
  */
 float hannWindow(int n, int size) noexcept;

 /**
  * @brief Pad the vector with specified mode.
  * CONSTANT mode: [1, 2, 3, 4, 5] -> [0, 0, 1, 2, 3, 4, 5, 0, 0]
  * REFLECT mode: [1, 2, 3, 4, 5] -> [3, 2, 1, 2, 3, 4, 5, 4, 3]
  *
  * @param data - data to pad
  * @param size - size after padding
  * @param mode - mode (CONSTANT by default)
  * @return Padded vector.
  */
 std::vector<float> padCenter(const std::vector<float>& data, int size, PaddingMode mode=PaddingMode::CONSTANT) noexcept;

 /**
  * @brief Slice a data into overlapping frames.
  * For example, a one-dimensional input ``x = [0, 1, 2, 3, 4, 5, 6]``
  * can be framed with frame length 3 and hop length 2 would result:
  * [[0, 2, 4],
  *  [1, 3, 5],
  *  [2, 4, 6]]
  *
  * @param y - data.
  * @param frame_length - length of the frame.
  * @param hop_length - Number of steps to advance between frames.
  * @return Sliced data.
  */
 std::vector<std::vector<float>> frame(const std::vector<float>& y, int frame_length, int hop_length) noexcept;

 /**
  * @brief Compute the sum-square envelope of "hann" function at a given hop length
  * @param win_length - length of the window
  * @param n_frames - number of analysis frames
  * @param n_fft - length of each analysis frame
  * @param hop_length - number of samples to advance between frames
  * @return sum-squared envelope of the window function.
  */
 std::vector<float> getWindowSumsquare(int win_length, int n_frames, int n_fft, int hop_length) noexcept;

 /**
  * @brief Overlap add in the specified, pre-allocated output buffer.
  * @param y - pre-allocated output buffer.
  * @param i - frames to add.
  * @param hopLength - number of samples to advance between frames.
  */
 void overlappingAdd(std::vector<float>& y, const std::vector<std::vector<float>>& i, int hopLength) noexcept;

 /**
  * @brief Short-time Fourier transform.
  * @param y - time-series y values.
  * @param n_fft - length of the FFT.
  * @param hop_length - Number of frames between STFT columns (default: win_length / 4).
  * @param win_length - length of the window function (default: n_fft).
  * @return complex valued spectogram.
  */
 std::vector<std::vector<std::complex<float>>> stft(std::vector<float> y, int n_fft = 0,
                                                    int hop_length = 0, int win_length = 0) noexcept;

 /**
  * @brief Inverse short-time Fourier transform.
  * @param c - complex-valued spectrogram.
  * @param n_fft - length of the FFT.
  * @param hop_length - Number of frames between STFT columns (default: win_length / 4).
  * @param win_length - length of the window function (default: n_fft).
  * @return time-series y values.
  */
 std::vector<float> istft(const std::vector<std::vector<std::complex<float>>>& c, int n_fft = 0, int hop_length = 0,
                           int win_length = 0) noexcept;

/**
 * @brief Trim leading and trailing silence from an audio signal.
  * @param y - time-series y values.
 * @return Trimmed result.
 */
std::vector<float> trim(const std::vector<float>& y) noexcept;

std::vector<float> griffinLim(const std::vector<std::vector<std::complex<float>>>& y) noexcept;

}}}
