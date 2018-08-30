#ifndef __LOG_DIR_DIALOG_H__
#define __LOG_DIR_DIALOG_H__

#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>

struct LogDirSettings
{
    wxString defaultDir;
    wxString lastDir;
    bool useLastDir;
};

class LogDirDialog : public wxDialog
{
    wxTextCtrl* m_textDefaultDir;
    wxButton*   m_btnSelectDefaultDir;
    wxCheckBox* m_chkboxUseLastDir;

    wxButton*   m_btnOk;
    wxButton*   m_btnCancel;

    LogDirSettings*  m_settings;

public:
    LogDirDialog(LogDirSettings* settings, wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize);
    virtual ~LogDirDialog();

protected:
	void OnBtnOk_Clicked( wxCommandEvent& evt);
	void OnBtnSelecDefaulttDir_Clicked( wxCommandEvent& evt);
	void OnChkboxUseLastDir_Clicked( wxCommandEvent& evt);
};

#endif
