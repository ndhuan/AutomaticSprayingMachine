/*
 * File: LMSEstimate_data.c
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

/* Constant parameters (auto storage) */
const ConstParam_LMSEstimate LMSEstimate_ConstP = {
  /* Expression: 1000*eye(4,4)
   * Referenced by: '<S1>/P(k-1)'
   */
  { 1000.0, 0.0, 0.0, 0.0, 0.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 1000.0, 0.0, 0.0,
    0.0, 0.0, 1000.0 }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
