#include "textPreprocessor.hpp"

#include <gtest/gtest.h>

namespace VARAGAVAN {
namespace ARMTTS {
namespace TESTS {

class TextPreprocessorTests : public ::testing::Test {
protected:
   void SetUp()
   {
       textPreprocessor = new TextPreprocessor();
   }

   void TearDown()
   {
       delete textPreprocessor;
   }

   TextPreprocessor* textPreprocessor;
};

// Unit Tests
TEST_F(TextPreprocessorTests, getIndex) {
    ASSERT_EQ(3, textPreprocessor->getIndex(L'ա'));
}

// TODO: uncomment after private member support is added.
TEST_F(TextPreprocessorTests, splitIntoChunks) {
    // Case 1
    std::wstring s1 = L"Եթե Դու այթի ոլորտի սկսնակ մասնագետ ես և ցանկանում ես կարիերադ շարունակել պրոդուկտ մենեջմենթի ոլորտում, ապա այժմ ԴՈՒ ունես ԲԱՑԱՌԻԿ հնարավորություն շարունակելու կարիերադ որպես պրոդուկտ մենեջեր՝ մասնակցելով Պրոդուկտի կառավարման միջին մակարդակի դասընթացին:";
    std::wstring r1_1 = L"Եթե Դու այթի ոլորտի սկսնակ մասնագետ ես և ցանկանում ես կարիերադ շարունակել պրոդուկտ մենեջմենթի ոլորտում, ապա այժմ ԴՈՒ ունես ԲԱՑԱՌԻԿ հնարավորություն շարունակելու կարիերադ որպես";
    std::wstring r1_2 = L"պրոդուկտ մենեջեր՝ մասնակցելով Պրոդուկտի կառավարման միջին մակարդակի դասընթացին:";
    std::vector<std::wstring> result1 = textPreprocessor->splitIntoChunks(s1);
    ASSERT_EQ(2, result1.size());
    ASSERT_STREQ(r1_1.c_str(), result1[0].c_str());
    ASSERT_STREQ(r1_2.c_str(), result1[1].c_str());

    // Case 2
    std::wstring s2 = L"Եթե Դու ";
    std::wstring r2 = L"Եթե Դու ";
    std::vector<std::wstring> result2 = textPreprocessor->splitIntoChunks(s2);
    ASSERT_EQ(1, result2.size());
    ASSERT_STREQ(r2.c_str(), result2[0].c_str());

    // Case 3
    std::wstring s3 = L"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 11111111";
    std::wstring r3_1 = L"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    std::wstring r3_2 = L"11111111";
    std::vector<std::wstring> result3 = textPreprocessor->splitIntoChunks(s3);
    ASSERT_EQ(2, result3.size());
    ASSERT_STREQ(r3_1.c_str(), result3[0].c_str());
    ASSERT_STREQ(r3_2.c_str(), result3[1].c_str());

    // Case 4
    std::wstring s4 = L"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111111";
    std::wstring r4_1 = L"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    std::wstring r4_2 = L"11111111";
    std::vector<std::wstring> result4 = textPreprocessor->splitIntoChunks(s4);
    ASSERT_EQ(2, result4.size());
    ASSERT_STREQ(r4_1.c_str(), result4[0].c_str());
    ASSERT_STREQ(r4_2.c_str(), result4[1].c_str());
}

TEST_F(TextPreprocessorTests, preprocess) {
    std::wstring s = L"օրինակ";
    std::vector<std::vector<int>> result = textPreprocessor->preprocess(s);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ(s.size() + 2, result[0].size()); // ",E" is added at the end.
    ASSERT_EQ(37, result[0][0]);
    ASSERT_EQ(32, result[0][1]);
    ASSERT_EQ(12, result[0][2]);
    ASSERT_EQ(23, result[0][3]);
    ASSERT_EQ(3, result[0][4]);
    ASSERT_EQ(16, result[0][5]);
    ASSERT_EQ(39, result[0][6]);
    ASSERT_EQ(1, result[0][7]);
}

}}}