#include "textToMelEngine.hpp"
#include "textPreprocessor.hpp"

#include <gtest/gtest.h>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

class textToMelEngineTests : public ::testing::Test {
protected:
   void SetUp()
   {
       std::string model = "models/TextToMel.tflite";
       textToMelEngine = new TextToMelEngine(model, true);
       textPreprocessor = new TextPreprocessor();
   }

   void TearDown()
   {
       delete textToMelEngine;
       delete textPreprocessor;
   }

   TextToMelEngine* textToMelEngine;
   TextPreprocessor* textPreprocessor;
};

// Unit Tests
TEST_F(textToMelEngineTests, isInitialized) {
    ASSERT_TRUE(textToMelEngine->isInitialized());
}

TEST_F(textToMelEngineTests, runInference) {
    const std::wstring text = L"օրինակ,E";
    const std::vector<int> processedText = textPreprocessor->preprocess(text)[0];
    const std::vector<float>& y = textToMelEngine->runInference(processedText);
    ASSERT_EQ(TextToMelEngine::Y_SIZE, y.size());
}

}}}