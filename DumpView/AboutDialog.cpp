#include "AboutDialog.h"
#include "version.h"

//(*InternalHeaders(AboutDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/regex.h>

//(*IdInit(AboutDialog)
const long AboutDialog::ID_STATICBITMAP1 = wxNewId();
const long AboutDialog::ID_LABEL_COPYRIGHT = wxNewId();
//*)

BEGIN_EVENT_TABLE(AboutDialog,wxDialog)
	//(*EventTable(AboutDialog)
	//*)
END_EVENT_TABLE()

AboutDialog::AboutDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	// Parse date for Year
	wxRegEx reDate("([A-Za-z]+) ([0-9]+) ([0-9]+)");
	wxString strYear = ( reDate.IsValid() && reDate.Matches( BUILD_DATE)) ? reDate.GetMatch( BUILD_DATE, 3) : "2013";

	wxBoxSizer* BoxSizer1;
	
	Create(parent, wxID_ANY, _("About Dump View..."), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "wxID_ANY");
	SetClientSize(wxSize(346,101));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);

    wxIcon icon( "mondrian", wxBITMAP_TYPE_ICO_RESOURCE);
    wxBitmap bmp;
    bmp.CopyFromIcon( icon);
    m_imgLogo = new wxStaticBitmap(this, ID_STATICBITMAP1, bmp, wxDefaultPosition, wxDefaultSize, 0, "ID_STATICBITMAP1");
	BoxSizer1->Add(m_imgLogo, wxSizerFlags()
		.Border(wxALL, 5)
		.Align(wxCENTER)
		.Proportion(0));

	m_labelCopyright = new wxStaticText(this, ID_LABEL_COPYRIGHT, wxString::Format( _("Dump View\nVersion %s (%s-%s)\n\nKenneth Lo (closer.tw@gmail.com)\nCopyright (c) 2009-%s"), VER, BUILD_DATE, BUILD_TIME, strYear), wxDefaultPosition, wxDefaultSize, 0, "ID_LABEL_COPYRIGHT");
	BoxSizer1->Add(m_labelCopyright, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Align(wxCENTER));

	m_buttonOk = new wxButton(this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "wxID_OK");
	BoxSizer1->Add(m_buttonOk, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Align(wxCENTER));
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
}

AboutDialog::~AboutDialog()
{
	//(*Destroy(AboutDialog)
	//*)
}

