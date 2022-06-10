//
// CXX11 - demonstrates log stream abilities for C++11 features.
//

#include <plog/Log.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>

#include <unordered_map>
#include <unordered_set>
#include <array>

int main()
{
    plog::init<plog::TxtFormatter>(plog::debug, plog::streamStdOut); // Initialize logging

    std::unordered_map<std::string, int> unorderedMap;
    unorderedMap["red"] = 1;
    unorderedMap["green"] = 2;
    unorderedMap["blue"] = 4;
    PLOG_INFO << unorderedMap;

    std::unordered_set<std::string> unorderedSet;
    unorderedSet.insert("red");
    unorderedSet.insert("green");
    unorderedSet.insert("blue");
    PLOG_INFO << unorderedSet;

    std::array<int, 4> array = {{1, 2, 3, 4}};
    PLOG_INFO << array;

    return 0;
}
