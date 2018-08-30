#include "OutputBoxMouseHandler.h"
#include "DumpViewFrame.h"

OutputBoxMouseHandler::OutputBoxMouseHandler(void)
    : m_pOwner(0)
	, m_pPopupMenu(new wxMenu())
{
    //m_pPopupMenu = ;
    m_pPopupMenu->Append( DumpViewFrame::ID_MENU_COPY_ALL, "Copy all to clipboard");
    m_pPopupMenu->Append( DumpViewFrame::ID_MENU_COPY, "Copy selection to clipboard");
    m_pPopupMenu->Append( DumpViewFrame::ID_MENU_SAVEAS, "Save log...");
    m_pPopupMenu->AppendSeparator();
    m_pPopupMenu->Append( DumpViewFrame::ID_MENU_CLEAR, "Clear log");

	Bind(wxEVT_RIGHT_UP, &OutputBoxMouseHandler::OnRightButtonUp, this);
}

OutputBoxMouseHandler::~OutputBoxMouseHandler(void)
{
}

void OutputBoxMouseHandler::OnRightButtonUp(wxMouseEvent &evt)
{
    if ( m_pOwner)
    {
        m_pOwner->PopupMenu( m_pPopupMenu.get(), evt.GetPosition());
    }
}

void OutputBoxMouseHandler::SetOwner( wxWindow* owner)
{
    m_pOwner = owner;
}