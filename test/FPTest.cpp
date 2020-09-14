#include "gtest/gtest.h"
#include <FPGrowth/FPGrowth.hpp>
#include <DataComponent/DataComponent.h>

//    * {ASSERT|EXPECT}_EQ(v1, v2): Tests that v1 == v2
//    * {ASSERT|EXPECT}_NE(v1, v2): Tests that v1 != v2
//    * {ASSERT|EXPECT}_LT(v1, v2): Tests that v1 < v2
//    * {ASSERT|EXPECT}_LE(v1, v2): Tests that v1 <= v2
//    * {ASSERT|EXPECT}_GT(v1, v2): Tests that v1 > v2
//    * {ASSERT|EXPECT}_GE(v1, v2): Tests that v1 >= v2
struct FPGrowthPatternGroup1 : public ::testing::Test
{
    FP::PatternSet* ptrPatterns;
    const FP::Item a{ "A" };
    const FP::Item b{ "B" };
    const FP::Item c{ "C" };
    const FP::Item d{ "D" };
    const FP::Item e{ "E" };

    FPGrowthPatternGroup1()
    {
        ptrPatterns = new FP::PatternSet();

        const FP::DataBase transactions{
            { a, b },
            { b, c, d },
            { a, c, d, e },
            { a, d, e },
            { a, b, c },
            { a, b, c, d },
            { a },
            { a, b, c },
            { a, b, d },
            { b, c, e },
 
        };

        const uint64_t unMinRequiredTreshold = 2;

        const FP::Tree fpTree{ transactions, unMinRequiredTreshold };
        *ptrPatterns = FP::TreeGrowth::ConstructPatternSetFromTree(fpTree);
    }

    ~FPGrowthPatternGroup1()
    {
        delete ptrPatterns;
    }
};

struct FPGrowthPatternGroup2 : public ::testing::Test
{
    FP::PatternSet* ptrPatterns;
    const FP::Item a{ "A" };
    const FP::Item b{ "B" };
    const FP::Item c{ "C" };
    const FP::Item d{ "D" };
    const FP::Item e{ "E" };
    FPGrowthPatternGroup2()
    {
        ptrPatterns = new FP::PatternSet();

        const FP::DataBase transactions{
            { a, b, d, e },
            { b, c, e },
            { a, b, d, e },
            { a, b, c, e },
            { a, b, c, d, e },
            { b, c, d },
        };

        const uint64_t unMinRequiredTreshold = 3;

        const FP::Tree fpTree{ transactions, unMinRequiredTreshold };
        *ptrPatterns = FP::TreeGrowth::ConstructPatternSetFromTree(fpTree);
    }

    ~FPGrowthPatternGroup2()
    {
        delete ptrPatterns;
    }
};

struct FPGrowthPatternGroup3 : public ::testing::Test
{
    FP::PatternSet* ptrPatterns;
    const FP::Item a{ "A" };
    const FP::Item b{ "B" };
    const FP::Item c{ "C" };
    const FP::Item d{ "D" };
    const FP::Item e{ "E" };
    const FP::Item f{ "F" };
    const FP::Item g{ "G" };
    const FP::Item h{ "H" };
    const FP::Item i{ "I" };
    const FP::Item j{ "J" };
    const FP::Item k{ "K" };
    const FP::Item l{ "L" };
    const FP::Item m{ "M" };
    const FP::Item n{ "N" };
    const FP::Item o{ "O" };
    const FP::Item p{ "P" };
    const FP::Item s{ "S" };
    FPGrowthPatternGroup3()
    {
        ptrPatterns = new FP::PatternSet();

        const FP::DataBase transactions{
            { f, a, c, d, g, i, m, p },
            { a, b, c, f, l, m, o },
            { b, f, h, j, o },
            { b, c, k, s, p },
            { a, f, c, e, l, p, m, n }
        };

        const uint64_t unMinRequiredTreshold = 3;

        const FP::Tree fpTree{ transactions, unMinRequiredTreshold };
        *ptrPatterns = FP::TreeGrowth::ConstructPatternSetFromTree(fpTree);
    }

    ~FPGrowthPatternGroup3()
    {
        delete ptrPatterns;
    }
};


TEST(MyTest, test1)
{
    constexpr uint32_t value = 32;
    EXPECT_EQ(value, 32);
}

/**
 * @brief TEST_F - test fixture.
 * @param 1 - test fixture class name.
 * @param 2 - name of the current test.
 */

TEST_F(FPGrowthPatternGroup1, Test1)
{
    EXPECT_EQ(ptrPatterns->size(), 19);
}

