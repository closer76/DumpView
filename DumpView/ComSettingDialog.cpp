#include "windows.h"
#include "ComSettingDialog.h"

//(*InternalHeaders(ComSettingDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/sizer.h>
#include <wx/valtext.h>

//(*IdInit(ComSettingDialog)
const long ComSettingDialog::ID_LABEL_PORT_NUM = wxNewId();
const long ComSettingDialog::ID_LABEL_BAUD_RATE = wxNewId();
const long ComSettingDialog::ID_LABEL_MANUAL_BAUD_RATE = wxNewId();
const long ComSettingDialog::ID_LABEL_BYTE_SIZE = wxNewId();
const long ComSettingDialog::ID_LABEL_PARITY = wxNewId();
const long ComSettingDialog::ID_LABEL_STOP_BIT = wxNewId();
const long ComSettingDialog::ID_CHOICE_PORT_NUM = wxNewId();
const long ComSettingDialog::ID_CHOICE_BAUD_RATE = wxNewId();
const long ComSettingDialog::ID_TEXT_MANUAL_BAUD_RATE = wxNewId();
const long ComSettingDialog::ID_CHOICE_BYTE_SIZE = wxNewId();
const long ComSettingDialog::ID_CHOICE_PARITY = wxNewId();
const long ComSettingDialog::ID_CHOICE_STOP_BIT = wxNewId();
const long ComSettingDialog::ID_BUTTON_OK = wxID_OK;
const long ComSettingDialog::ID_BUTTON_CANCEL = wxID_CANCEL;
//*)

BEGIN_EVENT_TABLE(ComSettingDialog,wxDialog)
	//(*EventTable(ComSettingDialog)
	EVT_CHOICE( ID_CHOICE_BAUD_RATE, ComSettingDialog::OnBaudRateChanged)
	//*)
END_EVENT_TABLE()

