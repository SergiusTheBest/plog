#pragma once
#include <string>
#include <windows.h>
#include <plog/Appenders/IAppender.h>
#include <plog/Record.h>
#include <plog/Severity.h>
#include <plog/Util.h>

namespace plog
{
#ifdef _WIN32

template <class Formatter>
class  WinEventLogAppender : public IAppender
{
public:
    WinEventLogAppender(const util::nstring& sourceName,
                        const util::nstring& applicationName)
        : m_sourceName(sourceName)
        , m_applicationName(applicationName)
    {
        openAppender();
    }

    ~WinEventLogAppender() {
        closeAppender();
    }

protected:
    void openAppender() {
        addRegistryInfo(m_sourceName, m_applicationName);
        m_eventSource = ::RegisterEventSource(NULL, m_sourceName.c_str());
    }

    void closeAppender() {
        if (m_eventSource != NULL) {
            ::DeregisterEventSource(m_eventSource);
            m_eventSource = NULL;
        }
    }

    virtual void write(const Record& record) {
        util::nstring str = Formatter::format(record);
        util::MutexLock lock(m_mutex);
        const util::nchar * logMessagePtr[1];
        logMessagePtr[0] = str.c_str();

        const DWORD messageId = 0x1000;

        ::ReportEvent(m_eventSource, logSeverityToType(record.getSeverity()),
                      logSeverityToCategory(record.getSeverity()),
                      messageId, NULL, 1, 0, logMessagePtr, NULL);
    }

    WORD logSeverityToCategory(plog::Severity severity) {
        return static_cast<WORD>(severity / 100 + 1);
    }

    WORD logSeverityToType(plog::Severity severity) {
        WORD result;
        switch(severity) {
        case plog::fatal:
        case plog::error:
            result = EVENTLOG_ERROR_TYPE;
            break;
        case plog::warning:
            result = EVENTLOG_WARNING_TYPE;
            break;
        case plog::info:
        case plog::debug:
        case plog::verbose:
        default:
            result = EVENTLOG_INFORMATION_TYPE;
            break;
        }
        return result;
    }

    void addRegistryInfo(const util::nstring& sourceName,
                         const util::nstring& applicationName)
    {
        const util::nstring prefix = TEXT("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
        util::nstring subKeyName = prefix + sourceName;
        DWORD disposition;

        HKEY hkey = registryGetKey(subKeyName, &disposition);
        if (disposition == REG_CREATED_NEW_KEY) {
            setRegistryString(hkey, TEXT("EventMessageFile"), applicationName);
            setRegistryString(hkey, TEXT("CategoryMessageFile"), applicationName);
            setRegistryDWord(hkey,  TEXT("TypesSupported"), (DWORD)7);
            setRegistryDWord(hkey,  TEXT("CategoryCount") , (DWORD)8);
        }
        ::RegCloseKey(hkey);
    }

    void setRegistryString(HKEY hKey, const util::nstring& name, const util::nstring& value)
    {
        ::RegSetValueEx(hKey, name.c_str(), 0, REG_SZ,
                      (LPBYTE)value.c_str(), value.length() * sizeof(util::nchar));
    }

    void setRegistryDWord(HKEY hKey, const util::nstring& name, DWORD value)
    {
        ::RegSetValueEx(hKey, name.c_str(), 0, REG_DWORD,
                      (LPBYTE)&value, sizeof(DWORD));
    }

    HKEY registryGetKey(const util::nstring& subkey, DWORD* disposition) {
        HKEY hkey = 0;
        ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, NULL,
                       REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL,
                       &hkey, disposition);
        return hkey;
    }

private:
    util::nstring m_sourceName;
    util::nstring m_applicationName;
    HANDLE        m_eventSource;
    util::Mutex   m_mutex;
};

#endif
}
