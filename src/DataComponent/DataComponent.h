#pragma once
#include <string>
#include <vector>

namespace FP
{
    using Item = std::string;
    using Transaction = std::vector<Item>;
}

/**
 * @brief The DataComponent struct - holds
 */
struct DataComponent
{

    /**
     * @brief DataComponent - default constructor.
     * @param strTime - initalization value for m_strTime.
     * @param strEventContext - initalization value for m_strEventContext.
     * @param strComponent - initalization value for m_strComponent.
     * @param strEventName - initalization value for m_strEventName.
     * @param strDescription - initalization value for m_strDescription.
     * @param strOrigin - initalization value for m_strOrigin.
     * @param strIPAddress - initalization value for m_strIPAddress.
     */
    DataComponent(const std::string& strTime,
                  const std::string& strEventContext,
                  const std::string& strComponent,
                  const std::string& strEventName,
                  const std::string& strDescription,
                  const std::string& strOrigin,
                  const std::string& strIPAddress);

    /**
     * @brief m_strTime - value from csv Time column.
     */
    std::string m_strTime;

    /**
     * @brief m_strEventContext - value from csv Event context column.
     */
    std::string m_strEventContext;

    /**
     * @brief m_strComponent - value from csv Component column.
     */
    std::string m_strComponent;

    /**
     * @brief m_strEventName - value from csv Event Name column.
     */
    std::string m_strEventName;

    /**
     * @brief m_strDescription - value from csv Description column.
     */
    std::string m_strDescription;

    /**
     * @brief m_strOrigin - value from csv Origin column.
     */
    std::string m_strOrigin;

    /**
     * @brief m_strIPAddress - value from csv IP Address column.
     */
    std::string m_strIPAddress;

    /**
     * @brief m_strUserID - user id value, taken from m_strDescription.
     */
    std::string m_strUserID;

    /**
     * @brief m_strCourseID - course id value, taken from m_strDescription.
     */
    std::string m_strCourseID;

    /**
     * @brief m_strYYMMDD - date format in YY/MM/DD, taken from m_strTime.
     */
    std::string m_strYYMMDD;

    /**
     * @brief m_strHHMM - time format in HH:MM, taken from m_strTime.
     */
    std::string m_strHHMM;

    /**
     * @brief TransformCompositeFields - fill composite member fields like
     * m_strUserID and m_strCourseID from more general fields, using
     * string operations and regular expressions. */
    void TransformCompositeFields();

    /**
     * @brief Print - print the member fields of this data component.
     */
    void Print() const;

    /**
     * @brief ToStringVector - create a transaction (vector) from all the member
     * fields of this DataComponent.
     * @return a transaction (vector of Items(strings)), created from all the
     * member fields of this struct. */
    FP::Transaction ToTransaction() const;
private:
    void SetUserAndCourseIDs(const std::string& strDescription);
    void SetDateTime(const std::string& strTime);
};

/**
 * @brief The DataReader struct - Read the data from a *.csv file and parse it
 * to a vector of DataComponents. Singleton instance. Use DataReader::GetIntance()
 * to access methods.
 */
struct DataReader
{
    static DataReader& GetInstance();

    /** Delete copy constructor, move constructor and operator= */
    DataReader(DataReader const&) = delete;
    void operator=(DataReader const&) = delete;
    DataReader(DataReader&&) = delete;
    void operator=(DataReader&&) = delete;

    /**
     * @brief ParseCSV - read a *c
     * @param strFilePath
     * @return
     */
    bool ParseCSV(const std::string& strFilePath);

    /**
     * @brief GetComponents - return the components.
     * @return m_vecDataComponents.
     */
    const std::vector<DataComponent> GetComponents() const { return m_vecDataComponents; }

private:
    /** Constructor and copy constructor are private, so this class can't be
     * instatiated outside of the GetIntanceMethod() */
    DataReader() = default;
    ~DataReader() = default;

    void TransformComponents();
    void TransformComponentsMultiThreaded();
private:
    /**
     * @brief m_vecDataComponents - holds all data components, parsed from
     * the csv file.
     */
    std::vector<DataComponent> m_vecDataComponents;

};
