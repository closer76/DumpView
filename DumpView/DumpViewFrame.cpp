// DumpViewFrame.cpp : implementation of the  class
//
#include "DumpViewFrame.h"
#include "ComSettingDialog.h"
#include <wx/sizer.h>
#include <wx/fontdlg.h>
//#include <process.h>


unsigned char DumpViewFrame::m_textBuffer[BUF_SIZE] = {0};

const long DumpViewFrame::ID_BUTTON_START = wxNewId();
const long DumpViewFrame::ID_BUTTON_STOP = wxNewId();
const long DumpViewFrame::ID_BUTTON_REC = wxNewId();
const long DumpViewFrame::ID_BUTTON_PAUSE = wxNewId();
const long DumpViewFrame::ID_BUTTON_CLEAR = wxNewId();
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
    EVT_MENU( ID_MENU_SAVEAS, DumpViewFrame::OnSaveAs)
    EVT_MENU( ID_MENU_QUIT, DumpViewFrame::OnExit)
    EVT_MENU( ID_MENU_FIND , DumpViewFrame::OnFind)
    EVT_MENU( ID_MENU_COPY_ALL , DumpViewFrame::OnCopyAll)
    EVT_MENU( ID_MENU_COPY , DumpViewFrame::OnCopySelection)
    EVT_MENU( ID_MENU_SETCOM, DumpViewFrame::OnComPortSetting)
    EVT_MENU( ID_MENU_SETFONT , DumpViewFrame::OnFontSetting)
    EVT_MENU( ID_MENU_SETFOLDER , DumpViewFrame::OnFolderSetting)
    EVT_MENU( ID_MENU_ABOUT , DumpViewFrame::OnAbout)
    EVT_BUTTON( ID_BUTTON_START , DumpViewFrame::OnStart )
    EVT_BUTTON( ID_BUTTON_STOP , DumpViewFrame::OnStop )
    EVT_BUTTON( ID_BUTTON_REC , DumpViewFrame::OnRec )
    EVT_BUTTON( ID_BUTTON_PAUSE , DumpViewFrame::OnPause )
    EVT_BUTTON( ID_BUTTON_CLEAR , DumpViewFrame::OnClear )
    ////Manual Code End
END_EVENT_TABLE()

