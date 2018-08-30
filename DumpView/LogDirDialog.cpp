#include "LogDirDialog.h"
#include "wx/sizer.h"
#include "wx/dirdlg.h"
#include "wx/dir.h"

LogDirDialog::LogDirDialog(LogDirSettings* settings, wxWindow* parent, const wxPoint& pos,const wxSize& size)
	: wxDialog( parent, wxID_ANY, "Set Default Log Directory", pos, size)
	, m_settings(settings)
{
	wxBoxSizer *sizerLogDirAndBtn, *sizerOkCancelBtn, *sizerTop;

    m_textDefaultDir = new wxTextCtrl( this, wxID_ANY, settings->defaultDir);
	m_btnSelectDefaultDir = new wxButton(this, wxID_ANY, "...", wxDefaultPosition, wxSize(30, -1));
	m_btnSelectDefaultDir->Bind(wxEVT_BUTTON,
		&LogDirDialog::OnBtnSelecDefaulttDir_Clicked,
		this);
	sizerLogDirAndBtn = new wxBoxSizer( wxHORIZONTAL); 
	sizerLogDirAndBtn->Add(m_textDefaultDir, wxSizerFlags()
		.Proportion(1)
		.Border(wxTOP | wxBOTTOM | wxLEFT, 5)
		.Center());
	sizerLogDirAndBtn->Add( m_btnSelectDefaultDir, wxSizerFlags()
		.Proportion(0)
		.Border(wxTOP | wxBOTTOM | wxRIGHT, 5)
		.Center());

    m_chkboxUseLastDir = new wxCheckBox( this, wxID_ANY, "Use last-visited directory.");
	m_chkboxUseLastDir->Bind(wxEVT_CHECKBOX,
		&LogDirDialog::OnChkboxUseLastDir_Clicked,
		this);

	m_btnOk = new wxButton( this, wxID_ANY, "OK");
	m_btnOk->Bind(wxEVT_BUTTON,
		&LogDirDialog::OnBtnOk_Clicked,
		this);

	m_btnCancel = new wxButton( this, wxID_CANCEL, "Cancel");

	sizerOkCancelBtn = new wxBoxSizer( wxHORIZONTAL);
	sizerOkCancelBtn->Add( m_btnOk, wxSizerFlags()
		.Proportion(0)
		.Border(wxRIGHT, 10));
	sizerOkCancelBtn->Add( m_btnCancel, wxSizerFlags()
		.Proportion(0)
		.Border(wxLEFT, 10));

	sizerTop = new wxBoxSizer( wxVERTICAL);
	sizerTop->Add( sizerLogDirAndBtn, wxSizerFlags()
		.Proportion(0)
		.Expand()
		.Border(wxALL, 5));
	sizerTop->Add( m_chkboxUseLastDir, wxSizerFlags()
		.Proportion(0)
		.Border(wxLEFT | wxRIGHT | wxBOTTOM, 10));
	sizerTop->Add( sizerOkCancelBtn, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 10)
		.Center());

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
			"Select default directory...",
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

