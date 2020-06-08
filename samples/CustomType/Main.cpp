//
// CustomType - shows how to print a custom type to the log stream.
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

struct Point // This is our custom type that we want to print to the log stream.
{
    int x;
    int y;
};

namespace plog
{
    Record& operator<<(Record& record, const Point& pt) // Implement a stream operator for our type.
    {
        return record << "(" << pt.x << ";" << pt.y << ")";
    }
}

int main()
{
    plog::init(plog::debug, "CustomType.txt"); // Initialize the logger.

    Point pt1 = { 0, 0 };
    Point pt2 = { 10, -5 };

    PLOGI << "We've got a line with coords: " << pt1 << pt2; // Print our type to the log stream.
    PLOGI << pt1 << pt2 << " - test for a custom type at begin of the stream"; // Print our type to the log stream.

    return 0;
}
