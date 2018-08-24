// DumpViewApp.cpp : Defines the class behaviors for the application.
//
#include "DumpViewApp.h"
#include "DumpViewFrame.h"

IMPLEMENT_APP(DumpViewApp)

bool DumpViewApp::OnInit()
{
    DumpViewFrame* frame = new DumpViewFrame(wxT("DumpView"));
    SetTopWindow(frame);
    frame->Show(true);

    return true;
}

int DumpViewApp::OnExit()
{
    return 0;
}
