//
// Path - a test sample to check that std::filesystem::path can be logged.
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

#if __has_include(<filesystem>)
    #include <filesystem>
    namespace fs = std::filesystem;
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif

int main()
{
    plog::init(plog::debug, "Path.txt");

    PLOGI << "Current path: " << fs::current_path();

    return 0;
}
