/*
 * MotorControl.h
 *
 *  Created on: Oct 6, 2015
 *      Author: Huan
 */

#ifndef SRC_STEERINGCONTROL_STEERINGCONTROL_H_
#define SRC_STEERINGCONTROL_STEERINGCONTROL_H_

void steeringSet(int32_t setPoint, int32_t delta);
void steeringControl();
void steeringControlInit();

#endif /* SRC_STEERINGCONTROL_STEERINGCONTROL_H_ */
