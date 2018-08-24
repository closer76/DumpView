#ifndef __MONITOR_THREAD_H__
#define __MONITOR_THREAD_H__

#include "windows.h"
#include "wx/thread.h"
#include "wx/event.h"

#include "ComPortSetting.h"

const int XFER_BUF_SIZE = 4096;

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

    int m_PortNum;
    DWORD m_BaudRate;
    int m_Parity;
    int m_ByteSize;
    int m_StopBit;

    static wxMutex s_mutexDataBuffer;
    static unsigned char s_buf[XFER_BUF_SIZE];
    static int s_nBufStartPos;

    bool m_InitSerialPort();
    bool m_ReleaseSerialPort();

public:

    MonitorThread(wxEvtHandler* pParent):
        wxThread(),
        m_pParent(pParent),
        m_hSerialPort(INVALID_HANDLE_VALUE),
        m_CurrentState( MONITOR_STATE_STOPPED),
        m_NextState( MONITOR_STATE_STOPPED),
        m_ErrorCode(ERROR_SUCCESS),
        m_PortNum(1),
        m_BaudRate(CBR_115200),
        m_Parity(NOPARITY),
        m_ByteSize(8),
        m_StopBit(ONESTOPBIT)
    {}

    wxThread::ExitCode Entry();

    int CopyBuffer(unsigned char* out_buf);

    void StartMonitoring(void)  { m_NextState = MONITOR_STATE_RUNNING;}
    void StopMonitoring(void)   { m_NextState = MONITOR_STATE_STOPPED;}
    bool IsMonitoring(void)     { return (m_CurrentState == MONITOR_STATE_RUNNING);}

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
