//
// CustomType - this sample shows how to write a custom type to a log stream.
//

#include <plog/Log.h>

struct Point // This is our custom type that we want to write to a log stream.
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
    plog::init(plog::debug, "CustomType.txt");

    Point pt1 = { 0, 0 };
    Point pt2 = { 10, -5 };

    LOGI << "We've got a line with coords: " << pt1 << pt2; // Write our custom type to a log stream.

    return 0;
}
