#include "AboutDialog.h"
#include "version.h"

//(*InternalHeaders(AboutDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/regex.h>

AboutDialog::AboutDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	// Parse date for Year
	wxRegEx reDate("([A-Za-z]+) ([0-9]+) ([0-9]+)");
	wxString strYear = ( reDate.IsValid() && reDate.Matches( BUILD_DATE)) ? 
		reDate.GetMatch( BUILD_DATE, 3) :
		"2013";

	wxBoxSizer* BoxSizer1;
	
	Create(parent, id, _("About Dump View..."));
	SetClientSize(wxSize(346,101));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);

    wxIcon icon( "mondrian", wxBITMAP_TYPE_ICO_RESOURCE);
    wxBitmap bmp;
    bmp.CopyFromIcon( icon);
    m_imgLogo = new wxStaticBitmap(this, wxID_ANY, bmp);
	BoxSizer1->Add(m_imgLogo, wxSizerFlags()
		.Border(wxALL, 5)
		.Align(wxCENTER)
		.Proportion(0));

	m_labelCopyright = new wxStaticText(this, wxID_ANY,
		wxString::Format( _("Dump View\nVersion %s (%s-%s)\n\nKenneth Lo (closer.tw@gmail.com)\nCopyright (c) 2009-%s"), VER, BUILD_DATE, BUILD_TIME, strYear));
	BoxSizer1->Add(m_labelCopyright, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Align(wxCENTER));

	m_buttonOk = new wxButton(this, wxID_OK, _("OK"));
	BoxSizer1->Add(m_buttonOk, wxSizerFlags()
		.Proportion(1)
		.Border(wxALL, 5)
		.Align(wxCENTER));
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
}

AboutDialog::~AboutDialog()
{
}

