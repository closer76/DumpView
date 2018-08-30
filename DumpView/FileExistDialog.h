#ifndef FILEEXISTDIALOG_H
#define FILEEXISTDIALOG_H

//(*Headers(FileExistDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

enum {
    FILEEXIST_OVERWRITE,
    FILEEXIST_APPEND,
    FILEEXIST_CANCEL
};

class FileExistDialog: public wxDialog
{
	public:

		FileExistDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~FileExistDialog();

        void SetFilename( const wxString &filename);

	protected:

		static const long ID_BUTTON_OVERWRITE;
		static const long ID_BUTTON_APPEND;
		static const long ID_BUTTON_CANCEL;

	private:

		wxButton* m_buttonAppend;
		wxButton* m_buttonCancel;
		wxStaticText* m_labelFilename;
		wxButton* m_buttonOverwrite;

        void OnButtonClicks( wxCommandEvent& evt);
};

#endif
