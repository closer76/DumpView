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
const long DumpViewFrame::ID_STATUSBAR1 = wxNewId();

DEFINE_EVENT_TYPE( wxEVT_THREAD_CALLBACK)

BEGIN_EVENT_TABLE(DumpViewFrame, wxFrame)
    ////Manual Code Start
    EVT_CLOSE(DumpViewFrame::OnClose)
    EVT_COMMAND( wxID_ANY, wxEVT_THREAD_CALLBACK, DumpViewFrame::OnThreadCallback)
    EVT_MENU( ID_MENU_QUIT, DumpViewFrame::OnExit)
    ////Manual Code End
END_EVENT_TABLE()

DumpViewFrame::DumpViewFrame(const wxString& title) : 
    wxFrame(NULL, wxID_ANY, title),
    m_OutputBox(0),
    m_PortMonitor(0)
{
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;

    //------- Set up UI
    m_InitMenuBar();

    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    m_buttonStart = new wxButton(this, ID_BUTTON_START, _("Start"), wxDefaultPosition, wxSize(40,40), 0, wxDefaultValidator, _T("ID_BUTTON_START"));
    BoxSizer2->Add(m_buttonStart, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_buttonStop = new wxButton(this, ID_BUTTON_STOP, _("Stop"), wxDefaultPosition, wxSize(40,40), 0, wxDefaultValidator, _T("ID_BUTTON_STOP"));
    BoxSizer2->Add(m_buttonStop, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_buttonRec = new wxButton(this, ID_BUTTON_REC, _("Rec"), wxDefaultPosition, wxSize(40,40), 0, wxDefaultValidator, _T("ID_BUTTON_REC"));
    BoxSizer2->Add(m_buttonRec, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_buttonPause = new wxButton(this, ID_BUTTON_PAUSE, _("Pause"), wxDefaultPosition, wxSize(40,40), 0, wxDefaultValidator, _T("ID_BUTTON_PAUSE"));
    BoxSizer2->Add(m_buttonPause, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_buttonClear = new wxButton(this, ID_BUTTON5, _("Clear logs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer2->Add(m_buttonClear, 1, wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_textDefaultFolder = new wxTextCtrl(this, ID_TEXT_DEFAULT_FOLDER, _(""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXT_DEFAULT_FOLDER"));
    BoxSizer1->Add(m_textDefaultFolder, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_OutputBox = new wxRichTextCtrl( this, ID_OUTPUT_BOX, wxEmptyString, wxDefaultPosition, wxSize(160,120), wxRE_READONLY | wxRE_MULTILINE);
    BoxSizer1->Add(m_OutputBox, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    m_statusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    m_statusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    m_statusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(m_statusBar1);

    //------- Create the thread that monitors serial port
    m_PortMonitor = new MonitorThread(this);
    
    if (m_PortMonitor->Create() != wxTHREAD_NO_ERROR || m_PortMonitor->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_PortMonitor;
        m_PortMonitor = 0;
    }
}

void DumpViewFrame::m_InitMenuBar(void)
{
    wxMenuBar* MenuBar1;
    wxMenu* m_menuFile;
    wxMenu* m_menuHelp;
    wxMenuItem* m_menuAbout;
    wxMenuItem* m_menuExit;

    MenuBar1 = new wxMenuBar();
    m_menuFile = new wxMenu();
    m_menuSaveAs = new wxMenuItem(m_menuFile, ID_MENU_SAVEAS, _("Save &as...\tCtrl-S"), wxEmptyString, wxITEM_NORMAL);
    m_menuFile->Append(m_menuSaveAs);
    m_menuFile->AppendSeparator();
    m_menuExit = new wxMenuItem(m_menuFile, ID_MENU_QUIT, _("E&xit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    m_menuFile->Append(m_menuExit);
    MenuBar1->Append(m_menuFile, _("&File"));
    m_menuEdit = new wxMenu();
    m_menuFind = new wxMenuItem(m_menuEdit, ID_MENU_FIND, _("&Find...\tCtrl-F"), wxEmptyString, wxITEM_NORMAL);
    m_menuEdit->Append(m_menuFind);
    m_menuCopyAll = new wxMenuItem(m_menuEdit, ID_MENU_COPY_ALL, _("Copy all"), wxEmptyString, wxITEM_NORMAL);
    m_menuEdit->Append(m_menuCopyAll);
    m_menuCopy = new wxMenuItem(m_menuEdit, ID_MENU_COPY, _("Copy selected\tCtrl-C"), wxEmptyString, wxITEM_NORMAL);
    m_menuEdit->Append(m_menuCopy);
    MenuBar1->Append(m_menuEdit, _("Edit"));
    m_menuSettings = new wxMenu();
    m_menuSetCom = new wxMenuItem(m_menuSettings, ID_MENU_SETCOM, _("COM port..."), wxEmptyString, wxITEM_NORMAL);
    m_menuSettings->Append(m_menuSetCom);
    m_menuSetFont = new wxMenuItem(m_menuSettings, ID_MENU_SETFONT, _("Display font..."), wxEmptyString, wxITEM_NORMAL);
    m_menuSettings->Append(m_menuSetFont);
    m_menuSetFolder = new wxMenuItem(m_menuSettings, ID_MENU_SETFOLDER, _("Default log folder..."), wxEmptyString, wxITEM_NORMAL);
    m_menuSettings->Append(m_menuSetFolder);
    MenuBar1->Append(m_menuSettings, _("&Settings"));
    m_menuHelp = new wxMenu();
    m_menuAbout = new wxMenuItem(m_menuHelp, ID_MENU_ABOUT, _("About...\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    m_menuHelp->Append(m_menuAbout);
    MenuBar1->Append(m_menuHelp, _("Help"));
    SetMenuBar(MenuBar1);
}

void DumpViewFrame::OnExit(wxCommandEvent& evt)
{
    Close();
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
