// DumpViewFrame.cpp : implementation of the  class
//
#include "version.h"
#include "DumpViewFrame.h"
#include "ComSettingDialog.h"
#include "FileExistDialog.h"
#include "AboutDialog.h"
#include <wx/sizer.h>
#include <wx/fontdlg.h>
#include <wx/textfile.h>
#include <wx/valnum.h>
//#include <process.h>


char DumpViewFrame::m_textBuffer[XFER_BUF_SIZE] = {0};
wchar_t DumpViewFrame::m_wcTextBuffer[XFER_BUF_SIZE] = {0};

const long DumpViewFrame::ID_MENU_OPEN = wxNewId();
const long DumpViewFrame::ID_MENU_SAVEAS = wxNewId();
const long DumpViewFrame::ID_MENU_QUIT = wxNewId();
const long DumpViewFrame::ID_MENU_COPY_ALL = wxNewId();
const long DumpViewFrame::ID_MENU_COPY = wxNewId();
const long DumpViewFrame::ID_MENU_SETCOM = wxNewId();
const long DumpViewFrame::ID_MENU_SETFONT = wxNewId();
const long DumpViewFrame::ID_MENU_SETFOLDER = wxNewId();
const long DumpViewFrame::ID_MENU_LOADGUIDDEF = wxNewId();
const long DumpViewFrame::ID_MENU_ABOUT = wxNewId();
const long DumpViewFrame::ID_MENU_CLEAR = wxNewId();

wxDEFINE_EVENT(klEVT_THREAD_CALLBACK, wxCommandEvent);

const int SWITCH_ON = 0;
const int SWITCH_OFF = 1;

// Used for registry settings
const wxString REG_WINDOW_WIDTH     = "Window Width";
const wxString REG_WINDOW_HEIGHT    = "Window Height";
const wxString REG_POS_X            = "Position X";
const wxString REG_POS_Y            = "Position Y";
const wxString REG_COM_NUM          = "COM Number";
const wxString REG_BAUD_RATE        = "BAUD Rate";
const wxString REG_MANUAL_BAUD_RATE = "Manual Baud Rate";
const wxString REG_PARITY           = "Parity";
const wxString REG_BYTE_SIZE        = "ByteSize";
const wxString REG_STOP_BIT         = "StopBit";
const wxString REG_FONT             = "Font";
const wxString REG_FONT_COLOR       = "FontColor";
const wxString REG_DEFAULT_LOG_DIR  = "DefaultLogDir";
const wxString REG_LAST_LOG_DIR     = "LastVisitedLogDir";
const wxString REG_USE_LAST_DIR     = "UseLastLogDir";
const wxString REG_GUID_DEF_FILE	= "GuidDefFile";
const wxString REG_GUID_AUTO_LOAD	= "GuidAutoLoad";
const wxString REG_RETAIN_TAIL_LINES	= "RetainOnlyTailLines";
const wxString REG_TAIL_COUNT		= "TailCount";

const int STATUS_COLS = 3;

