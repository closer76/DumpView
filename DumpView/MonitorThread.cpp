#include "MonitorThread.h"
#include "DumpViewFrame.h"

wxMutex MonitorThread::s_mutexDataBuffer;
unsigned char MonitorThread::s_buf[BUF_SIZE] = {0};
int MonitorThread::s_nBufStartPos = 0;

wxThread::ExitCode MonitorThread::Entry()
{
    DWORD sizeRead;
    wxCommandEvent evt(wxEVT_THREAD_CALLBACK, wxID_ANY);

    if ( !m_InitSerialPort(1))
    {
        return (wxThread::ExitCode)0;
    }

    while( !TestDestroy())
    {
        sizeRead = 0;
        s_mutexDataBuffer.Lock();
        if (!::ReadFile(m_hSerialPort, s_buf+s_nBufStartPos, BUF_SIZE-s_nBufStartPos, &sizeRead, NULL))
        {
            ::MessageBox( NULL, wxT("Read file error"), wxT("Error"), MB_OK);
        }
        s_nBufStartPos += sizeRead;
        if ( s_nBufStartPos >= 0)
            m_pParent->AddPendingEvent( evt);
        s_mutexDataBuffer.Unlock();

        wxThread::This()->Sleep(1);
    }

    // Clean-up
    m_ReleaseSerialPort(1);

    return (wxThread::ExitCode)0;
}

bool MonitorThread::m_InitSerialPort(int portNum)
{
    wxString port_name = wxT("COM1");
    //error_message = wxT("");

    wxChar err_str[128];

    err_str[0] = wxT('\0');

    m_hSerialPort = ::CreateFile(
        port_name.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);

    if ( m_hSerialPort == INVALID_HANDLE_VALUE)
    {
        //error_message = m_GetErrorString(::GetLastError());
        return false;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);

    if ( !::GetCommState( m_hSerialPort, &dcb))
    {
        //error_message = m_GetErrorString(::GetLastError());
        return false;
    }

    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if ( !::SetCommState( m_hSerialPort, &dcb))
    {
        //error_message = m_GetErrorString(::GetLastError());
        return false;
    }

    return true;
}

bool MonitorThread::m_ReleaseSerialPort( int portNum)
{
    if ( m_hSerialPort != INVALID_HANDLE_VALUE)
    {
        CloseHandle( m_hSerialPort);
        m_hSerialPort = INVALID_HANDLE_VALUE;
    }

    return true;
}

int MonitorThread::CopyBuffer(unsigned char* out_buf)
{
    int count;

    s_mutexDataBuffer.Lock();
    memcpy( out_buf, s_buf, s_nBufStartPos);
    count = s_nBufStartPos;
    s_nBufStartPos = 0;
    s_mutexDataBuffer.Unlock();

    return count;
}
