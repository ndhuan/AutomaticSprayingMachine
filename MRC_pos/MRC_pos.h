/*
 * File: MRC_pos.h
 *
 * Code generated for Simulink model 'MRC_pos'.
 *
 * Model version                  : 1.3
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Thu Aug 20 10:24:17 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM 10
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. Traceability
 * Validation result: Passed (9), Warnings (3), Error (0)
 */

#ifndef RTW_HEADER_MRC_pos_h_
#define RTW_HEADER_MRC_pos_h_
#ifndef MRC_pos_COMMON_INCLUDES_
# define MRC_pos_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* MRC_pos_COMMON_INCLUDES_ */

#include "MRC_pos_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((void) 0)
#endif

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay5_DSTATE;            /* '<S1>/Unit Delay5' */
  real_T UnitDelay2_DSTATE;            /* '<S1>/Unit Delay2' */
  real_T UnitDelay_DSTATE;             /* '<S1>/Unit Delay' */
  real_T UnitDelay3_DSTATE;            /* '<S1>/Unit Delay3' */
  real_T UnitDelay1_DSTATE;            /* '<S1>/Unit Delay1' */
  real_T UnitDelay4_DSTATE;            /* '<S1>/Unit Delay4' */
} D_Work_MRC_pos;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T theta[6];                     /* '<Root>/theta' */
  real_T uc;                           /* '<Root>/uc' */
  real_T y;                            /* '<Root>/y' */
} ExternalInputs_MRC_pos;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T u;                            /* '<Root>/u' */
} ExternalOutputs_MRC_pos;

/* Block signals and states (auto storage) */
extern D_Work_MRC_pos MRC_pos_DWork;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_MRC_pos MRC_pos_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_MRC_pos MRC_pos_Y;

/* Model entry point functions */
extern void MRC_pos_initialize(void);
extern void MRC_pos_step(void);

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
 * '<Root>' : 'MRC_pos'
 * '<S1>'   : 'MRC_pos/Ru=Tuc - Sy'
 */

/*-
 * Requirements for '<Root>': MRC_pos
 */
#endif                                 /* RTW_HEADER_MRC_pos_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
