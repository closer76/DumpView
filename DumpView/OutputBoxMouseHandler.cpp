#include "OutputBoxMouseHandler.h"
#include "DumpViewFrame.h"

BEGIN_EVENT_TABLE( OutputBoxMouseHandler, wxEvtHandler)
    EVT_RIGHT_UP( OutputBoxMouseHandler::OnRightButtonUp)
END_EVENT_TABLE()

OutputBoxMouseHandler::OutputBoxMouseHandler(void)
    : m_pOwner(0),
    m_pPopupMenu(0)
{
    m_pPopupMenu = new wxMenu();
    m_pPopupMenu->Append( DumpViewFrame::ID_MENU_COPY_ALL, wxT("Copy all to clipboard"));
    m_pPopupMenu->Append( DumpViewFrame::ID_MENU_COPY, wxT("Copy selection to clipboard"));
    m_pPopupMenu->Append( DumpViewFrame::ID_MENU_SAVEAS, wxT("Save log..."));
    m_pPopupMenu->AppendSeparator();
    m_pPopupMenu->Append( DumpViewFrame::ID_BUTTON_CLEAR, wxT("Clear log"));
}

OutputBoxMouseHandler::~OutputBoxMouseHandler(void)
{
    delete m_pPopupMenu;
    m_pPopupMenu = 0;
}

void OutputBoxMouseHandler::OnRightButtonUp(wxMouseEvent &evt)
{
    if ( m_pOwner)
    {
        m_pOwner->PopupMenu( m_pPopupMenu, evt.GetPosition().x, evt.GetPosition().y);
    }
}

void OutputBoxMouseHandler::SetOwner( wxWindow* owner)
{
    m_pOwner = owner;
}