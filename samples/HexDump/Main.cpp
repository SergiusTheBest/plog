//
// HexDump - shows how to use plog::hexdump to dump binary buffers into hex.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Helpers/HexDump.h>

#include <vector>
#include <string>

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    std::vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);
    v.push_back(1000000);
    v.push_back(2000000);
    v.push_back(3000000);

    PLOGI << "v: " << plog::hexdump(v);
    PLOGI << "v: " << plog::hexdump(v).group(0);
    PLOGI << "v: " << plog::hexdump(v).separator("", "");
    PLOGI << "v: " << plog::hexdump(v).group(4).separator(" ", "|");
    PLOGI << "v: " << plog::hexdump(v).separator("", " ");

    std::string s("Hello!");
    PLOGI << "s: " << plog::hexdump(s);

    int arr[] = {255, 511, 65535};
    PLOGI << "arr: " << plog::hexdump(arr);

    void* p = malloc(100);
    PLOGI << "p: " << plog::hexdump(p, 100);

    return 0;
}
