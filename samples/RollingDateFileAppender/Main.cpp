//
// Hello - a minimal introduction sample, shows the basic 3 steps to start using plog.
//

#include <plog/Log.h> // Step1: include the header.
#include <plog/Appenders/RollingDateFileAppender.h>
#include <thread>

int main() {
    using plog::TxtFormatter;
    static plog::RollingDateFileAppender<TxtFormatter> dateFileAppender("Hello_date.%Y-%m-%d.log");


    plog::init(plog::debug, &dateFileAppender); // Step2: initialize the logger.

    // Step3: write log messages using a special macro. There are several log macros, use the macro you liked the most.
    for (int i = 0; i < 100; i++) {
        PLOGD << "Hello log!"; // short macro
        PLOG_DEBUG << "Hello log!"; // long macro
        PLOG(plog::debug) << "Hello log!"; // function-style macro
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
