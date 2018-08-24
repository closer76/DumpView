#include "LoadGuidDefDialog.h"
#include "wx/sizer.h"
#include "wx/filedlg.h"
#include "wx/filename.h"

const long LoadGuidDefDialog::ID_LOAD_GUID_DEF_DIALOG = wxNewId();
const long LoadGuidDefDialog::ID_TEXT_PATH = wxNewId();
const long LoadGuidDefDialog::ID_BTN_SELECT_PATH = wxNewId();
const long LoadGuidDefDialog::ID_CHKBOX_AUTO_LOAD = wxNewId();
const long LoadGuidDefDialog::ID_BTN_OK = wxID_OK;
const long LoadGuidDefDialog::ID_BTN_CANCEL = wxID_CANCEL;

BEGIN_EVENT_TABLE( LoadGuidDefDialog, wxDialog)
	EVT_BUTTON( ID_BTN_SELECT_PATH, LoadGuidDefDialog::OnBtnSelecGuidDefFile_Clicked)
	EVT_CHECKBOX( ID_CHKBOX_AUTO_LOAD, LoadGuidDefDialog::OnChkboxAutoLoad_Clicked)
END_EVENT_TABLE()

LoadGuidDefDialog::LoadGuidDefDialog(GuidDefSettings* settings, wxWindow* parent) :
	wxDialog( parent, ID_LOAD_GUID_DEF_DIALOG, wxT("Select GUID definition file...")),
    m_settings(settings)
{
	wxBoxSizer *sizerDefPathAndBtn, *sizerOkCancelBtn, *sizerTop;

	m_textGuidDefFilePath = new wxTextCtrl( this, ID_TEXT_PATH, m_settings->pathToDefFile);
    m_btnSelectGuidDefFile = new wxButton( this, ID_BTN_SELECT_PATH, wxT("..."), wxDefaultPosition, wxSize( 30, wxDefaultSize.GetY()));
	sizerDefPathAndBtn = new wxBoxSizer( wxHORIZONTAL); 
	sizerDefPathAndBtn->Add( m_textGuidDefFilePath, 1, wxTOP | wxBOTTOM | wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
	sizerDefPathAndBtn->Add( m_btnSelectGuidDefFile, 0, wxTOP | wxBOTTOM | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

    m_chkboxAutoLoad = new wxCheckBox( this, ID_CHKBOX_AUTO_LOAD, wxT("Automatically load GUID table when start-up."));

	m_btnOk = new wxButton( this, ID_BTN_OK, wxT("OK"));
    m_btnCancel = new wxButton( this, ID_BTN_CANCEL, wxT("Cancel"));
	sizerOkCancelBtn = new wxBoxSizer( wxHORIZONTAL);
	sizerOkCancelBtn->Add( m_btnOk, 0, wxRIGHT, 10);
	sizerOkCancelBtn->Add( m_btnCancel, 0, wxLEFT, 10);

	sizerTop = new wxBoxSizer( wxVERTICAL);
	sizerTop->Add( sizerDefPathAndBtn, 0, wxEXPAND | wxALL, 5);
	sizerTop->Add( m_chkboxAutoLoad, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
	sizerTop->Add( sizerOkCancelBtn, 0, wxALL | wxALIGN_CENTER, 10);

	this->SetSizer( sizerTop);
	sizerTop->Fit( this);
	sizerTop->SetSizeHints(this);

	wxSize new_size = this->GetSize();
	new_size.SetWidth( 400);
	this->SetSize( new_size);

    if ( m_settings)
    {
		m_chkboxAutoLoad->SetValue(m_settings->autoLoad);
    }
}

LoadGuidDefDialog::~LoadGuidDefDialog(void)
{
}

void LoadGuidDefDialog::OnBtnSelecGuidDefFile_Clicked( wxCommandEvent& evt)
{
	wxFileName filename(m_settings->pathToDefFile);

	wxFileDialog* dlg = new wxFileDialog( this, wxT("Select GUID definition file..."), filename.GetPath(), filename.GetFullName());

	if ( dlg->ShowModal() == wxID_OK)
	{
		m_textGuidDefFilePath->SetValue( dlg->GetPath());
		m_settings->pathToDefFile = m_textGuidDefFilePath->GetValue();
	}

	dlg->Destroy();
}

void LoadGuidDefDialog::OnChkboxAutoLoad_Clicked( wxCommandEvent& evt)
{
	m_settings->autoLoad = m_chkboxAutoLoad->IsChecked();
}
