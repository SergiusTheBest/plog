//
// Container - shows how to log std containers.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <vector>

struct data
{
    int i;
    std::string s;
};

namespace plog
{
    Record &operator<<(Record &record, const data &data)
    {
        return record << "\n"
                      << "struct data {\n"
                      << "      int i: " << data.i << "\n"
                      << "      std::string s: " << data.s << "\n"
                      << "}";
    }
} // namespace plog

void logSingle()
{
    PLOGI << "Log a single custom data type";
    data data;
    data.i = 42;
    data.s = "Hellow World!";
    PLOGN << data;
}

void logContainer()
{
    PLOGI << "Log a vector holding elements of the custom data type";

    std::vector<data> vdata;
    data data1;
    data1.i = 42;
    data1.s = "Hellow World!";
    data data2;
    data2.i = 12;
    data2.s = "Good bye!";
    vdata.push_back(data1);
    vdata.push_back(data2);
    PLOGN << vdata;

    // like always you can also combine this with other strings or elements
    PLOGN << "Prepend to this " << vdata << " Add to this";
}

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    logSingle();
    logContainer();
}