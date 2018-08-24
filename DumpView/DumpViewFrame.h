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
    wxRichTextCtrl* m_OutputBox;
    MonitorThread* m_PortMonitor;

    static unsigned char m_textBuffer[BUF_SIZE];

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

    DECLARE_EVENT_TABLE();

public:
    DumpViewFrame(const wxString& title);

    void OnClose(wxCloseEvent& event);
    void OnDataReady(void);
    void OnThreadCallback(wxCommandEvent& evt);

};

#endif
