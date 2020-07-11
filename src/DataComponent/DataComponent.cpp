#include <DataComponent/DataComponent.h>
#include <DataComponent/CSV.h>
#include <iostream>
#include <regex>
#include <sstream>

/** Data Component methods. */
DataComponent::DataComponent(const std::string& strTime,
                             const std::string& strEventContext,
                             const std::string& strComponent,
                             const std::string& strEventName,
                             const std::string& strDescription,
                             const std::string& strOrigin,
                             const std::string& strIPAddress)
    : m_strTime(strTime),
      m_strEventContext(strEventContext),
      m_strComponent(strComponent),
      m_strEventName(strEventName),
      m_strDescription(strDescription),
      m_strOrigin(strOrigin),
      m_strIPAddress(strIPAddress),
      m_strUserID(""),
      m_strCourseID("")
{
}

void DataComponent::TransformCompositeFields()
{
    SetUserAndCourseIDs(m_strDescription);
    SetDateTime(m_strTime);
}

void DataComponent::Print() const
{
    std::cout << "DataComponent: time: \"" << m_strTime << "\""
              << "\nevent context: \"" << m_strEventContext << "\""
              << "\ncomponent: \"" << m_strComponent << "\""
              << "\nevent name: \"" << m_strEventName << "\""
              << "\ndescription: \"" << m_strDescription << "\""
              << "\norigin: \"" << m_strOrigin << "\""
              << "\nip address: \"" << m_strIPAddress << "\""
              << "\nuser id: \"" << m_strUserID << "\""
              << "\ncourse id: \"" << m_strCourseID << "\""
              << "\nYYMMDD: \"" << m_strYYMMDD << "\""
              << "\nHHMM: \"" << m_strHHMM << "\""
              << "\n\n";
}

FP::Transaction DataComponent::ToTransaction() const
{
    FP::Transaction rawTransaction
    {
        m_strTime,
        m_strEventContext,
        m_strComponent,
        m_strEventName,
        m_strDescription,
        m_strOrigin,
        m_strIPAddress,
        m_strUserID,
        m_strCourseID,
        m_strYYMMDD,
        m_strHHMM
    };

    FP::Transaction formattedTransaction;
    for (const auto& item : rawTransaction)
    {
        if (!item.empty())
        {
            formattedTransaction.emplace_back(item);
        }
    }

    return formattedTransaction;
}

void DataComponent::SetUserAndCourseIDs(const std::string& strDescription)
{
    std::regex re(R"('([^'\\]*(?:\\[\s\S][^'\\]*)*)')");
    uint32_t count = 0;
    std::vector<std::string> vecTokens;
    for(std::sregex_iterator i = std::sregex_iterator(strDescription.begin(), strDescription.end(), re);
                             i != std::sregex_iterator();
                             ++i)
    {
        const std::smatch& m = *i;
        vecTokens.emplace_back(m.str(1));
        count++;
    }

    const size_t tokenSize = vecTokens.size();
    if(tokenSize > 1)
    {
        m_strUserID = "UserID_";
        m_strUserID.append(vecTokens.at(0));
        m_strCourseID = "CourseID_";
        m_strCourseID.append(vecTokens.at(tokenSize - 1));
    }
}

void DataComponent::SetDateTime(const std::string& strTime)
{
    static const auto callbackSplit = [](std::string strSource, const std::string& strDelimiter) -> std::vector<std::string>
    {
        std::vector<std::string> vecTokens;
        size_t unCurrPos = 0;
        std::string strToken;
        while ((unCurrPos = strSource.find(strDelimiter)) != std::string::npos)
        {
            strToken = strSource.substr(0, unCurrPos);
            if (!strToken.empty())
            {
                vecTokens.emplace_back(strToken);
            }
            strSource.erase(0, unCurrPos + strDelimiter.length());
        }
        if (!strSource.empty())
        {
            vecTokens.emplace_back(strSource);
        }
        return vecTokens;
    };

    static const auto callbackYYMMDD = [](std::string strDDMMYY) -> std::string
    {
        const auto& date = callbackSplit(strDDMMYY, "/");
        std::stringstream streamYYMMDD;
        constexpr uint64_t EXPECTED_LENGTH{ 2 };
        for (int unCurrComp = date.size() - 1; unCurrComp >= 0; unCurrComp--)
        {
            auto time = date.at(unCurrComp);
            if (time.length() < EXPECTED_LENGTH)
            {
                std::string temp("0");
                temp.append(time);
                time = temp;
            }

            streamYYMMDD << time;
            if (unCurrComp > 0)
            {
                streamYYMMDD << "/";
            }
        }
        return streamYYMMDD.str();
    };

    size_t unBegin = strTime.find_first_of('\"') + 1;
    assert(unBegin != std::string::npos);

    size_t unEnd = strTime.find_last_of('\"');
    assert(unEnd != std::string::npos);

    std::string strInitialFormat = strTime.substr(unBegin, unEnd - unBegin);
    const auto& dateTime = callbackSplit(strInitialFormat, "; ");
    constexpr uint64_t EXPECTED_FIELDS{ 2 };
    if (dateTime.size() == EXPECTED_FIELDS)
    {
        m_strYYMMDD = callbackYYMMDD(dateTime.at(0));
        m_strHHMM = dateTime.at(1);
    }
}

/** Data Reader Methods. */
DataReader& DataReader::GetInstance()
{
    static DataReader instance;
    return instance;
}

bool DataReader::ParseCSV(const std::string& strFilePath)
{    
    std::cout << "DataReader::ParseCSV Reading \"" << strFilePath << "\"...\n";
    try
    {
        static constexpr uint64_t TOTAL_FIELDS{7};
        io::CSVReader<TOTAL_FIELDS> in(strFilePath);
        in.read_header(io::ignore_no_column, "Time", "Event context", "Component",
                       "Event name", "Description", "Origin", "IP address");
        std::string strTime;
        std::string strEventContext;
        std::string strComponent;
        std::string strEventName;
        std::string strDescription;
        std::string strOrigin;
        std::string strIPAddress;
        while(in.read_row(strTime, strEventContext, strComponent, strEventName,
                          strDescription, strOrigin, strIPAddress))
        {
            m_vecDataComponents.emplace_back(DataComponent(strTime, strEventContext, strComponent, strEventName,
                                                           strDescription, strOrigin, strIPAddress));
        }
        std::cout << "DataReader::ParseCSV Finished reading \"" << strFilePath << "\".\n";
        TransformComponents();
    }
    catch (std::exception& e)
    {
        std::cout << "DataReader::ParseCSV exception! What: " << e.what() << "\n";
        return false;
    }
    return true;
}

void DataReader::TransformComponents()
{
    std::cout << "Transforming data components...\n";
    uint64_t unCurrComponent = 0;
    for (auto& dataComp : m_vecDataComponents)
    {
        std::cout << "\r Transforming component " << unCurrComponent << "/"
            << m_vecDataComponents.size() << std::flush;
        dataComp.TransformCompositeFields();
        unCurrComponent++;
    }
    std::cout << "\nFinished transforming data components.\n";
}
