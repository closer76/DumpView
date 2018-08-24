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
#include "SerialCommHelper.h"
#include "MonitorThread.h"

DECLARE_EVENT_TYPE( wxEVT_THREAD_CALLBACK, -1)

class DumpViewFrame : public wxFrame
{
private:
    MonitorThread* m_PortMonitor;

    static unsigned char m_textBuffer[BUF_SIZE];

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
    wxButton* m_buttonRec;
    wxButton* m_buttonClear;
    wxButton* m_buttonStart;
    wxButton* m_buttonStop;
    wxButton* m_buttonPause;
    wxTextCtrl* m_textDefaultFolder;
    wxStatusBar* m_statusBar1;
    wxRichTextCtrl* m_OutputBox;


    // ID for GUI controls
    static const long ID_BUTTON_START;
    static const long ID_BUTTON_STOP;
    static const long ID_BUTTON_REC;
    static const long ID_BUTTON_PAUSE;
    static const long ID_BUTTON5;
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

    DECLARE_EVENT_TABLE();

public:
    DumpViewFrame(const wxString& title);

    void OnExit(wxCommandEvent& evt);
    void OnClose(wxCloseEvent& event);
    void OnDataReady(void);
    void OnThreadCallback(wxCommandEvent& evt);

};

#endif
