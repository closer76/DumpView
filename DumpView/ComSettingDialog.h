#ifndef COMSETTINGDIALOG_H
#define COMSETTINGDIALOG_H

//(*Headers(ComSettingDialog)
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/button.h>
//*)

class ComSettingDialog: public wxDialog
{
	public:

		ComSettingDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ComSettingDialog();

		//(*Declarations(ComSettingDialog)
		wxStaticText* m_lablePortNumber;
		wxStaticText* m_lableBaudRate;
		wxStaticText* m_labelByteSize;
		wxStaticText* m_lableParity;
		wxStaticText* m_labelStopBit;
		wxChoice* m_choicePortNumber;
		wxChoice* m_choiceBaudRate;
		wxChoice* m_choiceByteSize;
		wxChoice* m_choiceParity;
		wxChoice* m_choiceStopBit;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		//*)

	protected:

		//(*Identifiers(ComSettingDialog)
		static const long ID_LABEL_PORT_NUM;
		static const long ID_LABEL_BAUD_RATE;
		static const long ID_LABEL_BYTE_SIZE;
		static const long ID_LABEL_PARITY;
		static const long ID_LABEL_STOP_BIT;
		static const long ID_CHOICE_PORT_NUM;
		static const long ID_CHOICE_BAUD_RATE;
		static const long ID_CHOICE_BYTE_SIZE;
		static const long ID_CHOICE_PARITY;
		static const long ID_CHOICE_STOP_BIT;
		static const long ID_BUTTON_OK;
		static const long ID_BUTTON_CANCEL;
		//*)

	private:

		//(*Handlers(ComSettingDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
