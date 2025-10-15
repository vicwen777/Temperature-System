#ifndef __I2C_H__
#define __I2C_H__

void I2C_Start(void);								//开始
void I2C_Stop(void);								//结束
void I2C_SendByte(unsigned char Byte);				//发送一个字节
unsigned char I2C_ReceiveByte(void);				//接收一个字节
void I2C_SendAck(unsigned char AckBit);				//发送应答,0为应答，1为非应答
unsigned char I2C_ReceiveAck(void);					//接收应答,0为应答，1为非应答

#endif