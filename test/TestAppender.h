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
            m_func = record.getFunc();
        }

        const util::nstring& getMessage() const
        {
            return m_message;
        }

        const util::nstring& getFunc() const
        {
            return m_func;
        }

    private:
        util::nstring m_message;
        util::nstring m_func;
    };
}