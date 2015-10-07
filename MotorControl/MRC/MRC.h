/*
 * File: MRC.h
 *
 * Code generated for Simulink model 'MRC'.
 *
 * Model version                  : 1.13
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Wed Aug 19 13:40:21 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. Traceability
 * Validation result: Passed (10), Warnings (2), Error (0)
 */

#ifndef RTW_HEADER_MRC_h_
#define RTW_HEADER_MRC_h_
#ifndef MRC_COMMON_INCLUDES_
# define MRC_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* MRC_COMMON_INCLUDES_ */

#include "MRC_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((void) 0)
#endif

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay_DSTATE;             /* '<S1>/Unit Delay' */
  real_T UnitDelay1_DSTATE;            /* '<S1>/Unit Delay1' */
} D_Work_MRC;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T In1[4];                       /* '<Root>/In1' */
  real_T y_input;                      /* '<Root>/y_input' */
  real_T uc_input;                     /* '<Root>/uc_input' */
} ExternalInputs_MRC;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T u_output;                     /* '<Root>/u_output' */
} ExternalOutputs_MRC;

/* Block signals and states (auto storage) */
extern D_Work_MRC MRC_DWork;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_MRC MRC_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_MRC MRC_Y;

/* Model entry point functions */
extern void MRC_initialize(void);
extern void MRC_step(void);

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
 * '<Root>' : 'MRC'
 * '<S1>'   : 'MRC/Ru=Tuc - Sy'
 */

/*-
 * Requirements for '<Root>': MRC
 */
#endif                                 /* RTW_HEADER_MRC_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
