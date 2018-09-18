#ifndef COMSETTINGDIALOG_H
#define COMSETTINGDIALOG_H

//(*Headers(ComSettingDialog)
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/textctrl.h>
//*)

#include <list>
#include "ComPortSetting.h"

class ComSettingDialog: public wxDialog
{
	public:

		ComSettingDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ComSettingDialog();

		wxStaticText* m_labelPortName;
		wxStaticText* m_labelBaudRate;
		wxStaticText* m_labelManualBaudRate;
		wxStaticText* m_labelByteSize;
		wxStaticText* m_labelParity;
		wxStaticText* m_labelStopBit;
		wxChoice* m_choicePortName;
		wxChoice* m_choiceBaudRate;
		wxTextCtrl* m_textManualBaudRate;
		wxChoice* m_choiceByteSize;
		wxChoice* m_choiceParity;
		wxChoice* m_choiceStopBit;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;

        void SetPortSettings(const ComPortSetting &settings);
		ComPortSetting GetPortSettings( );

		void SetAvailableComPorts(const std::list<wxString>& port_list);

	private:

		// Event handlers
		void OnBaudRateChanged( wxCommandEvent& evt);
};

#endif
