#pragma once
#include <plog/Appenders/IAppender.h>

namespace plog
{
    class TestAppender : public IAppender
    {
    public:
        virtual void write(const Record& record) PLOG_OVERRIDE
        {
            m_message = record.getMessage();
        }

        const util::nstring& getMessage() const
        {
            return m_message;
        }

    private:
        util::nstring m_message;
    };
}