DumpViewFrame::DumpViewFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
	, m_IsRecording(false)
	, m_fpLog(0)
	, m_ResetPort(false)
	, m_bufPause(0)
	, m_iTailCount(-1)
	, m_GuidDefSettings(0)
	, m_LogDirSettings(0)
	, m_pAppConfig(0)
	, m_sizeTopWindow(0, 0)
	, m_menuPopup(nullptr)
{
    int pos_x = -1, pos_y = -1;

    //------- Initiate internal variables
	m_strDefaultPath = ".";
    m_strDumpFilename = "dump.txt";

    //------- Init buffer
    m_iCurPauseBufSize = 0;
    m_bufPause = new unsigned char[PAUSE_BUF_SIZE];

    //------- Set up UI
    SetIcon( wxIcon("mondrian", wxBITMAP_TYPE_ICO_RESOURCE));

    wxPanel* panel = new wxPanel( this, wxID_ANY);
    wxBoxSizer* panelSizer = new wxBoxSizer( wxVERTICAL);
    panelSizer->Add( panel, 1, wxALL | wxEXPAND);
    SetSizer( panelSizer);
    panelSizer->SetSizeHints(this);

    m_InitMenuBar();

    m_InitSizedComponents(panel);

    m_InitStatusBar();

	m_InitEventHandlers();

    //------- Get size & position setting from registry
    m_pAppConfig = new wxConfig(APP_NAME);
    m_sizeTopWindow.x = m_pAppConfig->Read( REG_WINDOW_WIDTH, static_cast<int>(800));
    m_sizeTopWindow.y = m_pAppConfig->Read( REG_WINDOW_HEIGHT, static_cast<int>(540));
    pos_x = m_pAppConfig->Read( REG_POS_X, static_cast<int>(-1));
    pos_y = m_pAppConfig->Read( REG_POS_Y, static_cast<int>(-1));

    SetSize( m_sizeTopWindow);
    if ( pos_x >= 0 && pos_y >= 0)
    {
        this->SetPosition( wxPoint(pos_x, pos_y));
    }

    //------- Get port settings from registry
    ComPortSetting settings;
    settings.PortNum = m_pAppConfig->Read( REG_COM_NUM, static_cast<int>(1));
    settings.BaudRate = m_pAppConfig->Read( REG_BAUD_RATE, static_cast<int>(CBR_115200));
	settings.ManualBaudRate = m_pAppConfig->Read( REG_MANUAL_BAUD_RATE, static_cast<int>(CBR_115200));
    settings.Parity = m_pAppConfig->Read( REG_PARITY, static_cast<long>(NOPARITY));
    settings.ByteSize = m_pAppConfig->Read( REG_BYTE_SIZE, static_cast<int>(8));
    settings.StopBit = m_pAppConfig->Read( REG_STOP_BIT, static_cast<long>(ONESTOPBIT));
    m_ShowPortInfoOnStatusBar( settings);

    //------- Get font info from registry
    if ( m_OutputBox)
    {
        wxString tmp_str = m_pAppConfig->Read( REG_FONT, wxEmptyString);
        if ( tmp_str != wxEmptyString)
        {
            wxFont display_font;
            display_font.SetNativeFontInfo(tmp_str);
            m_OutputBox->SetFont( display_font);
        }

        tmp_str = m_pAppConfig->Read( REG_FONT_COLOR, wxEmptyString);
        if ( tmp_str != wxEmptyString)
        {
            m_OutputBox->SetForegroundColour( wxColour(tmp_str));
        }
    }

	//------- Get GUID definition settings from registry
	m_GuidDefSettings = new GuidDefSettings();
	m_GuidDefSettings->pathToDefFile = m_pAppConfig->Read( REG_GUID_DEF_FILE, wxEmptyString);
	m_GuidDefSettings->autoLoad = (m_pAppConfig->Read( REG_GUID_AUTO_LOAD, static_cast<long>(0)) == 1) ? true : false;
	if ( m_GuidDefSettings->autoLoad)
	{
		m_LoadGuidDefFile_body();
	}

	//------- Get log directory settings from registry
	m_LogDirSettings = new LogDirSettings();
	m_LogDirSettings->defaultDir = m_pAppConfig->Read( REG_DEFAULT_LOG_DIR, wxEmptyString);
	m_LogDirSettings->lastDir = m_pAppConfig->Read( REG_LAST_LOG_DIR, wxEmptyString);
	m_LogDirSettings->useLastDir = (m_pAppConfig->Read( REG_USE_LAST_DIR, static_cast<long>(1)) == 1) ? true : false;
	if ( m_LogDirSettings->useLastDir)
	{
		if ( m_LogDirSettings->lastDir != wxEmptyString)
		{
			m_strDefaultPath = m_LogDirSettings->lastDir;
		}
	}
	else
	{
		if ( m_LogDirSettings->defaultDir != wxEmptyString)
		{
			m_strDefaultPath = m_LogDirSettings->defaultDir;
		}
	}

	//------- Get "show last logs" settings from registry
	m_checkboxRetainTailLines->SetValue(m_pAppConfig->Read(REG_RETAIN_TAIL_LINES, static_cast<long>(0)) == 1 ? true : false);
	m_textTailCount->SetValue(wxString::Format("%d", m_pAppConfig->ReadLong(REG_TAIL_COUNT, 100)));
	if (!m_checkboxRetainTailLines->GetValue())
	{
		m_textTailCount->Disable();
	}
	else
	{
		m_UpdateTailCount();
	}

	//------- Create the thread that monitors serial port
    m_PortMonitor = new MonitorThread(this, settings);
    
    if (m_PortMonitor->Create() != wxTHREAD_NO_ERROR || m_PortMonitor->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_PortMonitor;
        m_PortMonitor = 0;
    }

    m_radioSwitch->Disable();
}

