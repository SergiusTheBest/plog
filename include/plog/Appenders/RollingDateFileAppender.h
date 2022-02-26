//
// Created by caesar kekxv on 2022/2/26.
//

#ifndef TERMINALSERVICE_ROLLINGDATEFILEAPPENDER_H
#define TERMINALSERVICE_ROLLINGDATEFILEAPPENDER_H

#include <string>
#include <plog/Log.h>
#include <plog/Appenders/IAppender.h>
#include <plog/Converters/UTF8Converter.h>
#include <plog/Converters/NativeEOLConverter.h>
#include <plog/Appenders/RollingFileAppender.h>

#ifdef _WIN32
#include <windows.h>
#endif
namespace plog {

    template<class Formatter, class Converter = NativeEOLConverter <UTF8Converter> >
    class RollingDateFileAppender : public RollingFileAppender<Formatter, Converter> {
    private:
        time_t last_date;
        size_t m_fileSize;
        const util::nchar *m_fileName;

    public:
#ifdef _WIN32
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

        inline void write(const Record &record) {
            if (record.getTime().time >= last_date) {
                std::string path = util::get_file_name(m_fileName);
                this->setFileName(path.c_str());
                if (m_fileSize > 0) {
                    int size = (int) m_fileSize;
                    util::nstring lastFileName = util::get_file_name(m_fileName, -size);
                    util::File::unlink(lastFileName.c_str());
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
