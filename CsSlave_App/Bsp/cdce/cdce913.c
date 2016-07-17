/***************************************************************
* File: I2C.c
* Date : 2015-04-24
* Author: Weyne

* Version: V01
* Brief introduction: 
	实现了I2c接口软件模拟通讯协议,通讯速度200Khz左右
***************************************************************/

#include "cdce913.h"
#include <stdio.h>
#include "stdlib.h"
#include "math.h"

static void Delay_us(volatile uint8_t t)
{
	uint8_t i,n;
	for(n=0;n<t;n++)
	for(i=0;i<35;i++)
	{;}
}

static void I2C_Start(void)
{
	I2C_SDA=1;	  	  
	I2C_SCL=1;
	Delay_us(40);
 	I2C_SDA=0;
	Delay_us(6);
	I2C_SCL=0;
}	  


static void I2C_Stop(void)
{
	I2C_SCL=0;
	I2C_SDA=0;
 	Delay_us(4);
	I2C_SCL=1; 
	Delay_us(4);
	I2C_SDA=1;
	Delay_us(4);							   	
}

static uint8_t I2C_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	I2C_SDA=1;Delay_us(1);	
	I2C_SCL=1;Delay_us(3);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			
			return TP_NO_ACK;
		}
	}
	I2C_SCL=0;
	return TP_ACK;  
} 

static void I2C_Ack(void)
{
	I2C_SCL=0;
	I2C_SDA=0;
	Delay_us(3);
	I2C_SCL=1;
	Delay_us(3);
	I2C_SCL=0;
	Delay_us(1);
	I2C_SDA=1;
	Delay_us(4);
}

static void I2C_NAck(void)
{
	I2C_SCL=0;
	I2C_SDA=1;
	Delay_us(3);
	I2C_SCL=1;
	Delay_us(3);
	I2C_SCL=0;
}					 				     

static void I2C_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
    I2C_SCL=0;
    for(t=0;t<8;t++)
    {              
    I2C_SDA=(txd&0x80)>>7;
    txd<<=1; 	  
		Delay_us(2);  
		I2C_SCL=1;
		Delay_us(4); 
		I2C_SCL=0;	
		Delay_us(2);
    }
		I2C_SDA=0;
} 	    

static uint8_t I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
    for(i=0;i<8;i++ )
	{
        I2C_SCL=0; 
        Delay_us(3);
		   I2C_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
	   	Delay_us(2);
  }					 
    if (!ack)
        I2C_NAck();
    else
        I2C_Ack(); 
    return receive;
}


uint8_t CDCE_WriteByte(uint8_t Register,uint8_t data)
{
	I2C_Start(); 
	I2C_Send_Byte(SLAVE_WRITE);
  if(	I2C_Wait_Ack()==TP_NO_ACK)
	  return TP_NO_ACK;	
	I2C_Send_Byte(Register |0x80);	
  if(	I2C_Wait_Ack()==TP_NO_ACK)
	  return TP_NO_ACK;	
	I2C_Send_Byte(data);	
  if(	I2C_Wait_Ack()==TP_NO_ACK)
	  return TP_NO_ACK;			
	I2C_Stop();	
	  return TP_ACK;
}


uint8_t *CDCE_Read_8bit(uint8_t Register,uint16_t ParaAmount)
{
	static uint8_t data[1024]={0,0,0,0};  
	uint16_t i;
	uint8_t ack;
	if(ParaAmount==0)  
		ParaAmount=1;

	I2C_Start(); 
	I2C_Send_Byte(SLAVE_WRITE);
	ack=I2C_Wait_Ack();
	if(ack != TP_ACK)
	{
		printf("Error:I2C no ack\n");
	}
	I2C_Send_Byte(Register |0x80);	
	ack=I2C_Wait_Ack();		
	
	if(ack != TP_ACK)
	{
		printf("Error:I2C no ack\n");
	}	
	
	I2C_Stop();

	I2C_Start(); 
	I2C_Send_Byte(SLAVE_READ);	
	ack=I2C_Wait_Ack();	
	
	if(ack != TP_ACK)
	{
		printf("Error:I2C no ack\n");
	}
for(i=0;i<ParaAmount-1;i++)
{	
	data[i]=I2C_Read_Byte(1);
  Delay_us(100);	
}
	data[ParaAmount-1]=I2C_Read_Byte(0);	
	I2C_Stop();

	
	return data;
}

