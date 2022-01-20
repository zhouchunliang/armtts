#include "searchReplacePattern.hpp"

#include <gtest/gtest.h>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

// Unit Tests

TEST(SearchReplacePatternTests, checkMatch) {
    // Case 1
    SearchReplacePattern p1(L"ա", L"բ", 1);
    bool result = p1.checkMatch(L"ա");
    ASSERT_TRUE(result);
    result = p1.checkMatch(L"բ");
    ASSERT_FALSE(result);
}

TEST(SearchReplacePatternTests, process) {
    // case 1
    SearchReplacePattern p1(L"ա", L"բ", 0);
    std::wstring s = p1.process(L"աա");
    ASSERT_STREQ(L"բբ", s.c_str());
    // case 2
    SearchReplacePattern p2(L"ա", L"բ", 1);
    s = p2.process(L"աա");
    ASSERT_STREQ(L"բա", s.c_str());
    // case 3
    SearchReplacePattern p3(L"ա", L"բ", 2);
    s = p3.process(L"աաա");
    ASSERT_STREQ(L"բբա", s.c_str());
}

}}}
