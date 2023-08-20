#include "doctest.h"
#include <plog/Log.h>
#include "TestAppender.h"

SCENARIO("simple types")
{
    GIVEN("logger is initialised")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("type is bool")
        {
            bool var = true;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("1"));
            }
        }

        WHEN("type is char")
        {
            char var = 'a';
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("a"));
            }
        }

        WHEN("type is signed char")
        {
            signed char var = 'a';
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("a"));
            }
        }

        WHEN("type is unsigned char")
        {
            unsigned char var = 'a';
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("a"));
            }
        }

        WHEN("type is short")
        {
            short var = -1000;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("-1000"));
            }
        }

        WHEN("type is unsigned short")
        {
            unsigned short var = 1000;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("1000"));
            }
        }

        WHEN("type is int")
        {
            int var = -1000;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("-1000"));
            }
        }

        WHEN("type is unsigned int")
        {
            unsigned int var = 1000;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("1000"));
            }
        }

        WHEN("type is long")
        {
            long var = -1000;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("-1000"));
            }
        }

        WHEN("type is unsigned long")
        {
            unsigned long var = 1000;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("1000"));
            }
        }

        WHEN("type is long long")
        {
            long long var = -1000;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("-1000"));
            }
        }

        WHEN("type is unsigned long long")
        {
            unsigned long long var = 1000;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("1000"));
            }
        }

        WHEN("type is float")
        {
            float var = 1.2345f;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("1.2345"));
            }
        }

        WHEN("type is double")
        {
            double var = 1.2345;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("1.2345"));
            }
        }

        WHEN("type is void pointer")
        {
            void* var = &var;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK(testAppender.getMessage().size() >= 4);

                // could be 2 hex formats for pointers: with 0x prefix and without it
                if (testAppender.getMessage().at(1) == PLOG_NSTR('x'))
                {
                    CHECK_EQ(testAppender.getMessage().at(0), PLOG_NSTR('0'));

                    for (size_t i = 2; i < testAppender.getMessage().size(); ++i)
                    {
                        CHECK(std::isxdigit(testAppender.getMessage().at(i)));
                    }
                }
                else
                {
                    for (size_t i = 0; i < testAppender.getMessage().size(); ++i)
                    {
                        CHECK(std::isxdigit(testAppender.getMessage().at(i)));
                    }
                }
            }
        }
    }
}
