#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <sstream>

namespace csv
{

class CsvWriter 
{
public:
    CsvWriter() = delete;

    template <typename... Columns>
    CsvWriter(const std::string & aFileName, const Columns & ... aColumns);

    template <typename... Fields>
    void write(const Fields & ... aFields);

private:
    std::ofstream theCsvFile;
    std::size_t theNumberOfCols;
        
    template<typename... T>
    std::size_t getNumOfCols(const T... anArgs) const;
    inline bool fileExists(const std::string& filename);
    inline std::vector<std::string> splitCsvLine(const std::string& line);
        
};

inline std::vector<std::string> CsvWriter::splitCsvLine(const std::string& line)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

inline bool CsvWriter::fileExists(const std::string& filename)
{
    return access(filename.c_str(), F_OK) != -1;
}

template<typename... T>
std::size_t CsvWriter::getNumOfCols(const T... anArgs) const
{
    return sizeof...(anArgs);
}

template <typename ...Columns>
CsvWriter::CsvWriter(const std::string & aFileName, const Columns & ... aColumns)
    :  theNumberOfCols(getNumOfCols(aColumns...))
{
    
    bool myFileExists = fileExists(aFileName); 
    
    if (myFileExists)
    {
        std::ifstream myExistingCsvFile(aFileName);
        std::string myHeaderLine;

        if (!std::getline(myExistingCsvFile, myHeaderLine))
            throw std::runtime_error("Could not read header from existing file");

        std::vector<std::string> existingHeaders = splitCsvLine(myHeaderLine);
        std::vector<std::string> newHeaders = { aColumns... };

        if (existingHeaders != newHeaders)
            throw std::runtime_error("CSV headers do not match existing file");

        myExistingCsvFile.close();
    }

    theCsvFile.open(aFileName, std::ios::app);

    if (!theCsvFile.is_open())
    {
        throw std::runtime_error("what? Could not open file");
    }

    if (!myFileExists)
        write(aColumns...);
}

template <typename... Field>
void CsvWriter::write(const Field & ... aFields) 
{
    auto myRecievedNumberOfFields = getNumOfCols(aFields...);

    if (myRecievedNumberOfFields != theNumberOfCols)
    {
        throw std::runtime_error("what? wrong number of fields \n");
    }

    [&](auto && aFirstArg, auto && ... aRemainingFields) {
        theCsvFile << aFirstArg;

        ([&](auto && aField)
        {
            theCsvFile << "," << aField;
        }(aRemainingFields), ...);

    } (aFields...);

    theCsvFile << "\n";
}

} 
