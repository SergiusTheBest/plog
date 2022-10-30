//
// Performance - measures time per a log call.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ConsoleAppender.h>

enum
{
    Console = 1
};

int main()
{
    // Initialize the logger that will be measured.
    plog::init(plog::debug, "Performance.txt");

    // Initialize the logger for printing info messages.
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init<Console>(plog::debug, &consoleAppender);

    PLOGI_(Console) << "Test started";

    plog::util::Time startTime;
    plog::util::ftime(&startTime);

    const int kCount = 50000;

    // Performance measure loop.
    for (int i = 0; i < kCount; ++i)
    {
        PLOGD << "Hello log!";
    }

    plog::util::Time finishTime;
    plog::util::ftime(&finishTime);

    time_t timeDiff = (finishTime.millitm - startTime.millitm) + (finishTime.time - startTime.time) * 1000;

    PLOGI_(Console) << "Test finished: " << static_cast<double>(timeDiff) * 1000 / kCount << " microsec per call";

    return 0;
}
