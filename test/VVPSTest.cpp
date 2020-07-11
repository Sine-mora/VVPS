#include "gtest/gtest.h"
#include <Windows.h>
#include <FPGrowth/FPGrowth.hpp>
#include <DataComponent/DataComponent.h>

/**
 * @brief The VVPSTestSuit struct - a test fixture class, used for unit tests
 * of type google test TEST_F.
 */
struct VVPSTestSuit
{

    /**
     * @brief s_isDataParsed - flag, used to check the csv file is parsed
     * only once during runtime.
     */
    static bool s_isDataParsed;

    /**
     * @brief m_dataBase - data base, constructed from the data, parsed
     * from a specified *.csv file.
     */
     static FP::DataBase VVPSTestSuit:: m_dataBase;

    /**
     * @brief m_patternSet - holds the resulting pattern set, created
     * from the Frequent pattern growth algorithm.
     */
    FP::PatternSet m_patternSet;

    /**
     * @brief VVPSTestSuit - constructor, used to initialize member fields.
     */
    VVPSTestSuit();

    /** Default destructor. */
    ~VVPSTestSuit() = default;

    /**
     * @brief CreatetPatternSet - create a pattern set, using the Frequent
     * Pattern growth algorithm.
     * @param unMinRequiredTreshold - minimum required number of occurences
     * for each item set in the patterns.
     * @return the resulting pattern set, holding a set of patterns (a list
     * of items and their occurence).
     */
    FP::PatternSet CreatetPatternSet(uint64_t unMinRequiredTreshold);
private:
    /**
     * @brief CreateDataBase - create a data base from a *.csv file.
     * @param strFilePath - path to the *.csv file to be parsed,
     * relative from the current executable file.
     * @return a data base, contructed from the *.csv file
     */
    FP::DataBase CreateDataBase(const std::string& strFilePath);
};

bool VVPSTestSuit::s_isDataParsed = false;
FP::DataBase VVPSTestSuit::m_dataBase{};
static VVPSTestSuit g_vvps;

VVPSTestSuit::VVPSTestSuit()
{
    //SetConsoleCP(1251);// ��������� ������� �������� win-cp 1251 � ����� �����
    //SetConsoleOutputCP(1251);

    if(!VVPSTestSuit::s_isDataParsed)
    {
        m_dataBase = CreateDataBase("../../../../logs_BCS37_20181103.csv");
        VVPSTestSuit::s_isDataParsed = true;
    }

    constexpr uint64_t MIN_REQUIRED_TRESHOLD{ 700 };
    m_patternSet = CreatetPatternSet(MIN_REQUIRED_TRESHOLD);
}

FP::DataBase VVPSTestSuit::CreateDataBase(const std::string& strFilePath)
{
    /**
     * @brief dataBase - the data base to be returned.
     */
     FP::DataBase dataBase;                                                                                                                                                     

    /**
     * @brief dataReader - reference to DataReader Singleton. */
    auto& dataReader = DataReader::GetInstance();

    /** Parse the data from the specified *.csv file and populate the vector
     * of data components. */
    dataReader.ParseCSV(strFilePath);

    /**
     * @brief vecDataComponents - const reference to the vector of data
     * components. */
    const auto& vecDataComponents = dataReader.GetComponents();

    std::cout << "Filling DataBase with transactions. Please wait...\n";
    /** Iterate through the data components and fill the data base with
     * transactions, created from the data component fields. */
    for(const auto& dataComp: vecDataComponents)
    {
        /** Check if the transaction is valid. */
        const auto& transaction = dataComp.ToTransaction();
        if (!transaction.empty())
        {
            dataBase.emplace_back(transaction);
        }
    }
    std::cout << "DataBase created successfully.\n";
    return dataBase;
}



FP::PatternSet VVPSTestSuit::CreatetPatternSet(uint64_t unMinRequiredTreshold)
{
    std::cout << "Running FrequentPatternGrowth Algorithm. Go drink some coffee, this may take a few minutes.\n";
    const FP::Tree fpTree{ m_dataBase, unMinRequiredTreshold };
    const FP::PatternSet patternSet{FP::TreeGrowth::ConstructPatternSetFromTree(fpTree)};
    std::cout << "FrequentPatternGrowh completed successfully.";
    return patternSet;
}

/** Unit Tests */
TEST(VVPSTestSuit, TestPatternParsing)
{
    FP::PrintPatternSet(g_vvps.m_patternSet);
    constexpr uint64_t EXPECTED_PATTERN_SIZE{ 3532};
    EXPECT_NE(EXPECTED_PATTERN_SIZE, g_vvps.m_patternSet.size());
}

//
TEST(VVPSTestSuit, TestPattern1) 
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_5464","web"}, 4744 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern2)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_6361","web"}, 1456 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern3)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_6361"}, 1456 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern4)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_5464"}, 4744 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern5)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_4773","web"}, 1255 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern6)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_4774"}, 1225 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern7)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_2","web"}, 1416 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern8)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_2"}, 1416 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern9)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_109","web"}, 1657 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern10)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"UserID_109"}, 1657 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern11)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"User unenrolled from course","web"}, 1032 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}

TEST(VVPSTestSuit, TestPattern12)
{
    EXPECT_EQ(g_vvps.m_patternSet.count({ {"User report","web"}, 1362 }), 1);
    std::cout << g_vvps.m_patternSet.size() << "\n";
}