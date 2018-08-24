// DumpViewFrame.cpp : implementation of the  class
//
#include "DumpViewFrame.h"
#include <wx/sizer.h>
#include <process.h>

BEGIN_EVENT_TABLE(DumpViewFrame, wxFrame)
    ////Manual Code Start
    EVT_CLOSE(DumpViewFrame::OnClose)
    ////Manual Code End
END_EVENT_TABLE()

DumpViewFrame::DumpViewFrame(const wxString& title) : 
    wxFrame(NULL, wxID_ANY, title),
    m_OutputBox(0),
    m_hSerialPort(INVALID_HANDLE_VALUE),
    m_SerialHelper(),
    m_hThreadMonitor(INVALID_HANDLE_VALUE)
{
    //------- Set up UI
    m_OutputBox = new wxRichTextCtrl( this, ID_OUTPUT_BOX, wxEmptyString, wxDefaultPosition, wxSize(160,120), wxRE_READONLY | wxRE_MULTILINE);

    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

    topSizer->Add(m_OutputBox, 1, wxALL | wxEXPAND);

    SetSizer(topSizer);
    topSizer->SetSizeHints(this);

    //-------- Initialize serial port
    /*
    wxString err_msg;
    if ( m_InitSerialPort(1, err_msg))
    {
        m_OutputBox->AppendText( wxT("COM1 initialize OK.\n"));
    }
    else
    {
        m_OutputBox->AppendText( wxString::Format( wxT("There might be something wrong: %s\n"), err_msg));
    }
    */

    ::InitializeCriticalSection( &m_CriticalSection);

    if ( S_OK == m_SerialHelper.Init( wxT("COM1"), 115200))
    {
        m_hTerminate = ::CreateEvent( 0, 0, 0, 0);

        m_hThreadMonitor = (HANDLE)_beginthreadex( 0, 0, &DumpViewFrame::m_threadMonitor, (void*) this, 0, 0);

        m_SerialHelper.Start();
    }
}

bool DumpViewFrame::m_InitSerialPort( int port_number, wxString& error_message)
{
    wxString port_name = wxT("COM1");
    error_message = wxT("");

    wxChar err_str[128];

    err_str[0] = wxT('\0');

    m_hSerialPort = ::CreateFile(
        port_name.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        0);

    if ( m_hSerialPort == INVALID_HANDLE_VALUE)
    {
        error_message = m_GetErrorString(::GetLastError());
        return false;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);

    if ( !::GetCommState( m_hSerialPort, &dcb))
    {
        error_message = m_GetErrorString(::GetLastError());
        return false;
    }

    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if ( !::SetCommState( m_hSerialPort, &dcb))
    {
        error_message = m_GetErrorString(::GetLastError());
        return false;
    }

    return true;
}

void DumpViewFrame::m_ReleaseSerialPort( int port_number)
{
    if ( m_hSerialPort != INVALID_HANDLE_VALUE)
    {
        CloseHandle( m_hSerialPort);
        m_hSerialPort = INVALID_HANDLE_VALUE;
    }

    m_SerialHelper.UnInit();
}

wxString DumpViewFrame::m_GetErrorString( DWORD errId)
{
    const int BUF_LEN = 128;
    wxChar err_str[BUF_LEN];

    ::FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM,
        0,
        errId,
        0,
        err_str,
        BUF_LEN,
        0);

    return wxString(err_str);
}

void DumpViewFrame::OnClose(wxCloseEvent& event)
{
    if ( m_hThreadMonitor != INVALID_HANDLE_VALUE)
    {
        m_SerialHelper.Stop();

        SetEvent( m_hTerminate);
        WaitForSingleObject( m_hThreadMonitor, INFINITE);

        CloseHandle( m_hThreadMonitor);
        m_hThreadMonitor = INVALID_HANDLE_VALUE;

        m_SerialHelper.UnInit();
    }
    delete m_OutputBox;
//    m_ReleaseSerialPort(1);

    event.Skip();
}

unsigned __stdcall DumpViewFrame::m_threadMonitor( void* app_void)
{
    DumpViewFrame* app = (DumpViewFrame*) app_void;
    HANDLE events[2] = {0};
    DWORD wait;

    events[0] = app->m_hTerminate;
    events[1] = app->m_SerialHelper.GetWaitForEvent();

    while (true)
    {
        wait = ::WaitForMultipleObjects( 2, events, false, INFINITE);
        switch (wait)
        {
        case WAIT_OBJECT_0:
            ::_endthreadex(0);
            break;

        case WAIT_OBJECT_0 + 1:
            app->OnDataReady();
            ResetEvent(events[1]);
            break;
        }
    }
    return 0;
}

void DumpViewFrame::OnDataReady(void)
{
    std::string data;
    if ( S_OK == m_SerialHelper.ReadAvailable( data))
    {
        ::EnterCriticalSection( &m_CriticalSection);
        m_OutputBox->AppendText(wxString(data.c_str(), *wxConvCurrent));
        ::LeaveCriticalSection( &m_CriticalSection);
    }
}
