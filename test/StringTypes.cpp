#include "doctest.h"
#include <plog/Log.h>
#include "TestAppender.h"

SCENARIO("string types") 
{
    GIVEN("logger is initialised") 
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("type is const char*") 
        {
            const char* var = "test";
            PLOGI << var;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        //TODO: add more tests
    }
}
