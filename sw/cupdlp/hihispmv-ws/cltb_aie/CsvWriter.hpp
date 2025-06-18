#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <unistd.h>

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
};

template<typename... T>
std::size_t CsvWriter::getNumOfCols(const T... anArgs) const
{
    return sizeof...(anArgs);
}

template <typename ...Columns>
CsvWriter::CsvWriter(const std::string & aFileName, const Columns & ... aColumns)
    :  theNumberOfCols(getNumOfCols(aColumns...))
{
    theCsvFile.open(aFileName);

    if (!theCsvFile.is_open())
    {
        throw std::runtime_error("what? Could not open file");
    }
    
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

    }(aFields...);

    theCsvFile << "\n";
}

} 