void DumpViewFrame::m_InitMenuBar(void)
{
	wxMenuBar* mainMenuBar = new wxMenuBar();
	wxMenu* menuFile = new wxMenu();
    
	menuFile->Append(ID_MENU_OPEN, _("&Open...\tCtrl-O"));
	Bind(wxEVT_MENU, &DumpViewFrame::OnOpen, this, ID_MENU_OPEN);

    menuFile->Append(ID_MENU_SAVEAS, _("Save &as...\tCtrl-S"));
	Bind(wxEVT_MENU, &DumpViewFrame::OnSaveAs, this, ID_MENU_SAVEAS);

	menuFile->AppendSeparator();
    
	menuFile->Append(ID_MENU_QUIT, _("E&xit\tAlt-F4"), _("Quit the application"));
	Bind(wxEVT_MENU, &DumpViewFrame::OnExit, this, ID_MENU_QUIT);

	mainMenuBar->Append(menuFile, _("&File"));

	wxMenu* m_menuEdit = new wxMenu();

    m_menuEdit->Append(ID_MENU_COPY_ALL, _("Copy all\tCtrl-A"));
	Bind(wxEVT_MENU, &DumpViewFrame::OnCopyAll, this, ID_MENU_COPY_ALL);

    m_menuEdit->Append(ID_MENU_COPY, _("Copy selected\tCtrl-C"));
	Bind(wxEVT_MENU, &DumpViewFrame::OnCopySelection, this, ID_MENU_COPY);

	mainMenuBar->Append(m_menuEdit, _("Edit"));

	wxMenu* m_menuSettings = new wxMenu();

    m_menuSettings->Append(ID_MENU_SETCOM, _("COM port..."));
	Bind(wxEVT_MENU, &DumpViewFrame::OnComPortSetting, this, ID_MENU_SETCOM);
	
    m_menuSettings->Append(ID_MENU_SETFONT, _("Display font..."));
	Bind(wxEVT_MENU, &DumpViewFrame::OnFontSetting, this, ID_MENU_SETFONT);

    m_menuSettings->Append(ID_MENU_SETFOLDER, _("Default log folder..."));
	Bind(wxEVT_MENU, &DumpViewFrame::OnFolderSetting, this, ID_MENU_SETFOLDER);

	m_menuSettings->Append(ID_MENU_LOADGUIDDEF, "Load GUID definitions...");
	Bind(wxEVT_MENU, &DumpViewFrame::OnLoadGuidDef, this, ID_MENU_LOADGUIDDEF);

	mainMenuBar->Append(m_menuSettings, _("&Settings"));
    
	wxMenu* menuHelp = new wxMenu();
	
    menuHelp->Append(ID_MENU_ABOUT, _("About...\tF1"),
		_("Show info about this application"));
	Bind(wxEVT_MENU, &DumpViewFrame::OnAbout, this, ID_MENU_ABOUT);
	
	mainMenuBar->Append(menuHelp, _("Help"));
    SetMenuBar(mainMenuBar);

	// Define popup menu for "output box".
	m_menuPopup = std::make_unique<wxMenu>();
	m_menuPopup->Append(ID_MENU_COPY_ALL, "Copy all to clipboard");
	m_menuPopup->Append(ID_MENU_COPY, "Copy selection to clipboard");
	m_menuPopup->Append(ID_MENU_SAVEAS, "Save log...");
	m_menuPopup->AppendSeparator();
	m_menuPopup->Append(ID_MENU_CLEAR, "Clear log");
	Bind(wxEVT_MENU, &DumpViewFrame::OnClear, this, ID_MENU_CLEAR);
}

