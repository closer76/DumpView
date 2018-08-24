#ifndef __LOG_DIR_DIALOG_H__
#define __LOG_DIR_DIALOG_H__

#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>

struct LogDirSetting
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

    LogDirSetting*  m_settings;

public:
    LogDirDialog(LogDirSetting* settings, wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize);
    virtual ~LogDirDialog();

protected:
	static const long ID_LOG_DIR_DIALOG;
    static const long ID_TEXT_DEFAULT_DIR;
    static const long ID_BTN_SELECT_DEFAULT_DIR;
    static const long ID_CHKBOX_USE_LAST_DIR;
    static const long ID_BTN_OK;
    static const long ID_BTN_CANCEL;

	void OnBtnOk_Clicked( wxCommandEvent& evt);
	void OnBtnSelecDefaulttDir_Clicked( wxCommandEvent& evt);
	void OnChkboxUseLastDir_Clicked( wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()
};

#endif
