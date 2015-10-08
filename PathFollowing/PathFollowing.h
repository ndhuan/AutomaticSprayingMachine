/*
 * PathFollow.h
 *
 *  Created on: Oct 7, 2015
 *      Author: Huan
 */

#ifndef SRC_PATHFOLLOWING_PATHFOLLOWING_H_
#define SRC_PATHFOLLOWING_PATHFOLLOWING_H_

void PathFollowInit();
void PathFollow(float x, float y, float angle);
void setParamsDistance(float Kp,float Ki,float Kd);
void setParamsAngle(float Kp,float Ki,float Kd);

#endif /* SRC_PATHFOLLOWING_PATHFOLLOWING_H_ */
