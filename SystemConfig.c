/*
 * SystemConfig.c
 *
 *  Created on: May 2, 2015
 *      Author: AntoniTran
 */

#include "include.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"

#define MID_DUTY 85

UARTType UART_Type;


static volatile uint32_t ui32Load;
static volatile uint32_t ui32PWMClock;

uint32_t PIDVerLoop = 0;

void ConfigSystem(void)
{
	//Enable FPU
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
	// Configure clock
	//80 MHz
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

void ConfigGPIO(void)
{
	//H-Bridges Control
	ROM_SysCtlPeripheralEnable(ENAPORT_PERIPHERAL);
	ROM_GPIOPinTypeGPIOOutput(ENABLE_PORT, ENABLE_PIN);
	ROM_GPIOPinWrite(ENABLE_PORT, ENABLE_PIN, 0);

	// Read pulse from Remote by PD0
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
}

void ConfigPulseTimer_SStop_Throttle(void){
	// WTIMER 2, PD0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	ROM_GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
	ROM_GPIOPinConfigure(GPIO_PD0_WT2CCP0);
	ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	ROM_GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1);
	ROM_GPIOPinConfigure(GPIO_PD1_WT2CCP1);
	ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	ROM_TimerConfigure(WTIMER2_BASE,
			(TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP));

	ROM_TimerControlEvent(WTIMER2_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
	ROM_TimerControlEvent(WTIMER2_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

	ROM_IntMasterEnable();
	TimerIntRegister(WTIMER2_BASE, TIMER_A, &SStop_WTimer2AISR);
	ROM_IntEnable(INT_WTIMER2A);
	ROM_TimerIntEnable(WTIMER2_BASE, TIMER_CAPA_EVENT);
	ROM_TimerEnable(WTIMER2_BASE, TIMER_A);
	IntPrioritySet(INT_WTIMER2A, 0x20);

	TimerIntRegister(WTIMER2_BASE, TIMER_B, &Throttle_WTimer2BISR);
	ROM_IntEnable(INT_WTIMER2B);
	ROM_TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_EVENT);
	ROM_TimerEnable(WTIMER2_BASE, TIMER_B);
	IntPrioritySet(INT_WTIMER2B, 0x40);
}

void ConfigPulseTimer_Steering_Mode(void){
	//WTIMER3, PD2
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	ROM_GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_2);
	ROM_GPIOPinConfigure(GPIO_PD2_WT3CCP0);
	ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	ROM_GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_3);
	ROM_GPIOPinConfigure(GPIO_PD3_WT3CCP1);
	ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	ROM_TimerConfigure(WTIMER3_BASE,
			(TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP));

	ROM_TimerControlEvent(WTIMER3_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
	ROM_TimerControlEvent(WTIMER3_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

	ROM_IntMasterEnable();
	TimerIntRegister(WTIMER3_BASE, TIMER_A, &Steering_WTimer3AISR);
	ROM_IntEnable(INT_WTIMER3A);
	ROM_TimerIntEnable(WTIMER3_BASE, TIMER_CAPA_EVENT);
	ROM_TimerEnable(WTIMER3_BASE, TIMER_A);
	IntPrioritySet(INT_WTIMER3A, 0x40);

	TimerIntRegister(WTIMER3_BASE, TIMER_B, &Mode_WTimer3BISR);
	ROM_IntEnable(INT_WTIMER3B);
	ROM_TimerIntEnable(WTIMER3_BASE, TIMER_CAPB_EVENT);
	ROM_TimerEnable(WTIMER3_BASE, TIMER_B);
	IntPrioritySet(INT_WTIMER3B, 0x40);
}

void ConfigPIDTimer(uint32_t TimerIntervalms)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
	ROM_TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER5_BASE, TIMER_A, (ROM_SysCtlClockGet() / 1000) * TimerIntervalms - 1);	//Interval: //1:150

	ROM_IntMasterEnable();
	TimerIntRegister(TIMER5_BASE, TIMER_A, &PID_Timer5ISR);
	ROM_IntEnable(INT_TIMER5A);
	ROM_TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerEnable(TIMER5_BASE, TIMER_A);
	IntPrioritySet(INT_TIMER5A, 0x20);
}

