/*
 * File: LMSEstimate_pos.h
 *
 * Code generated for Simulink model 'LMSEstimate_pos'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Thu Aug 20 10:06:00 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. Traceability
 * Validation result: Not run
 */

#ifndef RTW_HEADER_LMSEstimate_pos_h_
#define RTW_HEADER_LMSEstimate_pos_h_

#ifndef LMSEstimate_pos_COMMON_INCLUDES_
# define LMSEstimate_pos_COMMON_INCLUDES_
#include <string.h>
#include "../../SteeringControl/LMSEstimate_pos/rtwtypes.h"
#endif                                 /* LMSEstimate_pos_COMMON_INCLUDES_ */

#include "../../SteeringControl/LMSEstimate_pos/LMSEstimate_pos_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((void) 0)
#endif

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real_T thetak1_DSTATE[6];            /* '<S1>/theta(k-1)' */
  real_T Pk1_DSTATE[36];               /* '<S1>/P(k-1)' */
  real_T UnitDelay2_DSTATE;            /* '<S1>/Unit Delay2' */
  real_T UnitDelay3_DSTATE;            /* '<S1>/Unit Delay3' */
  real_T UnitDelay4_DSTATE;            /* '<S1>/Unit Delay4' */
  real_T UnitDelay_DSTATE;             /* '<S1>/Unit Delay' */
  real_T UnitDelay1_DSTATE;            /* '<S1>/Unit Delay1' */
  real_T UnitDelay5_DSTATE;            /* '<S1>/Unit Delay5' */
} D_Work_LMSEstimate_pos;

/* Constant parameters (auto storage) */
typedef struct {
  /* Expression: [-1;1;0.1;1;0.1;0.1]
   * Referenced by: '<S1>/theta(k-1)'
   */
  real_T thetak1_X0[6];

  /* Expression: 1000*eye(6,6)
   * Referenced by: '<S1>/P(k-1)'
   */
  real_T Pk1_X0[36];
} ConstParam_LMSEstimate_pos;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T y;                            /* '<Root>/y' */
  real_T u;                            /* '<Root>/u' */
} ExternalInputs_LMSEstimate_pos;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T theta[6];                     /* '<Root>/theta' */
} ExternalOutputs_LMSEstimate_pos;

/* Block signals and states (auto storage) */
extern D_Work_LMSEstimate_pos LMSEstimate_pos_DWork;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_LMSEstimate_pos LMSEstimate_pos_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_LMSEstimate_pos LMSEstimate_pos_Y;

/* Constant parameters (auto storage) */
extern const ConstParam_LMSEstimate_pos LMSEstimate_pos_ConstP;

/* Model entry point functions */
extern void LMSEstimate_pos_initialize(void);
extern void LMSEstimate_pos_step(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/phi(k)' : Reshape block reduction
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'LMSEstimate_pos'
 * '<S1>'   : 'LMSEstimate_pos/LMS Estimation'
 * '<S2>'   : 'LMSEstimate_pos/LMS Estimation/lambda'
 */

/*-
 * Requirements for '<Root>': LMSEstimate_pos
 */
#endif                                 /* RTW_HEADER_LMSEstimate_pos_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
