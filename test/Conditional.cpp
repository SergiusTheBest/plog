#include "doctest.h"
#include <plog/Log.h>
#include "TestAppender.h"

SCENARIO("conditional logging")
{
    GIVEN("logger is initialised")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::info);
        logger.addAppender(&testAppender);

        WHEN("condition is true")
        {
            int var = 0;
            PLOG_INFO_IF(var == 0) << "message";

            THEN("the message is printed")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("message"));
            }
        }

        WHEN("condition is false")
        {
            int var = 0;
            PLOG_INFO_IF(var != 0) << "message";

            THEN("the message is not printed")
            {
                CHECK(testAppender.getMessage().empty());
            }
        }

        WHEN("log level check is true")
        {
            int var = 0;
            IF_PLOG(plog::info) var = 5; // one line

            IF_PLOG(plog::info) // block
            {
                var++;
            }

            THEN("statements were executed")
            {
                CHECK_EQ(var, 5 + 1);
            }
        }
        
        WHEN("log level check is false")
        {
            int var = 0;
            IF_PLOG(plog::debug) var = 5; // one line

            IF_PLOG(plog::debug) // block
            {
                var++;
            }

            THEN("statements were not executed")
            {
                CHECK_EQ(var, 0);
            }
        }

        WHEN("log macros are used in 'then-else' clauses without braces and condition is true")
        {
            int var = 0;
            if (var == 0) PLOGI << "then clause"; else PLOGI << "else clause";

            THEN("nothing is broken, 'then' clause is executed")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("then clause"));
            }
        }

        WHEN("log macros are used in 'then-else' clauses without braces and condition is false")
        {
            int var = 0;
            if (var != 0) PLOGI << "then clause"; else PLOGI << "else clause";

            THEN("nothing is broken, 'else' clause is executed")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("else clause"));
            }
        }
    }
}
