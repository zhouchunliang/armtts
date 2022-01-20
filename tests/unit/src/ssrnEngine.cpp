#include "textToMelEngine.hpp"
#include "ssrnEngine.hpp"
#include "textPreprocessor.hpp"

#include <gtest/gtest.h>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

class SSRNEngineTests : public ::testing::Test {
protected:
   void SetUp()
   {
       textPreprocessor = new TextPreprocessor();
       std::string ssrnModel = "models/SSRN.tflite";
       std::string textToMelModel = "models/TextToMel.tflite";
       textToMelEngine = new TextToMelEngine(textToMelModel, false);
       ssrnEngine = new SSRNEngine(ssrnModel);
   }

   void TearDown()
   {
       delete textToMelEngine;
       delete ssrnEngine;
       delete textPreprocessor;
   }

   TextToMelEngine* textToMelEngine;
   SSRNEngine* ssrnEngine;
   TextPreprocessor* textPreprocessor;
};

// Unit Tests
TEST_F(SSRNEngineTests, isInitialized) {
    ASSERT_TRUE(ssrnEngine->isInitialized());
}

TEST_F(SSRNEngineTests, runInferrence) {
    const std::wstring text = L"օրինակ,E";
    const std::vector<int> processedText = textPreprocessor->preprocess(text)[0];
    const std::vector<float>& y = textToMelEngine->runInference(processedText);
    const std::vector<std::vector<float>> result = ssrnEngine->runInference(y);
    ASSERT_EQ(SSRNEngine::Z_ROWS, result.size());
    ASSERT_EQ(SSRNEngine::Z_COLUMNS, result[0].size());
}

}}}