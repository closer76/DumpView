// DumpViewApp.h : main header file for the DumpView application
//
#ifndef __DumpViewApp_H__
#define __DumpViewApp_H__

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#else
    #include <wx/wxprec.h>
#endif

class DumpViewApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

#endif
