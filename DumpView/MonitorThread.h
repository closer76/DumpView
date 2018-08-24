#ifndef __MONITOR_THREAD_H__
#define __MONITOR_THREAD_H__

#include "windows.h"
#include "wx/thread.h"
#include "wx/event.h"

const int BUF_SIZE = 256;

class MonitorThread :
    public wxThread
{
private:
    wxEvtHandler* m_pParent;
    HANDLE m_hSerialPort;

    static wxMutex s_mutexDataBuffer;
    static unsigned char s_buf[BUF_SIZE];
    static int s_nBufStartPos;

    bool m_InitSerialPort(int portNum);
    bool m_ReleaseSerialPort(int portNum);

public:

    MonitorThread(wxEvtHandler* pParent):
        wxThread(),
        m_pParent(pParent),
        m_hSerialPort(INVALID_HANDLE_VALUE)
    {}

    wxThread::ExitCode Entry();

    int CopyBuffer(unsigned char* out_buf);
};

#endif
