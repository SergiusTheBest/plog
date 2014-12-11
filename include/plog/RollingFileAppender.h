#pragma once
#include <plog/Util.h>

namespace plog
{
    template<class Formatter>
    class RollingFileAppender : public Appender
    {
    public:
        RollingFileAppender(const char* fileName, Level maxSeverity, size_t maxFileSize, size_t maxFiles) 
            : Appender(maxSeverity), m_maxFileSize(maxFileSize), m_maxFiles(maxFiles)
        {
        }

        virtual void write(const Entry& entry)
        {
            std::string str = Formatter::format(entry);

            {
                util::MutexLock lock(m_mutex);

                if (m_fileSize + str.size() > m_maxFileSize)
                {
                    rollLogFiles();
                }

                m_fileSize += str.size();
            }

            m_file.write(str.c_str(), str.size());
        }

    private:
        void rollLogFiles()
        {
        }

    private:
        util::Mutex     m_mutex;
        util::File      m_file;
        size_t          m_fileSize;
        const size_t    m_maxFileSize;
        const size_t    m_maxFiles;
    };
}