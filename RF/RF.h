/*
 * RF.h
 *
 *  Created on: Sep 11, 2015
 *      Author: Huan
 */

#ifndef RF_H_
#define RF_H_

extern void RFinit();
extern int RFsend(const uint8_t* pcBuf,uint32_t ui32Len);
extern void RFprint(const char * restrict format, ...);


#endif /* RF_H_ */
