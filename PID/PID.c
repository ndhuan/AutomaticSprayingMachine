/*
 * PID.c
 *
 *  Created on: May 9, 2015
 *      Author: AntoniTran
 */
#include "include.h"

void pidSet(PIDType *pidName, int32_t setPoint)
{
	(*pidName).Enable = 0;

//	if (setPoint < -pidName.MaxSetpoint)
//		setPoint = -pidName.MaxSetpoint;
//	else if (setPoint > pidName.MaxSetpoint)
//		setPoint = pidName.MaxSetpoint;

//	pidName.tempSetPoint = setPoint;
//	pidName.deltaSetPoint = deltaSetPoint;
	(*pidName).SetPoint = setPoint;

	(*pidName).Enable = 1;
}



void pidCalc(PIDType *PIDname, int32_t Feedback, float MaxResponse)
{
	static float k1,k2,k3;

	(*PIDname).PIDError = (*PIDname).SetPoint - Feedback;

	k1=(*PIDname).Kp+(*PIDname).Ki+(*PIDname).Kd;
	k2=-(*PIDname).Kp-2*(*PIDname).Kd;
	k3=(*PIDname).Kd;

	(*PIDname).PIDResult=(*PIDname).PIDResultTemp +
			k1*(*PIDname).PIDError +
			k2*(*PIDname).PIDErrorTemp1 +
			k3*(*PIDname).PIDErrorTemp2;
	(*PIDname).PIDErrorTemp2=(*PIDname).PIDErrorTemp1;
	(*PIDname).PIDErrorTemp1=(*PIDname).PIDError;

	if ((*PIDname).PIDResult > MaxResponse)
		(*PIDname).PIDResult = MaxResponse;
	else if ((*PIDname).PIDResult < -MaxResponse)
		(*PIDname).PIDResult = -MaxResponse;

	(*PIDname).PIDResultTemp = (*PIDname).PIDResult;
}


void initPID(PIDType *PIDname,float Kp, float Ki, float Kd)
{
	(*PIDname).Kp=Kp;
	(*PIDname).Ki=Ki;
	(*PIDname).Kd=Kd;
	(*PIDname).PIDErrorTemp1=0;
	(*PIDname).PIDErrorTemp2=0;
	(*PIDname).PIDResultTemp=0;
	(*PIDname).Enable = 0;
}

void pidReset(PIDType *pidName)
{
	(*pidName).PIDErrorTemp1=0;
	(*pidName).PIDErrorTemp2=0;
	(*pidName).PIDResultTemp=0;
}
