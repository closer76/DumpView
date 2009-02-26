#ifndef __COM_PORT_SETTINGS_H__
#define __COM_PORT_SETTINGS_H__

#include <wx/string.h>

struct ComPortSetting
{
    int PortNum;
    long BaudRate;
    int ByteSize;
    int Parity;
    int StopBit;
};

#endif