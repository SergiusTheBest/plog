#define PLOG_NO_CAPTURE_FUNCTION_NAME

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

            THEN("the function entry is empty")
            {
                CHECK_EQ(testAppender.getFunc(), "");
            }
        }
    }
}
