#include <plog/Log.h>

struct Point
{
    int x;
    int y;
};

namespace plog
{
    Record& operator<<(Record& record, const Point& pt)
    {
        return record << "(" << pt.x << ";" << pt.y << ")";
    }
}

int main()
{
    plog::init(plog::debug, "CustomType.txt");

    Point pt1 = { 0, 0 };
    Point pt2 = { 10, -5 };

    LOGI << "We've got a line with coords: " << pt1 << pt2;

    return 0;
}
