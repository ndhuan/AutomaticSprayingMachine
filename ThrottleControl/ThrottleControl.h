/*
 * ThrottleControl.h
 *
 *  Created on: Oct 13, 2015
 *      Author: Huan
 */

#ifndef SRC_THROTTLECONTROL_THROTTLECONTROL_H_
#define SRC_THROTTLECONTROL_THROTTLECONTROL_H_

void throttleControlInit();
void throttleSet(int32_t setPoint);
void throttleStop();
void throttleReset();
void throttleHome();
void Home_Timer2ISR();
#endif /* SRC_THROTTLECONTROL_THROTTLECONTROL_H_ */
