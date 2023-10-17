#include "Common.h"
#include <vector>
#include <utility>

SCENARIO("handling null char pointers")
{
    GIVEN("logger is initialised")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("type is char* and its value is NULL")
        {
            char* var = NULL;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("(null)"));
            }
        }

        WHEN("type is std::vector<char*> and value of elements is NULL")
        {
            std::vector<char*> var;
            var.push_back(NULL);
            var.push_back(NULL);
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[(null), (null)]"));
            }
        }

        WHEN("type is std::pair<char*, char*> and value of elements is NULL")
        {
            std::pair<char*, char*> var;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("(null):(null)"));
            }
        }

#if PLOG_ENABLE_WCHAR_INPUT
        WHEN("type is wchar_t* and its value is NULL")
        {
            wchar_t* var = NULL;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("(null)"));
            }
        }

        WHEN("type is std::vector<wchar_t*> and value of elements is NULL")
        {
            std::vector<wchar_t*> var;
            var.push_back(NULL);
            var.push_back(NULL);

            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[(null), (null)]"));
            }
        }

        WHEN("type is std::pair<wchar_t*, wchar_t*> and value of elements is NULL")
        {
            std::pair<wchar_t*, wchar_t*> var;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("(null):(null)"));
            }
        }
#endif
    }
}
