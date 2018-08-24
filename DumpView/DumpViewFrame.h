// DumpViewFrame.h : interface of the  class
//
#ifndef __DumpViewFrame_H__
#define __DumpViewFrame_H__

//#define USE_RICH_EDIT
#define USE_BITMAP_BUTTON


#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/frame.h>
    #include <wx/file.h>
    #include <wx/filename.h>
    #include <wx/config.h>
#else
    #include <wx/wxprec.h>
#endif

#ifdef USE_RICH_EDIT
#include <wx/richtext/richtextctrl.h>
#endif
#include "MonitorThread.h"
#include "OutputBoxMouseHandler.h"

const int PAUSE_BUF_SIZE = 1024*1024;

DECLARE_EVENT_TYPE( wxEVT_THREAD_CALLBACK, -1)

class DumpViewFrame : public wxFrame
{
private:
    friend class OutputBoxMouseHandler;

    enum MainState {
        STATE_START,
        STATE_STOP,
        STATE_PAUSE
    };

    MainState m_state;
    MonitorThread* m_PortMonitor;
    bool m_ResetPort;

    static unsigned char m_textBuffer[XFER_BUF_SIZE];

    bool m_IsRecording;
    wxFile* m_fpLog;

    int m_iCurPauseBufSize;
    unsigned char* m_bufPause;

    wxString m_strDefaultPath;
    wxString m_strDumpFilename;

    wxConfig* m_pAppConfig;
    wxSize m_sizeTopWindow;
//    int m_iWindowWidth;
//    int m_iWindowHeight;

    OutputBoxMouseHandler* m_pMouseEvtHandler;

    // GUI objects
    wxMenuItem* m_menuSetFont;
    wxMenuItem* m_menuSetCom;
    wxMenuItem* m_menuCopyAll;
    wxMenuItem* m_menuFind;
    wxMenuItem* m_menuSaveAs;
    wxMenuItem* m_menuCopy;
    wxMenuItem* m_menuSetFolder;
    wxMenu* m_menuEdit;
    wxMenu* m_menuSettings;

    wxRadioBox* m_radioSwitch;
    wxCheckBox* m_checkboxPause;
    wxCheckBox* m_checkboxRec;
    wxButton* m_buttonSelectFile;
    wxButton* m_buttonClear;
    wxTextCtrl* m_textLogFilename;
    wxStatusBar* m_statusBar1;
#ifdef USE_RICH_EDIT
    wxRichTextCtrl* m_OutputBox;
#else
    wxTextCtrl* m_OutputBox;
#endif
    wxButton* m_buttonFind;
    wxTextCtrl* m_textFindTarget;
    wxCheckBox* m_checkboxCaseSensitive;


    // ID for GUI controls
    static const long ID_RADIOBOX_SWITCH;
    static const long ID_CHECKBOX_PAUSE;
    static const long ID_CHECKBOX_REC;
    static const long ID_BUTTON_CLEAR;
    static const long ID_STATICTEXT1;
    static const long ID_TEXT_FIND_TARGET;
    static const long ID_CHECKBOX_CASE_SENSITIVE;
    static const long ID_BUTTON_FIND;
    static const long ID_TEXT_DEFAULT_FOLDER;
    static const long ID_BUTTON_SELECT_FILE;
    static const long ID_OUTPUT_BOX;
    static const long ID_PANEL1;
    static const long ID_MENU_OPEN;
    static const long ID_MENU_SAVEAS;
    static const long ID_MENU_QUIT;
    static const long ID_MENU_FIND;
    static const long ID_MENU_COPY_ALL;
    static const long ID_MENU_COPY;
    static const long ID_MENU_SETCOM;
    static const long ID_MENU_SETFONT;
    static const long ID_MENU_SETFOLDER;
    static const long ID_MENU_ABOUT;
    static const long ID_STATUSBAR1;

    void m_InitMenuBar(void);
    void m_InitSizedComponents(wxWindow* parent);
    void m_InitStatusBar(void);

    bool m_SelectFile_body(bool prompt_overwrite);
    void m_SwitchSelect_body(int selection);

    static wxString m_FormatErrorMessage(DWORD error_no);

    void m_ShowPortInfoOnStatusBar( ComPortSetting &settings);

    DECLARE_EVENT_TABLE();

public:

    DumpViewFrame(const wxString& title);

    void OnResize(wxSizeEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnThreadCallback(wxCommandEvent& evt);

    void OnOpen(wxCommandEvent& evt);           // Menu->File->Open...
    void OnSaveAs(wxCommandEvent& evt);         // Menu->File->Save as...
    void OnExit(wxCommandEvent& evt);           // Menu->File->Exit
    void OnFind(wxCommandEvent& evt);           // Menu->Edit->Find...
    void OnCopyAll(wxCommandEvent& evt);        // Menu->Edit->Copy all
    void OnCopySelection(wxCommandEvent& evt);  // Menu->Edit->Copy Selection
    void OnComPortSetting(wxCommandEvent& evt); // Menu->Settings->Com Port
    void OnFontSetting(wxCommandEvent& evt);    // Menu->Settings->Display font
    void OnFolderSetting(wxCommandEvent& evt);  // Menu->Settings->Default Folder
    void OnAbout(wxCommandEvent& evt);          // Menu->Help->About...

    void OnClear(wxCommandEvent& evt);          // Button Clear
    void OnSelectFile(wxCommandEvent& evt);     // Button Select File

    void OnSwitchSelected(wxCommandEvent& evt); // Radiobox Switch

    void OnRec(wxCommandEvent& evt);            // Checkbox Record
    void OnPause(wxCommandEvent& evt);          // Checkbox Pause
};

#endif
