#include <REGX52.H>
#include "I2C.h"
#include "Delay.h"

#define AT24C02_ADDRESS		0xA0

/**
  * @brief 	在AT24C02中写入一个字节
  * @param  WordAddress 要写入字节的地址（0~255）
  * @param	Data 		要写入的数据
  * @retval 无
  */

void AT24C02_WhiteByte(unsigned char WordAddress,Data)
{
	I2C_Start();									//开始I2C通信
	I2C_SendByte(AT24C02_ADDRESS);					//主机发送从设备（AT24C02）的地址
	I2C_ReceiveAck();								//主机接收AT24C02的应答,建立通信
	I2C_SendByte(WordAddress);						//主机向AT24C02发送要写入数据的字地址
	I2C_ReceiveAck();								//主机接收AT24C02的应答
	I2C_SendByte(Data);								//主机向AT24C02发送要写入数据
	I2C_ReceiveAck();								//主机接收AT24C02的应答
	I2C_Stop();										//结束I2C通信
	Delay(5);
}

/**
  * @brief 	从AT24C02中读取一个字节
  * @param  WordAddress 要读取字节的地址（0~255）
  * @retval 读出的数据
  */

unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
	unsigned char Data;
	
	I2C_Start();									//开始I2C通信
	I2C_SendByte(AT24C02_ADDRESS);					//主机发送从设备（AT24C02）的地址
	I2C_ReceiveAck();								//主机接收AT24C02的应答,建立通信
	I2C_SendByte(WordAddress);						//主机向AT24C02发送要读出数据的字地址
	I2C_ReceiveAck();								//主机接收AT24C02的应答
	I2C_Start();									//开始I2C通信
	I2C_SendByte(AT24C02_ADDRESS|0x01);				//主机发送从设备（AT24C02）读的地址
	I2C_ReceiveAck();								//主机接收AT24C02的应答
	Data = I2C_ReceiveByte();						//主机读取数据
	I2C_SendAck(1);									//主机向AT24C02发送应答
	I2C_Stop();										//结束I2C通信
	
	return Data;									//返回读取的数据
}