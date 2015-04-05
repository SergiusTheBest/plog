#pragma once
#include <algorithm>
#include <plog/Util.h>

namespace plog
{
    template<class Formatter>
    class RollingFileAppender : public IAppender
    {
    public:
        RollingFileAppender(const char* fileName, size_t maxFileSize, int maxFiles) 
            : m_fileSize()
            , m_maxFileSize((std::max)(maxFileSize, m_kMaxFileSizeLowerLimit))
            , m_lastFileNumber((std::max)(maxFiles - 1, 0))
        {
            util::splitFileName(fileName, m_fileNameNoExt, m_fileExt);
            openLogFile();
        }

        virtual void write(const Record& record)
        {
            util::nstring str = Formatter::format(record);

            if (m_lastFileNumber > 0)
            {
                util::MutexLock lock(m_mutex);

                if (m_fileSize > m_maxFileSize)
                {
                    rollLogFiles();
                }
            }

            size_t bytesWritten = m_file.writeAsUTF8(str);

            if (bytesWritten > 0)
            {
                m_fileSize += bytesWritten;
            }
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

            util::nstring str = Formatter::header();
            m_file.writeAsUTF8(str);

            m_fileSize = m_file.getSize();
        }

        std::string buildFileName(int fileNumber = 0)
        {
            std::stringstream ss;
            ss << m_fileNameNoExt;
            
            if (fileNumber > 0)
            {
                ss << '.' << fileNumber;
            }
            
            if (!m_fileExt.empty())
            {
                ss << '.' << m_fileExt;
            }

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
        static const size_t m_kMaxFileSizeLowerLimit = 1000;
    };
}