ComSettingDialog::ComSettingDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	wxBoxSizer* top_sizer = new wxBoxSizer( wxVERTICAL);
	wxFlexGridSizer* grid_sizer = new wxFlexGridSizer( 2, 5, 5);
	wxBoxSizer* button_sizer = new wxBoxSizer( wxHORIZONTAL);

	//(*Initialize(ComSettingDialog)
	Create(parent, id, _("COM Port Settings..."), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "id");
	SetClientSize(wxSize(306,244));
	Move(wxDefaultPosition);
	m_labelPortNumber = new wxStaticText(this, ID_LABEL_PORT_NUM, _("Port number:"), wxDefaultPosition, wxSize(104,14), wxALIGN_RIGHT, "ID_LABEL_PORT_NUM");
	m_labelBaudRate = new wxStaticText(this, ID_LABEL_BAUD_RATE, _("Baud rate:"), wxDefaultPosition, wxSize(104,14), wxALIGN_RIGHT, "ID_LABEL_BAUD_RATE");
	m_labelManualBaudRate = new wxStaticText(this, ID_LABEL_MANUAL_BAUD_RATE, _("(Manual)"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, "ID_LABEL_MANUAL");
	m_labelByteSize = new wxStaticText(this, ID_LABEL_BYTE_SIZE, _("Byte size:"), wxDefaultPosition, wxSize(104,14), wxALIGN_RIGHT, "ID_LABEL_BYTE_SIZE");
	m_labelParity = new wxStaticText(this, ID_LABEL_PARITY, _("Parity:"), wxDefaultPosition, wxSize(104,14), wxALIGN_RIGHT, "ID_LABEL_PARITY");
	m_labelStopBit = new wxStaticText(this, ID_LABEL_STOP_BIT, _("Stop bit:"), wxDefaultPosition, wxSize(104,14), wxALIGN_RIGHT, "ID_LABEL_STOP_BIT");
	m_choicePortNumber = new wxChoice(this, ID_CHOICE_PORT_NUM, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, "ID_CHOICE_PORT_NUM");
	m_choicePortNumber->Append(_("COM1"));
	m_choicePortNumber->Append(_("COM2"));
	m_choicePortNumber->Append(_("COM3"));
	m_choicePortNumber->Append(_("COM4"));
	m_choicePortNumber->Append(_("COM5"));
	m_choicePortNumber->Append(_("COM6"));
	m_choicePortNumber->Append(_("COM7"));
	m_choicePortNumber->Append(_("COM8"));
	m_choiceBaudRate = new wxChoice(this, ID_CHOICE_BAUD_RATE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, "ID_CHOICE_BAUD_RATE");
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
	m_choiceBaudRate->Append(_("Manual..."));
	m_textManualBaudRate = new wxTextCtrl( this, ID_TEXT_MANUAL_BAUD_RATE, _("115200"), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC), "ID_TEXT_MANUAL_BAUD_RATE");
	m_textManualBaudRate->Disable();
	m_choiceByteSize = new wxChoice(this, ID_CHOICE_BYTE_SIZE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, "ID_CHOICE_BYTE_SIZE");
	m_choiceByteSize->Append(_("5"));
	m_choiceByteSize->Append(_("6"));
	m_choiceByteSize->Append(_("7"));
	m_choiceByteSize->Append(_("8"));
	m_choiceParity = new wxChoice(this, ID_CHOICE_PARITY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, "ID_CHOICE_PARITY");
	m_choiceParity->Append(_("None"));
	m_choiceParity->Append(_("Odd"));
	m_choiceParity->Append(_("Even"));
	m_choiceParity->Append(_("Mark"));
	m_choiceParity->Append(_("Space"));
	m_choiceStopBit = new wxChoice(this, ID_CHOICE_STOP_BIT, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, "ID_CHOICE_STOP_BIT");
	m_choiceStopBit->Append(_("1"));
	m_choiceStopBit->Append(_("1.5"));
	m_choiceStopBit->Append(_("2"));
	m_buttonOk = new wxButton(this, ID_BUTTON_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "ID_BUTTON_OK");
	m_buttonCancel = new wxButton(this, ID_BUTTON_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "ID_BUTTON_CANCEL");
	//*)

	wxSizerFlags flags(1);
	flags.Expand();
	grid_sizer->Add(m_labelPortNumber, flags);
	grid_sizer->Add(m_choicePortNumber, flags);
	grid_sizer->Add(m_labelBaudRate,flags);
	grid_sizer->Add(m_choiceBaudRate, flags);
	grid_sizer->Add(m_labelManualBaudRate, flags);
	grid_sizer->Add(m_textManualBaudRate, flags);
	grid_sizer->Add(m_labelByteSize, flags);
	grid_sizer->Add(m_choiceByteSize, flags);
	grid_sizer->Add(m_labelParity, flags);
	grid_sizer->Add(m_choiceParity, flags);
	grid_sizer->Add(m_labelStopBit, flags);
	grid_sizer->Add(m_choiceStopBit, flags);

	button_sizer->Add( m_buttonOk, wxSizerFlags().Proportion(1).Expand().Border(wxRIGHT, 5));
	button_sizer->Add( m_buttonCancel, wxSizerFlags().Proportion(1).Expand().Border(wxLEFT, 5));

	top_sizer->Add( grid_sizer, wxSizerFlags().Expand().Border(wxALL, 10).Align(wxALIGN_CENTER_HORIZONTAL));
	top_sizer->Add( button_sizer, wxSizerFlags().Expand().Border(wxALL, 10).Align(wxALIGN_CENTER_HORIZONTAL));

	SetSizer( top_sizer);
	top_sizer->SetSizeHints(this);
}

ComSettingDialog::~ComSettingDialog()
{
	//(*Destroy(ComSettingDialog)
	//*)
}

long BaudRateTable [] =
{
    CBR_110,
    CBR_300,
    CBR_600,
    CBR_1200,
    CBR_2400,
    CBR_4800,
    CBR_9600,
    CBR_14400,
    CBR_19200,
    CBR_38400,
    CBR_57600,
    CBR_115200,
    CBR_128000,
    CBR_256000,
    -1
};

int ParityTable [] =
{
    NOPARITY,
    ODDPARITY,
    EVENPARITY,
    MARKPARITY,
    SPACEPARITY,
    -1
};

int StopBitTable [] =
{
    ONESTOPBIT,
    ONE5STOPBITS,
    TWOSTOPBITS,
    -1
};

