#include "MonitorThread.h"
#include "DumpViewFrame.h"

wxMutex MonitorThread::s_mutexDataBuffer;
char MonitorThread::s_buf[XFER_BUF_SIZE] = {0};
int MonitorThread::s_nBufStartPos = 0;

wxThread::ExitCode MonitorThread::Entry()
{
    DWORD sizeRead;
    int retry_count;
    wxCommandEvent evt(wxEVT_THREAD_CALLBACK, wxID_ANY);

    evt.SetInt(MONITOR_EVENT_TYPE_NONE);
    m_NextState = m_NextState = MONITOR_STATE_RUNNING;

    while( !TestDestroy())
    {
        switch (m_CurrentState)
        {
        case MONITOR_STATE_STOPPED:
            if ( m_NextState == MONITOR_STATE_RUNNING)
            {
                for ( retry_count = 0; retry_count < 5; retry_count++)
                {
                    if ( m_InitSerialPort())
                    {
                        m_CurrentState = MONITOR_STATE_RUNNING;
                        evt.SetInt(MONITOR_EVENT_TYPE_STARTED);
                        break;
                    }
                    else
                    {
                        m_ReleaseSerialPort();
                        wxThread::This()->Sleep(100);
                    }
                }
                
                if ( retry_count == 5)
                {
                    m_NextState = MONITOR_STATE_STOPPED;
                    evt.SetInt(MONITOR_EVENT_TYPE_INIT_FAILED);
                }
            }
            break;

        case MONITOR_STATE_RUNNING:
            if ( m_NextState == MONITOR_STATE_STOPPED)
            {
                m_ReleaseSerialPort();
                m_CurrentState = MONITOR_STATE_STOPPED;
                evt.SetInt(MONITOR_EVENT_TYPE_STOPPED);
            }
            else
            {
                sizeRead = 0;
                s_mutexDataBuffer.Lock();
                if (::ReadFile(m_hSerialPort, s_buf+s_nBufStartPos, XFER_BUF_SIZE-s_nBufStartPos, &sizeRead, NULL))
                {
                    s_nBufStartPos += sizeRead;
                    if ( s_nBufStartPos >= 0)
                    {
                        evt.SetInt( MONITOR_EVENT_TYPE_DATAREADY);
                    }
                }
                else
                {
                    // ReadFile() returns errors. Report error code to primary thread.
                    m_ErrorCode = ::GetLastError();
                    m_ReleaseSerialPort();
                    m_CurrentState = MONITOR_STATE_STOPPED;
                    if ( m_ErrorCode == ERROR_OPERATION_ABORTED)
                    {
                        // Ignore these errors and restart
                        m_ErrorCode = ERROR_SUCCESS;
                        m_NextState = MONITOR_STATE_RUNNING;
                    }
                    else
                    {
                        m_NextState = MONITOR_STATE_STOPPED;
                        evt.SetInt( MONITOR_EVENT_TYPE_TERMINATED);
                    }
                }
                s_mutexDataBuffer.Unlock();
            }
            break;

        default:
            break;
        }

        if (evt.GetInt() != MONITOR_EVENT_TYPE_NONE)
        {
            m_pParent->AddPendingEvent(evt);
            evt.SetInt( MONITOR_EVENT_TYPE_NONE);
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
    if ( m_PortNum <= 0 || m_PortNum > m_MaxPortNum)
    {
        return false;
    }

    wxString port_name = wxString::Format( wxT("\\\\.\\COM%d"), m_PortNum);

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
        m_ErrorCode = ::GetLastError();
        return false;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);

    if ( !::GetCommState( m_hSerialPort, &dcb))
    {
        m_ErrorCode = ::GetLastError();
        return false;
    }

    dcb.BaudRate = (-1 == m_BaudRate) ? (m_ManualBaudRate) : (m_BaudRate);
    dcb.ByteSize = m_ByteSize;
    dcb.Parity = m_Parity;
    dcb.StopBits = m_StopBit;

    if ( !::SetCommState( m_hSerialPort, &dcb))
    {
        m_ErrorCode = ::GetLastError();
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
        m_ErrorCode = ::GetLastError();
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

int MonitorThread::CopyBuffer(char* out_buf)
{
    int count;

    s_mutexDataBuffer.Lock();
    memcpy( out_buf, s_buf, s_nBufStartPos);
    count = s_nBufStartPos;
    s_nBufStartPos = 0;
    s_mutexDataBuffer.Unlock();

    return count;
}

std::list<int>* MonitorThread::GetAvailableComPorts()
{
	// Rescan available com ports
	COMMCONFIG cc;
	DWORD size = sizeof(cc);

	m_AvailComPorts.clear();

	for ( int i = 0; i <= m_MaxPortNum; i++)
	{
		if ( ::GetDefaultCommConfig( wxString::Format( wxT("COM%d"), i).c_str(), &cc, &size))
		{
			m_AvailComPorts.push_back(i);
		}
	}

	return &m_AvailComPorts;
}

void MonitorThread::GetPortSettings(ComPortSetting &settings)
{
    settings.PortNum = m_PortNum;
    settings.BaudRate = m_BaudRate;
	settings.ManualBaudRate = m_ManualBaudRate;
    settings.ByteSize = m_ByteSize;
    settings.Parity = m_Parity;
    settings.StopBit = m_StopBit;
}

void MonitorThread::SetPortSettings(const ComPortSetting &settings)
{
    m_PortNum = settings.PortNum;
    m_BaudRate = settings.BaudRate;
	m_ManualBaudRate = settings.ManualBaudRate;
    m_ByteSize = settings.ByteSize;
    m_Parity = settings.Parity;
    m_StopBit = settings.StopBit;
}