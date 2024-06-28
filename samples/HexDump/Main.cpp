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

    // `plog::hexdump` can be used with `printf`. Note that `.str()` returns `nstring`,
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

    PLOGI.printf(PLOG_NSTR("printf: %s"), plog::hexdump(p, 100).str().c_str());

#if PLOG_CHAR_IS_UTF8
    PLOGI.printf("printf: %s", plog::hexdump(p, 100).str().c_str());
#else
    PLOGI.printf(L"printf: %s", plog::hexdump(p, 100).str().c_str());
    PLOGI.printf("printf: %S", plog::hexdump(p, 100).str().c_str());
#endif

    return 0;
}