DumpViewFrame::DumpViewFrame(const wxString& title) : 
    wxFrame(NULL, wxID_ANY, title)
{
    //------- Initiate internal variables
    m_strDefaultPath = wxT(".");
    m_strDumpFilename = wxT("dump.txt");

    //------- Set up UI
    m_InitMenuBar();

    m_InitSizedComponents();

    m_InitStatusBar();

    this->SetSize( 800, 540);

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

void DumpViewFrame::m_InitSizedComponents(void)
{
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;

    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);

#ifdef USE_BITMAP_BUTTON
    wxBitmap image_red( wxT("red"), wxBITMAP_TYPE_BMP_RESOURCE);
    wxBitmap image_black( wxT("black"), wxBITMAP_TYPE_BMP_RESOURCE);
    wxMask* mask_rec_default = new wxMask( wxBitmap( wxT("rec_default"), wxBITMAP_TYPE_BMP_RESOURCE));
    wxMask* mask_rec_disabled = new wxMask( wxBitmap( wxT("rec_disabled"), wxBITMAP_TYPE_BMP_RESOURCE));
    wxMask* mask_start_default = new wxMask( wxBitmap( wxT("start_default"), wxBITMAP_TYPE_BMP_RESOURCE));
    wxMask* mask_start_disabled = new wxMask( wxBitmap( wxT("start_disabled"), wxBITMAP_TYPE_BMP_RESOURCE));
    wxMask* mask_stop_default = new wxMask( wxBitmap( wxT("stop_default"), wxBITMAP_TYPE_BMP_RESOURCE));
    wxMask* mask_stop_disabled = new wxMask( wxBitmap( wxT("stop_disabled"), wxBITMAP_TYPE_BMP_RESOURCE));
    wxMask* mask_pause_default = new wxMask( wxBitmap( wxT("pause_default"), wxBITMAP_TYPE_BMP_RESOURCE));
    wxMask* mask_pause_disabled = new wxMask( wxBitmap( wxT("pause_disabled"), wxBITMAP_TYPE_BMP_RESOURCE));

    image_red.SetMask( mask_rec_default);
    m_buttonRec = new wxBitmapButton( this, ID_BUTTON_REC, image_red, wxDefaultPosition, wxSize(40, 40), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BUTTON_REC"));
    image_red.SetMask( mask_rec_disabled);
    m_buttonRec->SetBitmapDisabled( image_red);

    image_black.SetMask( mask_start_default);
    m_buttonStart = new wxBitmapButton( this, ID_BUTTON_START, image_black, wxDefaultPosition, wxSize(40, 40), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BUTTON_START"));
    image_black.SetMask( mask_start_disabled);
    m_buttonStart->SetBitmapDisabled( image_black);

    image_black.SetMask( mask_stop_default);
    m_buttonStop = new wxBitmapButton( this, ID_BUTTON_STOP, image_black, wxDefaultPosition, wxSize(40, 40), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BUTTON_STOP"));
    image_black.SetMask( mask_stop_disabled);
    m_buttonStop->SetBitmapDisabled( image_black);

    image_black.SetMask( mask_pause_default);
    m_buttonPause = new wxBitmapButton( this, ID_BUTTON_PAUSE, image_black, wxDefaultPosition, wxSize(40, 40), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BUTTON_PAUSE"));
    image_black.SetMask( mask_pause_disabled);
    m_buttonPause->SetBitmapDisabled( image_black);
#else
    m_buttonStart = new wxButton(this, ID_BUTTON_START, _("Start"), wxDefaultPosition, wxSize(40,40), 0, wxDefaultValidator, _T("ID_BUTTON_START"));
    m_buttonStop = new wxButton(this, ID_BUTTON_STOP, _("Stop"), wxDefaultPosition, wxSize(40,40), 0, wxDefaultValidator, _T("ID_BUTTON_STOP"));
    m_buttonRec = new wxButton(this, ID_BUTTON_REC, _("Rec"), wxDefaultPosition, wxSize(40,40), 0, wxDefaultValidator, _T("ID_BUTTON_REC"));
    m_buttonPause = new wxButton(this, ID_BUTTON_PAUSE, _("Pause"), wxDefaultPosition, wxSize(40,40), 0, wxDefaultValidator, _T("ID_BUTTON_PAUSE"));
#endif
    m_buttonClear = new wxButton(this, ID_BUTTON_CLEAR, _("Clear logs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer2->Add(m_buttonStart, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(m_buttonStop, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(m_buttonRec, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(m_buttonPause, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(m_buttonClear, 1, wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_textDefaultFolder = new wxTextCtrl(this, ID_TEXT_DEFAULT_FOLDER, _(""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXT_DEFAULT_FOLDER"));
    BoxSizer1->Add(m_textDefaultFolder, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
#ifdef USE_RICH_EDIT
    m_OutputBox = new wxRichTextCtrl( this, ID_OUTPUT_BOX, wxEmptyString, wxDefaultPosition, wxSize(160,120), wxRE_READONLY | wxRE_MULTILINE);
#else
    m_OutputBox = new wxTextCtrl( this, ID_OUTPUT_BOX, wxEmptyString, wxDefaultPosition, wxSize(160, 120), wxTE_MULTILINE | wxTE_READONLY);
#endif
    BoxSizer1->Add(m_OutputBox, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

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

    event.Skip();
}

void DumpViewFrame::OnThreadCallback(wxCommandEvent& evt)
{
    int data_read = m_PortMonitor->CopyBuffer(m_textBuffer);
    if ( data_read >= BUF_SIZE)
        data_read = BUF_SIZE - 1;

    m_textBuffer[data_read] = 0;
    if ( data_read > 0)
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

    if ( dlg->ShowModal() == wxID_OK)
    {
        wxMessageBox( wxT("Hello!"));
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
}

void DumpViewFrame::OnStart(wxCommandEvent& evt)
{
    m_PortMonitor->StartMonitoring();
    m_buttonStart->Disable();
    m_buttonStop->Enable();
}

void DumpViewFrame::OnStop(wxCommandEvent& evt)
{
    m_PortMonitor->StopMonitoring();
    m_buttonStart->Enable();
    m_buttonStop->Disable();
}

void DumpViewFrame::OnRec(wxCommandEvent& evt)
{
}

void DumpViewFrame::OnPause(wxCommandEvent& evt)
{
}

void DumpViewFrame::OnClear(wxCommandEvent& evt)
{
    m_OutputBox->Clear();
}