TEST_F(FPGrowthPatternGroup1, Test2)
{
    /** Count elements with a specific value
    Searches the container for elements equivalent to val and returns
    the number of matches.
    Because all elements in a set container are unique, the function can only
    return 1 (if the element is found) or zero (otherwise). */
    EXPECT_EQ(ptrPatterns->count({ { a }, 8 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test3)
{
    EXPECT_EQ(ptrPatterns->count({ { a, b }, 5 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test4)
{
    EXPECT_EQ(ptrPatterns->count({ { b }, 7 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test5)
{
    EXPECT_EQ(ptrPatterns->count({ { c, a, b }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test6)
{
    EXPECT_EQ(ptrPatterns->count({ { c, a }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test7)
{
    EXPECT_EQ(ptrPatterns->count({ { c }, 6 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test8)
{
    EXPECT_EQ(ptrPatterns->count({ { d, a }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test9)
{
    EXPECT_EQ(ptrPatterns->count({ { d, c, a }, 2 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test10)
{
    EXPECT_EQ(ptrPatterns->count({ { d, c }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test11)
{
    EXPECT_EQ(ptrPatterns->count({ { d, b, a }, 2 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test12)
{
    EXPECT_EQ(ptrPatterns->count({ { d, b, c }, 2 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test13)
{
    EXPECT_EQ(ptrPatterns->count({ { d, b }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test14)
{
    EXPECT_EQ(ptrPatterns->count({ { d }, 5 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test15)
{
    EXPECT_EQ(ptrPatterns->count({ { e, d }, 2 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test16)
{
    EXPECT_EQ(ptrPatterns->count({ { e, c }, 2 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test17)
{
    EXPECT_EQ(ptrPatterns->count({ { e, a, d }, 2 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test18)
{
    EXPECT_EQ(ptrPatterns->count({ { e, a }, 2 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test19)
{
    EXPECT_EQ(ptrPatterns->count({ { e }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup1, Test20)
{
    EXPECT_NE(ptrPatterns->count({ { e, a }, 12 }), 1);
}

/**
 * @brief TEST_F - test fixture.
 * @param 1 - test fixture class name.
 * @param 2 - name of the current test.
 */
TEST_F(FPGrowthPatternGroup2, Test1)
{
        EXPECT_EQ(ptrPatterns->size(), 19);
}

TEST_F(FPGrowthPatternGroup2, Test2)
{
    /** Count elements with a specific value
    Searches the container for elements equivalent to val and returns
    the number of matches.
    Because all elements in a set container are unique, the function can only
    return 1 (if the element is found) or zero (otherwise). */
    EXPECT_EQ(ptrPatterns->count({ { e, b }, 5 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test3)
{
    //Pattern count is 4
    EXPECT_EQ(ptrPatterns->count({ { a, b, e }, 5 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test4)
{
    EXPECT_EQ(ptrPatterns->count({ { a, b }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test5)
{
    EXPECT_EQ(ptrPatterns->count({ { a, e }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test6)
{
    EXPECT_EQ(ptrPatterns->count({ { a }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test7)
{
    EXPECT_EQ(ptrPatterns->count({ { d, a, b }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test8)
{
    EXPECT_EQ(ptrPatterns->count({ { d, a }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test9)
{
    EXPECT_EQ(ptrPatterns->count({ { d, e, b, a }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test10)
{
    EXPECT_EQ(ptrPatterns->count({ { d, e, b }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test11)
{
    EXPECT_EQ(ptrPatterns->count({ { d, e, a }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test12)
{
    EXPECT_EQ(ptrPatterns->count({ { d, e }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test13)
{
    EXPECT_EQ(ptrPatterns->count({ { d, b }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test14)
{
    EXPECT_EQ(ptrPatterns->count({ { d }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test15)
{
    EXPECT_EQ(ptrPatterns->count({ { c, e, b }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test16)
{
    EXPECT_EQ(ptrPatterns->count({ { c, e }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test17)
{
    EXPECT_EQ(ptrPatterns->count({ { c, b }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test18)
{
    EXPECT_EQ(ptrPatterns->count({ { c }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test19)
{
    EXPECT_EQ(ptrPatterns->count({ { b }, 6 }), 1);
}

TEST_F(FPGrowthPatternGroup2, Test20)
{
    EXPECT_NE(ptrPatterns->count({ { e, b }, 11 }), 1);
}


/**
 * @brief TEST_F - test fixture.
 * @param 1 - test fixture class name.
 * @param 2 - name of the current test.
 */
TEST_F(FPGrowthPatternGroup3, Test1)
{
    EXPECT_EQ(ptrPatterns->size(), 18);
}

TEST_F(FPGrowthPatternGroup3, Test2)
{
    /** Count elements with a specific value
    Searches the container for elements equivalent to val and returns
    the number of matches.
    Because all elements in a set container are unique, the function can only
    return 1 (if the element is found) or zero (otherwise). */
   EXPECT_EQ(ptrPatterns->count({ { f }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test3)
{
    EXPECT_EQ(ptrPatterns->count({ { c, f }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test4)
{
    EXPECT_EQ(ptrPatterns->count({ { c }, 4 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test5)
{
    EXPECT_EQ(ptrPatterns->count({ { b }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test6)
{
    EXPECT_EQ(ptrPatterns->count({ { p, c }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test7)
{
    EXPECT_EQ(ptrPatterns->count({ { p }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test8)
{
    EXPECT_EQ(ptrPatterns->count({ { m, f, c }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test9)
{
    EXPECT_EQ(ptrPatterns->count({ { m, f }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test10)
{
    EXPECT_EQ(ptrPatterns->count({ { m, c }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test11)
{
    EXPECT_EQ(ptrPatterns->count({ { m }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test12)
{
    EXPECT_EQ(ptrPatterns->count({ { a, f, c, m }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test13)
{
    EXPECT_EQ(ptrPatterns->count({ { a, f, c }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test14)
{
    EXPECT_EQ(ptrPatterns->count({ { a, f, m }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test15)
{
    EXPECT_EQ(ptrPatterns->count({ { a, f }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test16)
{
    EXPECT_EQ(ptrPatterns->count({ { a, c, m }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test17)
{
    EXPECT_EQ(ptrPatterns->count({ { a, c }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test18)
{
    EXPECT_EQ(ptrPatterns->count({ { a, m }, 3 }), 1);
}

TEST_F(FPGrowthPatternGroup3, Test19)
{
    EXPECT_EQ(ptrPatterns->count({ { a }, 3 }), 1);
}