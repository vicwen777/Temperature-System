#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"
#include "DS18B20.h"
#include "AT24C02.h"
#include "Key.h"
#include "Timer0.h"

sbit BEEP = P2^5;
sbit FUN = P1^0;

float T,TShow;						//温度
int TX;								//温度小数位
char THigh;							//温度报警线
unsigned char KeyNum;	  			//按下的独立按键键值
unsigned char Beep;	  				//蜂鸣器状态
unsigned char Warn;	  				//警报解除





void main()
{
	THigh = AT24C02_ReadByte(0);				//读取报警阈值
	if(THigh>125 || THigh<-55)		//判断阈值是否合理，如不合理赋予默认值
	{
		THigh = 30;
	}
	DS18B20_ConvertT();							//清除温度传感器的初始值
	Delay(1000);
	Timer0_Init();								//初始化定时器0
	LCD_Init();									//LCD初始化 
	LCD_ShowString(1,1,"T:");
	LCD_ShowString(2,1,"TH:");
		
	while(1)
	{
		KeyNum = Key();							//读取按键值
		
		/*温度读取及显示*/

		if(T<0)
		{
			LCD_ShowChar(1,3,'-');
			TShow = -T;
		}
		else
		{
			TShow = T;
			LCD_ShowChar(1,3,'+');
		}
		TX = (unsigned long)(TShow*100)&100;	//求出温度的两位小数位
		LCD_ShowNum(1,4,TShow,3);				//显示温度整数部分
		LCD_ShowChar(1,7,'.');					//显示小数点
		LCD_ShowNum(1,8,TX,2);					//显示温度小数部分

		
		/*阈值判断及显示*/
		
		if(KeyNum)
		{
			if(KeyNum == 1)						//按键1按下THigh加1
			{
				THigh++;
				if(THigh > 125){THigh=125;}
			}
			if(KeyNum == 2)						//按键1按下THigh减1
			{
				THigh--;
				if(THigh < -55){THigh=-55;}
			}
			if(KeyNum == 3)						//按键3按下存储温度阈值设置
			{
				AT24C02_WhiteByte(0,THigh);	
			}
			if(KeyNum == 4)						//按键4按下解除警报，10秒后若温度还超过安全阈值，警报继续
			{
				Warn = 1;	
			}
			
		}	
		LCD_ShowSignedNum(2,4,THigh,3);			//显示温度报警线
				
		/*温度状态判断及显示*/
		
		if(T>THigh && Warn == 0)				//温度过高且，未手动解除警报，声光报警，风扇开启
		{
			LCD_ShowString(1,10,"  High ");
			LCD_ShowString(2,8,"   !!!!!!");
			Beep = 1;
			FUN = 1;
			P2 &= 0xE0;
		}
		else if(T>THigh && Warn == 1)			//温度过高且，手动解除警报，声光报警解除，风扇开启
		{
			LCD_ShowString(1,10,"  High ");
			LCD_ShowString(2,8,"   !!!!!!");				
			Beep = 0;
			FUN = 1;
			P2 |= 0x1F;
		}
		else
		{
			LCD_ShowString(1,10," Normal");
			LCD_ShowString(2,8,"         ");
			Beep = 0;
			FUN = 0;	
			P2 |= 0x1F;
		}

	}
	
}


/*定时器0中断函数*/

void Timer0_Rountine() interrupt 1	
{
	static unsigned int T0Count1,T0Count2,T0Count3,T0Count4;

	TL0 = 0x66;					//设置定时初值
	TH0 = 0xFC;					//设置定时初值
	T0Count1++;
	T0Count2++;
	T0Count3++;
	if(Warn){T0Count4++;}
		
	
	if(T0Count1 >= 20)			
	{
		T0Count1 = 0;
		Key_Loop();				//扫描独立按键
	}
	if(T0Count2 >= 10)			//每隔10ms对温度进行扫描
	{
		T0Count2 = 0;
		DS18B20_ConvertT();
		T = DS18B20_ReadT();
	}
	if(T0Count3 >= 50)			//每隔10ms对温度进行扫描
	{
		T0Count3 = 0;
		if(Beep)
		{
			BEEP = ~BEEP;
		}	
	}
	if(T0Count4 >= 10000)			//每隔10ms对温度进行扫描
	{
		T0Count4 = 0;
		Warn = 0;
	}
}