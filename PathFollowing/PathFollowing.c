/*
 * PathFollowing.c
 *
 *  Created on: Sep 11, 2015
 *      Author: Huan
 */
#include "include.h"

//#define FOLLOW_ALGORITHM_PID
//#define FOLLOW_ALGORITHM_P
#define FOLLOW_ALGORITHM_PURE_PERSUIT
//#define _DEBUG_PID

#define LINE_DISTANCE (float)(5.0*D_SCALE)

#define SMALL_CULVATURE (float)(0.1e-4)
#define LARGE_CULVATURE (float)(0.5e-4)//2m
#define DELTA_CULVATURE (float)(0.1e-4)

#define TURN_DISTANCE (float)(4.0*D_SCALE)
#define POINT_DISTANCE (float)(4.0*D_SCALE)
#define TURN_ANGLE (float)(27.0)
#define DELTA_POS_STRAIGHT 200
#define DELTA_POS_TURN 80

#define LOOK_AHEAD_D (float)(2.0*D_SCALE)
#define WHEEL_BASE (float)(1.1*D_SCALE)
#define D_OFFSET (float)(0.4*D_SCALE)//distance form back wheels to robot center

typedef enum{
	INIT,
	LINE_FOLLOWING,
	TURN_LEFT,
	TURN_RIGHT,
	PRE_STOP,
	FINISH
}eSTATE;
typedef enum{
	UP=0,
	RIGHT=1,
	DOWN=2,
	LEFT=3
}eDIRECTION;

static PIDType pidDistance, pidAngle;

static eSTATE eState;
static eDIRECTION eDir;
static uint32_t linePassed=0;

static float x_corner[4];
static float y_corner[4];
static float angle_03,angle_10,angle_23;
static float m_03,m_10,m_23;
static float c_23;
static float delta_c;
static uint32_t totalLine=0;
static float K;

void setParamsDistance(float Kp,float Ki,float Kd)
{
	pidSetParams(&pidDistance,Kp,Ki,Kd);
}
void setParamsAngle(float Kp,float Ki,float Kd)
{
	pidSetParams(&pidAngle,Kp,Ki,Kd);
}
float SubMod(float angle1,float angle2)//result:-PI->PI
{
	if (angle1-angle2 > PI)
		return angle1-angle2-2*PI;
	else if (angle1-angle2 < -PI)
		return angle1-angle2+2*PI;
	else
		return angle1-angle2;
}
//			   ___
//	-			*|
//			*
//		*
//	*			+

