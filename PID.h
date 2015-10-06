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
		int32_t tempSetPoint;
		int32_t deltaPulse;
		float PIDResult;
		float PIDResultTemp;
		int32_t PIDError;
		int32_t PIDErrorTemp1;
		int32_t PIDErrorTemp2;
		uint8_t Enable;
		int32_t MaxSetpoint;
} PIDType;

void PIDSpeedSet(int32_t SpeedSet);
void PIDPositionSet(int32_t SetPoint,int32_t deltaPulse);
void PIDPosCalc(int32_t Position, int32_t MaxResponse);
void PIDCalc(PIDType *PIDname, int32_t Feedback, float MaxResponse); //T (ms)
void PIDVerCalc(int32_t Speed, int32_t MaxResponse);
void InitPID(void);



#endif /* PID_H_ */
