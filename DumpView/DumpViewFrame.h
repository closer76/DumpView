// DumpViewFrame.h : interface of the  class
//
#ifndef __DumpViewFrame_H__
#define __DumpViewFrame_H__

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/frame.h>
#else
    #include <wx/wxprec.h>
#endif

#include <wx/richtext/richtextctrl.h>
#include "MonitorThread.h"

//#define USE_RICH_EDIT
#define USE_BITMAP_BUTTON

DECLARE_EVENT_TYPE( wxEVT_THREAD_CALLBACK, -1)

class DumpViewFrame : public wxFrame
{
private:
    enum MainState {
        STATE_START,
        STATE_STOP,
        STATE_PAUSE
    };

    MainState m_state;
    bool m_IsRecording;
    MonitorThread* m_PortMonitor;

    static unsigned char m_textBuffer[BUF_SIZE];

    wxString m_strDefaultPath;
    wxString m_strDumpFilename;

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
#ifdef USE_BITMAP_BUTTON
    wxBitmapButton* m_buttonRec;
    wxBitmapButton* m_buttonStart;
    wxBitmapButton* m_buttonStop;
    wxBitmapButton* m_buttonPause;
#else
    wxButton* m_buttonRec;
    wxButton* m_buttonStart;
    wxButton* m_buttonStop;
    wxButton* m_buttonPause;
#endif
    wxButton* m_buttonClear;
    wxTextCtrl* m_textDefaultFolder;
    wxStatusBar* m_statusBar1;
#ifdef USE_RICH_EDIT
    wxRichTextCtrl* m_OutputBox;
#else
    wxTextCtrl* m_OutputBox;
#endif


    // ID for GUI controls
    static const long ID_BUTTON_START;
    static const long ID_BUTTON_STOP;
    static const long ID_BUTTON_REC;
    static const long ID_BUTTON_PAUSE;
    static const long ID_BUTTON_CLEAR;
    static const long ID_TEXT_DEFAULT_FOLDER;
    static const long ID_OUTPUT_BOX;
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


    DECLARE_EVENT_TABLE();

public:
    DumpViewFrame(const wxString& title);

    void OnClose(wxCloseEvent& event);
    void OnThreadCallback(wxCommandEvent& evt);

    void OnSaveAs(wxCommandEvent& evt);         // Menu->File->Save as...
    void OnExit(wxCommandEvent& evt);           // Menu->File->Exit
    void OnFind(wxCommandEvent& evt);           // Menu->Edit->Find...
    void OnCopyAll(wxCommandEvent& evt);        // Menu->Edit->Copy all
    void OnCopySelection(wxCommandEvent& evt);  // Menu->Edit->Copy Selection
    void OnComPortSetting(wxCommandEvent& evt); // Menu->Settings->Com Port
    void OnFontSetting(wxCommandEvent& evt);    // Menu->Settings->Display font
    void OnFolderSetting(wxCommandEvent& evt);  // Menu->Settings->Default Folder
    void OnAbout(wxCommandEvent& evt);          // Menu->Help->About...

    void OnStart(wxCommandEvent& evt);          // Button Start
    void OnStop(wxCommandEvent& evt);           // Button Stop
    void OnRec(wxCommandEvent& evt);            // Button Rec
    void OnPause(wxCommandEvent& evt);          // Button Pause
    void OnClear(wxCommandEvent& evt);          // Button Clear
};

#endif
