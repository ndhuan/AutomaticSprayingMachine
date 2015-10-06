/*
 * define.h
 *
 *  Created on: May 9, 2015
 *      Author: AntoniTran
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#define PI (float)3.14159265
#define RAD2PULSES (float)31830.98862//200000/2PI
#define DEGREE2PULSES (float)(5000/9)
#define D_SCALE (float)10000.0

#define ENAPORT_PERIPHERAL	SYSCTL_PERIPH_GPIOE
#define ENABLE_PORT			GPIO_PORTE_BASE
#define ENABLE_PIN			GPIO_PIN_2

#define PID_POSITION
#define USE_QEI

#define PWM_THROTTLE 	50

#define PWM_SSTOP 	50
#define SSTOP_STOP_DUTY 65
#define SSTOP_START_DUTY 95
#define SSTOP_STOP SetPWM_Servo_SStop(PWM_SSTOP,SSTOP_STOP_DUTY)
#define SSTOP_START SetPWM_Servo_SStop(PWM_SSTOP,SSTOP_START_DUTY)

#define PWM_SPRAY_VALVE 	50
#define PWM_STEERING		20000	//H-Bridge Freq (Hz)

#define PID_PERIOD_MS	5
#define PIDVER_LOOP		2

#define UART_GPS UART1_BASE//b0-b1
#define UART_RF UART3_BASE//c6-c7
#define UART_DEBUG UART0_BASE

#define LED1_ON ROM_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,0XFF)
#define LED1_OFF ROM_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,0)
#define LED1_TOGGLE ROM_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,~GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4))
#define LED2_ON ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7,0XFF)
#define LED2_OFF ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7,0)
#define LED2_TOGGLE ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7,~GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_7))
#define LED3_ON ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6,0XFF)
#define LED3_OFF ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6,0)
#define LED3_TOGGLE ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6,~GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_6))
#define LED4_ON ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,0XFF)
#define LED4_OFF ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,0)
#define LED4_TOGGLE ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,~GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_5))


#define LED_RED_ON ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0XFF)
#define LED_BLUE_ON ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0XFF)
#define LED_GREEN_ON ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0XFF)
#define LED_RED_OFF ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0)
#define LED_BLUE_OFF ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0)
#define LED_GREEN_OFF ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0)
#define LED_RED_TOGGLE ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,~GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1))
#define LED_BLUE_TOGGLE ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,~GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2))
#define LED_GREEN_TOGGLE ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,~GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_3))

#endif /* DEFINE_H_ */
