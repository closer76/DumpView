// DumpViewFrame.cpp : implementation of the  class
//
#include "DumpViewFrame.h"
#include "ComSettingDialog.h"
#include "FileExistDialog.h"
#include "AboutDialog.h"
#include <wx/sizer.h>
#include <wx/fontdlg.h>
//#include <process.h>


unsigned char DumpViewFrame::m_textBuffer[XFER_BUF_SIZE] = {0};

const long DumpViewFrame::ID_RADIOBOX_SWITCH = wxNewId();
const long DumpViewFrame::ID_CHECKBOX_PAUSE = wxNewId();
const long DumpViewFrame::ID_CHECKBOX_REC = wxNewId();
const long DumpViewFrame::ID_BUTTON_CLEAR = wxNewId();
const long DumpViewFrame::ID_TEXT_DEFAULT_FOLDER = wxNewId();
const long DumpViewFrame::ID_BUTTON_SELECT_FILE = wxNewId();
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
    EVT_MENU( ID_MENU_SAVEAS, DumpViewFrame::OnSaveAs)
    EVT_MENU( ID_MENU_QUIT, DumpViewFrame::OnExit)
    EVT_MENU( ID_MENU_FIND , DumpViewFrame::OnFind)
    EVT_MENU( ID_MENU_COPY_ALL , DumpViewFrame::OnCopyAll)
    EVT_MENU( ID_MENU_COPY , DumpViewFrame::OnCopySelection)
    EVT_MENU( ID_MENU_SETCOM, DumpViewFrame::OnComPortSetting)
    EVT_MENU( ID_MENU_SETFONT , DumpViewFrame::OnFontSetting)
    EVT_MENU( ID_MENU_SETFOLDER , DumpViewFrame::OnFolderSetting)
    EVT_MENU( ID_MENU_ABOUT , DumpViewFrame::OnAbout)

    EVT_BUTTON( ID_BUTTON_CLEAR , DumpViewFrame::OnClear )
    EVT_BUTTON( ID_BUTTON_SELECT_FILE, DumpViewFrame::OnSelectFile )

    EVT_RADIOBOX( ID_RADIOBOX_SWITCH, DumpViewFrame::OnSwitchSelected)

    EVT_CHECKBOX( ID_CHECKBOX_REC, DumpViewFrame::OnRec)
    EVT_CHECKBOX( ID_CHECKBOX_PAUSE, DumpViewFrame::OnPause)
    ////Manual Code End
END_EVENT_TABLE()

const int SWITCH_ON = 0;
const int SWITCH_OFF = 1;

