#include "FileExistDialog.h"

#include <wx/intl.h>
#include <wx/string.h>

const long FileExistDialog::ID_BUTTON_OVERWRITE = wxNewId();
const long FileExistDialog::ID_BUTTON_APPEND = wxNewId();
const long FileExistDialog::ID_BUTTON_CANCEL = wxNewId();

FileExistDialog::FileExistDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
	: wxDialog(parent, id, _("File exists!"))
{
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxStaticText* StaticText3;
	wxBoxSizer* BoxSizer1;
	
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, wxID_ANY, _("This file already exists:"));
	BoxSizer1->Add(StaticText1, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 5)
		.Left());

	m_labelFilename = new wxStaticText(this, wxID_ANY, _("(filename)"));
	BoxSizer1->Add(m_labelFilename, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 5)
		.Center());
	StaticText3 = new wxStaticText(this, wxID_ANY, _("What do you want to do\?"));
	BoxSizer1->Add(StaticText3, wxSizerFlags()
		.Proportion(0)
		.Border(wxALL, 5)
		.Left());

	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_buttonOverwrite = new wxButton(this, ID_BUTTON_OVERWRITE, _("Overwrite"));
	BoxSizer2->Add(m_buttonOverwrite, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Center());

	m_buttonAppend = new wxButton(this, ID_BUTTON_APPEND, _("Append"));
	BoxSizer2->Add(m_buttonAppend, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Center());

	m_buttonCancel = new wxButton(this, ID_BUTTON_CANCEL, _("Cancel"));
	BoxSizer2->Add(m_buttonCancel, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Center());
	BoxSizer1->Add(BoxSizer2, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Center());
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	// Route all button events to one function
	Bind(wxEVT_BUTTON, &FileExistDialog::OnButtonClicks, this);
}

FileExistDialog::~FileExistDialog()
{
}

void FileExistDialog::OnButtonClicks(wxCommandEvent &evt)
{
    int retCode = evt.GetId();

    if ( retCode == ID_BUTTON_OVERWRITE)
    {
        retCode = FILEEXIST_OVERWRITE;
    }
    else if ( retCode == ID_BUTTON_APPEND)
    {
        retCode = FILEEXIST_APPEND;
    }
    else
    {
        retCode = FILEEXIST_CANCEL;
    }

    EndModal(retCode);
}

void FileExistDialog::SetFilename(const wxString &filename)
{
    m_labelFilename->SetLabel( filename);
}

