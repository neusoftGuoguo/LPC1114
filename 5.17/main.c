#include "LPC11xx.h"
#define KEY1_DOWN() ((LPC_GPIO3->DATA & (1<<2)) != (1<<2))
#define KEY2_DOWN() ((LPC_GPIO3->DATA & (1<<3))!=(1<<3))
//uint32_t interval = SystemCoreClock/1000 ;
//uint32_t interval_1 = SystemCoreClock/2000;
uint16_t t = 0;
void CT32B1_Init(uint32_t interval,uint32_t interval_1)
{
	LPC_SYSCON ->SYSAHBCLKCTRL |= (1<<16);
	LPC_IOCON ->R_PIO1_2 &= ~(0x07);
	LPC_IOCON ->R_PIO1_2 |= 0x03;
	LPC_SYSCON ->SYSAHBCLKCTRL &= ~(1<<16);
	LPC_SYSCON ->SYSAHBCLKCTRL |= (1<<10);
	LPC_TMR32B1 ->TCR = 0x02;
	LPC_TMR32B1 ->PR =0;
	LPC_TMR32B1 ->MCR =0x02 << 9;
	LPC_TMR32B1 ->PWMC = 0x02;
	LPC_TMR32B1 ->MR1 = interval_1;
	LPC_TMR32B1 ->MR3 = interval;
	LPC_TMR32B1 ->TCR = 0x01;
}
void PIOINT3_IRQHandler()
{
	
    if((LPC_GPIO3->MIS & (1<<2))==(1<<2)) // ???P1.9?????
    {
        t++;
			if(t > 10){
				t = 1;
			}
			else{
//        while(KEY1_DOWN());
        CT32B1_Init(SystemCoreClock/1000,(SystemCoreClock*t/10000));
			}
        LPC_GPIO3->IC = (1<<2);  // ???
    }
    if((LPC_GPIO3->MIS & (1<<3))==(1<<3)) // ???P1.10?????
    {
        t--;
			if(t <= 0){
				t = 10;
			}
			else{
//        while(KEY2_DOWN());
        CT32B1_Init(SystemCoreClock/1000,(SystemCoreClock*t/10000));
			}
        LPC_GPIO3->IC = (1<<3);  // ???
    }
}
int main(void)
{
	  CT32B1_Init(SystemCoreClock/1000,SystemCoreClock/1000);
	  LPC_IOCON->PIO3_2 &= ~(0x07);//P1_9???GPIO
    LPC_GPIO3->DIR    &= ~(1<<2);//P1_9?????
    LPC_IOCON->PIO3_3 &= ~(0x07);//P1_10???GPIO
    LPC_GPIO3->DIR &= ~(1<<3);//P1_10?????
    
    LPC_GPIO3->IE |= (1<<2); // ??P1.9??????
    LPC_GPIO3->IE |= (1<<3); // ??P1.10??????
    LPC_GPIO3->IS &= ~(1<<2); // ??P1.9???????
    LPC_GPIO3->IS &= ~(1<<3); // ??P1.10???????
    LPC_GPIO3->IEV &= ~(1<<2); // ??P1.9????????
    LPC_GPIO3->IEV &= ~(1<<3); // ??P1.10????????
    NVIC_EnableIRQ(EINT3_IRQn); // ??GPIO1??
	while(1);
}
