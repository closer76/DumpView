#ifndef __COM_PORT_SETTINGS_H__
#define __COM_PORT_SETTINGS_H__

#include <wx/string.h>

struct ComPortSetting
{
    wxString PortName;
    long BaudRate;
	long ManualBaudRate;
    int ByteSize;
    int Parity;
    int StopBit;

	ComPortSetting() 
		: PortName("")
		, BaudRate(0)
		, ManualBaudRate(0)
		, ByteSize(0)
		, Parity(0)
		, StopBit(0) { }

	ComPortSetting(const ComPortSetting& other)
		: PortName(other.PortName)
		, BaudRate(other.BaudRate)
		, ManualBaudRate(other.ManualBaudRate)
		, ByteSize(other.ByteSize)
		, Parity(other.Parity)
		, StopBit(other.StopBit) { }

	ComPortSetting& operator=(const ComPortSetting& other) {
		this->PortName = other.PortName;
		this->BaudRate = other.BaudRate;
		this->ManualBaudRate = other.ManualBaudRate;
		this->ByteSize = other.ByteSize;
		this->Parity = other.Parity;
		this->StopBit = other.StopBit;

		return *this;
	}

	bool operator!=(const ComPortSetting& rhs) {
		return (this->PortName != rhs.PortName ||
			this->BaudRate != rhs.BaudRate ||
			this->ManualBaudRate != rhs.ManualBaudRate ||
			this->ByteSize != rhs.ByteSize ||
			this->Parity != rhs.Parity ||
			this->StopBit != rhs.StopBit);
	}
};

#endif