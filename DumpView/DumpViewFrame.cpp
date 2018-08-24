// DumpViewFrame.cpp : implementation of the  class
//
#include "DumpViewFrame.h"
#include <wx/sizer.h>
#include <process.h>


unsigned char DumpViewFrame::m_textBuffer[BUF_SIZE] = {0};

const long DumpViewFrame::ID_BUTTON_START = wxNewId();
const long DumpViewFrame::ID_BUTTON_STOP = wxNewId();
const long DumpViewFrame::ID_BUTTON_REC = wxNewId();
const long DumpViewFrame::ID_BUTTON_PAUSE = wxNewId();
const long DumpViewFrame::ID_BUTTON5 = wxNewId();
const long DumpViewFrame::ID_TEXT_DEFAULT_FOLDER = wxNewId();
const long DumpViewFrame::ID_OUTPUT_BOX = wxNewId();
const long DumpViewFrame::ID_MENU_SAVEAS = wxNewId();
const long DumpViewFrame::ID_MENU_QUIT = wxNewId();
const long DumpViewFrame::ID_MENU_FIND = wxNewId();
const long DumpViewFrame::ID_MENU_COPY_ALL = wxNewId();
const long DumpViewFrame::ID_MENU_COPY = wxNewId();
const long DumpViewFrame::ID_MENU_SETCOM = wxNewId();
const long DumpViewFrame::ID_MENU_SETFONT = wxNewId();
const long DumpViewFrame::ID_MENU_SETFOLDER = wxNewId();
const long DumpViewFrame::ID_MENU_ABOUT = wxNewId();

DEFINE_EVENT_TYPE( wxEVT_THREAD_CALLBACK)

BEGIN_EVENT_TABLE(DumpViewFrame, wxFrame)
    ////Manual Code Start
    EVT_CLOSE(DumpViewFrame::OnClose)
    EVT_COMMAND( wxID_ANY, wxEVT_THREAD_CALLBACK, DumpViewFrame::OnThreadCallback)
    ////Manual Code End
END_EVENT_TABLE()

DumpViewFrame::DumpViewFrame(const wxString& title) : 
    wxFrame(NULL, wxID_ANY, title),
    m_OutputBox(0),
    m_PortMonitor(0)
{
    //------- Set up UI
    m_OutputBox = new wxRichTextCtrl( this, ID_OUTPUT_BOX, wxEmptyString, wxDefaultPosition, wxSize(160,120), wxRE_READONLY | wxRE_MULTILINE);

    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

    topSizer->Add(m_OutputBox, 1, wxALL | wxEXPAND);

    SetSizer(topSizer);
    topSizer->SetSizeHints(this);

    //------- Create the thread that monitors serial port
    m_PortMonitor = new MonitorThread(this);
    
    if (m_PortMonitor->Create() != wxTHREAD_NO_ERROR || m_PortMonitor->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_PortMonitor;
        m_PortMonitor = 0;
    }
}

void DumpViewFrame::OnClose(wxCloseEvent& event)
{
    if ( m_PortMonitor && m_PortMonitor->IsAlive())
    {
        m_PortMonitor->Delete();
    }
    delete m_OutputBox;

    event.Skip();
}

void DumpViewFrame::OnThreadCallback(wxCommandEvent& evt)
{
//    std::string data;
    int data_read = m_PortMonitor->CopyBuffer(m_textBuffer);
    if ( data_read >= BUF_SIZE)
        data_read = BUF_SIZE - 1;

    m_textBuffer[data_read] = 0;
    if ( data_read > 0)
    {
        m_OutputBox->AppendText(wxString((char*)m_textBuffer, *wxConvCurrent));
    }
}
