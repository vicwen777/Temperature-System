#include <REGX52.H>
#include "OneWire.h"

#define DS18B20_SKIP_ROM			0xCC
#define DS18B20_CONVERT_T			0x44
#define DS18B20_READ_SCRATCHPAD		0xBE

void DS18B20_ConvertT(void)
{
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_CONVERT_T);
}

float DS18B20_ReadT(void)
{
	unsigned char TLSB,THSB;
	int Temp;
	float T;
	
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
	TLSB = OneWire_ReceiveByte();
	THSB = OneWire_ReceiveByte();
	Temp = (THSB<<8) | TLSB;
	T = Temp/16.0;
	
	return T;
}