//
// CXX17 - demonstrates log stream abilities for C++17 features.
//

#include <plog/Log.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>

#include <string_view>
#include <filesystem>

int main()
{
    plog::init<plog::TxtFormatter>(plog::debug, plog::streamStdOut); // Initialize logging

    std::string_view strView = "string view";
    PLOG_INFO << strView;

#if PLOG_ENABLE_WCHAR_INPUT
    std::wstring_view wstrView = L"wstring view";
    PLOG_INFO << wstrView; // on Linux is printed as a container, will be fixed in future
#endif

    PLOG_INFO << "Current path: " << std::filesystem::current_path();

    return 0;
}
