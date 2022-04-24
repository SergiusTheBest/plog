//
// AscDump - shows how to use plog::ascdump to dump binary buffers into ASCII.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Helpers/AscDump.h>

#include <vector>

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    std::vector<short> v;
    v.push_back(0x6548);
    v.push_back(0x6c6c);
    v.push_back(0x216f);
    v.push_back(0);
    v.push_back(-1);

    PLOGI << "v: " << plog::ascdump(v);

    unsigned char arr[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xff};
    PLOGI << "arr: " << plog::ascdump(arr);

    void* p = malloc(100);
    PLOGI << "p: " << plog::ascdump(p, 100);

    return 0;
}
