/*
 * PID.h
 *
 *  Created on: May 9, 2015
 *      Author: AntoniTran
 */

#ifndef PID_H_
#define PID_H_

typedef struct
{
		float Kp;
		float Ki;
		float Kd;
		float pPart;
		float iPart;
		float dPart;
		int32_t SetPoint;
		//int32_t tempSetPoint;
		//int32_t deltaSetPoint;
		//int32_t MaxSetpoint;
		float PIDResult;
		float PIDResultTemp;
		int32_t PIDError;
		int32_t PIDErrorTemp1;
		int32_t PIDErrorTemp2;
		uint8_t Enable;
} PIDType;

void pidReset(PIDType *pidName);
void pidSet(PIDType *pidName, int32_t setPoint);
void pidCalc(PIDType *pidName, int32_t feedback, float maxResponse); //T (ms)
void initPID(PIDType *PIDname,float Kp, float Ki, float Kd);



#endif /* PID_H_ */