void DumpViewFrame::m_InitSizedComponents(wxWindow* parent)
{
    wxBoxSizer* BoxSizer2, *BoxSizer3, *BoxSizer4, *BoxSizer5, *BoxSizer6, *BoxSizer7, *BoxSizer8, *BoxSizer9;
    wxStaticText* labelQuickSearch;


    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    wxString switch_opt[2] =
    {
    	_("On"),
    	_("Off")
    };
    m_radioSwitch = new wxRadioBox(parent,
		wxID_ANY,
		_("Port Switch"),
		wxDefaultPosition,
		wxDefaultSize,
		2,
		switch_opt,
		1);
	m_radioSwitch->Bind(wxEVT_RADIOBOX, &DumpViewFrame::OnSwitchSelected, this);

	BoxSizer2->Add(m_radioSwitch, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 5)
		.Center());
    BoxSizer8 = new wxBoxSizer(wxVERTICAL);
    BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    
	m_checkboxPause = new wxCheckBox(parent, wxID_ANY, _("Pause"));
    m_checkboxPause->SetValue(false);
	m_checkboxPause->Bind(wxEVT_BUTTON, &DumpViewFrame::OnPause, this);
    
	BoxSizer4->Add(m_checkboxPause, wxSizerFlags()
		.Proportion(0)
		.Border(wxLEFT|wxRIGHT, 5)
		.Align(wxALIGN_LEFT));
    
	m_checkboxRec = new wxCheckBox(parent, wxID_ANY, _("Record"));
    m_checkboxRec->SetValue(false);
	m_checkboxRec->Bind(wxEVT_CHECKBOX, &DumpViewFrame::OnRec, this);

    BoxSizer4->Add(m_checkboxRec, wxSizerFlags()
		.Proportion(0)
		.Border(wxLEFT|wxRIGHT|wxTOP, 5)
		.Align(wxALIGN_LEFT));
    BoxSizer7->Add(BoxSizer4, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 5)
		.Expand());

	m_buttonTranslateGuid = new wxButton( parent, wxID_ANY, "Translate GUID");
	m_buttonTranslateGuid->Bind(wxEVT_BUTTON, &DumpViewFrame::OnTranslateGuid, this);

	BoxSizer7->Add( m_buttonTranslateGuid, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 5)
		.Center());

	m_buttonClear = new wxButton(parent, wxID_ANY, _("Clear logs"));
	m_buttonClear->Bind(wxEVT_BUTTON, &DumpViewFrame::OnClear, this);

	BoxSizer7->Add(m_buttonClear, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Center());
    BoxSizer8->Add(BoxSizer7, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 0)
		.Expand());

	m_checkboxRetainTailLines =
		new wxCheckBox(parent, wxID_ANY, "Retain only last ");
	m_checkboxRetainTailLines->Bind(wxEVT_CHECKBOX,
		&DumpViewFrame::OnRetainTailLinesSelected, this);

	BoxSizer9->Add(m_checkboxRetainTailLines, wxSizerFlags()
		.Proportion(0)
		.Border(wxLEFT, 10)
		.Center());

	wxIntegerValidator<int> val;
	val.SetMax(1000000);
	m_textTailCount = new wxTextCtrl(parent,
		wxID_ANY, wxEmptyString, wxDefaultPosition,
		wxSize(60, -1),
		wxTE_RIGHT | wxTE_PROCESS_ENTER,
		val);
	m_textTailCount->Bind(wxEVT_SET_FOCUS, &DumpViewFrame::OnTailCountFocusOn, this);
	m_textTailCount->Bind(wxEVT_KILL_FOCUS, &DumpViewFrame::OnTailCountFocusOff, this);
	m_textTailCount->Bind(wxEVT_TEXT_ENTER, &DumpViewFrame::OnTailCountEntered, this);

	BoxSizer9->Add(m_textTailCount, wxSizerFlags()
		.Proportion(0)
		.Border(wxLEFT | wxRIGHT, 5)
		.Center());
	BoxSizer9->Add(new wxStaticText(parent, wxID_ANY, " lines of log."), wxSizerFlags()
		.Proportion(0)
		.Border(wxRIGHT, 5)
		.Center());
	BoxSizer8->Add(BoxSizer9, wxSizerFlags()
		.Border(wxALL, 0));
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    labelQuickSearch = new wxStaticText(parent, wxID_ANY, _("Quick Search:"));
    BoxSizer6->Add(labelQuickSearch, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 5)
		.Center());

	m_textFindTarget = new wxTextCtrl(parent, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(112, 24), wxTE_PROCESS_ENTER);
	m_textFindTarget->Bind(wxEVT_TEXT_ENTER, &DumpViewFrame::OnFind, this);
    BoxSizer6->Add(m_textFindTarget, wxSizerFlags()
		.Proportion(1)
		.Border(wxTOP|wxBOTTOM|wxLEFT, 5)
		.Expand());

	m_checkboxCaseSensitive = new wxCheckBox(parent, wxID_ANY, _("Case Sensitive"));
    m_checkboxCaseSensitive->SetValue(false);
    BoxSizer6->Add(m_checkboxCaseSensitive, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 5)
		.Center());

	m_buttonFind = new wxButton(parent, wxID_ANY, _("Find"));
	m_buttonFind->SetMinSize(wxSize(61, 24));
	m_buttonFind->Bind(wxEVT_BUTTON, &DumpViewFrame::OnFind, this);
	BoxSizer6->Add(m_buttonFind, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 0)
		.Center());

	BoxSizer8->Add(BoxSizer6, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 0)
		.Expand());
    BoxSizer2->Add(BoxSizer8, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Center());
    BoxSizer3->Add(BoxSizer2, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 0)
		.Expand());
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);

	m_textLogFilename = new wxTextCtrl(parent, wxID_ANY);
    BoxSizer5->Add(m_textLogFilename, wxSizerFlags()
		.Proportion(1)
		.Border(wxTOP|wxBOTTOM|wxLEFT, 5)
		.Expand());

	m_buttonSelectFile = new wxButton(parent, wxID_ANY, _("..."));
	m_buttonSelectFile->SetMinSize(wxSize(29, 24));
	m_buttonSelectFile->Bind(wxEVT_BUTTON, &DumpViewFrame::OnSelectFile, this);

	BoxSizer5->Add(m_buttonSelectFile, wxSizerFlags()
		.Proportion(0)
		.Border(wxTOP|wxBOTTOM|wxRIGHT, 5)
		.Center());
    BoxSizer3->Add(BoxSizer5, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 0)
		.Expand());

