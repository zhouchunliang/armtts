#include "phoneticProcessingEngine.hpp"

#include <gtest/gtest.h>
#include <json.hpp>

#include <string>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

class PhoneticProcessingEngineTests : public ::testing::Test {
protected:
    void SetUp()
    {
        std::string configurationString = R"(
{
    "excluding_pattern": "է",
    "exceptions": [
        ["^([չ]?)եմ$", "$1էմ", 1],
        ["^([չ]?)ես$", "$1էս", 1],
        ["^([չ]?)ենք$", "$1էնք", 1],
        ["^([չ]?)եք$", "$1էք", 1],
        ["^([չ]?)են$", "$1էն", 1]
    ],
    "general_patterns": [
        ["([բգդզթժլխծկհձղմյնչպջռսվտրցփքֆ])\\1", "$1ը$1", 0],
        ["^ե", "յէ", 1],
        ["([բգդզթժլխծկհձղմյնչպջռսվտրցփքֆ])ե", "$1է", 0],
        ["([աէեըիօւ])ե", "$1յէ", 0],
        ["ե", "է", 0]
    ]
}
)";
        nlohmann::json configurationJSON = nlohmann::json::parse(configurationString);
        phoneticProcessingEngine = new PhoneticProcessingEngine(configurationJSON, "testName");
    }

    void TearDown()
    {
        delete phoneticProcessingEngine;
    }

    PhoneticProcessingEngine* phoneticProcessingEngine;
};

// Unit Tests

TEST_F(PhoneticProcessingEngineTests, getName) {
    ASSERT_STREQ("testName", phoneticProcessingEngine->getName().c_str());
}

TEST_F(PhoneticProcessingEngineTests, process) {
    // Check excluding pattern
    ASSERT_STREQ(L"է", phoneticProcessingEngine->process(L"է").c_str());
    // Check exception
    ASSERT_STREQ(L"չէս", phoneticProcessingEngine->process(L"չես").c_str());
    ASSERT_STREQ(L"տըտիպ", phoneticProcessingEngine->process(L"տտիպ").c_str());
    // Check general patterns
    ASSERT_STREQ(L"յէ անէլանէլի", phoneticProcessingEngine->process(L"ե անելանելի").c_str());
}

}}}