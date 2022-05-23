//
// PrintVar - shows how to use PLOG_PRINT_VAR to print variables.
//

#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Helpers/PrintVar.h>

class C
{
public:
    C(int value) : m_value(value)
    {
        PLOGD << PLOG_PRINT_VAR(this, m_value) << " - can print 'this' (useful for distinguishing class instances)";
    }

    ~C()
    {
        PLOGD << PLOG_PRINT_VAR(this);
    }

private:
    int m_value;
};

int main()
{
    plog::init<plog::TxtFormatter>(plog::verbose, plog::streamStdOut);

    C c1(42);
    C c2(42);

    int x = 10;
    int y = 20;
    int z = 30;

    PLOGI << PLOG_PRINT_VAR(x);
    PLOGI << PLOG_PRINT_VAR(x, y);
    PLOGI << PLOG_PRINT_VAR(x, y, z);
    PLOGI << PLOG_PRINT_VAR(x, y, z, x);
    PLOGI << PLOG_PRINT_VAR(x, y, z, x, y);
    PLOGI << PLOG_PRINT_VAR(x, y, z, x, y, z);
    PLOGI << PLOG_PRINT_VAR(x, y, z, x, y, z, x);
    PLOGI << PLOG_PRINT_VAR(x, y, z, x, y, z, x, y);
    PLOGI << PLOG_PRINT_VAR(x, y, z, x, y, z, x, y, z) << " - can print up to 9 variables";

    PLOGI << PLOG_PRINT_VAR(x + y, z * 2, &c1) << " - can execute expressions";
    PLOGI << "can print some data before, " << PLOG_PRINT_VAR(x, y) << " - in the middle, " PLOG_PRINT_VAR(z) << " - and after";

    return 0;
}
