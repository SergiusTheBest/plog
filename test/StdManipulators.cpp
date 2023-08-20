#include "doctest.h"
#include <plog/Log.h>
#include "TestAppender.h"

SCENARIO("std manipulators") 
{
    GIVEN("logger is initialised") 
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("std::boolalpha") 
        {
            PLOGI << std::boolalpha << true;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("true"));
            }
        }

        WHEN("std::hex") 
        {
            PLOGI << std::hex << 65534;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("fffe"));
            }
        }

        WHEN("use manipulator for the 1st message") 
        {
            bool var = true;
            PLOGI << std::boolalpha << var;
            
            AND_WHEN("log the 2nd message")
            {
                PLOGI << var;

                THEN("manipulators are cleared for the 2nd message") 
                {
                    CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("1"));
                }
            }
        }
    }
}