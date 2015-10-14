/*
 * File: LMSEstimate.c
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

#include <SteeringControl/LMSEstimate/LMSEstimate.h>
#include <SteeringControl/LMSEstimate/LMSEstimate_private.h>

#define VARIABLE_FORGET_FACTOR

/* Block signals and states (auto storage) */
D_Work_LMSEstimate LMSEstimate_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_LMSEstimate LMSEstimate_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_LMSEstimate LMSEstimate_Y;

/* Model step function */
void LMSEstimate_step(void)
{
  real_T rtb_UnitDelay2;
  real_T rtb_UnitDelay;
  real_T rtb_Add1;
  real_T rtb_lambda;
  real_T rtb_ek;
  real_T rtb_Pk1phik[4];
  int32_T i;
  real_T rtb_phikPk1phik_0;
  real_T rtb_Pk1phik_0[16];
  real_T rtb_Pk1phik_1[16];
  int32_T i_0;
  real_T rtb_Lk_idx;
  real_T rtb_Lk_idx_0;

  /* UnitDelay: '<S1>/Unit Delay2' */
  rtb_UnitDelay2 = LMSEstimate_DWork.UnitDelay2_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay' */
  rtb_UnitDelay = LMSEstimate_DWork.UnitDelay_DSTATE;

  /* SignalConversion: '<S1>/TmpSignal ConversionAtP(k-1)phi(k)Inport2' incorporates:
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay3'
   */
  rtb_Lk_idx = LMSEstimate_DWork.UnitDelay3_DSTATE;
  rtb_Lk_idx_0 = LMSEstimate_DWork.UnitDelay1_DSTATE;

  /* Product: '<S1>/P(k-1)phi(k)' incorporates:
   *  UnitDelay: '<S1>/P(k-1)'
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay2'
   *  UnitDelay: '<S1>/Unit Delay3'
   */
  for (i = 0; i < 4; i++) {
    rtb_ek = LMSEstimate_DWork.Pk1_DSTATE[i + 12] *
      LMSEstimate_DWork.UnitDelay1_DSTATE + (LMSEstimate_DWork.Pk1_DSTATE[i + 8]
      * LMSEstimate_DWork.UnitDelay_DSTATE + (LMSEstimate_DWork.Pk1_DSTATE[i + 4]
      * LMSEstimate_DWork.UnitDelay3_DSTATE + LMSEstimate_DWork.Pk1_DSTATE[i] *
      LMSEstimate_DWork.UnitDelay2_DSTATE));
    rtb_Pk1phik[i] = rtb_ek;
  }

  /* End of Product: '<S1>/P(k-1)phi(k)' */

  /* Product: '<S1>/phi(k)'P(k-1)phi(k)' incorporates:
   *  Math: '<S1>/phi(k)''
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay2'
   *  UnitDelay: '<S1>/Unit Delay3'
   */
  rtb_phikPk1phik_0 = ((LMSEstimate_DWork.UnitDelay2_DSTATE * rtb_Pk1phik[0] +
                        LMSEstimate_DWork.UnitDelay3_DSTATE * rtb_Pk1phik[1]) +
                       LMSEstimate_DWork.UnitDelay_DSTATE * rtb_Pk1phik[2]) +
    LMSEstimate_DWork.UnitDelay1_DSTATE * rtb_Pk1phik[3];

  /* Sum: '<S1>/e(k)' incorporates:
   *  Inport: '<Root>/y'
   *  Math: '<S1>/phi(k)''
   *  Product: '<S1>/Product1'
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay2'
   *  UnitDelay: '<S1>/Unit Delay3'
   *  UnitDelay: '<S1>/theta(k-1)'
   */
  rtb_ek = LMSEstimate_U.y - (((LMSEstimate_DWork.UnitDelay2_DSTATE *
    LMSEstimate_DWork.thetak1_DSTATE[0] + LMSEstimate_DWork.UnitDelay3_DSTATE *
    LMSEstimate_DWork.thetak1_DSTATE[1]) + LMSEstimate_DWork.UnitDelay_DSTATE *
    LMSEstimate_DWork.thetak1_DSTATE[2]) + LMSEstimate_DWork.UnitDelay1_DSTATE *
    LMSEstimate_DWork.thetak1_DSTATE[3]);

  /* MATLAB Function: '<S1>/lambda' incorporates:
   *  Product: '<S1>/phi(k)'P(k-1)phi(k)'
   */
  /* MATLAB Function 'LMS Estimation/lambda': '<S2>:1' */
  /* lambda=zeros(1); */
  /* '<S2>:1:4' */
#ifdef VARIABLE_FORGET_FACTOR
  rtb_lambda = 1.0 - rtb_ek * rtb_ek / ((1.0 + rtb_phikPk1phik_0) * 1000.0);
//  if (rtb_lambda > 1.0) {
//    /* '<S2>:1:6' */
//    /* '<S2>:1:7' */
//    rtb_lambda = 1.0;
//  } else {
    if (rtb_lambda < 0.98) {
      /* '<S2>:1:8' */
      /* '<S2>:1:9' */
    	rtb_lambda = 0.98;
    }
//  }
#else
  rtb_lambda = 0.99;
#endif
  /* End of MATLAB Function: '<S1>/lambda' */

  /* Sum: '<S1>/Add1' incorporates:
   *  Product: '<S1>/phi(k)'P(k-1)phi(k)'
   */
  rtb_Add1 = rtb_phikPk1phik_0 + rtb_lambda;

  /* Sum: '<S1>/Add' incorporates:
   *  Product: '<S1>/L(k)'
   *  Product: '<S1>/L(k)e(k)'
   *  UnitDelay: '<S1>/theta(k-1)'
   */
  LMSEstimate_Y.theta[0] = 1.0 / rtb_Add1 * rtb_Pk1phik[0] * rtb_ek +
    LMSEstimate_DWork.thetak1_DSTATE[0];
  LMSEstimate_Y.theta[1] = 1.0 / rtb_Add1 * rtb_Pk1phik[1] * rtb_ek +
    LMSEstimate_DWork.thetak1_DSTATE[1];
  LMSEstimate_Y.theta[2] = 1.0 / rtb_Add1 * rtb_Pk1phik[2] * rtb_ek +
    LMSEstimate_DWork.thetak1_DSTATE[2];
  LMSEstimate_Y.theta[3] = 1.0 / rtb_Add1 * rtb_Pk1phik[3] * rtb_ek +
    LMSEstimate_DWork.thetak1_DSTATE[3];

  if (LMSEstimate_Y.theta[2]==0.0)
	  LMSEstimate_Y.theta[2]=1.0;

  /* Update for UnitDelay: '<S1>/theta(k-1)' */
  LMSEstimate_DWork.thetak1_DSTATE[0] = LMSEstimate_Y.theta[0];
  LMSEstimate_DWork.thetak1_DSTATE[1] = LMSEstimate_Y.theta[1];
  LMSEstimate_DWork.thetak1_DSTATE[2] = LMSEstimate_Y.theta[2];
  LMSEstimate_DWork.thetak1_DSTATE[3] = LMSEstimate_Y.theta[3];

  /* Update for UnitDelay: '<S1>/Unit Delay2' incorporates:
   *  Gain: '<S1>/Gain'
   *  Inport: '<Root>/y'
   */
  LMSEstimate_DWork.UnitDelay2_DSTATE = -LMSEstimate_U.y;

  /* Update for UnitDelay: '<S1>/Unit Delay3' */
  LMSEstimate_DWork.UnitDelay3_DSTATE = rtb_UnitDelay2;

  /* Update for UnitDelay: '<S1>/Unit Delay' incorporates:
   *  Update for Inport: '<Root>/u'
   */
  LMSEstimate_DWork.UnitDelay_DSTATE = LMSEstimate_U.u;

  /* Update for UnitDelay: '<S1>/Unit Delay1' */
  LMSEstimate_DWork.UnitDelay1_DSTATE = rtb_UnitDelay;

  /* Product: '<S1>/P(k)' */
  rtb_phikPk1phik_0 = 1.0 / rtb_lambda;

  /* Product: '<S1>/product' */
  rtb_ek = 1.0 / rtb_Add1;

  /* Product: '<S1>/P(k-1)phi(k)phi(k)'' incorporates:
   *  Math: '<S1>/phi(k)''
   *  Product: '<S1>/P(k-1)phi(k)phi(k)'P(k-1)'
   */
  for (i = 0; i < 4; i++) {
    rtb_Pk1phik_0[i] = 0.0;
    rtb_Pk1phik_0[i] += rtb_Pk1phik[i] * rtb_UnitDelay2;
    rtb_Pk1phik_0[i + 4] = 0.0;
    rtb_Pk1phik_0[i + 4] += rtb_Pk1phik[i] * rtb_Lk_idx;
    rtb_Pk1phik_0[i + 8] = 0.0;
    rtb_Pk1phik_0[i + 8] += rtb_Pk1phik[i] * rtb_UnitDelay;
    rtb_Pk1phik_0[i + 12] = 0.0;
    rtb_Pk1phik_0[i + 12] += rtb_Pk1phik[i] * rtb_Lk_idx_0;
  }

  /* End of Product: '<S1>/P(k-1)phi(k)phi(k)'' */

  /* Product: '<S1>/P(k-1)phi(k)phi(k)'P(k-1)' incorporates:
   *  Product: '<S1>/product'
   *  UnitDelay: '<S1>/P(k-1)'
   */
  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      rtb_Pk1phik_1[i + (i_0 << 2)] = 0.0;
      rtb_Pk1phik_1[i + (i_0 << 2)] = rtb_Pk1phik_1[(i_0 << 2) + i] +
        LMSEstimate_DWork.Pk1_DSTATE[i_0 << 2] * rtb_Pk1phik_0[i];
      rtb_Pk1phik_1[i + (i_0 << 2)] = LMSEstimate_DWork.Pk1_DSTATE[(i_0 << 2) +
        1] * rtb_Pk1phik_0[i + 4] + rtb_Pk1phik_1[(i_0 << 2) + i];
      rtb_Pk1phik_1[i + (i_0 << 2)] = LMSEstimate_DWork.Pk1_DSTATE[(i_0 << 2) +
        2] * rtb_Pk1phik_0[i + 8] + rtb_Pk1phik_1[(i_0 << 2) + i];
      rtb_Pk1phik_1[i + (i_0 << 2)] = LMSEstimate_DWork.Pk1_DSTATE[(i_0 << 2) +
        3] * rtb_Pk1phik_0[i + 12] + rtb_Pk1phik_1[(i_0 << 2) + i];
    }
  }

  /* Update for UnitDelay: '<S1>/P(k-1)' incorporates:
   *  Product: '<S1>/P(k)'
   *  Product: '<S1>/product'
   *  Sum: '<S1>/Subtract'
   *  UnitDelay: '<S1>/P(k-1)'
   */
  for (i = 0; i < 4; i++) {
    LMSEstimate_DWork.Pk1_DSTATE[i << 2] = (LMSEstimate_DWork.Pk1_DSTATE[i << 2]
      - rtb_Pk1phik_1[i << 2] * rtb_ek) * rtb_phikPk1phik_0;
    LMSEstimate_DWork.Pk1_DSTATE[1 + (i << 2)] = (LMSEstimate_DWork.Pk1_DSTATE
      [(i << 2) + 1] - rtb_Pk1phik_1[(i << 2) + 1] * rtb_ek) * rtb_phikPk1phik_0;
    LMSEstimate_DWork.Pk1_DSTATE[2 + (i << 2)] = (LMSEstimate_DWork.Pk1_DSTATE
      [(i << 2) + 2] - rtb_Pk1phik_1[(i << 2) + 2] * rtb_ek) * rtb_phikPk1phik_0;
    LMSEstimate_DWork.Pk1_DSTATE[3 + (i << 2)] = (LMSEstimate_DWork.Pk1_DSTATE
      [(i << 2) + 3] - rtb_Pk1phik_1[(i << 2) + 3] * rtb_ek) * rtb_phikPk1phik_0;
  }

  /* End of Update for UnitDelay: '<S1>/P(k-1)' */
}

/* Model initialize function */
void LMSEstimate_initialize(void)
{
  /* InitializeConditions for UnitDelay: '<S1>/theta(k-1)' */
  LMSEstimate_DWork.thetak1_DSTATE[0] = -1.0;
  LMSEstimate_DWork.thetak1_DSTATE[1] = 1.0;
  LMSEstimate_DWork.thetak1_DSTATE[2] = 0.1;
  LMSEstimate_DWork.thetak1_DSTATE[3] = 0.1;

  /* InitializeConditions for UnitDelay: '<S1>/P(k-1)' */
  memcpy(&LMSEstimate_DWork.Pk1_DSTATE[0], LMSEstimate_ConstP.Pk1_X0, sizeof
         (real_T) << 4U);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
