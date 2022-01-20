#include "utilities.hpp"
#include "logger.hpp"

#include <sndfile.h>
#include <boost/algorithm/string.hpp>

#include <filesystem>
#include <regex>
#include <string>
#include <vector>

namespace VARAGAVAN {
namespace ARMTTS {
namespace UTILITIES {

std::vector<std::wstring> splitStringIntoWords(const std::wstring& s) noexcept {
    std::vector<std::wstring> words;
    boost::split(words, s, boost::is_space());
    return words;
}

std::wstring join(const std::vector<std::wstring>& w) noexcept {
    std::wostringstream os(L"");
    std::for_each(w.begin(), w.end(), [&os](auto& w){
        os << w << L" ";
    });
    const std::wstring& result = os.str();
    return result.substr(0, result.length()-1);
}

std::wstring removeDoubleSpaces(const std::wstring& s) noexcept {
    std::wregex p(LR"(\s{2,})");
    return std::regex_replace(s, p, L" ", std::regex_constants::match_any);
}

std::vector<std::wstring> getDiff(const std::wstring& a, const std::wstring& b) noexcept {
    std::vector<std::wstring> diff(a.size(), L"");
    if (a.size() == b.size()) {
        // abc abd
        for (int i = 0; i != a.size(); ++i) {
            if (a[i] != b[i]) {
                diff[i] = b[i];
            }
        }
        return diff;
    } else if (a.size() > b.size()) {
        // abc ab
        int count = a.size() - b.size();
        int result = b.size();
        for (int i = 0; i != b.size(); ++i) {
            if (a[i] != b[i]) {
                result = i;
                break;
            }
        }
        for (int i = 0; i != count; ++i) {
            diff[result + i] = L"-";
        }
    } else {
        // ab abc
        int count = b.size() - a.size();
        int result = a.size() - 1;
        for (int i = 0; i != a.size(); ++i) {
            if (a[i] != b[i]) {
                result = i - 1;
                break;
            }
        }
        diff[result] = b.substr(result, count+1);
    }
    return diff;
}

std::vector<std::wstring> mergeDiffs(const std::vector<std::wstring>& a, const std::vector<std::wstring>& b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("Merge conflict occurred!");
    }
    std::vector<std::wstring> result = a;
    for (int i = 0; i != b.size(); ++i) {
        if (b[i] != L"") {
            result[i] = b[i];
        }
    }
    return result;
}

std::wstring applyDiff(const std::wstring& s, const std::vector<std::wstring>& d) {
    if (s.size() != d.size()) {
        throw std::runtime_error("Incorrect diff!\n the sizes of the string and diff should match.");
    }
    std::wostringstream result(L"");
    for (int i = 0; i != s.size(); ++i) {
        if (L"-" == d[i]) {
            continue;
        } else if (L"" == d[i]) {
            result << s[i];
        }
        result << d[i];
    }
    return result.str();
}

void checkFieldExistence(const nlohmann::json& j, const std::string& f) {
    if (! j.contains(f)) {
        Logger::getLogger().error("Invalid configuration: '%s' field is missing", f.c_str());
        throw std::runtime_error("Invalid configuration: '" + f + "' field is missing.");
    }
}

bool isVowel(const wchar_t w) noexcept {
    static std::vector vowels = {L'ա', L'է', L'ե', L'ը', L'ի', L'օ', L'ւ', L'ո'};
    return std::count(vowels.begin(), vowels.end(), w);
}

bool hasVowel(const std::wstring& w) noexcept {
    for (const auto& c : w) {
        if (isVowel(c)) {
            return true;
        }
    }
    return false;
}

bool isConsonant(const wchar_t w) noexcept {
    return ! isVowel(w);
}

std::vector<bool> getVowelFlagsForWord(const std::wstring& w) noexcept {
    std::vector<bool> result( w.length(), false);
    for (int i = 0; i != w.size(); ++i) {
        result[i] = isVowel(w[i]);
    }
    return result;
}

void setSyllables(const std::wstring& word, std::vector<std::wstring>& syllables) noexcept {
    int endIndex = word.length();
    std::vector<bool> vowels = getVowelFlagsForWord(word);
    // No vowels
    if (std::count(vowels.begin(), vowels.end(), true) == false) {
        syllables.emplace(syllables.begin(), word.substr(0, endIndex));
        return;
    }
    // One or more vowels
    auto it = std::find(vowels.rbegin(), vowels.rend(), true);
    int rightIndex = std::distance(vowels.begin(), it.base() - 1);
    if (rightIndex > 0 && isConsonant(word[rightIndex - 1])) {
        --rightIndex;
        if (word[rightIndex] == L'յ') { // յ կիսաձայն
            if (rightIndex > 0 && isConsonant(word[rightIndex - 1])) {
                --rightIndex;
            }
        }
    }
    syllables.emplace(syllables.begin(), word.substr(rightIndex, endIndex - rightIndex));
    // Recursive call
    if (0 != rightIndex) {
        setSyllables(word.substr(0, rightIndex), syllables);
    }
}


std::pair<std::vector<float>, int> readWavFile(const std::string& file) {
    CHECK(std::filesystem::exists(file), file + " file does not exist.");
    SF_INFO sfInfo;
    SNDFILE* sndFile = sf_open(file.c_str(), SFM_READ, &sfInfo);
    std::vector<float> samples(sfInfo.frames);
    sf_read_float(sndFile, samples.data(), samples.size());
    sf_close(sndFile);
    return std::make_pair(samples, sfInfo.samplerate);
}

void writeWavFile(const std::string& file, std::vector<float>& samples, const int sr) {
    CHECK(! samples.empty(), "Samples array is empty, nothing to write.");
    SF_INFO sfInfo;
    sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    sfInfo.channels = 1;
    sfInfo.sections = 1;
    sfInfo.samplerate = sr;
    sfInfo.frames = samples.size();
    SNDFILE* sndFile = sf_open(file.c_str(), SFM_WRITE, &sfInfo);
    sf_write_float(sndFile, samples.data(), samples.size());
    CHECK(! sf_error(sndFile), file + ": Write failure: " + sf_error_number(sf_error(sndFile)));
    sf_write_sync(sndFile);
    sf_close(sndFile);
}

std::vector<std::complex<float>> rfft(std::vector<float>&& r) noexcept {
    const int outputSize = (r.size() / 2) + 1;
    fftwf_complex out[outputSize];
    fftwf_plan p;
    p = fftwf_plan_dft_r2c_1d(r.size(), r.data(), out, FFTW_ESTIMATE);

    fftwf_execute(p);

    fftwf_destroy_plan(p);
    fftwf_cleanup();

    std::vector<std::complex<float>> result(outputSize);
    for (int i = 0; i < outputSize; ++i) {
        result[i] = std::complex<float>(out[i][0], out[i][1]);
    }
    return result;
}

std::vector<float> irfft(const std::vector<std::complex<float>>& c) noexcept {
    fftwf_complex fftw_c[c.size()];
    for (int i = 0; i < c.size(); ++i) {
        fftw_c[i][0] = c[i].real();
        fftw_c[i][1] = c[i].imag();
    }
    const int outputSize = (c.size() - 1) * 2;
    std::vector<float> out(outputSize);

    fftwf_plan p;
    p = fftwf_plan_dft_c2r_1d(outputSize, fftw_c, out.data(), FFTW_ESTIMATE);
    fftwf_execute(p);
    // Normalize
    std::for_each(out.begin(), out.end(), [outputSize](auto & i){ i/= outputSize;});

    fftwf_destroy_plan(p);
    fftwf_cleanup();
    return out;
}

float hannWindow(int n, int size) noexcept {
    return 0.5 * (1 - cos(2 * M_PI * n / (size - 1)));
}

std::vector<float> padCenter(const std::vector<float>& data, int size, PaddingMode mode) noexcept {
    int n = data.size();
    int lpad = abs((n - size) / 2);
    std::vector<float> result(size);
    if (! lpad) {
        // zero padding.
        return data;
    }

    switch (mode) {
        case PaddingMode::CONSTANT:
            // Leading pad
            for (int i = 0; i < lpad; ++i) {
                result[i] = 0;
            }
            for(int i = 0; i < data.size(); ++i) {
                result[i+lpad] = data[i];
            }
            // Ending pad
            for (int i = data.size() + lpad; i < size; ++i) {
                result[i] = 0;
            }
            break;
        case PaddingMode::REFLECT:
            // Set values
            for(int i = lpad; i < lpad + n; ++i) {
                result[i] = data[i-lpad];
            }

            // Leading pad
            int i = lpad;
            int mirroring_index = 0;  //  0 .. n - 1
            bool is_ascending = true; // descending, ascending
            while (i != 0) {
                --i;
                if (is_ascending) {
                    if (mirroring_index != n-1) {
                        ++mirroring_index;
                    } else {
                        --mirroring_index;
                        is_ascending = false;
                    }
                } else {
                    if (mirroring_index != 0) {
                        --mirroring_index;
                    } else {
                        ++mirroring_index;
                        is_ascending = true;
                    }
                }
                result[i] = data[mirroring_index];
            }

            // Ending pad
            i = lpad + n - 1;
            mirroring_index = n - 1; //  n - 1 ... 0
            is_ascending = false;    // descending, ascending
            while (i != result.size()) {
                ++i;
                if (is_ascending) {
                    if (mirroring_index != n-1) {
                        ++mirroring_index;
                    } else {
                        --mirroring_index;
                        is_ascending = false;
                    }
                } else {
                    if (mirroring_index != 0) {
                        --mirroring_index;
                    } else {
                        ++mirroring_index;
                        is_ascending = true;
                    }
                }
                result[i] = data[mirroring_index];
            }
            break;
    }
    return result;
}

std::vector<std::vector<float>> frame(const std::vector<float>& y, int frame_length, int hop_length) noexcept {
    int n_frames = 1 + (y.size() - frame_length) / hop_length;
    std::vector<std::vector<float>> result(n_frames, std::vector<float>(frame_length));
    const int shift = frame_length - hop_length;
    int counter = 0;
    for(int i = 0; i < result.size(); ++i) {
        for(int j = 0; j < result[0].size(); ++j) {
            const int index = counter - (i * shift);
            result[i][j] = y[index];
            ++counter;
        }
    }
    return result;
}

std::vector<float> getWindowSumsquare(int win_length, int n_frames, int n_fft, int hop_length) noexcept {
    const int n = n_fft + hop_length * (n_frames - 1);
    std::vector<float> result(n, 0);

    std::vector<float> window_sq(win_length, 0);
    for (int index = 0; index < window_sq.size(); ++index) {
        window_sq[index] = pow(UTILITIES::hannWindow(index, win_length), 2);
    }
    std::vector<float> window = UTILITIES::padCenter(window_sq, n_fft);

    // Fill
    for (int i = 0; i < n_frames; ++i) {
        const int sample = i * hop_length;
        const int window_end = std::max(0, std::min(n_fft, n - sample));
        for (int index = 0; index < window_end; ++index) {
            result[sample + index] += window_sq[index];
        }
    }

    return result;
}

void overlappingAdd(std::vector<float>& y, const std::vector<std::vector<float>>& i, int hopLength) noexcept {
    const int n_fft = i[0].size();
    for (int frame = 0; frame < i.size(); ++frame) {
        int sample = frame * hopLength;
        for (int index = 0; index < n_fft; ++index) {
            y[sample + index] += i[frame][index];
        }
    }
}

std::vector<std::vector<std::complex<float>>>
stft(std::vector<float> y, int n_fft, int hop_length, int win_length) noexcept {
    // Set the default values
    if (! n_fft) { n_fft = 2048; }
    if (! win_length) { win_length = n_fft; }
    if (! hop_length) { hop_length = win_length / 4; }

    std::vector<float> fft_window(win_length);
    for (int i = 0; i < fft_window.size(); ++i) {
        fft_window[i] = hannWindow(i, win_length);
    }
    fft_window = padCenter(fft_window, n_fft);

    y = padCenter(y, y.size() + n_fft, PaddingMode::REFLECT);
    std::vector<std::vector<float>> y_frames = frame(y, n_fft, hop_length);

    // FFT
    std::vector<std::vector<std::complex<float>>> result;
    result.reserve(y_frames.size());
    for (auto& i : y_frames) {
        for(int index = 0; index < i.size(); ++index) {
            i[index] *= fft_window[index];
        }
        result.push_back(UTILITIES::rfft(std::move(i)));
    }
    return result;
}

std::vector<float> istft(const std::vector<std::vector<std::complex<float>>>& c, int n_fft, int hop_length,
                          int win_length) noexcept {
    // Set the default values
    if (! n_fft) { n_fft = 2 * (c[0].size() - 1); }
    if (! win_length) { win_length = n_fft; }
    if (! hop_length) { hop_length = win_length / 4; }

    const int n_frames = c.size();
    const int expected_signal_len = n_fft + hop_length * (n_frames - 1);

    std::vector<float> ifft_window(win_length);
    int index = 0;
    std::for_each(ifft_window.begin(), ifft_window.end(), [&index, &win_length](auto &i){
        i = UTILITIES::hannWindow(index, win_length);
        ++index;
    });
    ifft_window = UTILITIES::padCenter(ifft_window, n_fft);

    std::vector<float> y(expected_signal_len, 0);
    std::vector<std::vector<float>> y_tmp(c.size());

    // IFFT
    for (int frame = 0; frame < c.size(); ++frame) {
        y_tmp[frame] = UTILITIES::irfft(std::move(c[frame]));
        for(int i = 0; i < y_tmp[0].size(); ++i) {
            y_tmp[frame][i] *= ifft_window[i];
        }
    }

    // Overlap-add the istft block starting at the i'th frame
    UTILITIES::overlappingAdd(y, y_tmp, hop_length);

    std::vector<float> ifft_window_sum = UTILITIES::getWindowSumsquare(win_length, n_frames, n_fft, hop_length);

    // Set non-zero indexes.
    for (int index = 0; index < ifft_window_sum.size(); ++index) {
        if (ifft_window_sum[index] > std::numeric_limits<float>::min()) {
            y[index] /= ifft_window_sum[index];
        }
    }

    // Center trimming.
    std::vector result(y.begin() + (n_fft/2), y.end() - (n_fft/2));
    return result;
}

std::vector<float> griffinLim(const std::vector<std::vector<std::complex<float>>>& y) noexcept {
    std::vector<std::vector<std::complex<float>>> tmp(y.size(), std::vector<std::complex<float>>(y[0].size()));
    for (int i = 0; i < y.size(); ++i) {
        for (int j = 0; j < y[0].size(); ++j) {
            tmp[i][j] = y[i][j];
        }
    }
    const int N = 50;
    const int nnft = 2048;
    const int hopLength = 275;
    const int winLength = 1102;

    for (int n = 0; n < N; ++n) {
        std::vector<float> X_t = istft(tmp, 0, hopLength, winLength);
        std::vector<std::vector<std::complex<float>>> est = stft(X_t, nnft, hopLength, winLength);
        for (int i = 0; i < y.size(); ++i) {
            for (int j = 0; j < y[0].size(); ++j) {
                tmp[i][j] = (est[i][j] / std::max(1e-8f, abs(est[i][j]))) * y[i][j];
            }
        }
    }
    auto k = istft(tmp, 0, hopLength, winLength);
    int index = 0;
    return k;
}

std::vector<float> trim(const std::vector<float>& y) noexcept {
    // Compute the MSE for the signal


    return std::vector<float>();
}


}}}