#ifdef USE_RICH_EDIT
	m_OutputBox = new wxRichTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize(160, 120),
		wxRE_READONLY | wxRE_MULTILINE);
#else
	m_OutputBox = new wxTextCtrl(
		parent,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize(160, 120),
		wxTE_MULTILINE | wxTE_READONLY | wxTE_PROCESS_ENTER);
#endif
	m_OutputBox->Bind(wxEVT_TEXT_ENTER, &DumpViewFrame::OnFind, this);
	m_OutputBox->Bind(wxEVT_RIGHT_UP, &DumpViewFrame::OnOutputRightUp, this);

	BoxSizer3->Add(m_OutputBox, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Expand());

	parent->SetSizer(BoxSizer3);
    BoxSizer3->Fit(parent);
    BoxSizer3->SetSizeHints(parent);
}

void DumpViewFrame::m_InitStatusBar(void)
{
    m_statusBar1 = new wxStatusBar(this, wxID_ANY);
    int __wxStatusBarWidths_1[STATUS_COLS] = { 60, 100, -1 };
    int __wxStatusBarStyles_1[STATUS_COLS] = { wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL };
    m_statusBar1->SetFieldsCount(STATUS_COLS,__wxStatusBarWidths_1);
    m_statusBar1->SetStatusStyles(STATUS_COLS,__wxStatusBarStyles_1);
    SetStatusBar(m_statusBar1);
}

void DumpViewFrame::m_InitEventHandlers(void)
{
	Bind(wxEVT_CLOSE_WINDOW, &DumpViewFrame::OnClose, this);
	Bind(wxEVT_SIZE, &DumpViewFrame::OnResize, this);
	Bind(klEVT_THREAD_CALLBACK, &DumpViewFrame::OnThreadCallback, this);
}

wxString DumpViewFrame::m_FormatErrorMessage(DWORD error_no)
{
    wxChar buf[128];
    ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, error_no, 0, buf, 128, 0);
    return wxString(buf);
}

void DumpViewFrame::m_ShowPortInfoOnStatusBar( ComPortSetting &settings)
{
    m_statusBar1->SetStatusText( wxString::Format( "COM%d", settings.PortNum), 0);

    wxString result;
    result.Printf( "%d %d-", (-1 == settings.BaudRate) ? settings.ManualBaudRate : settings.BaudRate, settings.ByteSize);
    switch( settings.Parity)
    {
    case 0:
        result += "N-";
        break;
    case 1:
        result += "O-";
        break;
    case 2:
        result += "E-";
        break;
    case 3:
        result += "M-";
        break;
    case 4:
        result += "S-";
        break;
    default:
        result += "?-";
        break;
    }

    switch( settings.StopBit)
    {
    case 0:
        result += "1";
        break;
    case 1:
        result += "1.5";
        break;
    case 2:
        result += "2";
        break;
    default:
        result += "?";
        break;
    }

    m_statusBar1->SetStatusText( result, 1);
}

void DumpViewFrame::OnResize(wxSizeEvent& event)
{
    if ( !IsMaximized())
    {
        m_sizeTopWindow = GetSize();
    }
    wxFrame::OnSize(event);
}

