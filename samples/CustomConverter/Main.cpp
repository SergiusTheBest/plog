//
// CustomConverter - shows how to implement a custom converter that encrypts log messages.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>

namespace plog
{
    class MyConverter
    {
    public:
        static std::string header(const util::nstring& str)
        {
            return convert(str); // We have no special header for a file, so just call convert.
        }

        static std::string convert(const util::nstring& str)
        {
            const std::string& in = UTF8Converter::convert(str); // Convert to UTF8 first as it is more compact.

            std::string out;
            out.resize(in.size());

            // This is an encryption key.
            const char kKey[] = "\x56\x5a\x43\x4d\x5f\x81\x4c\x4e\x19\x29\x2e\x13\x7c\x31\x14\x17\x5d\x63\x32\x39";

            // Simple XOR encryption.
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
    static plog::RollingFileAppender<plog::TxtFormatter, plog::MyConverter> appender("CustomConverter.txt"); // Create an appender and pass our converter as a template parameter.
    plog::init(plog::debug, &appender); // Initialize the logger with the appender.

    PLOGD << "A debug message!";
    PLOGD << "Another one debug message!";

    return 0;
}
