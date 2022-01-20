#include "phoneticEngine.hpp"

#include <gtest/gtest.h>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

class PhoneticEngineTests : public ::testing::Test {
protected:
    void SetUp()
    {
        phoneticEngine = new PhoneticEngine();
    }

    void TearDown()
    {
        delete phoneticEngine;
    }

    PhoneticEngine* phoneticEngine;
};

// Unit Tests
TEST_F(PhoneticEngineTests, toLowerCase) {
    // Case 1
    std::wstring text = L"Նա Քանդեց։";
    phoneticEngine->toLoverCase(text);
    ASSERT_STREQ(L"նա քանդեց։", text.c_str());
}

TEST_F(PhoneticEngineTests, processInitialEngine) {
    // Case 1
    std::wstring result1 = phoneticEngine->processInitialEngine(L"ու");
    ASSERT_STREQ(L"ւ", result1.c_str());
}

TEST_F(PhoneticEngineTests, processCaseSensitiveEngines) {
    // Nothing to test for now.
}

// Fix with Boost library
TEST_F(PhoneticEngineTests, processContextSensitiveEngines) {
    // Case 1
    std::wstring result1 = phoneticEngine->processContextSensitiveEngines(L"Քանի՞ լիտր ջուր կխնայվի, եթե 20 հոգի միանա այս մարտահրավերին։");
    ASSERT_STREQ(L"Քանի՞ լիտր ջուր կխնայվի եթե 20 հոգի միանա այս մարտահրավերին։", result1.c_str());
}

TEST_F(PhoneticEngineTests, processEnginesWithMergedExceptions) {
    // Case 1
    std::wstring result1 = phoneticEngine->processEnginesWithMergedExceptions(L"ողջւյն");
    ASSERT_STREQ(L"ոխչւյն", result1.c_str());
}

TEST_F(PhoneticEngineTests, processEngines) {
    ASSERT_STREQ(L"օվ անէլանէլի", phoneticEngine->processEngines(L"ով անելանելի").c_str());
}

TEST_F(PhoneticEngineTests, processHiddenLettersEngines) {
    // Case 1
    ASSERT_STREQ(L"զգուշանալ", phoneticEngine->processHiddenLettersEngine(L"զգուշանալ").c_str());
    // Case 2
    ASSERT_STREQ(L"տըտիպ", phoneticEngine->processHiddenLettersEngine(L"տտիպ").c_str());
}

TEST_F(PhoneticEngineTests, process) {
    // case 1
    const std::wstring s1(L"Նա այդ ասաց միանգամայն հասարակ ձեւով, \
իսկ նրա աչքերի առաջ երեւաց հեղձուցիչ ու աստղալից գիշերվա պատկերը Սալինա-Կրացում,");
    const std::wstring r1(L"նա այդ ասաց միանքամայն հասարակ ձէվօվ, \
իսկ նըրա աչքէրի առաչ յէրէվաց հէղձւցիչ ւ աստղալից գիշէրվա պատկէրը սալինակրացւմ,");
    std::wstring result = phoneticEngine->process(s1);
    ASSERT_STREQ(r1.c_str(), result.c_str());
    // case 2
    const std::wstring s2(L"բեռնատար շոգենավերի լույսերը պիտի լավ դուրս գան, \
իսկ մեջտեղում, ավազի վրա, կնկարեր կռվողների շուրջը խռնված մութ ամբոխը,");
    const std::wstring r2(L"բէռնատար շօքէնավէրի լւյսէրը պիտի լավ դւրս գան, \
իսկ մէչտէղւմ, ավազի վըրա, կընկարէր կըռվօղնէրի շւրջը խըռնված մւթ ամբօխը,");
    result = phoneticEngine->process(s2);
    ASSERT_STREQ(r2.c_str(), result.c_str());
}

}}}
