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

        WHEN("type is char*")
        {
            char* var = const_cast<char*>("test");
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is const char*")
        {
            const char* var = "test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is char[]")
        {
            char var[] = "test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is const char[]")
        {
            const char var[] = "test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is std::string")
        {
            std::string var = "test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is const std::string")
        {
            const std::string var = "test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

#if PLOG_ENABLE_WCHAR_INPUT
        WHEN("type is wchar_t*")
        {
            wchar_t* var = const_cast<wchar_t*>(L"test");
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is const wchar_t*")
        {
            const wchar_t* var = L"test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is wchar_t[]")
        {
            wchar_t var[] = L"test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is const wchar_t[]")
        {
            const wchar_t var[] = L"test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is std::wstring")
        {
            std::wstring var = L"test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }

        WHEN("type is const std::wstring")
        {
            const std::wstring var = L"test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }
#endif

#ifdef __cplusplus_cli
        WHEN("type is const System::String^")
        {
            System::String^ var = "test";
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("test"));
            }
        }
#endif

        //TODO: add more tests
    }
}