void DumpViewFrame::OnClose(wxCloseEvent& event)
{
    // Save size/position and options to registry
    if ( m_pAppConfig)
    {
        m_pAppConfig->Write(REG_WINDOW_WIDTH, static_cast<int>(m_sizeTopWindow.x));
        m_pAppConfig->Write(REG_WINDOW_HEIGHT, static_cast<int>(m_sizeTopWindow.y));
        m_pAppConfig->Write(REG_POS_X, static_cast<int>(GetPosition().x));
        m_pAppConfig->Write(REG_POS_Y, static_cast<int>(GetPosition().y));

        if ( m_PortMonitor)
        {
            ComPortSetting settings = {0};
            m_PortMonitor->GetPortSettings( settings);
            m_pAppConfig->Write(REG_COM_NUM , static_cast<int>(settings.PortNum ));
            m_pAppConfig->Write(REG_BAUD_RATE , static_cast<int>(settings.BaudRate ));
			m_pAppConfig->Write(REG_MANUAL_BAUD_RATE, static_cast<int>(settings.ManualBaudRate));
            m_pAppConfig->Write(REG_PARITY , static_cast<int>(settings.Parity ));
            m_pAppConfig->Write(REG_BYTE_SIZE , static_cast<int>(settings.ByteSize ));
            m_pAppConfig->Write(REG_STOP_BIT , static_cast<int>(settings.StopBit ));
        }

        if ( m_OutputBox)
        {
            m_pAppConfig->Write( REG_FONT, m_OutputBox->GetFont().GetNativeFontInfoDesc());
            m_pAppConfig->Write( REG_FONT_COLOR, m_OutputBox->GetForegroundColour().GetAsString(wxC2S_CSS_SYNTAX));
        }

		if ( m_LogDirSettings)
		{
			m_pAppConfig->Write( REG_DEFAULT_LOG_DIR, m_LogDirSettings->defaultDir);
			m_pAppConfig->Write( REG_LAST_LOG_DIR, m_LogDirSettings->lastDir);
			m_pAppConfig->Write( REG_USE_LAST_DIR, (long)(m_LogDirSettings->useLastDir ? 1 : 0));

			delete m_LogDirSettings;
			m_LogDirSettings = 0;
		}

		if ( m_GuidDefSettings)
		{
			m_pAppConfig->Write( REG_GUID_DEF_FILE, m_GuidDefSettings->pathToDefFile);
			m_pAppConfig->Write( REG_GUID_AUTO_LOAD, static_cast<long>(m_GuidDefSettings->autoLoad ? 1 : 0));
			delete m_GuidDefSettings;
			m_GuidDefSettings = 0;
		}

		m_pAppConfig->Write(REG_RETAIN_TAIL_LINES, static_cast<long>(m_checkboxRetainTailLines->GetValue() ? 1 : 0));
		long lTmp;
		if (m_textTailCount->GetValue().ToLong(&lTmp))
		{
			m_pAppConfig->Write(REG_TAIL_COUNT, static_cast<long>(lTmp));
		}
		else
		{
			m_pAppConfig->Write(REG_TAIL_COUNT, static_cast<long>(0));
		}

        delete m_pAppConfig;
        m_pAppConfig = 0;
    }

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

		if ( data_read > 0)
        {
			//+++ Filter out all null charaters.
			int cur_pos, null_count;

			// skip copying if no null character encountered
			for ( cur_pos = 0, null_count = 0; cur_pos < data_read; cur_pos++)
			{
				if ( m_textBuffer[cur_pos] == '\0')
				{
					null_count = 1;
					break;
				}
			}

			if ( null_count)
			{
				// there is at least one null charater in buffer, so we have to move following
				// characters ahead.
				for ( ++cur_pos; cur_pos < data_read; cur_pos++)
				{
					if ( m_textBuffer[cur_pos] == '\0')
					{
						++null_count;
					}
					else
					{
						m_textBuffer[cur_pos - null_count] = m_textBuffer[cur_pos];
					}
				}

				data_read -= null_count;
			}
			//--- Filter out all null charaters.

			m_textBuffer[data_read] = '\0';

			// Convert manually because wxWidgets would drop result string if convertion failed.
			::MultiByteToWideChar( CP_ACP, 0, (LPCSTR)m_textBuffer, -1, m_wcTextBuffer, XFER_BUF_SIZE);

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
                m_OutputBox->AppendText(wxString(m_wcTextBuffer));
				if (m_iTailCount > 0)
				{
					int line_count = m_OutputBox->GetNumberOfLines();
					if (line_count > m_iTailCount)
					{
						m_OutputBox->Remove(0, m_OutputBox->XYToPosition(0, line_count - m_iTailCount) - 1);
					}
				}
#ifdef USE_RICH_EDIT
                m_statusBar1->SetStatusText( wxString::Format( "(%d,%d,%d)",
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

    case MONITOR_EVENT_TYPE_TERMINATED:
        wxMessageBox( wxString::Format( "Cannot read from serial port:\n%s", m_FormatErrorMessage(m_PortMonitor->GetErrorCode())));
        m_radioSwitch->SetSelection(1);
        m_ResetPort = false;
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
        wxMessageBox( wxString::Format( "Specific port cannot be initialized:\n%s", m_FormatErrorMessage(m_PortMonitor->GetErrorCode()) ));
        m_radioSwitch->Enable();
        m_radioSwitch->SetSelection( SWITCH_OFF);
        m_state = STATE_STOP;
        m_checkboxPause->Disable();
        break;
    }
}

void DumpViewFrame::OnOpen( wxCommandEvent& evt)
{
    wxFileDialog* dlg = new wxFileDialog(this, "Open...", m_strDefaultPath, m_strDumpFilename, "*.*", wxFD_OPEN);
    if (wxID_OK == dlg->ShowModal())
    {
        m_OutputBox->Clear();
		if ( !m_OutputBox->LoadFile( dlg->GetPath()))
		{
			wxMessageBox( "Load file failed.");
		}
    }
    dlg->Destroy();
}

void DumpViewFrame::OnSaveAs(wxCommandEvent& evt)
{
    wxFileDialog* dlg = new wxFileDialog(this, "Save as...", m_strDefaultPath, m_strDumpFilename, "*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (wxID_OK == dlg->ShowModal())
    {
        m_OutputBox->SaveFile( dlg->GetPath());

		m_LogDirSettings->lastDir = dlg->GetDirectory();
		if ( m_LogDirSettings->useLastDir)
		{
			m_strDefaultPath = m_LogDirSettings->lastDir;
		}
    }
    dlg->Destroy();
}

void DumpViewFrame::OnExit(wxCommandEvent& evt)
{
    Close();
}

void DumpViewFrame::OnFind(wxCommandEvent& evt)
{
    const int LENGTH_PER_READ = 100 * 1024;
    wxString pattern, content;

    m_statusBar1->SetStatusText( "", STATUS_COLS-1);

    pattern = m_textFindTarget->GetValue();

    if ( !pattern.IsEmpty())
    {
        long start = 0, end = 0, text_end = 0, result = 0;
        bool case_sensitive = false, has_warpped = false;

        // Prepare for the range to find
        m_OutputBox->GetSelection( &start, &result);
        if ( start != result)
        {
            start = result;
        }
        else
        {
            start = m_OutputBox->GetInsertionPoint();
        }
        text_end = m_OutputBox->GetLastPosition();
        end = (start + LENGTH_PER_READ <= text_end) ? (start + LENGTH_PER_READ) : (text_end);
        result = wxNOT_FOUND;

        case_sensitive = m_checkboxCaseSensitive->IsChecked();

        if ( !case_sensitive)
        {
            pattern.MakeLower();
        }

        while ( 1)
        {
            content = wxTextFile::Translate( m_OutputBox->GetRange( start, end), wxTextFileType_Dos);
            if ( !case_sensitive)
            {
                content.MakeLower();
            }
            result = content.find( pattern.c_str(), 0);

            if ( result != wxNOT_FOUND)
                break;

            if ( end == text_end)
            {
                if ( has_warpped)
                    break;

                text_end = start;
                start = 0;
                has_warpped = true;
            }
            else
            {
                start = end - pattern.Length();     // the pattern might be located at the boundary of search blocks...
            }
            end = (start + LENGTH_PER_READ <= text_end) ? (start + LENGTH_PER_READ) : (text_end);
        }

        if ( result != wxNOT_FOUND)
        {
            result += start;
            m_OutputBox->SetFocus();
            m_OutputBox->SetInsertionPoint( result);
            m_OutputBox->SetSelection( result, result + pattern.Length());
            //m_statusBar1->SetStatusText( wxString::Format( "%d", result));
            if ( has_warpped)
            {
                m_statusBar1->SetStatusText( "Warpped to start...", STATUS_COLS-1);
            }
        }
        else
        {
            m_statusBar1->SetStatusText( "Not found.", STATUS_COLS-1);
        }
    }
}

void DumpViewFrame::OnTranslateGuid( wxCommandEvent &evt)
{
	int line_count = m_OutputBox->GetNumberOfLines();
	wxString new_content = "";
	wxString line_text;

	for ( int i = 0; i < line_count; i++)
	{
		line_text = m_OutputBox->GetLineText(i);
		new_content += m_GuidTranslator.TranslateString( line_text);
		new_content += "\n";
	}

	m_OutputBox->ChangeValue( new_content);
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
	dlg->SetAvailableComPorts( m_PortMonitor->GetAvailableComPorts());
    dlg->SetPortSettings( setting);

    if ( dlg->ShowModal() == wxID_OK)
    {
        dlg->GetPortSettings(result);
        if ( setting.PortNum != result.PortNum ||
             setting.BaudRate != result.BaudRate ||
			 setting.ManualBaudRate != result.ManualBaudRate ||
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
            m_ShowPortInfoOnStatusBar( result);

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
	LogDirDialog* dlg = new LogDirDialog(m_LogDirSettings, this);

	if ( dlg->ShowModal() == wxID_OK)
	{
		if ( m_LogDirSettings->useLastDir)
		{
			m_strDefaultPath = m_LogDirSettings->lastDir;
		}
		else
		{
			m_strDefaultPath = m_LogDirSettings->defaultDir;
		}
	}

	dlg->Destroy();
}

void DumpViewFrame::OnLoadGuidDef(wxCommandEvent& evt)
{
	LoadGuidDefDialog* dlg = new LoadGuidDefDialog( m_GuidDefSettings, this);
    if (wxID_OK == dlg->ShowModal())
    {
		m_LoadGuidDefFile_body();
    }
    dlg->Destroy();
}

void DumpViewFrame::m_LoadGuidDefFile_body(void)
{
	int result;

	result = m_GuidTranslator.ReadTranslationTable( m_GuidDefSettings->pathToDefFile);
	if ( result >= 0)
	{
		m_buttonTranslateGuid->Enable(true);
	}
	else
	{
		wxString reason = "(Reason unknown)";
		if ( result == -1)
		{
			reason = "Definition file not found.";
		}
		else if ( result == -2)
		{
			reason = "Definition file format error.";
		}
		wxMessageBox( "Error: " + reason, "Failed to open GUID definition file!");
		m_buttonTranslateGuid->Enable(false);
	}
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
        if ( filename.GetFullName() == "")
        {
            if ( !m_SelectFile_body(false))
            {
                // Do nothing if user didn't select any files
                m_IsRecording = false;
                m_checkboxRec->SetValue(false);
                return;
            }
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

bool DumpViewFrame::m_SelectFile_body( bool prompt_overwrite)
{
    bool result = false;

    wxFileDialog* dlg = new wxFileDialog(
        this,
        "Select file to store logs...",
        m_strDefaultPath,
        m_strDumpFilename,
        "*.*",
        wxFD_SAVE | (prompt_overwrite?wxFD_OVERWRITE_PROMPT:0));

    if ( wxID_OK == dlg->ShowModal())
    {
        m_textLogFilename->ChangeValue( dlg->GetPath());

		// Save the last-visited directory information
		m_LogDirSettings->lastDir = dlg->GetDirectory();
		if ( m_LogDirSettings->useLastDir)
		{
			m_strDefaultPath = m_LogDirSettings->lastDir;
		}

		result = true;
    }

    dlg->Destroy();

    return result;
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

bool DumpViewFrame::m_UpdateTailCount(void)
{
	long lTmp;
	if (m_textTailCount->GetValue().ToLong(&lTmp))
	{
		m_iTailCount = lTmp;
		return true;
	}
	else
	{
		return false;
	}
}

void DumpViewFrame::OnTailCountEntered(wxCommandEvent& evt)
{
	if (m_UpdateTailCount())
	{
		m_strTailCountOld = m_textTailCount->GetValue();
	}
	else
	{
		m_textTailCount->SetValue(m_strTailCountOld);
	}
}

void DumpViewFrame::OnTailCountFocusOn(wxFocusEvent& evt)
{
	wxObject* obj = evt.GetEventObject();

	// Backup old setting
	if (obj && obj == m_textTailCount)
	{
		m_strTailCountOld = m_textTailCount->GetValue();
	}

	evt.Skip();
}

void DumpViewFrame::OnTailCountFocusOff(wxFocusEvent& evt)
{
	wxObject* obj = evt.GetEventObject();

	if (obj && obj == m_textTailCount)
	{
		m_textTailCount->SetValue(m_strTailCountOld);
	}

	evt.Skip();
}

void DumpViewFrame::OnOutputRightUp(wxMouseEvent & evt)
{
	PopupMenu(m_menuPopup.get());
}

void DumpViewFrame::OnRetainTailLinesSelected(wxCommandEvent& evt)
{
	if (m_checkboxRetainTailLines->GetValue())
	{
		m_textTailCount->Enable();
		if (!m_UpdateTailCount())
		{
			m_iTailCount = -1;
			m_textTailCount->SetValue("-1");
		}
	}
	else
	{
		m_iTailCount = -1;
		m_textTailCount->Disable();
	}
}