void ConfigPWM_SStop_Throttle(void)
{
	//Configures the rate of the clock provided to the PWM module
	//= System Clock / 32 = 2.5 MHz
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_32);

	//Enable PWM0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	//Enable GPIO B
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	//Configures PB4, PB5 for use by the PWM peripheral.
	ROM_GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	ROM_GPIOPinConfigure(GPIO_PE4_M0PWM4);
	ROM_GPIOPinConfigure(GPIO_PE5_M0PWM5);
	//Set the mode of operation for a PWM generator 1
	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN |
                    PWM_GEN_MODE_NO_SYNC);

	ui32PWMClock = SysCtlClockGet() / 32;
	ui32Load = ui32PWMClock / PWM_THROTTLE;

	//Sets the period of the specified PWM generator block
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, ui32Load);
	//Sets the pulse width for the specified PWM output
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,	MID_DUTY * ui32Load / 1000);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5,	MID_DUTY * ui32Load / 1000);

	//Enable specified PWM output
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
	//Enable PWM generator 2
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void ConfigPWM_Steering(void)
{
	//Configures the rate of the clock provided to the PWM module
	//= System Clock / 1
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	//Enable PWM0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	//Enable GPIO B
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	//Configures PB6, PB7 for use by the PWM peripheral.
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7 | GPIO_PIN_6);
	ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);
	ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
	//Set the mode of operation for a PWM generator 0
	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
	//Sets the period of the specified PWM generator block
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ROM_SysCtlClockGet() / PWM_STEERING);
	//Sets the pulse width for the specified PWM output
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,	(ROM_SysCtlClockGet() / PWM_STEERING-1) / 2 - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,	(ROM_SysCtlClockGet() / PWM_STEERING-1) / 2 - 1);
	//Enable specified PWM output
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
	//Select the inversion mode for the selected PWM outputs
	ROM_PWMOutputInvert(PWM0_BASE, PWM_OUT_0_BIT, false);
	ROM_PWMOutputInvert(PWM0_BASE, PWM_OUT_1_BIT, true);
	//Enable PWM generator 0
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}

void ConfigPWM_Steering_usingTimer(void)
{
	// Using Timer0 PB6, PB7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	ROM_GPIOPinConfigure(GPIO_PB6_T0CCP0);
	ROM_GPIOPinConfigure(GPIO_PB7_T0CCP1);
	ROM_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6|GPIO_PIN_7 );

	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR| TIMER_CFG_B_PWM | TIMER_CFG_A_PWM );
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() / PWM_STEERING-1);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_B, ROM_SysCtlClockGet() / PWM_STEERING-1);
	ROM_TimerMatchSet(TIMER0_BASE, TIMER_A,TimerLoadGet(TIMER0_BASE, TIMER_A) / 2-1);
	ROM_TimerMatchSet(TIMER0_BASE, TIMER_B,TimerLoadGet(TIMER0_BASE, TIMER_B) / 2-1);
	ROM_TimerControlLevel(TIMER0_BASE,TIMER_A, false);
	ROM_TimerControlLevel(TIMER0_BASE,TIMER_B, true);

	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	ROM_TimerEnable(TIMER0_BASE, TIMER_B);
}

void ConfigPWM_SprayValve(void){

		// PWM7 - PD1
		//Configures the rate of the clock provided to the PWM module
		//= System Clock / 32 = 2.5 MHz
		ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_32);

		//Enable PWM0
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
		//Enable GPIO B
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		//Configures PD1 for use by the PWM peripheral.
		ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
		ROM_GPIOPinConfigure(GPIO_PB4_M0PWM2);
		//Set the mode of operation for a PWM generator 1
		ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN |
	                    PWM_GEN_MODE_NO_SYNC);

		ui32PWMClock = SysCtlClockGet() / 32;
		ui32Load = (ui32PWMClock / PWM_SPRAY_VALVE);

//		//Sets the period of the specified PWM generator block
		ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ui32Load);
//		//Sets the pulse width for the specified PWM output
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,	MID_DUTY * ui32Load / 1000-1);

		//Enable specified PWM output
		ROM_PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
		//Enable PWM generator 0
		ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);

}

