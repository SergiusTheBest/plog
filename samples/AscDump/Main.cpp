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

    // `plog::ascdump` can be used with `printf`. Note that `.str()` returns `nstring`,
    // so it can be narrow or wide and thus requires a proper format specifier.
    //
    // If `nstring` is wide:
    // - use wide format string and %s (preferable)
    // - use narrow format string and %S
    // If `nstring` is narrow:
    // - use narrow format string and %s (preferable)
    //
    // Wrap format string with `PLOG_NSTR` to automatically support wide and narrow strings with %s.
    //
    // `nstring` is narrow if char encoding is UTF-8 otherwise it's wide. This allows to
    // support all symbols from different languages.

    PLOGI.printf(PLOG_NSTR("printf: %s"), plog::ascdump(p, 100).str().c_str());

#if PLOG_CHAR_IS_UTF8
    PLOGI.printf("printf: %s", plog::ascdump(p, 100).str().c_str());
#else
    PLOGI.printf(L"printf: %s", plog::ascdump(p, 100).str().c_str());
    PLOGI.printf("printf: %S", plog::ascdump(p, 100).str().c_str());
#endif

    return 0;
}
