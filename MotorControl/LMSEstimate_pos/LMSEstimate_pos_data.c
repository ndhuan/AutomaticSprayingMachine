/*
 * File: LMSEstimate_pos_data.c
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

#include "LMSEstimate_pos.h"
#include "LMSEstimate_pos_private.h"

/* Constant parameters (auto storage) */
const ConstParam_LMSEstimate_pos LMSEstimate_pos_ConstP = {
  /* Expression: [-1;1;0.1;1;0.1;0.1]
   * Referenced by: '<S1>/theta(k-1)'
   */
  { -1.0, 1.0, 0.1, 1.0, 0.1, 0.1 },

  /* Expression: 1000*eye(6,6)
   * Referenced by: '<S1>/P(k-1)'
   */
  { 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1000.0 }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
