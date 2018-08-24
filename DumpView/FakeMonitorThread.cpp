#include "FakeMonitorThread.h"
#include "DumpViewFrame.h"
#include <wx/filedlg.h>

wxThread::ExitCode FakeMonitorThread::Entry()
{
 //   DWORD sizeRead;
//    int retry_count;
    wxCommandEvent evt(wxEVT_THREAD_CALLBACK, wxID_ANY);

    evt.SetInt(MONITOR_EVENT_TYPE_NONE);
    m_NextState = MONITOR_STATE_RUNNING;

    while( !TestDestroy())
    {
        switch (m_CurrentState)
        {
        case MONITOR_STATE_STOPPED:
            if ( m_NextState == MONITOR_STATE_RUNNING)
            {
				// open file...
				wxFileDialog dlg( m_pParentWnd);
				if ( wxID_OK == dlg.ShowModal())
				{
					m_streamInput = new wxFileInputStream( dlg.GetPath());
					if ( m_streamInput && m_streamInput->IsOk())
					{
						m_CurrentState = MONITOR_STATE_RUNNING;
						m_NextState = MONITOR_STATE_RUNNING;
						evt.SetInt(MONITOR_EVENT_TYPE_STARTED);
					}
					else
					{
						m_NextState = MONITOR_STATE_STOPPED;
						evt.SetInt(MONITOR_EVENT_TYPE_INIT_FAILED);
					}
				}
				else
                {
                    m_NextState = MONITOR_STATE_STOPPED;
                    evt.SetInt(MONITOR_EVENT_TYPE_INIT_FAILED);
                }
            }
            break;

        case MONITOR_STATE_RUNNING:
            if ( m_NextState == MONITOR_STATE_STOPPED)
            {
				delete m_streamInput;
				m_streamInput = 0;
                m_CurrentState = MONITOR_STATE_STOPPED;
                evt.SetInt(MONITOR_EVENT_TYPE_STOPPED);
            }
            else
            {
				//this->Start(1000);
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
	delete m_streamInput;
	m_streamInput = 0;

    return (wxThread::ExitCode)0;
}

void FakeMonitorThread::Notify()
{
    wxCommandEvent evt(wxEVT_THREAD_CALLBACK, wxID_ANY);

	if ( m_streamInput && m_streamInput->IsOk())
	{
		evt.SetInt( MONITOR_EVENT_TYPE_DATAREADY);
        m_pParent->AddPendingEvent(evt);
        evt.SetInt( MONITOR_EVENT_TYPE_NONE);
    }
}

int FakeMonitorThread::CopyBuffer(char* out_buf)
{
	m_streamInput->Read( out_buf, 200);

    return 200;
}

std::list<int>* FakeMonitorThread::GetAvailableComPorts(int max)
{
	return &this->m_AvailComPorts;
}

void FakeMonitorThread::GetPortSettings( ComPortSetting& settings)
{
    settings.PortNum = 1;
    settings.BaudRate = 115200;
    settings.ByteSize = 1;
    settings.Parity = 0;
    settings.StopBit = 1;
}

void FakeMonitorThread::SetPortSettings( const ComPortSetting& settings)
{
}

