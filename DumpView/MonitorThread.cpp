#include "MonitorThread.h"
#include "DumpViewFrame.h"

wxMutex MonitorThread::s_mutexDataBuffer;
unsigned char MonitorThread::s_buf[BUF_SIZE] = {0};
int MonitorThread::s_nBufStartPos = 0;

wxThread::ExitCode MonitorThread::Entry()
{
    DWORD sizeRead;
    wxCommandEvent evt(wxEVT_THREAD_CALLBACK, wxID_ANY);

    m_NextState = m_NextState = MONITOR_STATE_RUNNING;

    while( !TestDestroy())
    {
        switch (m_CurrentState)
        {
        case MONITOR_STATE_STOPPED:
            if ( m_NextState == MONITOR_STATE_RUNNING)
            {
                if ( !m_InitSerialPort())
                {
                    return (wxThread::ExitCode)0;
                }
                m_CurrentState = MONITOR_STATE_RUNNING;
            }
            break;

        case MONITOR_STATE_RUNNING:
            if ( m_NextState == MONITOR_STATE_STOPPED)
            {
                m_ReleaseSerialPort();
                m_CurrentState = MONITOR_STATE_STOPPED;
            }
            else
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
            }
            break;

        default:
            break;
        }

        wxThread::This()->Sleep(1);
    }

    // Clean-up
    m_CurrentState = m_NextState = MONITOR_STATE_STOPPED;
    m_ReleaseSerialPort();

    return (wxThread::ExitCode)0;
}

bool MonitorThread::m_InitSerialPort()
{
    if ( m_PortNum <= 0 || m_PortNum > 8)
    {
        return false;
    }

    wxString port_name = wxString::Format( wxT("COM%d"), m_PortNum);
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

    dcb.BaudRate = m_BaudRate;
    dcb.ByteSize = m_ByteSize;
    dcb.Parity = m_Parity;
    dcb.StopBits = m_StopBit;

    if ( !::SetCommState( m_hSerialPort, &dcb))
    {
        //error_message = m_GetErrorString(::GetLastError());
        return false;
    }

    // Set timeout parameters
    COMMTIMEOUTS timeout = {0};
    timeout.ReadIntervalTimeout = 10;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.ReadTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 0;
    timeout.WriteTotalTimeoutConstant = 0;

    if (!::SetCommTimeouts( m_hSerialPort, &timeout))
    {
        return false;
    }

    return true;
}

bool MonitorThread::m_ReleaseSerialPort()
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
