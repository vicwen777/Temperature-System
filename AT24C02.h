#ifndef __AT24C02_H__
#define __AT24C02_H__

void AT24C02_WhiteByte(unsigned char WordAddress,Data);		//在AT24C02中写入一个字节
unsigned char AT24C02_ReadByte(unsigned char WordAddress); 	//从AT24C02中读取一个字节


#endif