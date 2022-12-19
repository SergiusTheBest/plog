//
// Created by caesar kekxv on 2022/2/26.
//

#ifndef TERMINALSERVICE_ROLLINGDATEFILEAPPENDER_H
#define TERMINALSERVICE_ROLLINGDATEFILEAPPENDER_H

#include <plog/Appenders/RollingFileAppender.h>

#ifdef _WIN32
#include <windows.h>
#endif
namespace plog {

    template<class Formatter, class Converter = NativeEOLConverter <UTF8Converter> >
    class PLOG_LINKAGE_HIDDEN RollingDateFileAppender : public RollingFileAppender<Formatter, Converter> {
    private:
        time_t last_date;
        size_t m_fileSize;
        util::nstring m_fileName;

    public:
#if !defined(PLOG_DISABLE_WCHAR_T) && defined(_WIN32)
        /**
         *
         * @param fileName "%Y-%m-%d-%H-%M-%S"
         * @param maxFileSize
         * @param maxFiles
         */
        inline explicit RollingDateFileAppender(const char* fileName,int maxFiles = 0)
                : RollingFileAppender<Formatter, Converter>(
                util::get_file_name(fileName).c_str())
        {
            m_fileSize = maxFiles;
            m_fileName = util::toWide(fileName).c_str();
            last_date = util::get_zero_time();
        }
#endif

        inline void setFileName(const util::nchar *fileName) {
            m_fileName = fileName;
            RollingFileAppender<Formatter, Converter>::setFileName(util::get_file_name(fileName).c_str());
            if (!(util::exists(util::get_file_name(fileName).c_str()))) {
                RollingFileAppender<Formatter, Converter>::rollLogFiles();
            }
        }

#if !defined(PLOG_DISABLE_WCHAR_T) && defined(_WIN32)
        void setFileName(const char* fileName)
        {
            setFileName(util::toWide(fileName).c_str());
        }
#endif


        /**
         *
         * @param fileName "%Y-%m-%d-%H-%M-%S"
         * @param maxFileSize
         * @param maxFiles
         */
        inline explicit RollingDateFileAppender(const util::nchar *fileName, size_t maxFiles = 0)
                : RollingFileAppender<Formatter, Converter>(
                util::get_file_name(fileName).c_str()) {
            m_fileSize = maxFiles;
            m_fileName = fileName;
            last_date = 0;
        }

        inline void setMaxFiles(size_t maxFiles) {
            m_fileSize = maxFiles;
        }

        inline void write(const Record &record)  PLOG_OVERRIDE {
            if (record.getTime().time >= last_date) {
                std::string path = util::get_file_name(m_fileName.c_str());
                RollingFileAppender<Formatter, Converter>::setFileName(path.c_str());
#if !defined(PLOG_DISABLE_WCHAR_T) && defined(_WIN32)
                if (path != util::toNarrow(m_fileName.c_str(),CP_UTF8) && m_fileSize > 0) {
#else
                if (path != m_fileName && m_fileSize > 0) {
#endif
                    int size = (int) m_fileSize;
                    std::string lastFileName = util::get_file_name(m_fileName.c_str(), -size);
#if !defined(PLOG_DISABLE_WCHAR_T) && defined(_WIN32)
                    util::File::unlink(util::toWide(lastFileName.c_str()).c_str());
#else
                    util::File::unlink(lastFileName.c_str());
#endif
                }
                if (!(util::exists(path.c_str()))) {
                    RollingFileAppender<Formatter, Converter>::rollLogFiles();
                }
                last_date = util::get_zero_time();
            }
            RollingFileAppender<Formatter, Converter>::write(record);
        }
    };
}


#endif //TERMINALSERVICE_ROLLINGDATEFILEAPPENDER_H
