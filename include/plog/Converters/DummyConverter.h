#pragma once
#include <plog/Util.h>

namespace plog
{
    class DummyConverter
    {
    public:
        template<class T>
        static const T& header(const T& str)
        {
            return str;
        }

        template<class T>
        static const T& convert(const T& str)
        {
            return str;
        }
    };
}
