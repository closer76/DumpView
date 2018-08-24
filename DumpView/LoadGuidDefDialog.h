#ifndef __LOAD_GUID_DEF_DIALOG__
#define __LOAD_GUID_DEF_DIALOG__

#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>

struct GuidDefSettings
{
	wxString pathToDefFile;
	bool autoLoad;
};

class LoadGuidDefDialog :
	public wxDialog
{
    wxTextCtrl* m_textGuidDefFilePath;
    wxButton*   m_btnSelectGuidDefFile;
    wxCheckBox* m_chkboxAutoLoad;

    wxButton*   m_btnOk;
    wxButton*   m_btnCancel;

    GuidDefSettings*  m_settings;

public:
	LoadGuidDefDialog(GuidDefSettings* settings, wxWindow* parent);
	virtual ~LoadGuidDefDialog(void);

protected:
	static const long ID_LOAD_GUID_DEF_DIALOG;
    static const long ID_TEXT_PATH;
    static const long ID_BTN_SELECT_PATH;
    static const long ID_CHKBOX_AUTO_LOAD;
    static const long ID_BTN_OK;
    static const long ID_BTN_CANCEL;

	void OnBtnSelecGuidDefFile_Clicked( wxCommandEvent& evt);
	void OnChkboxAutoLoad_Clicked( wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()
};

#endif