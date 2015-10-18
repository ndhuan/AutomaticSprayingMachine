/*
 * PID.c
 *
 *  Created on: May 9, 2015
 *      Author: AntoniTran
 */
#include "include.h"

void pidSet(PIDType *pidName, int32_t setPoint)
{
//	(*pidName).Enable = 0;
	(*pidName).SetPoint = setPoint;
//	(*pidName).Enable = 1;
}

void pidCalc(PIDType *pidName, int32_t Feedback, float MaxResponse)
{
	static float k1,k2,k3;
	if (pidName->Enable)
	{
		(*pidName).PIDError = (*pidName).SetPoint - Feedback;

		k1=(*pidName).Kp+(*pidName).Ki+(*pidName).Kd;
		k2=-(*pidName).Kp-2*(*pidName).Kd;
		k3=(*pidName).Kd;

		(*pidName).PIDResult=(*pidName).PIDResultTemp +
				k1*(*pidName).PIDError +
				k2*(*pidName).PIDErrorTemp1 +
				k3*(*pidName).PIDErrorTemp2;
		(*pidName).PIDErrorTemp2=(*pidName).PIDErrorTemp1;
		(*pidName).PIDErrorTemp1=(*pidName).PIDError;

		if ((*pidName).PIDResult > MaxResponse)
			(*pidName).PIDResult = MaxResponse;
		else if ((*pidName).PIDResult < -MaxResponse)
			(*pidName).PIDResult = -MaxResponse;

		(*pidName).PIDResultTemp = (*pidName).PIDResult;
	}
}

void pidReset(PIDType *pidName)
{
	(*pidName).PIDErrorTemp1=0;
	(*pidName).PIDErrorTemp2=0;
	(*pidName).PIDResultTemp=0;
}

void pidSetParams(PIDType *pidName,float Kp, float Ki, float Kd)
{
	(*pidName).Kp=Kp;
	(*pidName).Ki=Ki;
	(*pidName).Kd=Kd;
	pidReset(pidName);
}

void initPID(PIDType *pidName,float Kp, float Ki, float Kd)
{
	pidSetParams(pidName,Kp, Ki, Kd);
	(*pidName).Enable = 0;
}

void enablePID(PIDType *pidName)
{
	(*pidName).Enable = 1;
	pidReset(pidName);
}

void disablePID(PIDType *pidName)
{
	(*pidName).Enable = 0;
}
