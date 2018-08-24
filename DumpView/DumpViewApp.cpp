// DumpViewApp.cpp : Defines the class behaviors for the application.
//
#include "DumpViewApp.h"
#include "DumpViewFrame.h"
#include "version.h"

IMPLEMENT_APP(DumpViewApp)

bool DumpViewApp::OnInit()
{
    DumpViewFrame* frame = new DumpViewFrame(WINDOW_TITLE);
    SetTopWindow(frame);
    frame->Show(true);

    return true;
}

int DumpViewApp::OnExit()
{
    return 0;
}
