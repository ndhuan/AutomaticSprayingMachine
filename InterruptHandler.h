/*
 * InterruptHandler.h
 *
 *  Created on: May 9, 2015
 *      Author: AntoniTran
 */

#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_

void HBridgeEnable(void);
void HBridgeDisable(void);

void Throttle_WTimer2BISR(void);
void Steering_WTimer3AISR(void);
void SStop_WTimer2AISR(void);
void Mode_WTimer3BISR(void);

void PID_Timer5ISR(void);

void UartGPSIntHandler(void);
void UartRFIntHandler(void);

#define MAX_GPS_MSG_BYTE 200

#endif /* INTERRUPTHANDLER_H_ */
