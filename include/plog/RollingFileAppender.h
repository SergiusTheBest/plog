#pragma once
#include <algorithm>
#include <plog/Util.h>

namespace plog
{
    template<class Formatter>
    class RollingFileAppender : public Appender
    {
    public:
        RollingFileAppender(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles) 
            : Appender(maxSeverity)
            , m_fileSize()
            , m_maxFileSize((std::max)(maxFileSize, size_t(1000)))
            , m_lastFileNumber((std::max)(maxFiles - 1, 0))            
        {
            util::splitFileName(fileName, m_fileNameNoExt, m_fileExt);
            openLogFile();
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
#ifdef _WIN32
                ++m_fileSize;
#endif
            }

            m_file.write(str.c_str(), str.size());
        }

    private:
        void rollLogFiles()
        {
            m_file.close();

            std::string lastFileName = buildFileName(m_lastFileNumber);
            util::File::unlink(lastFileName.c_str());

            for (int fileNumber = m_lastFileNumber - 1; fileNumber >= 0; --fileNumber)
            {
                std::string currentFileName = buildFileName(fileNumber);
                std::string nextFileName = buildFileName(fileNumber + 1);

                util::File::rename(currentFileName.c_str(), nextFileName.c_str());
            }

            openLogFile();
        }

        void openLogFile()
        {
            std::string fileName = buildFileName();
            m_file.open(fileName.c_str());

            std::string str = Formatter::header();
            m_file.write(str.c_str(), str.size());

            m_fileSize = m_file.getSize();
        }

        std::string buildFileName(int fileNumber = 0)
        {
            std::stringstream ss;
            ss << m_fileNameNoExt << '.';
            
            if (fileNumber > 0)
            {
                ss << fileNumber << '.';
            }
            
            ss << m_fileExt;
            return ss.str();
        }

    private:
        util::Mutex     m_mutex;
        util::File      m_file;
        size_t          m_fileSize;
        const size_t    m_maxFileSize;
        const int       m_lastFileNumber;
        std::string     m_fileExt;
        std::string     m_fileNameNoExt;
    };
}