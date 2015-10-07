/*
 * File: MRC.c
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

#include "MRC.h"
#include "MRC_private.h"
//POT 0%, Tqd=0.001s, T=0.005; wn=4000
//#define am1 -9.86e-5
//#define am2 2.016e-9
//#define bm1 1.0
//#define bm2 0.0004;

//POT 0%, Tqd=0.0002s, T=0.002; wn=20000
#define am1	-8.497e-18
#define am2	 1.805e-35
#define bm1 1.0
#define bm2 1.657e-16

/* Block signals and states (auto storage) */
D_Work_MRC MRC_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_MRC MRC_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_MRC MRC_Y;

/* Model step function */
void MRC_step(void)
{
  /* Sum: '<S1>/Subtract' incorporates:
   *  Fcn: '<S1>/r1'
   *  Fcn: '<S1>/s0'
   *  Fcn: '<S1>/s1'
   *  Fcn: '<S1>/t0'
   *  Fcn: '<S1>/t1'
   *  Inport: '<Root>/In1'
   *  Inport: '<Root>/uc_input'
   *  Inport: '<Root>/y_input'
   *  Product: '<S1>/Product'
   *  Product: '<S1>/Product1'
   *  Product: '<S1>/Product2'
   *  Product: '<S1>/Product3'
   *  Product: '<S1>/Product4'
   *  Sum: '<S1>/Add'
   *  Sum: '<S1>/Add1'
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay2'
   */
  MRC_Y.u_output = ((bm1 / MRC_U.In1[2] * MRC_U.uc_input + bm2 /
                     MRC_U.In1[2] * MRC_DWork.UnitDelay_DSTATE) - MRC_U.In1[3] /
                    MRC_U.In1[2] * MRC_Y.u_output) -
                    ((am1 - MRC_U.In1[0]) /MRC_U.In1[2] * MRC_U.y_input +
                    (am2 - MRC_U.In1[1]) / MRC_U.In1[2] *MRC_DWork.UnitDelay1_DSTATE);

  /* Saturate: '<S1>/Saturation' */
  if (MRC_Y.u_output >= MAX_STEERING_DUTY) {
    /* Sum: '<S1>/Subtract' */
    MRC_Y.u_output = MAX_STEERING_DUTY;
  } else {
    if (MRC_Y.u_output <= -MAX_STEERING_DUTY) {
      /* Sum: '<S1>/Subtract' */
      MRC_Y.u_output = -MAX_STEERING_DUTY;
    }
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Update for UnitDelay: '<S1>/Unit Delay' incorporates:
   *  Inport: '<Root>/uc_input'
   */
  MRC_DWork.UnitDelay_DSTATE = MRC_U.uc_input;

  /* Update for UnitDelay: '<S1>/Unit Delay1' incorporates:
   *  Inport: '<Root>/y_input'
   */
  MRC_DWork.UnitDelay1_DSTATE = MRC_U.y_input;
}

/* Model initialize function */
void MRC_initialize(void)
{
  /* (no initialization code required) */
	MRC_U.uc_input=0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
