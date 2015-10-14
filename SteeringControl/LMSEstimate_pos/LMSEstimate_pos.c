/*
 * File: LMSEstimate_pos.c
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

#include <SteeringControl/LMSEstimate_pos/LMSEstimate_pos.h>
#include <SteeringControl/LMSEstimate_pos/LMSEstimate_pos_private.h>

/* Block signals and states (auto storage) */
D_Work_LMSEstimate_pos LMSEstimate_pos_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_LMSEstimate_pos LMSEstimate_pos_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_LMSEstimate_pos LMSEstimate_pos_Y;

/* Model step function */
void LMSEstimate_pos_step(void)
{
  real_T rtb_UnitDelay2;
  real_T rtb_UnitDelay3;
  real_T rtb_UnitDelay;
  real_T rtb_UnitDelay1;
  real_T rtb_Add1;
  real_T rtb_lambda;
  real_T rtb_ek;
  real_T rtb_Lk[6];
  real_T rtb_Pk1phik[6];
  int32_T i;
  real_T rtb_Pk1phik_0[36];
  real_T rtb_Pk1phik_1[36];
  int32_T i_0;
  int32_T i_1;

  /* UnitDelay: '<S1>/Unit Delay2' */
  rtb_UnitDelay2 = LMSEstimate_pos_DWork.UnitDelay2_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay3' */
  rtb_UnitDelay3 = LMSEstimate_pos_DWork.UnitDelay3_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay' */
  rtb_UnitDelay = LMSEstimate_pos_DWork.UnitDelay_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay1' */
  rtb_UnitDelay1 = LMSEstimate_pos_DWork.UnitDelay1_DSTATE;

  /* SignalConversion: '<S1>/TmpSignal ConversionAtP(k-1)phi(k)Inport2' incorporates:
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay2'
   *  UnitDelay: '<S1>/Unit Delay3'
   *  UnitDelay: '<S1>/Unit Delay4'
   *  UnitDelay: '<S1>/Unit Delay5'
   */
  rtb_Lk[0] = LMSEstimate_pos_DWork.UnitDelay2_DSTATE;
  rtb_Lk[1] = LMSEstimate_pos_DWork.UnitDelay3_DSTATE;
  rtb_Lk[2] = LMSEstimate_pos_DWork.UnitDelay4_DSTATE;
  rtb_Lk[3] = LMSEstimate_pos_DWork.UnitDelay_DSTATE;
  rtb_Lk[4] = LMSEstimate_pos_DWork.UnitDelay1_DSTATE;
  rtb_Lk[5] = LMSEstimate_pos_DWork.UnitDelay5_DSTATE;

  /* Product: '<S1>/phi(k)'P(k-1)phi(k)' */
  rtb_Add1 = 0.0;

  /* Product: '<S1>/Product1' incorporates:
   *  Math: '<S1>/phi(k)''
   *  UnitDelay: '<S1>/theta(k-1)'
   */
  rtb_ek = 0.0;
  for (i = 0; i < 6; i++) {
    /* Product: '<S1>/P(k-1)phi(k)' incorporates:
     *  UnitDelay: '<S1>/P(k-1)'
     */
    rtb_Pk1phik[i] = 0.0;
    for (i_1 = 0; i_1 < 6; i_1++) {
      rtb_Pk1phik[i] += LMSEstimate_pos_DWork.Pk1_DSTATE[6 * i_1 + i] *
        rtb_Lk[i_1];
    }

    /* End of Product: '<S1>/P(k-1)phi(k)' */

    /* Product: '<S1>/phi(k)'P(k-1)phi(k)' incorporates:
     *  Math: '<S1>/phi(k)''
     */
    rtb_Add1 += rtb_Lk[i] * rtb_Pk1phik[i];
    rtb_ek += rtb_Lk[i] * LMSEstimate_pos_DWork.thetak1_DSTATE[i];
  }

  /* Sum: '<S1>/e(k)' incorporates:
   *  Inport: '<Root>/y'
   *  Product: '<S1>/Product1'
   */
  rtb_ek = LMSEstimate_pos_U.y - rtb_ek;

  /* MATLAB Function: '<S1>/lambda' incorporates:
   *  Product: '<S1>/phi(k)'P(k-1)phi(k)'
   */
  /* MATLAB Function 'LMS Estimation/lambda': '<S2>:1' */
  /* lambda=zeros(1); */
  /* '<S2>:1:4' */
  rtb_lambda = 1.0 - rtb_ek * rtb_ek / ((1.0 + rtb_Add1) * 100.0);
  if (rtb_lambda > 1.0) {
    /* '<S2>:1:6' */
    /* '<S2>:1:7' */
    rtb_lambda = 1.0;
  } else {
    if (rtb_lambda < 0.98) {
      /* '<S2>:1:8' */
      /* '<S2>:1:9' */
      rtb_lambda = 0.98;
    }
  }

  /* End of MATLAB Function: '<S1>/lambda' */

  /* Sum: '<S1>/Add1' incorporates:
   *  Product: '<S1>/phi(k)'P(k-1)phi(k)'
   */
  rtb_Add1 += rtb_lambda;
  for (i = 0; i < 6; i++) {
    /* Sum: '<S1>/Add' incorporates:
     *  Product: '<S1>/L(k)'
     *  Product: '<S1>/L(k)e(k)'
     *  UnitDelay: '<S1>/theta(k-1)'
     */
    LMSEstimate_pos_Y.theta[i] = 1.0 / rtb_Add1 * rtb_Pk1phik[i] * rtb_ek +
      LMSEstimate_pos_DWork.thetak1_DSTATE[i];

    /* Update for UnitDelay: '<S1>/theta(k-1)' */
    LMSEstimate_pos_DWork.thetak1_DSTATE[i] = LMSEstimate_pos_Y.theta[i];
  }
  if (LMSEstimate_pos_Y.theta[3]==0.0)
	  LMSEstimate_pos_Y.theta[3]=1.0;
  /* Update for UnitDelay: '<S1>/Unit Delay2' incorporates:
   *  Gain: '<S1>/Gain'
   *  Inport: '<Root>/y'
   */
  LMSEstimate_pos_DWork.UnitDelay2_DSTATE = -LMSEstimate_pos_U.y;

  /* Update for UnitDelay: '<S1>/Unit Delay3' */
  LMSEstimate_pos_DWork.UnitDelay3_DSTATE = rtb_UnitDelay2;

  /* Update for UnitDelay: '<S1>/Unit Delay4' */
  LMSEstimate_pos_DWork.UnitDelay4_DSTATE = rtb_UnitDelay3;

  /* Update for UnitDelay: '<S1>/Unit Delay' incorporates:
   *  Update for Inport: '<Root>/u'
   */
  LMSEstimate_pos_DWork.UnitDelay_DSTATE = LMSEstimate_pos_U.u;

  /* Update for UnitDelay: '<S1>/Unit Delay1' */
  LMSEstimate_pos_DWork.UnitDelay1_DSTATE = rtb_UnitDelay;

  /* Update for UnitDelay: '<S1>/Unit Delay5' */
  LMSEstimate_pos_DWork.UnitDelay5_DSTATE = rtb_UnitDelay1;

  /* Product: '<S1>/P(k)' */
  rtb_ek = 1.0 / rtb_lambda;

  /* Product: '<S1>/product' */
  rtb_Add1 = 1.0 / rtb_Add1;

  /* Product: '<S1>/P(k-1)phi(k)phi(k)'' incorporates:
   *  Math: '<S1>/phi(k)''
   *  Product: '<S1>/P(k-1)phi(k)phi(k)'P(k-1)'
   */
  for (i = 0; i < 6; i++) {
    for (i_1 = 0; i_1 < 6; i_1++) {
      rtb_Pk1phik_0[i + 6 * i_1] = 0.0;
      rtb_Pk1phik_0[i + 6 * i_1] = rtb_Pk1phik_0[6 * i_1 + i] + rtb_Pk1phik[i] *
        rtb_Lk[i_1];
    }
  }

  /* End of Product: '<S1>/P(k-1)phi(k)phi(k)'' */

  /* Product: '<S1>/P(k-1)phi(k)phi(k)'P(k-1)' incorporates:
   *  Product: '<S1>/product'
   *  UnitDelay: '<S1>/P(k-1)'
   */
  for (i = 0; i < 6; i++) {
    for (i_1 = 0; i_1 < 6; i_1++) {
      rtb_Pk1phik_1[i + 6 * i_1] = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        rtb_Pk1phik_1[i + 6 * i_1] = rtb_Pk1phik_0[6 * i_0 + i] *
          LMSEstimate_pos_DWork.Pk1_DSTATE[6 * i_1 + i_0] + rtb_Pk1phik_1[6 *
          i_1 + i];
      }
    }
  }

  /* Update for UnitDelay: '<S1>/P(k-1)' incorporates:
   *  Product: '<S1>/P(k)'
   *  Product: '<S1>/product'
   *  Sum: '<S1>/Subtract'
   *  UnitDelay: '<S1>/P(k-1)'
   */
  for (i = 0; i < 6; i++) {
    for (i_1 = 0; i_1 < 6; i_1++) {
      LMSEstimate_pos_DWork.Pk1_DSTATE[i_1 + 6 * i] =
        (LMSEstimate_pos_DWork.Pk1_DSTATE[6 * i + i_1] - rtb_Pk1phik_1[6 * i +
         i_1] * rtb_Add1) * rtb_ek;
    }
  }

  /* End of Update for UnitDelay: '<S1>/P(k-1)' */
}

/* Model initialize function */
void LMSEstimate_pos_initialize(void)
{
  {
    int32_T i;

    /* InitializeConditions for UnitDelay: '<S1>/theta(k-1)' */
    for (i = 0; i < 6; i++) {
      LMSEstimate_pos_DWork.thetak1_DSTATE[i] =
        LMSEstimate_pos_ConstP.thetak1_X0[i];
    }

    /* End of InitializeConditions for UnitDelay: '<S1>/theta(k-1)' */

    /* InitializeConditions for UnitDelay: '<S1>/P(k-1)' */
    memcpy(&LMSEstimate_pos_DWork.Pk1_DSTATE[0], LMSEstimate_pos_ConstP.Pk1_X0,
           36U * sizeof(real_T));
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
