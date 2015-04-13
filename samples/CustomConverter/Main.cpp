#include <plog/Log.h>

namespace plog
{
    class MyConverter
    {
    public:
        static std::string header(const util::nstring& str)
        {
            return convert(str);
        }

        static std::string convert(const util::nstring& str)
        {   
            const std::string& in = UTF8Converter::convert(str);
            
            std::string out;
            out.resize(in.size());
            
            const char kKey[] = "\x56\x5a\x43\x4d\x5f\x81\x4c\x4e\x19\x29\x2e\x13\x7c\x31\x14\x17\x5d\x63\x32\x39";

            for (size_t i = 0; i < out.size(); ++i)
            {
                out[i] = in[i] ^ kKey[i % (sizeof(kKey) - 1)];
            }

            return out;
        }
    };
}

int main()
{
    plog::RollingFileAppender<plog::TxtFormatter, plog::MyConverter> appender("CustomConverter.txt");
    plog::init(plog::debug, &appender);

    LOGD << "A debug message!";
    LOGD << "Another one debug message!";

    return 0;
}