void SetPWM_Steering_usingTimer(uint32_t ulBaseAddr, uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = SysCtlClockGet() / ulFrequency;
	ROM_TimerLoadSet(ulBaseAddr,TIMER_A, ulPeriod-1);
	ROM_TimerLoadSet(ulBaseAddr,TIMER_B, ulPeriod-1);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	ROM_TimerMatchSet(ulBaseAddr, TIMER_A, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
	ROM_TimerMatchSet(ulBaseAddr, TIMER_B, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

void SetPWM_Steering(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = ROM_SysCtlClockGet() / ulFrequency;
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

void SetPWM_Servo_Throttle(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod ;
	ulPeriod = ROM_SysCtlClockGet() / 32;
	ulPeriod = (ulPeriod / ulFrequency)-1;
	if (ucDutyCycle > 120)
		ucDutyCycle = 120;
	else if (ucDutyCycle < 50)
		ucDutyCycle = 50;
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, ulPeriod);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, ucDutyCycle * ulPeriod / 1000 - 1);

}

void SetPWM_Servo_SStop(uint32_t ulFrequency, int32_t ucDutyCycle){
	uint32_t ulPeriod ;
	ulPeriod = ROM_SysCtlClockGet() / 32;
	ulPeriod = (ulPeriod / ulFrequency);
	if (ucDutyCycle > 120)
		ucDutyCycle = 120;
	else if (ucDutyCycle < 50)
		ucDutyCycle = 50;
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, ulPeriod);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, ucDutyCycle * ulPeriod / 1000-1);

}

void SetPWM_Servo_SprayValve(uint32_t ulFrequency, int32_t ucDutyCycle){
	uint32_t ulPeriod ;
	ulPeriod = ROM_SysCtlClockGet() / 32;
	ulPeriod = (ulPeriod / ulFrequency);
	if (ucDutyCycle > 120)
		ucDutyCycle = 120;
	else if (ucDutyCycle < 50)
		ucDutyCycle = 50;
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ulPeriod);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, ucDutyCycle * ulPeriod / 1000-1);

}
void ConfigEncoder(void)
{
	//Unlock PD7 for GPIO functions
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) &= ~0x80;

#ifdef USE_QEI
    //Enable QEI0 Peripheral & GPIO D
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //Configure specified pins for use by the QEI0 module
    ROM_GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    ROM_GPIOPinConfigure(GPIO_PD6_PHA0);
    ROM_GPIOPinConfigure(GPIO_PD7_PHB0);
    //Configure QEI0 operation
    ROM_QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B|QEI_CONFIG_NO_RESET|QEI_CONFIG_QUADRATURE|QEI_CONFIG_SWAP, 0xFFFFFFFF);
    //Update Velocity every 5ms
    ROM_QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, ROM_SysCtlClockGet() / 200);
    //Enable QEI0 module
    ROM_QEIVelocityEnable(QEI0_BASE);
    ROM_QEIEnable(QEI0_BASE);
	ROM_QEIPositionSet(QEI0_BASE, 0);
#else
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_GPIODirModeSet (GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_DIR_MODE_IN);
	ROM_GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_FALLING_EDGE);
	ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

	GPIOIntRegister(GPIO_PORTD_BASE, &EncoderISR);
	IntEnable(INT_GPIOD);
	GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_6);
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_6);
#endif
}

void ConfigMyUART(void)
{
	UART_Type.PortName = UART_GPS;
	UART_Type.BaudRate = 230400;
	UART_Type.DataBits = 8;
	UART_Type.Parity = None;
	UART_Type.StopBits = 1;
	UART_Type.ISR = &UartGPSIntHandler;
	ConfigUART(&UART_Type);

	UART_Type.PortName = UART_RF;
	UART_Type.BaudRate = 57600;
	UART_Type.DataBits = 8;
	UART_Type.Parity = None;
	UART_Type.StopBits = 1;
	UART_Type.ISR = &UartRFIntHandler;
	ConfigUART(&UART_Type);

	UART_Type.PortName = UART_DEBUG;
	UART_Type.BaudRate = 115200;
	UART_Type.DataBits = 8;
	UART_Type.Parity = None;
	UART_Type.StopBits = 1;
	UART_Type.ISR = 0;
	ConfigUART(&UART_Type);
}