void CDCE_Init(uint16_t f_out)
{
	uint8_t *p;
	uint16_t M,N,Pdiv,Q,R;
	uint8_t reg18,reg19,reg1A,reg1B;
	int P;
	uint8_t fvco;
	float t,temp;
	
	for(Pdiv=1;Pdiv<127;Pdiv++)
	{
		t=(float)(f_out*Pdiv)/8;
		if((t>14)&&(t<28.75)) 
		{
			break;
		}
		
		if(t>28.75)
		{
			printf("Error:clk config error\n");
			return;
		}
	}
	
	for(N=4095;N>0;N--)
	{
		if(temp==t)
		{
			N++;
			break;
		}
			for(M=1;M<511;M++)
		{
			temp=(float)N/(float)M;
			if(temp==t)
				break;
		}
	}
	
	if(N==0)
	{
			printf("Error:clk config error\n");
			return;
	}

	P=4-abs(log((double)N/(double)M));
	if(P<0)
		P=0;
	Q=(int)((double)N*pow(2,(double)P)/(double)M);
	R=(double)N*pow(2,(double)P)-M*Q;

	fvco=8*N/M;
	
	//printf("M=%d,N=%d,Pdiv=%d,Q=%d,R=%d,fvco=%d,P=%d\n",M,N,Pdiv,Q,R,fvco,P);
	
	
	if(fvco<125)
		fvco=0;
	else if((fvco>=125)&&(fvco<150))
		fvco=1;
	else if((fvco>=150)&&(fvco<175))
		fvco=2;
	else
		fvco=3;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
	  
	p=CDCE_Read_8bit(0x00,1);
	if(*p==0x81)
		printf("Info:clk OK\n");
	else
		printf("Error:Clk communicate fail\n");
	
	CDCE_WriteByte(0X02,0XB4);     
	CDCE_WriteByte(0X03,(uint8_t)Pdiv);  
	CDCE_WriteByte(0X04,0X02);  
	CDCE_WriteByte(0X05,0XA0); 
	CDCE_WriteByte(0X06,0X40);  
	CDCE_WriteByte( 0X12,0X00);	 
	CDCE_WriteByte( 0X13,0X01);  
	CDCE_WriteByte( 0X14,0X6D);	 
	CDCE_WriteByte( 0X15,0X02);  
	CDCE_WriteByte( 0X16,0);		
	CDCE_WriteByte( 0X17,0);	

	reg18 =(N>>4)&0xFFF;
	reg19 =(N&0xf)<<4 |(R&0xf0)>>5;
	reg1A =(R&0x1f)<<3 | ((Q>>3)&0x7);
	reg1B =(Q&0x7)<<5 | (P&0x07)<<2  |(fvco&0x03);
	
	//printf("reg18=%x,reg19=%x,reg1A=%x,reg1B=%x\n",reg18,reg19,reg1A,reg1B);
	
	CDCE_WriteByte( 0x18,reg18);    
	CDCE_WriteByte( 0x19,reg19);    
	CDCE_WriteByte( 0X1A,reg1A);    
	CDCE_WriteByte( 0X1B,reg1B);	   

	CDCE_WriteByte( 0x1C,N);    
	CDCE_WriteByte( 0x1D,((N&0xf)<<4) | (R&0xf0));    
	CDCE_WriteByte( 0X1E,(R&0x0f) | (Q&0xf0));    
	CDCE_WriteByte( 0X1F,((Q&0x07)<<5)|((P&0x07)<<2) |(fvco&0x03));	   

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
}
