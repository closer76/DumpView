#ifndef __OUTPUT_BOX_MOUSE_HANDLER_H__
#define __OUTPUT_BOX_MOUSE_HANDLER_H__

#include <wx/wx.h>
#include <wx/event.h>

#include <memory>

class OutputBoxMouseHandler :
    public wxEvtHandler
{
private:
    wxWindow* m_pOwner;
    std::unique_ptr<wxMenu> m_pPopupMenu;

public:
    OutputBoxMouseHandler(void);
    ~OutputBoxMouseHandler(void);

    void SetOwner( wxWindow* owner);

    void OnRightButtonUp( wxMouseEvent& evt);

//    DECLARE_EVENT_TABLE()
};

#endif