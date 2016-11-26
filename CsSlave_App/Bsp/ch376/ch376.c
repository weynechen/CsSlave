/*******************************************************************************
 * * * 文件名称：ch376.c
 * * * 文件说明：CH376芯片 SPI串行连接的硬件抽象层 V1.0
 * * * 创建日期：PowerAVR / 2009-12-19
 * * * 修改日期： 
*******************************************************************************/
#include "stm32f10x.h"
#include "CH376INC.h"
#include "ch376.h"
#include "coolsaven_config.h"
#include <stdio.h>

//CH376 SPI Interface initization
void SPIx_Init(void){
   SPI_InitTypeDef SPI_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;

   /* Enable SPI1 and GPIOA clocks */
   RCC_SPI_CLOCK_CMD_ENABLE();

   /* Configure SPI1 pins SCK*/
   GPIO_InitStructure.GPIO_Pin = GPIO_SPI_CLK_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIO_SPI_CLK_PORT, &GPIO_InitStructure);
	
   /* Configure SPI1 pins MOSI */
   GPIO_InitStructure.GPIO_Pin = GPIO_SPI_MOSI_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIO_SPI_MOSI_PORT, &GPIO_InitStructure);	
	
	/* Configure SPI1 pins MISO */
   GPIO_InitStructure.GPIO_Pin = GPIO_SPI_MISO_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIO_SPI_MISO_PORT, &GPIO_InitStructure);	

	/* Configure CH376 pins CS */
   GPIO_InitStructure.GPIO_Pin = GPIO_SPI_CS_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIO_SPI_CS_PORT, &GPIO_InitStructure);           //CH376_CS

   /* SPIx configuration */ 
   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial = 7;
   SPI_Init(SPIx, &SPI_InitStructure);

   /* Enable SPIx  */
   SPI_Cmd(SPIx, ENABLE);
}

void CH376_INT_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable GPIOB clock */
	RCC_CH376_INT_CLK_ENABLE();

	/* Configure PB.08 pin as input floating */
	GPIO_InitStructure.GPIO_Pin = CH376_GPIO_INT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(CH376_GPIO_INT_PORT, &GPIO_InitStructure);  
}

//SPI1读写一字节数据
uint8_t CH376_ReadWrite(uint8_t writedat)
{
   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); /* Loop while DR register in not emplty */
   SPI_I2S_SendData(SPIx, writedat);  /* Send byte through the SPI1 peripheral */

   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET); /* Wait to receive a byte */
   return SPI_I2S_ReceiveData(SPIx); /* Return the byte read from the SPI bus */
}
/*****************************************************
 * Name:     mInitCH376Host
 * Function: 初始化CH376
 * Input: 	 no
 * Output:   no 
 * Author:   PowerAVR / 2009-12-19
 * Update:   	
*****************************************************/
void mDelay0_5uS(uint8_t cnt)  {
	uint16_t n=200;
	while(cnt--)
		for(; n; n--);
}

uint8_t CH376_Init(void)  
{
	 uint8_t res;
	
	 SPIx_Init();
	 CH376_INT_Init();
	
	
	//检测CH376 SPI 通信接口是否正常
	xWriteCH376Cmd( CMD11_CHECK_EXIST );   /* 测试单片机与CH376之间的通讯接口 */
	xWriteCH376Data( 0x65 );
	res = xReadCH376Data();
	SPI_CS_CLR( );								// 结束通信测试
	if ( res != 0x9A ) 
	{
		DEBUG(("ERR_USB_UNKNOWN:0x%x\n",res));//USART1_SendByte(0xe1);
		return( ERR_USB_UNKNOWN );  				/* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
	}
	
	 //设置CH376 工作模式
	 xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
	 xWriteCH376Data( 0x06 );						// 模式代码为0x06,表示切换到已启用的USB主机方式，自动产生SOF包
	 Delayus(20);
	 res = xReadCH376Data( );						// 返回操作状态
	 SPI_CS_CLR( );							        // 工作模式设置结束

	 //设置SPI SDO引脚在空闲时兼做中断方式
//	 xWriteCH376Cmd( CMD20_SET_SDO_INT ); /* 设置SPI的SDO引脚的中断方式 */
//	 xWriteCH376Data( 0x16 );
//	 xWriteCH376Data( 0x90 );  						/* SDO引脚在SCS片选无效时兼做中断请求输出 */
//	 SPI_CS_CLR( );							          // 结束设置SDO引脚方式

	 //返回初始化结果
	 if ( res == CMD_RET_SUCCESS ) {
		DEBUG(("CH376 Initization complete\n"));
		return( USB_INT_SUCCESS );
	 }
	DEBUG(("CH376 Initization fail! 0x%x\n",res));//USART1_SendByte(0xe2);
	return( ERR_USB_UNKNOWN );  				/* 设置模式错误 */
}
/*****************************************************
 * Name:     CH376_PORT_INIT
 * Function: 初始化CH376的SPI IO！
 * Input: 	 no
 * Output:   no 
 * Author:   PowerAVR / 2009-12-19
 * Update:   	
*****************************************************/
uint8_t mWaitInterrupt( void ) 
{								/* 等待CH376中断并获取状态,主机端等待操作完成,返回操作状态 */
	 while ( Query376Interrupt( ) == FALSE );  		/* 一直等中断 */ 				
	 xWriteCH376Cmd( CMD_GET_STATUS );  			/* 产生操作完成中断,获取中断状态 */
	 return( CH376_ReadWrite(0xFF) );
}

/*****************************************************
 * Name:     xWriteCH376Cmd
 * Function: 写命令到CH376去
 * Input: 	 no
 * Output:   no 
 * Author:   PowerAVR / 2009-12-19
 * Update:   	
*****************************************************/
void	xWriteCH376Cmd(uint8_t mCmd )  /* 向CH376写命令 */
{
	SPI_CS_SET();//GPIO_SetBits(GPIOA,GPIO_Pin_1);  /* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
	Delayus(5);
/* 对于双向I/O引脚模拟SPI接口,那么必须确保已经设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,SPI_SDO为输入方向 */
	SPI_CS_CLR();//GPIO_ResetBits(GPIOA,GPIO_Pin_1);  /* SPI片选有效 */
	CH376_ReadWrite( mCmd );  /* 发出命令码 */
	Delayus(3);;  /* 延时1.5uS确保读写周期大于1.5uS,或者用上面一行的状态查询代替 */
}

/*****************************************************
 * Name:     xWriteCH376Data
 * Function: 写数据到CH376去
 * Input: 	 no
 * Output:   no 
 * Author:   PowerAVR / 2009-12-19
 * Update:   	
*****************************************************/
void xWriteCH376Data(uint8_t mData){
	CH376_ReadWrite( mData );
	Delayus(5); 
}

/*****************************************************
 * Name:     xReadCH376Data
 * Function: 读取数据CH376
 * Input: 	 no
 * Output:   no 
 * Author:   PowerAVR / 2009-12-19
 * Update:   	
*****************************************************/
uint8_t xReadCH376Data(void){
	Delayus(2);  //确保时间大于0.6us
	return CH376_ReadWrite(0xff);
}

/*****************************************************
 * Name:     Query376Interrupt
 * Function: 查询CH376中断引脚状态
 * Input: 	 no
 * Output:   no 
 * Author:   PowerAVR / 2009-12-19
 * Update:   	
*****************************************************/
uint8_t	Query376Interrupt( void )
{
	return (GPIO_ReadInputDataBit(CH376_GPIO_INT_PORT, CH376_GPIO_INT_PIN) ? FALSE : TRUE);
}
