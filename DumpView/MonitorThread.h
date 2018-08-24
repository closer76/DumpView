#ifndef __MONITOR_THREAD_H__
#define __MONITOR_THREAD_H__

#include "wx/thread.h"
#include "wx/event.h"

class MonitorThread :
    public wxThread
{
private:
    wxEvtHandler* m_pParent;
public:
    MonitorThread(wxEvtHandler* pParent):
        wxThread(),
        m_pParent(pParent) {}

    wxThread::ExitCode Entry();
};

#endif
