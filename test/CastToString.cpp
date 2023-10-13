#include "Common.h"

class CastableToString
{
public:
    operator std::string() const
    {
        return "object";
    }
};

#if PLOG_ENABLE_WCHAR_INPUT
class CastableToWString
{
public:
    operator std::wstring() const
    {
        return L"object";
    }
};
#endif

SCENARIO("cast to string")
{
    GIVEN("logger is initialised")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("type is castable to std::string")
        {
            CastableToString var;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("object"));
            }
        }

#if PLOG_ENABLE_WCHAR_INPUT
        WHEN("type is castable to std::wstring")
        {
            CastableToWString var;
            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("object"));
            }
        }
#endif
    }
}
