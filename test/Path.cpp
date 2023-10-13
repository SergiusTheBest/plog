#include "Common.h"

#ifdef __cpp_lib_filesystem
#   include <filesystem>

SCENARIO("std::filesystem::path")
{
    GIVEN("logger is initialised")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("type is std::filesystem::path")
        {
            std::filesystem::path path("/usr/lib");
            PLOGI << path;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("/usr/lib"));
            }
        }
    }
}

#endif
