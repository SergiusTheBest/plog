//
// AscDump - shows how to use plog::ascdump to dump binary buffers into ASCII.
//

#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Helpers/AscDump.h>

#include <vector>

int main()
{
    plog::init<plog::TxtFormatter>(plog::verbose, plog::streamStdOut);

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
