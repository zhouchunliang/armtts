#include "searchPattern.hpp"

#include <gtest/gtest.h>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

// Unit Tests

TEST(SearchPatternTests, checkMatch) {
    // Case 1
    SearchPattern p1(L"(ա)|(բ)");
    ASSERT_TRUE(p1.checkMatch(L"ան"));
    ASSERT_TRUE(p1.checkMatch(L"բն"));
    ASSERT_FALSE(p1.checkMatch(L"գն"));
    // Case 2
    SearchPattern p2(L"^երբ");
    ASSERT_TRUE(p2.checkMatch(L"երբճ"));
}

}}}
