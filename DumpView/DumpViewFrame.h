// DumpViewFrame.h : interface of the  class
//
#ifndef __DumpViewFrame_H__
#define __DumpViewFrame_H__

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/frame.h>
#else
    #include <wx/wxprec.h>
#endif

#include <wx/richtext/richtextctrl.h>
#include "SerialCommHelper.h"

enum
{
    ID_OUTPUT_BOX   = 1000
};

DECLARE_EVENT_TYPE( wxEVT_THREAD_CALLBACK, -1)

class DumpViewFrame : public wxFrame
{
public:
    DumpViewFrame(const wxString& title);

private:
    wxRichTextCtrl* m_OutputBox;
    HANDLE m_hSerialPort;
    CSerialCommHelper m_SerialHelper;

    HANDLE m_hTerminate;
    HANDLE m_hRxDataReady;
    HANDLE m_hThreadMonitor;

    CRITICAL_SECTION m_CriticalSection;

    void OnClose(wxCloseEvent& event);
    void OnDataReady(void);
    void OnThreadCallback(wxCommandEvent& evt);

    bool m_InitSerialPort( int port_number, wxString& err_message);
    void m_ReleaseSerialPort( int port_number);
    wxString m_GetErrorString( DWORD errId);

    static unsigned __stdcall m_threadMonitor(void* app);

    DECLARE_EVENT_TABLE();
};

#endif
