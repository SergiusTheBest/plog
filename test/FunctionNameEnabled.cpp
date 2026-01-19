// Note: including the function name in the log Record is enabled
// by default.
#include "Common.h"
#include <vector>
#include <utility>

SCENARIO("disabling function name output")
{
    GIVEN("logger is initialised")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("log message is created")
        {
            PLOGI << "Log message text";

            THEN("the function entry is not empty")
            {
                CHECK_NE(testAppender.getFunc(), "");
            }
        }
    }
}
