#define PLOG_MESSAGE_PREFIX ""
#include "Common.h"

SCENARIO("message prefix")
{
    GIVEN("logger is initialised")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::info);
        logger.addAppender(&testAppender);

        WHEN("set PLOG_MESSAGE_PREFIX and print a message")
        {
#undef PLOG_MESSAGE_PREFIX
#define PLOG_MESSAGE_PREFIX "[test] "
            PLOGI << "message";

            THEN("the message is printed with the prefix")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[test] message"));
            }
        }

        WHEN("set PLOG_MESSAGE_PREFIX to an empty string and print a message")
        {
#undef PLOG_MESSAGE_PREFIX
#define PLOG_MESSAGE_PREFIX ""
            PLOGI << "message";

            THEN("the message is printed without the prefix as it's empty")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("message"));
            }
        }
    }

    GIVEN("logger is initialised and PLOG_MESSAGE_PREFIX is set to a variable")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::info);
        logger.addAppender(&testAppender);

#undef PLOG_MESSAGE_PREFIX
#define PLOG_MESSAGE_PREFIX plogPrefix
        std::string plogPrefix = "[value] ";

        WHEN("print a message")
        {
            PLOGI << "message";

            THEN("the message is printed with the prefix")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[value] message"));
            }

            AND_WHEN("change the variable and print a new message")
            {
                plogPrefix = "***";

                PLOGI << "new message";

                THEN("the new message is printed with the new prefix")
                {
                    CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("***new message"));
                }
            }
        }
    }
}
