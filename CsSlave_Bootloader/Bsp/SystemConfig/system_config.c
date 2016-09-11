/**
 * @file        system_config.c
 * @author      Weyne
 * @version     V01
 * @date        2016.09.11
 * @brief       系统配置部分
 * @note				
 * @attention   COYPRIGHT WEYNE
 */
 #include "system_config.h"
 
 
 IapFun Jump2App;
 
 /**
  * @brief  跳转到APP程序
  * @param  None
  * @retval None
  */
void JumpToApp(void)
{
	if(((*(vu32*)APP_BASE_ADDRESS)&0x2FFE0000) == 0x20000000)
	{
		SCB->VTOR = APP_BASE_ADDRESS;
		Jump2App = (IapFun)*(vu32*)(APP_BASE_ADDRESS+4);
		MSR_MSP(*(vu32*)APP_BASE_ADDRESS);
		Jump2App();
	}
}

 
 //THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(uint32_t addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

/************************ (C) COPYRIGHT  WEYNE *****END OF FILE****/

