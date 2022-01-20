#include "engine.hpp"
#include "utilities.hpp"

#include "gtest/gtest.h"

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

class EngineTests : public ::testing::Test {
protected:
    void SetUp()
    {
        _engine = new Engine();
    }

    void TearDown()
    {
        delete _engine;
    }

    Engine* _engine;
};

// Unit Tests
TEST_F(EngineTests, process) {
    std::wstring text = L"Օրինակ";
    const std::vector<std::string> results = _engine->process(text);
    ASSERT_EQ(1, results.size());
    std::cout << "Generated file: " << results[0] << std::endl;
    // Clean up.
    std::remove(results[0].c_str());
}

}}}