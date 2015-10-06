/*
 * PID.c
 *
 *  Created on: May 9, 2015
 *      Author: AntoniTran
 */
#include "include.h"

PIDType PIDVelocity, PIDPosition;
extern int32_t T0=20;

void PIDPositionSet(int32_t SetPoint,int32_t deltaPulse)
{
	PIDPosition.Enable = 0;
//	PIDPosition.PIDResultTemp=0;
//	PIDPosition.PIDErrorTemp1 = 0;
//	PIDPosition.PIDErrorTemp2=0;
	if (SetPoint < -PIDPosition.MaxSetpoint)
		SetPoint = -PIDPosition.MaxSetpoint;
	else if (SetPoint > PIDPosition.MaxSetpoint)
		SetPoint = PIDPosition.MaxSetpoint;
	PIDPosition.tempSetPoint = SetPoint;
	PIDPosition.deltaPulse = deltaPulse;
	PIDPosition.Enable = 1;

//	ROM_QEIPositionSet(QEI0_BASE, 0);
//	Position += SetPoint;
}



void PIDCalc(PIDType *PIDname, int32_t Feedback, float MaxResponse)
{
	static float k1,k2,k3;
	(*PIDname).PIDError = (*PIDname).SetPoint - Feedback;
	k1=(*PIDname).Kp+(*PIDname).Ki+(*PIDname).Kd;
	k2=-(*PIDname).Kp-2*(*PIDname).Kd;
	k3=(*PIDname).Kd;



	(*PIDname).PIDResult=(*PIDname).PIDResultTemp+k1*(*PIDname).PIDError+k2*(*PIDname).PIDErrorTemp1+k3*(*PIDname).PIDErrorTemp2;
	(*PIDname).PIDErrorTemp2=(*PIDname).PIDErrorTemp1;
	(*PIDname).PIDErrorTemp1=(*PIDname).PIDError;

	if ((*PIDname).PIDResult > MaxResponse)
		(*PIDname).PIDResult = MaxResponse;
	else if ((*PIDname).PIDResult < -MaxResponse)
		(*PIDname).PIDResult = -MaxResponse;

	(*PIDname).PIDResultTemp = (*PIDname).PIDResult;
}


//MaxResponse: Max Duty Cycle

void InitPID(void)
{
//	PIDVelocity.Kp = 0;
//	PIDVelocity.Ki = 0;
//	PIDVelocity.Kd = 0;
//	PIDVelocity.PIDErrorTemp1=0;
//	PIDVelocity.PIDErrorTemp2=0;
//	PIDVelocity.PIDResultTemp=0;

	PIDPosition.Kp=0.15;
	PIDPosition.Ki=0.0005;
	PIDPosition.Kd=0.0000;
	PIDPosition.PIDErrorTemp1=0;
	PIDPosition.PIDErrorTemp2=0;
	PIDPosition.PIDResultTemp=0;
	PIDPosition.MaxSetpoint=25000;//45 degree
	PIDPosition.deltaPulse=300;
//	PIDVelocity.Enable = 0;
	PIDPosition.Enable = 0;
}
