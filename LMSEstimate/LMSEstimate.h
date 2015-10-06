/*
 * File: LMSEstimate.h
 *
 * Code generated for Simulink model 'LMSEstimate'.
 *
 * Model version                  : 1.4
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Thu Aug 20 01:06:08 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. Traceability
 * Validation result: Passed (10), Warnings (2), Error (0)
 */

#ifndef RTW_HEADER_LMSEstimate_h_
#define RTW_HEADER_LMSEstimate_h_
#ifndef LMSEstimate_COMMON_INCLUDES_
# define LMSEstimate_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#endif                                 /* LMSEstimate_COMMON_INCLUDES_ */

#include "LMSEstimate_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((void) 0)
#endif

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real_T thetak1_DSTATE[4];            /* '<S1>/theta(k-1)' */
  real_T Pk1_DSTATE[16];               /* '<S1>/P(k-1)' */
  real_T UnitDelay2_DSTATE;            /* '<S1>/Unit Delay2' */
  real_T UnitDelay3_DSTATE;            /* '<S1>/Unit Delay3' */
  real_T UnitDelay_DSTATE;             /* '<S1>/Unit Delay' */
  real_T UnitDelay1_DSTATE;            /* '<S1>/Unit Delay1' */
} D_Work_LMSEstimate;

/* Constant parameters (auto storage) */
typedef struct {
  /* Expression: 1000*eye(4,4)
   * Referenced by: '<S1>/P(k-1)'
   */
  real_T Pk1_X0[16];
} ConstParam_LMSEstimate;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T u;                            /* '<Root>/u' */
  real_T y;                            /* '<Root>/y' */
} ExternalInputs_LMSEstimate;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T theta[4];                     /* '<Root>/theta' */
} ExternalOutputs_LMSEstimate;

/* Block signals and states (auto storage) */
extern D_Work_LMSEstimate LMSEstimate_DWork;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_LMSEstimate LMSEstimate_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_LMSEstimate LMSEstimate_Y;

/* Constant parameters (auto storage) */
extern const ConstParam_LMSEstimate LMSEstimate_ConstP;

/* Model entry point functions */
extern void LMSEstimate_initialize(void);
extern void LMSEstimate_step(void);

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
 * '<Root>' : 'LMSEstimate'
 * '<S1>'   : 'LMSEstimate/LMS Estimation'
 * '<S2>'   : 'LMSEstimate/LMS Estimation/lambda'
 */

/*-
 * Requirements for '<Root>': LMSEstimate
 */
#endif                                 /* RTW_HEADER_LMSEstimate_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
