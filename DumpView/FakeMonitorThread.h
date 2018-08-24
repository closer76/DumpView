#ifndef _FAKE_MONITOR_THREAD_H_
#define _FAKE_MONITOR_THREAD_H_

#include "windows.h"
#include "wx/thread.h"
#include "wx/event.h"
#include <wx/wfstream.h>
#include "wx/timer.h"

#include <list>

#include "ComPortSetting.h"

/*
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
*/

#include "MonitorThread.h"

class FakeMonitorThread :
	public wxThread,
	public wxTimer
{
    wxEvtHandler* m_pParent;
	wxWindow* m_pParentWnd;
    int m_CurrentState;
    int m_NextState;
    DWORD m_ErrorCode;

	wxFileInputStream* m_streamInput;

	std::list<int> m_AvailComPorts;

public:
    FakeMonitorThread(wxEvtHandler* pParent, ComPortSetting &settings):
        wxThread(),
		wxTimer(this),
        m_pParent(pParent),
		m_pParentWnd(0),
        m_CurrentState( MONITOR_STATE_STOPPED),
        m_NextState( MONITOR_STATE_STOPPED),
        m_ErrorCode(ERROR_SUCCESS),
		m_streamInput(0),
		m_AvailComPorts()
    {
		m_AvailComPorts.push_back(1);
		m_AvailComPorts.push_back(6);
	}

	// derived from wxThread
    wxThread::ExitCode Entry();

	// derived from wxTimer
	virtual void Notify();

	void SetParentWindow( wxWindow* pWnd) { m_pParentWnd = pWnd;}

    int CopyBuffer(char* out_buf);

    void StartMonitoring(void)  { m_NextState = MONITOR_STATE_RUNNING;}
    void StopMonitoring(void)   { m_NextState = MONITOR_STATE_STOPPED;}
    bool IsMonitoring(void)     { return (m_CurrentState == MONITOR_STATE_RUNNING);}

	std::list<int>* GetAvailableComPorts(int max = 16);

    void GetPortSettings( ComPortSetting& settings);
    void SetPortSettings( const ComPortSetting& settings);

    DWORD GetErrorCode(void)
    {
        DWORD result = m_ErrorCode;
        m_ErrorCode = ERROR_SUCCESS;
        return result;
    }
};

#endif