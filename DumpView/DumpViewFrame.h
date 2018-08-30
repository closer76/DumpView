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

#include "LogDirDialog.h"
#include "LoadGuidDefDialog.h"
#include "GuidTranslator.h"

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

    static char m_textBuffer[XFER_BUF_SIZE];
	static wchar_t m_wcTextBuffer[XFER_BUF_SIZE];

    bool m_IsRecording;
    wxFile* m_fpLog;

    int m_iCurPauseBufSize;
    unsigned char* m_bufPause;

	long m_iTailCount;

    wxString m_strDefaultPath;
    wxString m_strDumpFilename;
	wxString m_strTailCountOld;

	LogDirSettings* m_LogDirSettings;

	GuidDefSettings* m_GuidDefSettings;
	GuidTranslator m_GuidTranslator;

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
	wxMenuItem* m_menuLoadGuidDef;
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
	wxButton* m_buttonTranslateGuid;

	wxCheckBox* m_checkboxRetainTailLines;
	wxTextCtrl* m_textTailCount;


    // ID for GUI controls
    static const long ID_MENU_OPEN;
    static const long ID_MENU_SAVEAS;
    static const long ID_MENU_QUIT;
    static const long ID_MENU_FIND;
    static const long ID_MENU_COPY_ALL;
    static const long ID_MENU_COPY;
    static const long ID_MENU_SETCOM;
    static const long ID_MENU_SETFONT;
    static const long ID_MENU_SETFOLDER;
	static const long ID_MENU_LOADGUIDDEF;
    static const long ID_MENU_ABOUT;
	static const long ID_MENU_CLEAR;

    void m_InitMenuBar(void);
    void m_InitSizedComponents(wxWindow* parent);
    void m_InitStatusBar(void);
	void m_InitEventHandlers(void);

    bool m_SelectFile_body(bool prompt_overwrite);
    void m_SwitchSelect_body(int selection);
	void m_LoadGuidDefFile_body(void);

    static wxString m_FormatErrorMessage(DWORD error_no);

    void m_ShowPortInfoOnStatusBar( ComPortSetting &settings);

	bool m_UpdateTailCount(void);

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
	void OnLoadGuidDef(wxCommandEvent& evt);	// Menu->Settings->Load GUID definitions
    void OnAbout(wxCommandEvent& evt);          // Menu->Help->About...

    void OnClear(wxCommandEvent& evt);          // Button Clear
    void OnSelectFile(wxCommandEvent& evt);     // Button Select File
	void OnTranslateGuid(wxCommandEvent& evt);	// Button Translate GUID

    void OnSwitchSelected(wxCommandEvent& evt); // Radiobox Switch

    void OnRec(wxCommandEvent& evt);            // Checkbox Record
    void OnPause(wxCommandEvent& evt);          // Checkbox Pause

	void OnRetainTailLinesSelected(wxCommandEvent& evt);	// Checkbox "Show Last Logs"
	void OnTailCountEntered(wxCommandEvent& evt);	// Text LastLogLength
	void OnTailCountFocusOn(wxFocusEvent& evt);
	void OnTailCountFocusOff(wxFocusEvent& evt);	// Text LastLogLength
};

#endif
