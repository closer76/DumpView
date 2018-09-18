#ifndef __MONITOR_THREAD_H__
#define __MONITOR_THREAD_H__

#include "windows.h"
#include "wx/thread.h"
#include "wx/event.h"

#include <list>

#include "ComPortSetting.h"

const int XFER_BUF_SIZE = 8192;

enum {
    MONITOR_STATE_STOPPED,
    MONITOR_STATE_RUNNING
};

enum {
    MONITOR_EVENT_TYPE_NONE = 0,
    MONITOR_EVENT_TYPE_DATAREADY,
    MONITOR_EVENT_TYPE_STARTED,
    MONITOR_EVENT_TYPE_STOPPED,
    MONITOR_EVENT_TYPE_TERMINATED,
    MONITOR_EVENT_TYPE_INIT_FAILED
};

class MonitorThread :
    public wxThread
{
private:
    wxEvtHandler* m_pParent;
    HANDLE m_hSerialPort;
    int m_CurrentState;
    int m_NextState;
    DWORD m_ErrorCode;

	ComPortSetting m_settings;
	int m_MaxPortNum;

	std::list<wxString> m_AvailComPorts;

    static wxMutex s_mutexDataBuffer;
    static char s_buf[XFER_BUF_SIZE];
    static int s_nBufStartPos;

    bool m_InitSerialPort();
    bool m_ReleaseSerialPort();

	void m_scanPorts();

public:

	MonitorThread(wxEvtHandler* pParent, ComPortSetting &settings) :
		wxThread(),
		m_pParent(pParent),
		m_hSerialPort(INVALID_HANDLE_VALUE),
		m_CurrentState(MONITOR_STATE_STOPPED),
		m_NextState(MONITOR_STATE_STOPPED),
		m_ErrorCode(ERROR_SUCCESS),
		m_settings(settings),
		m_MaxPortNum(32),
		m_AvailComPorts()
    {
		m_scanPorts();
	}

    wxThread::ExitCode Entry();

    int CopyBuffer(char* out_buf);

    void StartMonitoring(void)  { m_NextState = MONITOR_STATE_RUNNING;}
    void StopMonitoring(void)   { m_NextState = MONITOR_STATE_STOPPED;}
    bool IsMonitoring(void)     { return (m_CurrentState == MONITOR_STATE_RUNNING);}

	const std::list<wxString>& GetAvailableComPorts();

    ComPortSetting GetPortSettings(void);
    void SetPortSettings( const ComPortSetting& settings);

    DWORD GetErrorCode(void)
    {
        DWORD result = m_ErrorCode;
        m_ErrorCode = ERROR_SUCCESS;
        return result;
    }

	void SetMaxPortNum(int max_port_num) { m_MaxPortNum = max_port_num; }
	int GetMaxPortNum() { return m_MaxPortNum; }
};

#endif
