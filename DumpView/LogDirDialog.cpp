#include "LogDirDialog.h"
#include "wx/sizer.h"
#include "wx/dirdlg.h"
#include "wx/dir.h"

const long LogDirDialog::ID_LOG_DIR_DIALOG = wxNewId();
const long LogDirDialog::ID_TEXT_DEFAULT_DIR = wxNewId();
const long LogDirDialog::ID_BTN_SELECT_DEFAULT_DIR = wxNewId();
const long LogDirDialog::ID_CHKBOX_USE_LAST_DIR = wxNewId();
const long LogDirDialog::ID_BTN_OK = wxNewId();
const long LogDirDialog::ID_BTN_CANCEL = wxID_CANCEL;

BEGIN_EVENT_TABLE( LogDirDialog, wxDialog)
	EVT_BUTTON( ID_BTN_OK, LogDirDialog::OnBtnOk_Clicked)
	EVT_BUTTON( ID_BTN_SELECT_DEFAULT_DIR, LogDirDialog::OnBtnSelecDefaulttDir_Clicked)
	EVT_CHECKBOX( ID_CHKBOX_USE_LAST_DIR, LogDirDialog::OnChkboxUseLastDir_Clicked)
END_EVENT_TABLE()

LogDirDialog::LogDirDialog(LogDirSettings* settings, wxWindow* parent, const wxPoint& pos,const wxSize& size) :
    wxDialog( parent, ID_LOG_DIR_DIALOG, wxT("Set Default Log Directory"), pos, size),
    m_settings(settings)
{
	wxBoxSizer *sizerLogDirAndBtn, *sizerOkCancelBtn, *sizerTop;

    m_textDefaultDir = new wxTextCtrl( this, ID_TEXT_DEFAULT_DIR, settings->defaultDir);
    m_btnSelectDefaultDir = new wxButton( this, ID_BTN_SELECT_DEFAULT_DIR, wxT("..."), wxDefaultPosition, wxSize( 30, wxDefaultSize.GetY()));
	sizerLogDirAndBtn = new wxBoxSizer( wxHORIZONTAL); 
	sizerLogDirAndBtn->Add( m_textDefaultDir, 1, wxTOP | wxBOTTOM | wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
	sizerLogDirAndBtn->Add( m_btnSelectDefaultDir, 0, wxTOP | wxBOTTOM | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

    m_chkboxUseLastDir = new wxCheckBox( this, ID_CHKBOX_USE_LAST_DIR, wxT("Use last-visited directory."));

	m_btnOk = new wxButton( this, ID_BTN_OK, wxT("OK"));
    m_btnCancel = new wxButton( this, ID_BTN_CANCEL, wxT("Cancel"));
	sizerOkCancelBtn = new wxBoxSizer( wxHORIZONTAL);
	sizerOkCancelBtn->Add( m_btnOk, 0, wxRIGHT, 10);
	sizerOkCancelBtn->Add( m_btnCancel, 0, wxLEFT, 10);

	sizerTop = new wxBoxSizer( wxVERTICAL);
	sizerTop->Add( sizerLogDirAndBtn, 0, wxEXPAND | wxALL, 5);
	sizerTop->Add( m_chkboxUseLastDir, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
	sizerTop->Add( sizerOkCancelBtn, 0, wxALL | wxALIGN_CENTER, 10);

	this->SetSizer( sizerTop);
	sizerTop->Fit( this);
	sizerTop->SetSizeHints(this);

	wxSize new_size = this->GetSize();
	new_size.SetWidth( 400);
	this->SetSize( new_size);

    if ( m_settings)
    {
        m_chkboxUseLastDir->SetValue(m_settings->useLastDir);
        m_textDefaultDir->Enable(!m_settings->useLastDir);
        m_btnSelectDefaultDir->Enable(!m_settings->useLastDir);
    }
}

LogDirDialog::~LogDirDialog()
{
}

void LogDirDialog::OnBtnOk_Clicked(wxCommandEvent &evt)
{
	if ( m_settings)
	{
		m_settings->defaultDir = m_textDefaultDir->GetValue();
		m_settings->useLastDir = m_chkboxUseLastDir->GetValue();
		EndModal( wxID_OK);
	}
	else
	{
		EndModal( wxID_CANCEL);
	}
}

void LogDirDialog::OnBtnSelecDefaulttDir_Clicked( wxCommandEvent& evt)
{
	wxDirDialog* dlg = 
		new wxDirDialog(
			this,
			wxT("Select default directory..."),
			wxDir::Exists(m_textDefaultDir->GetValue()) ? m_textDefaultDir->GetValue() : wxEmptyString);

	if ( dlg->ShowModal() == wxID_OK)
	{
		m_textDefaultDir->SetValue( dlg->GetPath());
	}

	dlg->Destroy();
}

void LogDirDialog::OnChkboxUseLastDir_Clicked( wxCommandEvent& evt)
{
	m_textDefaultDir->Enable(!m_chkboxUseLastDir->IsChecked());
	m_btnSelectDefaultDir->Enable(!m_chkboxUseLastDir->IsChecked());
}

