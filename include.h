/*
 * include.h
 *
 *  Created on: May 2, 2015
 *      Author: AntoniTran
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/qei.h"
#include "driverlib/pwm.h"
#include "driverlib/uart.h"

#include "SystemConfig.h"
#include "define.h"
#include "PID/PID.h"
#include "MYUART.h"
#include "InterruptHandler.h"
#include "RF/RF.h"
#include "PathFollowing/PathFollowing.h"
#include "SteeringControl/LMSEstimate_pos/LMSEstimate_pos.h"
#include "SteeringControl/MRC_pos/MRC_pos.h"
#include "SteeringControl/SteeringControl.h"
#include "ThrottleControl/ThrottleControl.h"
enum CMD_ID{
	STOP_CMD=1,
	START_CMD,
	SET_PID_DISTANCE_PARAMS,
	SET_PID_ANGLE_PARAMS,
	AUTO_MODE_CMD,
	MANUAL_MODE_CMD,
	SETPOINT_CMD
};

#endif /* INCLUDE_H_ */
