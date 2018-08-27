#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

//(*Headers(AboutDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class AboutDialog: public wxDialog
{
	public:

		AboutDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AboutDialog();

		wxStaticBitmap* m_imgLogo;
		wxStaticText* m_labelCopyright;
		wxButton* m_buttonOk;

	protected:

	private:
};

#endif
