#include "Common.h"
#include <string>
#include <ostream>

struct StdStreamable
{
    int id;
    std::string name;
};

inline std::ostream& operator<<(std::ostream& os, const StdStreamable& obj)
{
    os << "StdStreamable (id: " << obj.id << ", name: " << obj.name << ")";
    return os;
}

#if PLOG_ENABLE_WCHAR_INPUT
struct StdWStreamable
{
    int id;
    std::wstring name;
};

inline std::wostream& operator<<(std::wostream& os, const StdWStreamable& obj)
{
    os << L"StdWStreamable (id: " << obj.id << L", name: " << obj.name << L")";
    return os;
}
#endif

SCENARIO("cast to string")
{
    GIVEN("logger is initialised")
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("type is streamable to std::ostream")
        {
            StdStreamable var;
            var.id = 1;
            var.name = "water";

            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("StdStreamable (id: 1, name: water)"));
            }
        }

#if PLOG_ENABLE_WCHAR_INPUT
        WHEN("type is streamable to std::wostream")
        {
            StdWStreamable var;
            var.id = 1;
            var.name = L"water";

            PLOGI << var;

            THEN("the result is as expected")
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("StdWStreamable (id: 1, name: water)"));
            }
        }
#endif
    }
}
