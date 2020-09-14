#include "gtest/gtest.h"
#include <FPGrowth/FPGrowth.hpp>
#include <DataComponent/DataComponent.h>

struct PFExampleTest :public ::testing::Test
{
	FP::PatternSet* ptrPattern;
	const FP::Item a {"1"};
	const FP::Item b {"2"};
	const FP::Item v {"3"};
	const FP::Item g {"4"};
	const FP::Item d {"5"};

	PFExampleTest()
	{
		ptrPattern = new FP::PatternSet();

		const FP::DataBase transactions
		{
			{"1","3","4" },
			{"2","3","5" },
			{"1","2","3","5" },
			{"2","5" },
			{"1","2","3","5" },
		};
		
		const uint64_t unMinRequiredThreshold = 2;
		const FP::Tree fpTree{ transactions,unMinRequiredThreshold };
		*ptrPattern = FP::TreeGrowth::ConstructPatternSetFromTree(fpTree);
	}

	~PFExampleTest()
	{
		delete ptrPattern;
	}

};

TEST_F(PFExampleTest, SizeTest)
{
	EXPECT_EQ(ptrPattern->size(), 19);
	std::cout << "ptr Size: " + ptrPattern->size();
}

TEST_F(PFExampleTest, Test_PF_1)
{

	EXPECT_EQ(ptrPattern->count({ { "1" }, 3 }), 1);
	std::cout << "ptr Size: " + ptrPattern->size();
}

TEST_F(PFExampleTest, Test_PF_2)
{
	EXPECT_EQ(ptrPattern->count({ {"2" }, 4 }), 1);
}



TEST_F(PFExampleTest, Test_PF_3)
{
	EXPECT_EQ(ptrPattern->count({ {"3" }, 4 }), 1);
}

TEST_F(PFExampleTest, Test_PF_4)
{
	EXPECT_EQ(ptrPattern->count({ { "5"}, 4 }), 1);
}

TEST_F(PFExampleTest, Test_PF_5)
{
	
	EXPECT_EQ(ptrPattern->count({ {"1","2" }, 2 }), 1);
}

TEST_F(PFExampleTest, Test_PF_6)
{
	EXPECT_EQ(ptrPattern->count({ {"1","3" }, 3 }), 1);
}

TEST_F(PFExampleTest, Test_PF_7)
{
	EXPECT_EQ(ptrPattern->count({ {"1","5" }, 2 }), 1);
}

TEST_F(PFExampleTest, TestPF8)
{
	
	EXPECT_EQ(ptrPattern->count({ {"2","3"}, 3 }), 1);
		
}

TEST_F(PFExampleTest, TestPF9)
{
	EXPECT_EQ(ptrPattern->count({ {"2","5" }, 4 }), 1);
}

TEST_F(PFExampleTest, TestPF10)
{
	EXPECT_EQ(ptrPattern->count({ {"3","5" }, 3 }), 1);
}

TEST_F(PFExampleTest, TestPF11)
{
	EXPECT_EQ(ptrPattern->count({ {"1","2","3" }, 2 }), 1);
}

TEST_F(PFExampleTest, TestPF12)
{
	EXPECT_EQ(ptrPattern->count({ {"1","2","5" }, 2 }), 1);
}

TEST_F(PFExampleTest, TestPF13)
{
	
	EXPECT_EQ(ptrPattern->count({ {"1","3","5" }, 2 }), 1);
}

TEST_F(PFExampleTest, TestPF14)
{
	EXPECT_EQ(ptrPattern->count({ {"2","3","5" }, 3 }), 1);
}

TEST_F(PFExampleTest, TestPF15)
{
	EXPECT_EQ(ptrPattern->count({ {"1","2","3","5" }, 2 }), 1);
}