static float signedDistance(float x, float y, float x1, float y1, float x2, float y2)
{
	if ((x1==y1) && (x2==y2))
		return -1.0;
	return ((y2-y1)*x+(x1-x2)*y-x1*y2+x2*y1)/sqrtf((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
}
static float distanceLine(float x, float y, float x1, float y1, float x2, float y2)
{
	if ((x1==y1) && (x2==y2))
		return -1.0;
	return (fabs((y2-y1)*x+(x1-x2)*y-x1*y2+x2*y1))/sqrtf((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
}
static float distancePoints(float x1, float y1, float x2, float y2)
{
	return sqrtf((x1-y1)*(x1-y1)+(x2-y2)*(x2-y2));
}
static float squareDistancePoints(float x1, float y1, float x2, float y2)
{
	return (x1-y1)*(x1-y1)+(x2-y2)*(x2-y2);
}
void calculateLookAheadPoint(float x, float y, float m, float c,float deltaX,float *xg,float *yg);
void PathFollowInit()
{
	// 3	2
	//
	//
	// 0	1

	x_corner[0] = 0.0;//1m
	y_corner[0] = 0.0;

	x_corner[1] = 128471;
	y_corner[1] = -224777;

	x_corner[2] = 360734;
	y_corner[2] = -96816;

	x_corner[3] = 236586;
	y_corner[3] = 130838;

	eState = INIT;

#warning check x_corner[3]==x_corner[0]
	m_03=(y_corner[3]-y_corner[0])/(x_corner[3]-x_corner[0]);
	m_10=(y_corner[0]-y_corner[1])/(x_corner[0]-x_corner[1]);
	m_23=(y_corner[3]-y_corner[2])/(x_corner[3]-x_corner[2]);
	c_23=y_corner[2]-m_23*x_corner[2];

	if (m_03>=0)
		delta_c = -LINE_DISTANCE*sqrtf(1+m_03*m_03);//next line is in the right of current line
	else
		delta_c = LINE_DISTANCE*sqrtf(1+m_03*m_03);

	angle_03 = atan2f(y_corner[3]-y_corner[0],x_corner[3]-x_corner[0]);
	angle_23 = atan2f(y_corner[3]-y_corner[2],x_corner[3]-x_corner[2]);
	angle_10 = atan2f(y_corner[0]-y_corner[1],x_corner[0]-x_corner[1]);

	initPID(&pidDistance,0.8,0.002,0.05);
	initPID(&pidAngle,1.0,0.005,0.05);
	enablePID(&pidDistance);
	enablePID(&pidAngle);
	pidSet(&pidDistance, 0);
	pidSet(&pidAngle, 0);

	totalLine = (uint32_t)(distanceLine(x_corner[1],y_corner[1],
			x_corner[0],y_corner[0],x_corner[3],y_corner[3])/LINE_DISTANCE)+1;
}

#warning check special line
void calculateLookAheadPoint(float x, float y, float m, float c,float deltaX,float *xg,float *yg)
{
	float A,B,C,x1,x2;
	A=1+m*m;
	B=-2*x+2*m*c-2*m*y;
	C=x*x+(c-y)*(c-y)-LOOK_AHEAD_D*LOOK_AHEAD_D;
	x1=(-B+sqrtf(B*B-4*A*C))/(2*A);
	x2=(-B/A)-x1;
	if ((x2-x1)*deltaX>=0)
	{
		*xg = x2;
		*yg = m*x2+c;
	}
	else
	{
		*xg = x1;
		*yg = m*x1+c;
	}
}
float calculateSteering(float x, float y, float angle, float xg, float yg)
{
	float x_tmp;
	x_tmp = (xg-x)*sinf(angle)-(yg-y)*cosf(angle);//change to robot's coordinate
#if 0
	RFprint("steer %d\r\n",(int)x_tmp);
#endif
	return atanf(-2*WHEEL_BASE*x_tmp/(LOOK_AHEAD_D*LOOK_AHEAD_D));//-pi/2->pi/2
#if 0
	x_tmp = LOOK_AHEAD_D*LOOK_AHEAD_D/(2*x_tmp);
	return atanf(WHEEL_BASE/sqrtf(x_tmp*x_tmp+D_OFFSET*D_OFFSET))*((x_tmp<0.0)?1:-1);
#endif
}
#ifdef	FOLLOW_ALGORITHM_PID
void FollowLine(float x, float y, float angle, float signedDistance)
{
	//pidCalc(&pidAngle, SubMod(angle_dest,angle)*RAD2PULSES, 10000);
	pidCalc(&pidDistance, signedDistance, 15000);
	steeringSet(pidDistance.PIDResult,DELTA_POS_STRAIGHT);
#ifdef _DEBUG_PID
	//RFprint("pid %d,%d\r\n",(int)pidAngle.PIDResult,(int)pidDistance.PIDResult);
	RFprint("pid %d,%d\r\n",(int)signedDistance,(int)pidDistance.PIDResult);
#endif
}
#endif

#ifdef FOLLOW_ALGORITHM_PURE_PERSUIT
void FollowLine(float x, float y, float angle, float m,float c,float deltaX)
{
	float steeringAngle,xg,yg;
	calculateLookAheadPoint(x,y,m,c,deltaX,&xg,&yg);
	steeringAngle = calculateSteering(x,y,angle,xg,yg);
#if 0
	RFprint("ahead %d,%d,%d\r\n",(int)xg,(int)yg,(int)(angle*RAD2DEGREE));
#endif
	steeringSet(steeringAngle*RAD2PULSES,DELTA_POS_STRAIGHT);
}
#endif
/*
 * angle: current angle between robot's heading and desired line (absolute value 0->PI)
 * turnAngle: total turning angle
 */
void TurnRight(float deltaAngle, float turnAngle)
{
	//giam toc do
	//quay
	float steeringAngle;
	RFprint("right\r\n");
#if 1
	//if (deltaAngle>0.3*turnAngle)
		steeringSet((int32_t)(-TURN_ANGLE*DEGREE2PULSES),DELTA_POS_TURN);
	//else
	//	steeringSet((int32_t)(-TURN_ANGLE*DEGREE2PULSES*0.5),DELTA_POS_TURN);
#else
	steeringAngle = atanf(WHEEL_BASE*K);
	steeringSet((int32_t)(-steeringAngle*DEGREE2PULSES),DELTA_POS_TURN);
	if ((deltaAngle>0.8*turnAngle) && (K<LARGE_CULVATURE))
		K += DELTA_CULVATURE;
	else if ((deltaAngle<0.2*turnAngle) && (K>SMALL_CULVATURE))
		K -= DELTA_CULVATURE;
#endif
}
void TurnLeft(float deltaAngle, float turnAngle)
{
	//giam toc do
	//quay
	float steeringAngle;
	RFprint("left\r\n");
#if 1
	//if (deltaAngle>0.3*turnAngle)
		steeringSet((int32_t)(TURN_ANGLE*DEGREE2PULSES),DELTA_POS_TURN);
	//else
	//	steeringSet((int32_t)(TURN_ANGLE*DEGREE2PULSES*0.5),DELTA_POS_TURN);
#else

	steeringAngle = atanf(WHEEL_BASE*K);
	steeringSet((int32_t)(steeringAngle*DEGREE2PULSES),DELTA_POS_TURN);
	if ((deltaAngle>0.8*turnAngle) && (K<LARGE_CULVATURE))
		K += DELTA_CULVATURE;
	else if ((deltaAngle<0.2*turnAngle) && (K>SMALL_CULVATURE))
		K -= DELTA_CULVATURE;
#endif
}

bool PreStopUp(float x, float y, float angle)
{
	static int stage = 0;
	switch (stage)
	{
	case 0:
	{
		float deltaAngle = fabs(SubMod(angle,angle_23));
		TurnLeft(deltaAngle,PI_HALF);
		if (deltaAngle < PI_HALF*0.1)
			stage++;
		break;
	}
	case 1:
	{
		FollowLine(x,y,angle,m_23,c_23,x_corner[3]-x_corner[2]);
		if (squareDistancePoints(x,y,x_corner[3],y_corner[3])<POINT_DISTANCE*POINT_DISTANCE)
			stage++;
		break;
	}
	case 2:
	{
		float deltaAngle = fabs(SubMod(angle,SubMod(angle_03,PI)));
		TurnLeft(deltaAngle,PI_HALF);
		if (deltaAngle < PI_HALF*0.1)
			stage++;
		break;
	}
	case 3:
	{
		FollowLine(x,y,angle,m_03,0.0,x_corner[0]-x_corner[3]);
		if (squareDistancePoints(x,y,x_corner[0],y_corner[0])<POINT_DISTANCE*POINT_DISTANCE)
		{
			stage=0;
			return true;
		}
		break;
	}
	}
	return false;
}
bool PreStopDown(float x, float y, float angle)
{
	static int stage = 0;
	switch (stage)
	{
	case 0:
	{
		float deltaAngle = fabs(SubMod(angle,angle_10));
		TurnRight(deltaAngle,PI_HALF);
		if (deltaAngle < PI_HALF*0.1)
			stage++;
		break;
	}
	case 1:
	{
		FollowLine(x,y,angle,m_10,0.0,x_corner[0]-x_corner[1]);
		if (squareDistancePoints(x,y,x_corner[0],y_corner[0])<POINT_DISTANCE*POINT_DISTANCE)
		{
			stage = 0;
			return true;
		}
		break;
	}
	}
	return false;
}
void PathFollow(float x, float y, float angle)
{
	static float angle_dest;
#if 0
	RFprint("%d,%d\r\n",(int)(fabsf(SubMod(angle,angle_dest))*RAD2DEGREE)
			,(int)(distanceLine(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])));
#endif
	switch (eState)
	{
	case INIT:
	{
		eState = LINE_FOLLOWING;
		eDir = UP;
		angle_dest = angle_03;
	}
	case LINE_FOLLOWING:
	{
		if (eDir==UP)
		{
			if (distanceLine(x,y,x_corner[2],y_corner[2],x_corner[3],y_corner[3])<TURN_DISTANCE)
			{
				linePassed++;
				if (linePassed==totalLine)
					eState = PRE_STOP;
				else
				{
					eState = TURN_RIGHT;
					K=SMALL_CULVATURE;
					angle_dest = SubMod(angle_03,PI);
				}
			}
			else
			{
#ifdef FOLLOW_ALGORITHM_PID
				FollowLine(x,y,angle,
						-signedDistance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])+LINE_DISTANCE*linePassed);
#endif

#ifdef FOLLOW_ALGORITHM_PURE_PERSUIT
				FollowLine(x,y,angle,m_03,linePassed*delta_c,x_corner[3]-x_corner[0]);
#endif
			}
		}
		else if (eDir==DOWN)
		{
			if (distanceLine(x,y,x_corner[0],y_corner[0],x_corner[1],y_corner[1])<TURN_DISTANCE)
			{
				linePassed++;
				if (linePassed==totalLine)
					eState = PRE_STOP;
				else
				{
					eState = TURN_LEFT;
					K=SMALL_CULVATURE;
					angle_dest = angle_03;
				}
			}
			else
			{
#ifdef FOLLOW_ALGORITHM_PID
				FollowLine(x,y,angle,
						signedDistance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])-LINE_DISTANCE*linePassed);
#endif

#ifdef FOLLOW_ALGORITHM_PURE_PERSUIT
				FollowLine(x,y,angle,m_03,linePassed*delta_c,x_corner[0]-x_corner[3]);
#endif
			}
		}
		break;
	}
	case TURN_LEFT:
	{
		float deltaAngle=fabsf(SubMod(angle,angle_dest));
		TurnLeft(deltaAngle,PI);
		if (deltaAngle<PI*0.1)//18 degree
			//			|| (distance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])
			//					>(LINE_DISTANCE*linePassed-0.3*D_SCALE)))
		{
			eState=LINE_FOLLOWING;
			eDir=UP;

			pidReset(&pidAngle);
			pidReset(&pidDistance);
		}
		break;
	}
	case TURN_RIGHT:
	{
		float deltaAngle=fabsf(SubMod(angle,angle_dest));
		TurnRight(deltaAngle,PI);
		if (deltaAngle<PI*0.1)
			//			|| (distanceLine(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])
			//					>(LINE_DISTANCE*linePassed-0.3*D_SCALE)))
		{
			eState=LINE_FOLLOWING;
			eDir=DOWN;

			pidReset(&pidAngle);
			pidReset(&pidDistance);
		}
		break;
	}
	case PRE_STOP:
	{
		if (((eDir == UP) && PreStopUp(x,y,angle))
				|| ((eDir == DOWN) && PreStopDown(x,y,angle)))
		{
			throttleStop();
			SSTOP_STOP;
			//HBridgeDisable();
			RFprint("finish\r\n");
			eState = FINISH;
		}
		break;
	}
	}

}
