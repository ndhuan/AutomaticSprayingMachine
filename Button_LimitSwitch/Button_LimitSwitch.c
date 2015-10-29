/*
 * LimitSwitch.c
 *
 *  Created on: Oct 27, 2015
 *      Author: Huan
 */
#include "include.h"
#define DEBOUNCE_DELAY_MS 10

extern void Button1_ISR();
extern void Button2_ISR();

enum BUTTON_LIMIT_SW_TYPE{
	BUTTON1=1,
	BUTTON2=2,
	LIMIT_SW_THROTTLE_DOWN=4,
	LIMIT_SW_THROTTLE_UP=8,
	LIMIT_SW_STEERING_LEFT=16,
	LIMIT_SW_THROTTLE_RIGHT=32,
};
static int flag=0;

void DebounceISR()
{
	uint32_t status = GPIOIntStatus(GPIO_PORTA_BASE,true);
	GPIOIntClear(GPIO_PORTA_BASE,status);
	if (status & GPIO_PIN_2)
		flag |= BUTTON1;
	if (status & GPIO_PIN_3)
		flag |= BUTTON2;
	if (status & GPIO_PIN_4)
		flag |= LIMIT_SW_THROTTLE_DOWN;

	status = GPIOIntStatus(GPIO_PORTF_BASE,true);
	GPIOIntClear(GPIO_PORTF_BASE,status);

	if (status & GPIO_PIN_4)
		flag |= LIMIT_SW_THROTTLE_UP;

	if (flag)
	{
		ROM_TimerEnable(TIMER3_BASE,TIMER_A);
		ROM_TimerLoadSet(TIMER3_BASE,TIMER_A,80000*DEBOUNCE_DELAY_MS);
	}
}
void TimerDebounceISR()
{
	ROM_TimerIntClear(TIMER3_BASE,TIMER_A);
	ROM_TimerDisable(TIMER3_BASE,TIMER_A);
	if ((flag & BUTTON1) && !(ROM_GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_2)
			& GPIO_PIN_2))
	{
		Button1_ISR();
	}
	if ((flag & BUTTON2) && !(ROM_GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_3)
			& GPIO_PIN_3))
	{
		Button2_ISR();
	}
	if ((flag & LIMIT_SW_THROTTLE_DOWN) && (ROM_GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_4)
			& GPIO_PIN_4))
	{
		throttleStop();
		ROM_SysCtlDelay(ROM_SysCtlClockGet()/3000);
		throttleReset();
	}
	if ((flag & LIMIT_SW_THROTTLE_UP) && (ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)
			& GPIO_PIN_4))
	{
		throttleStop();
		SSTOP_STOP;
	}

	flag = 0;
}
void Config_Button_LimitSw(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

    ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3,
    		GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);
    ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_4,
    		GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPD);
    ROM_GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_FALLING_EDGE);
    ROM_GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);

    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,
    		GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPD);
    ROM_GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);

    GPIOIntRegister(GPIO_PORTA_BASE, &DebounceISR);
    GPIOIntRegister(GPIO_PORTF_BASE, &DebounceISR);

	ROM_IntEnable(INT_GPIOA);
	GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
	GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

	ROM_IntEnable(INT_GPIOF);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);//debounce timer
	ROM_TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER3_BASE,TIMER_A,80000*DEBOUNCE_DELAY_MS);
	TimerIntRegister(TIMER3_BASE, TIMER_A, &TimerDebounceISR);
	ROM_IntEnable(INT_TIMER3A);
	ROM_TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
}



