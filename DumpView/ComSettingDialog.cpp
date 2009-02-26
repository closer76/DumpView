#include "ComSettingDialog.h"

//(*InternalHeaders(ComSettingDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ComSettingDialog)
const long ComSettingDialog::ID_LABEL_PORT_NUM = wxNewId();
const long ComSettingDialog::ID_LABEL_BAUD_RATE = wxNewId();
const long ComSettingDialog::ID_LABEL_BYTE_SIZE = wxNewId();
const long ComSettingDialog::ID_LABEL_PARITY = wxNewId();
const long ComSettingDialog::ID_LABEL_STOP_BIT = wxNewId();
const long ComSettingDialog::ID_CHOICE_PORT_NUM = wxNewId();
const long ComSettingDialog::ID_CHOICE_BAUD_RATE = wxNewId();
const long ComSettingDialog::ID_CHOICE_BYTE_SIZE = wxNewId();
const long ComSettingDialog::ID_CHOICE_PARITY = wxNewId();
const long ComSettingDialog::ID_CHOICE_STOP_BIT = wxNewId();
const long ComSettingDialog::ID_BUTTON_OK = wxID_OK;
const long ComSettingDialog::ID_BUTTON_CANCEL = wxID_CANCEL;
//*)

BEGIN_EVENT_TABLE(ComSettingDialog,wxDialog)
	//(*EventTable(ComSettingDialog)
	//*)
END_EVENT_TABLE()

ComSettingDialog::ComSettingDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ComSettingDialog)
	Create(parent, id, _("COM Port Settings..."), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(306,244));
	Move(wxDefaultPosition);
	m_lablePortNumber = new wxStaticText(this, ID_LABEL_PORT_NUM, _("Port number:"), wxPoint(32,24), wxSize(104,14), wxALIGN_RIGHT, _T("ID_LABEL_PORT_NUM"));
	m_lableBaudRate = new wxStaticText(this, ID_LABEL_BAUD_RATE, _("Baud rate:"), wxPoint(32,56), wxSize(104,14), wxALIGN_RIGHT, _T("ID_LABEL_BAUD_RATE"));
	m_labelByteSize = new wxStaticText(this, ID_LABEL_BYTE_SIZE, _("Byte size:"), wxPoint(32,88), wxSize(104,14), wxALIGN_RIGHT, _T("ID_LABEL_BYTE_SIZE"));
	m_lableParity = new wxStaticText(this, ID_LABEL_PARITY, _("Parity:"), wxPoint(32,120), wxSize(104,14), wxALIGN_RIGHT, _T("ID_LABEL_PARITY"));
	m_labelStopBit = new wxStaticText(this, ID_LABEL_STOP_BIT, _("Stop bit:"), wxPoint(32,152), wxSize(104,14), wxALIGN_RIGHT, _T("ID_LABEL_STOP_BIT"));
	m_choicePortNumber = new wxChoice(this, ID_CHOICE_PORT_NUM, wxPoint(144,24), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_PORT_NUM"));
	m_choicePortNumber->Append(_("COM1"));
	m_choicePortNumber->Append(_("COM2"));
	m_choicePortNumber->Append(_("COM3"));
	m_choicePortNumber->Append(_("COM4"));
	m_choicePortNumber->Append(_("COM5"));
	m_choicePortNumber->Append(_("COM6"));
	m_choicePortNumber->Append(_("COM7"));
	m_choicePortNumber->Append(_("COM8"));
	m_choiceBaudRate = new wxChoice(this, ID_CHOICE_BAUD_RATE, wxPoint(144,56), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_BAUD_RATE"));
	m_choiceBaudRate->Append(_("110"));
	m_choiceBaudRate->Append(_("300"));
	m_choiceBaudRate->Append(_("600"));
	m_choiceBaudRate->Append(_("1200"));
	m_choiceBaudRate->Append(_("2400"));
	m_choiceBaudRate->Append(_("4800"));
	m_choiceBaudRate->Append(_("9600"));
	m_choiceBaudRate->Append(_("14400"));
	m_choiceBaudRate->Append(_("19200"));
	m_choiceBaudRate->Append(_("38400"));
	m_choiceBaudRate->Append(_("57600"));
	m_choiceBaudRate->Append(_("115200"));
	m_choiceBaudRate->Append(_("128000"));
	m_choiceBaudRate->Append(_("256000"));
	m_choiceByteSize = new wxChoice(this, ID_CHOICE_BYTE_SIZE, wxPoint(144,88), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_BYTE_SIZE"));
	m_choiceByteSize->Append(_("5"));
	m_choiceByteSize->Append(_("6"));
	m_choiceByteSize->Append(_("7"));
	m_choiceByteSize->Append(_("8"));
	m_choiceParity = new wxChoice(this, ID_CHOICE_PARITY, wxPoint(144,120), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_PARITY"));
	m_choiceParity->Append(_("None"));
	m_choiceParity->Append(_("Odd"));
	m_choiceParity->Append(_("Even"));
	m_choiceParity->Append(_("Mark"));
	m_choiceParity->Append(_("Space"));
	m_choiceStopBit = new wxChoice(this, ID_CHOICE_STOP_BIT, wxPoint(144,152), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_STOP_BIT"));
	m_choiceStopBit->Append(_("1"));
	m_choiceStopBit->Append(_("1.5"));
	m_choiceStopBit->Append(_("2"));
	m_buttonOk = new wxButton(this, ID_BUTTON_OK, _("OK"), wxPoint(56,200), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_OK"));
	m_buttonCancel = new wxButton(this, ID_BUTTON_CANCEL, _("Cancel"), wxPoint(176,200), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_CANCEL"));
	//*)
}

ComSettingDialog::~ComSettingDialog()
{
	//(*Destroy(ComSettingDialog)
	//*)
}
