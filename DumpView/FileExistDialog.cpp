#include "FileExistDialog.h"

//(*InternalHeaders(FileExistDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(FileExistDialog)
const long FileExistDialog::ID_STATICTEXT1 = wxNewId();
const long FileExistDialog::ID_STATICTEXT2 = wxNewId();
const long FileExistDialog::ID_STATICTEXT3 = wxNewId();
const long FileExistDialog::ID_BUTTON_OVERWRITE = wxNewId();
const long FileExistDialog::ID_BUTTON_APPEND = wxNewId();
const long FileExistDialog::ID_BUTTON_CANCEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(FileExistDialog,wxDialog)
	//(*EventTable(FileExistDialog)
    EVT_BUTTON( ID_BUTTON_OVERWRITE, FileExistDialog::OnButtonClicks)
    EVT_BUTTON( ID_BUTTON_APPEND, FileExistDialog::OnButtonClicks)
    EVT_BUTTON( ID_BUTTON_CANCEL, FileExistDialog::OnButtonClicks)
	//*)
END_EVENT_TABLE()

FileExistDialog::FileExistDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(FileExistDialog)
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxStaticText* StaticText3;
	wxBoxSizer* BoxSizer1;
	
	Create(parent, wxID_ANY, _("File exists!"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "wxID_ANY");
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("This file already exists:"), wxDefaultPosition, wxDefaultSize, 0, "ID_STATICTEXT1");
	BoxSizer1->Add(StaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	m_labelFilename = new wxStaticText(this, ID_STATICTEXT2, _("(filename)"), wxDefaultPosition, wxDefaultSize, 0, "ID_STATICTEXT2");
	BoxSizer1->Add(m_labelFilename, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("What do you want to do\?"), wxDefaultPosition, wxDefaultSize, 0, "ID_STATICTEXT3");
	BoxSizer1->Add(StaticText3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_buttonOverwrite = new wxButton(this, ID_BUTTON_OVERWRITE, _("Overwrite"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "ID_BUTTON_OVERWRITE");
	BoxSizer2->Add(m_buttonOverwrite, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_buttonAppend = new wxButton(this, ID_BUTTON_APPEND, _("Append"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "ID_BUTTON_APPEND");
	BoxSizer2->Add(m_buttonAppend, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_buttonCancel = new wxButton(this, ID_BUTTON_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "ID_BUTTON_CANCEL");
	BoxSizer2->Add(m_buttonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
}

FileExistDialog::~FileExistDialog()
{
	//(*Destroy(FileExistDialog)
	//*)
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