DumpViewFrame::DumpViewFrame(const wxString& title) : 
    wxFrame(NULL, wxID_ANY, title),
    m_IsRecording(false),
    m_fpLog(0),
    m_ResetPort(false),
    m_bufPause(0)
{
    //------- Initiate internal variables
    m_strDefaultPath = wxT(".");
    m_strDumpFilename = wxT("dump.txt");

    //------- Init buffer
    m_iCurPauseBufSize = 0;
    m_bufPause = new unsigned char[PAUSE_BUF_SIZE];

    //------- Set up UI
    wxPanel* panel = new wxPanel( this, wxID_ANY);
    wxBoxSizer* panelSizer = new wxBoxSizer( wxVERTICAL);
    panelSizer->Add( panel, 1, wxALL | wxEXPAND);
    SetSizer( panelSizer);
    panelSizer->SetSizeHints(this);

    m_InitMenuBar();

    m_InitSizedComponents(panel);

    m_InitStatusBar();

    this->SetSize( 800, 540);

    //------- Create the thread that monitors serial port
    m_PortMonitor = new MonitorThread(this);
    
    if (m_PortMonitor->Create() != wxTHREAD_NO_ERROR || m_PortMonitor->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_PortMonitor;
        m_PortMonitor = 0;
    }

    m_radioSwitch->Disable();
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
    m_menuCopyAll = new wxMenuItem(m_menuEdit, ID_MENU_COPY_ALL, _("Copy all\tCtrl-A"), wxEmptyString, wxITEM_NORMAL);
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

void DumpViewFrame::m_InitSizedComponents(wxWindow* parent)
{
    wxBoxSizer* BoxSizer2, *BoxSizer3, *BoxSizer4, *BoxSizer5;

    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    wxString __wxRadioBoxChoices_1[2] =
    {
    	_("On"),
    	_("Off")
    };
    m_radioSwitch = new wxRadioBox(parent, ID_RADIOBOX_SWITCH, _("Port Switch"), wxDefaultPosition, wxSize(82,71), 2, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX_SWITCH"));
    BoxSizer2->Add(m_radioSwitch, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    m_checkboxPause = new wxCheckBox(parent, ID_CHECKBOX_PAUSE, _("Pause"), wxDefaultPosition, wxSize(52,22), 0, wxDefaultValidator, _T("ID_CHECKBOX_PAUSE"));
    m_checkboxPause->SetValue(false);
    BoxSizer4->Add(m_checkboxPause, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    m_checkboxRec = new wxCheckBox(parent, ID_CHECKBOX_REC, _("Record"), wxDefaultPosition, wxSize(58,16), 0, wxDefaultValidator, _T("ID_CHECKBOX_REC"));
    m_checkboxRec->SetValue(false);
    BoxSizer4->Add(m_checkboxRec, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    m_buttonClear = new wxButton(parent, ID_BUTTON_CLEAR, _("Clear logs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_CLEAR"));
    BoxSizer4->Add(m_buttonClear, 0, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
    BoxSizer2->Add(BoxSizer4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    m_textLogFilename = new wxTextCtrl(parent, ID_TEXT_DEFAULT_FOLDER, _(""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXT_DEFAULT_FOLDER"));
    BoxSizer5->Add(m_textLogFilename, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_buttonSelectFile = new wxButton(parent, ID_BUTTON_SELECT_FILE, _("..."), wxDefaultPosition, wxSize(29,24), 0, wxDefaultValidator, _T("ID_BUTTON_SELECT_FILE"));
    BoxSizer5->Add(m_buttonSelectFile, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(BoxSizer5, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
#ifdef USE_RICH_EDIT
    m_OutputBox = new wxRichTextCtrl( this, ID_OUTPUT_BOX, wxEmptyString, wxDefaultPosition, wxSize(160,120), wxRE_READONLY | wxRE_MULTILINE);
#else
    m_OutputBox = new wxTextCtrl( parent, ID_OUTPUT_BOX, wxEmptyString, wxDefaultPosition, wxSize(160, 120), wxTE_MULTILINE | wxTE_READONLY);
#endif
    BoxSizer3->Add(m_OutputBox, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    parent->SetSizer(BoxSizer3);
    BoxSizer3->Fit(parent);
    BoxSizer3->SetSizeHints(parent);
}

void DumpViewFrame::m_InitStatusBar(void)
{
    m_statusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    m_statusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    m_statusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(m_statusBar1);
}

void DumpViewFrame::OnClose(wxCloseEvent& event)
{
    if ( m_PortMonitor && m_PortMonitor->IsAlive())
    {
        m_PortMonitor->Delete();
    }

    if ( m_bufPause)
    {
        delete [] m_bufPause;
        m_bufPause = 0;
    }

    if ( m_fpLog)
    {
        delete m_fpLog;
        m_fpLog = 0;
    }

    event.Skip();
}

void DumpViewFrame::OnThreadCallback(wxCommandEvent& evt)
{
    int data_read = 0;
    const char* buffer_full_message = "\n!!!<Buffer Full>!!!\n";

    switch ( evt.GetInt())
    {
    case MONITOR_EVENT_TYPE_DATAREADY:
        data_read = m_PortMonitor->CopyBuffer(m_textBuffer);
        if ( data_read >= XFER_BUF_SIZE)
            data_read = XFER_BUF_SIZE - 1;

        m_textBuffer[data_read] = 0;
        if ( data_read > 0)
        {
            if (m_state == STATE_PAUSE)
            {
                if ( m_iCurPauseBufSize + data_read < PAUSE_BUF_SIZE &&
                     m_iCurPauseBufSize + strlen( buffer_full_message) + 1 < PAUSE_BUF_SIZE)
                {
                    memcpy( m_bufPause+m_iCurPauseBufSize, m_textBuffer, sizeof(unsigned char)*data_read);
                    m_iCurPauseBufSize += data_read;
                }
                else
                {
                    memcpy( m_bufPause+m_iCurPauseBufSize, buffer_full_message, strlen( buffer_full_message));
                    m_iCurPauseBufSize += strlen(buffer_full_message);
                }
                m_bufPause[m_iCurPauseBufSize] = 0;
            }
            else
            {
                m_OutputBox->AppendText(wxString((char*)m_textBuffer, *wxConvCurrent));
#ifdef USE_RICH_EDIT
                m_statusBar1->SetStatusText( wxString::Format( wxT("(%d,%d,%d)"),
                    m_OutputBox->GetScrollPos( wxVERTICAL),
                    m_OutputBox->GetScrollRange( wxVERTICAL),
                    m_OutputBox->GetCaretPosition()));
                m_OutputBox->ScrollIntoView( m_OutputBox->GetCaretPosition(), WXK_DOWN);
#else
                m_OutputBox->ScrollPages(5);
#endif
            }

            if ( m_IsRecording)
            {
                // write messages into log file
                wxASSERT( m_fpLog);
                m_fpLog->Write( m_textBuffer, data_read);
            }
        }
        break;

    case MONITOR_EVENT_TYPE_STARTED:
        m_checkboxPause->Enable();
        m_radioSwitch->Enable();
        m_state = STATE_START;
        m_ResetPort = false;
        break;

    case MONITOR_EVENT_TYPE_STOPPED:
        m_checkboxPause->SetValue(false);
        m_radioSwitch->Enable();
        m_state = STATE_STOP;
        if ( m_ResetPort)
        {
            m_SwitchSelect_body( SWITCH_ON);
        }
        break;

    case MONITOR_EVENT_TYPE_INIT_FAILED:
        wxMessageBox( wxT("Specific port cannot be initialized.") );
        m_radioSwitch->Enable();
        m_radioSwitch->SetSelection( SWITCH_OFF);
        m_state = STATE_STOP;
        m_checkboxPause->Disable();
        break;
    }
}


void DumpViewFrame::OnSaveAs(wxCommandEvent& evt)
{
    wxFileDialog* dlg = new wxFileDialog(this, wxT("Save as..."), m_strDefaultPath, m_strDumpFilename, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (wxID_OK == dlg->ShowModal())
    {
        m_OutputBox->SaveFile( dlg->GetPath());

        m_strDefaultPath = dlg->GetDirectory();
    }
    dlg->Destroy();
}

void DumpViewFrame::OnExit(wxCommandEvent& evt)
{
    Close();
}

void DumpViewFrame::OnFind(wxCommandEvent& evt)
{
}

void DumpViewFrame::OnCopyAll(wxCommandEvent& evt)
{
    m_OutputBox->SetSelection( -1, -1);
    m_OutputBox->Copy();
    m_OutputBox->SetSelection( 0, 0);
}

void DumpViewFrame::OnCopySelection(wxCommandEvent& evt)
{
    m_OutputBox->Copy();
}

void DumpViewFrame::OnComPortSetting(wxCommandEvent &evt)
{
    ComSettingDialog* dlg = new ComSettingDialog(this);
    ComPortSetting setting = {0};
    ComPortSetting result = {0};

    m_PortMonitor->GetPortSettings( setting);
    dlg->SetPortSettings( setting);

    if ( dlg->ShowModal() == wxID_OK)
    {
        dlg->GetPortSettings(result);
        if ( setting.PortNum != result.PortNum ||
             setting.BaudRate != result.BaudRate ||
             setting.ByteSize != result.ByteSize ||
             setting.Parity != result.Parity ||
             setting.StopBit != result.StopBit)
        {
            m_PortMonitor->SetPortSettings( result);
            if ( m_PortMonitor->IsMonitoring())
            {
                m_ResetPort = true;
                m_SwitchSelect_body( SWITCH_OFF);
            }
        }
    }

    dlg->Destroy();
}

void DumpViewFrame::OnFontSetting(wxCommandEvent& evt)
{
    wxFontData font_data;

    font_data.SetInitialFont( m_OutputBox->GetFont());
    font_data.SetColour( m_OutputBox->GetForegroundColour());

    wxFontDialog* dlg = new wxFontDialog(this, font_data);

    if ( dlg->ShowModal() == wxID_OK)
    {
        m_OutputBox->SetFont( dlg->GetFontData().GetChosenFont());
        m_OutputBox->SetForegroundColour( dlg->GetFontData().GetColour());
    }

    dlg->Destroy();
}

void DumpViewFrame::OnFolderSetting(wxCommandEvent& evt)
{
}

void DumpViewFrame::OnAbout(wxCommandEvent& evt)
{
    AboutDialog* dlg = new AboutDialog(this);

    dlg->ShowModal();

    dlg->Destroy();
}

void DumpViewFrame::OnSwitchSelected(wxCommandEvent& evt)
{
    m_SwitchSelect_body( evt.GetInt());

}

void DumpViewFrame::m_SwitchSelect_body(int selection)
{
    switch( selection)
    {
    case SWITCH_ON:
        m_radioSwitch->Disable();
        if ( !m_PortMonitor->IsMonitoring())
            m_PortMonitor->StartMonitoring();
        break;

    case SWITCH_OFF:
        m_checkboxPause->Disable();
        m_radioSwitch->Disable();
        if ( m_PortMonitor->IsMonitoring())
            m_PortMonitor->StopMonitoring();
        break;
    }
}


void DumpViewFrame::OnRec(wxCommandEvent& evt)
{
    if ( m_IsRecording)
    {
        delete m_fpLog;
        m_fpLog = 0;
        m_IsRecording = false;
    }
    else
    {
        wxFileName filename( m_textLogFilename->GetValue());
        if ( filename.GetFullName() == wxT(""))
        {
            m_SelectFile_body(false);
            filename.Assign( m_textLogFilename->GetValue());
        }

        wxASSERT(m_fpLog == 0);
        wxFile::OpenMode open_mode = wxFile::write;
        if ( filename.FileExists())
        {
            FileExistDialog* dlg = new FileExistDialog(this);
            dlg->SetFilename( filename.GetFullPath());
            int result = dlg->ShowModal();
            dlg->Destroy();

            if ( result == FILEEXIST_APPEND)
            {
                open_mode = wxFile::write_append;
            }
            else if ( result == FILEEXIST_CANCEL)
            {
                m_IsRecording = false;
                m_checkboxRec->SetValue(false);
                return;
            }
        }

        m_fpLog = new wxFile( filename.GetFullPath().c_str(), open_mode);
        if ( !m_fpLog->IsOpened())
        {
            wxMessageBox( _("Cannot open file to write."));
            delete m_fpLog;
            m_fpLog = 0;
            m_IsRecording = false;
            m_checkboxRec->SetValue(false);
        }
        else
        {
            m_IsRecording = true;
        }
    }
}

void DumpViewFrame::OnSelectFile(wxCommandEvent& evt)
{
    m_SelectFile_body(false);
}

void DumpViewFrame::m_SelectFile_body( bool prompt_overwrite)
{
    wxFileDialog* dlg = new wxFileDialog(
        this,
        wxT("Select file to store logs..."),
        m_strDefaultPath,
        m_strDumpFilename,
        wxT("*.*"),
        wxFD_SAVE | (prompt_overwrite?wxFD_OVERWRITE_PROMPT:0));

    if ( wxID_OK == dlg->ShowModal())
    {
        m_strDefaultPath = dlg->GetDirectory();
        m_textLogFilename->ChangeValue( dlg->GetPath());
    }

    dlg->Destroy();
}

void DumpViewFrame::OnPause(wxCommandEvent& evt)
{
    if ( evt.GetSelection() == false)
    {
        if ( m_state == STATE_PAUSE)
        {
            m_OutputBox->AppendText(wxString((char*)m_bufPause, *wxConvCurrent));
            m_state = STATE_START;
            m_iCurPauseBufSize = 0;
            m_bufPause[0] = 0;
        }
    }
    else
    {
        if ( m_state == STATE_START)
        {
            m_iCurPauseBufSize = 0;
            m_bufPause[0] = 0;
            m_state = STATE_PAUSE;
        }
    }
}

void DumpViewFrame::OnClear(wxCommandEvent& evt)
{
    m_OutputBox->Clear();
}