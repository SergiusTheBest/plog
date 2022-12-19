//
// DynamicAppender - shows how to add/remove appenders dynamically.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/DynamicAppender.h>

int main()
{
    static plog::DynamicAppender dynamicAppender;
    plog::init(plog::verbose, &dynamicAppender);

    PLOGW << "This message goes nowhere as no real appenders exist";

    {
        plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        dynamicAppender.addAppender(&consoleAppender);

        PLOGI << "Message from a dynamically added appender";

        dynamicAppender.removeAppender(&consoleAppender);
    }

    PLOGW << "This message goes nowhere as no real appenders exist";

    return 0;
}
