#include "inferenceEngine.hpp"

#include <gtest/gtest.h>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

class InferenceEngineTests : public ::testing::Test {
protected:
    void SetUp()
    {
        std::string ssrnModel = "models/SSRN.tflite";
        std::string textToMelModel = "models/TextToMel.tflite";
        inferenceEngine = new InferenceEngine(textToMelModel, ssrnModel);
    }

    void TearDown()
    {
        delete inferenceEngine;
    }

    InferenceEngine* inferenceEngine;
};

// Unit Tests
TEST_F(InferenceEngineTests, isInitialized) {
    ASSERT_TRUE(inferenceEngine->isInitialized());
}

TEST_F(InferenceEngineTests, runInference) {
    const std::vector<std::vector<float>> result = inferenceEngine->runInference({});
    ASSERT_EQ(SSRNEngine::Z_ROWS, result.size());
    ASSERT_EQ(SSRNEngine::Z_COLUMNS, result[0].size());
}

}}}