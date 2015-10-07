/*
 * File: MRC_pos.c
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

#include "define.h"
#include "MRC_pos.h"
#include "MRC_pos_private.h"

#define bm1 1.0
#define bm2 4.372e-8
#define bm3 5.245e-20
#define am1 -4.122e-9
#define am2 4.248e-18
#define am3 2.095e-41

/* Block signals and states (auto storage) */
D_Work_MRC_pos MRC_pos_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_MRC_pos MRC_pos_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_MRC_pos MRC_pos_Y;

/* Model step function */
void MRC_pos_step(void)
{
  real_T rtb_UnitDelay;
  real_T rtb_UnitDelay1;

  /* UnitDelay: '<S1>/Unit Delay' */
  rtb_UnitDelay = MRC_pos_DWork.UnitDelay_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay1' */
  rtb_UnitDelay1 = MRC_pos_DWork.UnitDelay1_DSTATE;

  /* Sum: '<S1>/Subtract' incorporates:
   *  Fcn: '<S1>/r1'
   *  Fcn: '<S1>/r2'
   *  Fcn: '<S1>/s0'
   *  Fcn: '<S1>/s1'
   *  Fcn: '<S1>/s2'
   *  Fcn: '<S1>/t0'
   *  Fcn: '<S1>/t1'
   *  Fcn: '<S1>/t2'
   *  Inport: '<Root>/theta'
   *  Inport: '<Root>/uc'
   *  Inport: '<Root>/y'
   *  Product: '<S1>/Product'
   *  Product: '<S1>/Product1'
   *  Product: '<S1>/Product2'
   *  Product: '<S1>/Product3'
   *  Product: '<S1>/Product4'
   *  Product: '<S1>/Product5'
   *  Product: '<S1>/Product6'
   *  Product: '<S1>/Product7'
   *  Sum: '<S1>/Add'
   *  Sum: '<S1>/Add1'
   *  Sum: '<S1>/Add2'
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay2'
   *  UnitDelay: '<S1>/Unit Delay3'
   *  UnitDelay: '<S1>/Unit Delay4'
   *  UnitDelay: '<S1>/Unit Delay5'
   */
  MRC_pos_Y.u = (((bm1 / MRC_pos_U.theta[3] * MRC_pos_U.uc + bm2 /
                   MRC_pos_U.theta[3] * MRC_pos_DWork.UnitDelay_DSTATE) +
                  bm3 / MRC_pos_U.theta[3] *
                  MRC_pos_DWork.UnitDelay3_DSTATE) - (MRC_pos_U.theta[5] /
    MRC_pos_U.theta[3] * MRC_pos_DWork.UnitDelay5_DSTATE + MRC_pos_U.theta[4] /
    MRC_pos_U.theta[3] * MRC_pos_DWork.UnitDelay2_DSTATE)) - (((am1 -
    MRC_pos_U.theta[0]) / MRC_pos_U.theta[3] * MRC_pos_U.y + (am2 -
    MRC_pos_U.theta[1]) / MRC_pos_U.theta[3] * MRC_pos_DWork.UnitDelay1_DSTATE)
    + (am3 - MRC_pos_U.theta[2]) / MRC_pos_U.theta[3] *
    MRC_pos_DWork.UnitDelay4_DSTATE);

  /* Saturate: '<S1>/Saturation' */
  if (MRC_pos_Y.u >= MAX_STEERING_DUTY) {
    /* Sum: '<S1>/Subtract' */
    MRC_pos_Y.u = MAX_STEERING_DUTY;
  } else {
    if (MRC_pos_Y.u <= -MAX_STEERING_DUTY) {
      /* Sum: '<S1>/Subtract' */
      MRC_pos_Y.u = -MAX_STEERING_DUTY;
    }
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Update for UnitDelay: '<S1>/Unit Delay5' incorporates:
   *  UnitDelay: '<S1>/Unit Delay2'
   */
  MRC_pos_DWork.UnitDelay5_DSTATE = MRC_pos_DWork.UnitDelay2_DSTATE;

  /* Update for UnitDelay: '<S1>/Unit Delay2' */
  MRC_pos_DWork.UnitDelay2_DSTATE = MRC_pos_Y.u;

  /* Update for UnitDelay: '<S1>/Unit Delay' incorporates:
   *  Inport: '<Root>/uc'
   */
  MRC_pos_DWork.UnitDelay_DSTATE = MRC_pos_U.uc;

  /* Update for UnitDelay: '<S1>/Unit Delay3' */
  MRC_pos_DWork.UnitDelay3_DSTATE = rtb_UnitDelay;

  /* Update for UnitDelay: '<S1>/Unit Delay1' incorporates:
   *  Inport: '<Root>/y'
   */
  MRC_pos_DWork.UnitDelay1_DSTATE = MRC_pos_U.y;

  /* Update for UnitDelay: '<S1>/Unit Delay4' */
  MRC_pos_DWork.UnitDelay4_DSTATE = rtb_UnitDelay1;
}

/* Model initialize function */
void MRC_pos_initialize(void)
{
  /* (no initialization code required) */
	MRC_pos_U.uc=0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
