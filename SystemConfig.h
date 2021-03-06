/*
 * SystemConfig.h
 *
 *  Created on: May 2, 2015
 *      Author: AntoniTran
 */

#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

void ConfigSystem(void);
void ConfigGPIO(void);
void ConfigMyUART(void);

void ConfigPulseTimer_Steering_Mode(void);
void ConfigPulseTimer_SStop_Throttle(void);

void ConfigSteeringControlTimer(uint32_t TimerIntervalms);
void ConfigHomeTimeoutTimer();

void ConfigBattSense(void);
void ConfigEncoder(void);

void ConfigPWM_SStop_Throttle(void);
void ConfigPWM_Steering(void);
void ConfigPWM_Steering_usingTimer(void);
void ConfigPWM_SprayValve(void);

void SetPWM_Servo_Throttle(uint32_t ulFrequency, int32_t ucDutyCycle);
void SetPWM_Servo_SStop(uint32_t ulFrequency, int32_t ucDutyCycle);
void SetPWM_Steering(uint32_t ulFrequency, int32_t ucDutyCycle);
void SetPWM_Servo_SprayValve(uint32_t ulFrequency, int32_t ucDutyCycle);
void SetPWM_Steering_usingTimer(uint32_t ulBaseAddr, uint32_t ulFrequency, int32_t ucDutyCycle);

void Config_Button();

#endif /* SYSTEMCONFIG_H_ */