static int ParseComPortName( const wxString& name)
{
	int result = 0;
	for ( int i = 3; i <= 4 && i < (int)name.length(); i++)
	{
		if ( ::wxIsdigit(name[i]))
		{
			result *= 10;
			result += name[i] - '0';
		}
	}

	return result;
}

void ComSettingDialog::SetPortSettings( ComPortSetting &settings)
{
    int i;

    // Port
	for ( i = 0; i < (int)m_choicePortNumber->GetCount(); i++)
	{
		if ( settings.PortNum == ParseComPortName( m_choicePortNumber->GetString(i)))
		{
			m_choicePortNumber->SetSelection(i);
			break;
		}
	}

    if ( i == m_choicePortNumber->GetCount())
    {
		if ( settings.PortNum < 1)
		{
			m_choicePortNumber->SetSelection(0);
		}
		else
		{
			m_choicePortNumber->Append( wxString::Format( "COM%d (Not Available)", settings.PortNum));
		}
    }

    // Byte Size
    if ( settings.ByteSize >= 5 && settings.ByteSize <= 8)
    {
        m_choiceByteSize->SetSelection( settings.ByteSize - 5);
    }
    else
    {
        m_choiceByteSize->SetSelection( 8 - 5);
    }

    // Baud Rate
    m_choiceBaudRate->SetSelection(11);    // Default: 115200
    for ( i=0; BaudRateTable[i] != -1; i++)
    {
        if ( BaudRateTable[i] == settings.BaudRate)
        {
            m_choiceBaudRate->SetSelection(i);
			m_textManualBaudRate->Disable();
            break;
        }
    }

	m_textManualBaudRate->SetValue(wxString::Format(_("%d"), settings.ManualBaudRate));
	if ( BaudRateTable[i] == -1)
	{
		m_choiceBaudRate->SetSelection(m_choiceBaudRate->GetCount() - 1);
		m_textManualBaudRate->Enable();
	}

    // Parity
    m_choiceParity->SetSelection(0);        // Default: None
    for ( i=0; ParityTable[i] != -1; i++)
    {
        if ( ParityTable[i] == settings.Parity)
        {
            m_choiceParity->SetSelection(i);
            break;
        }
    }

    // Stop bit
    m_choiceStopBit->SetSelection(0);        // Default: One
    for ( i=0; StopBitTable[i] != -1; i++)
    {
        if ( StopBitTable[i] == settings.StopBit)
        {
            m_choiceStopBit->SetSelection(i);
            break;
        }
    }
}

void ComSettingDialog::GetPortSettings( ComPortSetting &settings)
{
    // Port
    //settings.PortNum = m_choicePortNumber->GetSelection() + 1;
	settings.PortNum = ParseComPortName(m_choicePortNumber->GetString( m_choicePortNumber->GetSelection()));

    // Byte Size
    settings.ByteSize = m_choiceByteSize->GetSelection() + 5;

    // Baud Rate
    settings.BaudRate = BaudRateTable[m_choiceBaudRate->GetSelection()];
	if ( !m_textManualBaudRate->GetValue().ToLong( &settings.ManualBaudRate))
	{
		settings.ManualBaudRate = 115200;
	}

    // Parity
    settings.Parity = ParityTable[m_choiceParity->GetSelection()];

    // Stop bit
    settings.StopBit = StopBitTable[m_choiceStopBit->GetSelection()];
}

void ComSettingDialog::SetAvailableComPorts( std::list<int>* port_list)
{
	m_choicePortNumber->Clear();

	if ( port_list->size() == 0)
	{
		m_choicePortNumber->Append( "COM1 (Not Available)");
	}
	else
	{
		for ( std::list<int>::const_iterator itor = port_list->begin(); itor != port_list->end(); itor++)
		{
			m_choicePortNumber->Append( wxString::Format( "COM%d", *itor));
		}
	}
}

void ComSettingDialog::OnBaudRateChanged( wxCommandEvent& evt)
{
	if ( m_choiceBaudRate->GetSelection() == m_choiceBaudRate->GetCount() - 1)
	{
		m_textManualBaudRate->Enable();
	}
	else
	{
		m_textManualBaudRate->Disable();
	}
}