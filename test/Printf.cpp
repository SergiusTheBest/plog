#include "doctest.h"
#include <plog/Log.h>
#include "TestAppender.h"

#ifndef __cplusplus_cli
SCENARIO("printf-style messages") 
{
    GIVEN("logger is initialised") 
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("use empty format") 
        {
            PLOGI.printf("");

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR(""));
            }
        }

        WHEN("use text only format") 
        {
            PLOGI.printf("test");

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("use format %d")
        {
            PLOGI.printf("test %d", 42);

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test 42"));
            }
        }

        WHEN("use format %s")
        {
            PLOGI.printf("hello %s", "world");

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("hello world"));
            }
        }

#ifdef _WIN32
        WHEN("use empty format (wide)") 
        {
            PLOGI.printf(L"");

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR(""));
            }
        }

        WHEN("use text only format (wide)") 
        {
            PLOGI.printf(L"test");

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("use format %d (wide)")
        {
            PLOGI.printf(L"test %d", 42);

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test 42"));
            }
        }

        WHEN("use format %s (wide)")
        {
            PLOGI.printf(L"hello %s", L"world");

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("hello world"));
            }
        }
#endif
    }
}
#endif
