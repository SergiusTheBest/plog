//
// MessagePrefix - demonstrates usage of PLOG_MESSAGE_PREFIX
//

#define PLOG_MESSAGE_PREFIX "[MyApp] " // define PLOG_MESSAGE_PREFIX before including plog headers
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

int main()
{
    plog::init(plog::debug, "MessagePrefix.log");
    PLOGD << "This is a debug message with a static prefix.";
    PLOGI << "This is an info message with a static prefix.";

    // Demonstrate redefining PLOG_MESSAGE_PREFIX to a variable
    #undef PLOG_MESSAGE_PREFIX
    const char* dynamicPrefix = "[DynamicModule] ";
    static_cast<void>(dynamicPrefix); // Ensure the variable is used to avoid unused variable warning
    #define PLOG_MESSAGE_PREFIX dynamicPrefix

    PLOGW << "This is a warning message with a dynamic prefix.";

    dynamicPrefix = "[DynamicModuleUpdated] ";
    PLOGE << "This is an error message with an updated dynamic prefix.";
    return 0;
}
