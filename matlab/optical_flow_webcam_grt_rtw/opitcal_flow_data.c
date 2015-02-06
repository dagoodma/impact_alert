/*
 * opitcal_flow_data.c
 *
 * Code generation for model "opitcal_flow".
 *
 * Model version              : 1.5
 * Simulink Coder version : 8.3 (R2012b) 20-Jul-2012
 * C source code generated on : Thu Feb  5 15:52:50 2015
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "opitcal_flow.h"
#include "opitcal_flow_private.h"

/* Block parameters (auto storage) */
Parameters_opitcal_flow opitcal_flow_P = {
  20.0,                                /* Expression: scaleFactor
                                        * Referenced by: '<S4>/Optical Flow Lines'
                                        */
  0.0F,                                /* Computed Parameter: Delay_InitialCondition
                                        * Referenced by: '<S2>/Delay'
                                        */
  1.0F,                                /* Computed Parameter: OpticalFlowEstimation_LAMBDA_RT
                                        * Referenced by: '<S2>/Optical Flow Estimation'
                                        */

  /*  Computed Parameter: DrawShapes_RTP_FILLCOLOR
   * Referenced by: '<S1>/Draw Shapes'
   */
  { 255.0F, 255.0F, 0.0F },
  10,                                  /* Computed Parameter: OpticalFlowEstimation_MAXITER_R
                                        * Referenced by: '<S2>/Optical Flow Estimation'
                                        */
  1U,                                  /* Computed Parameter: Delay_DelayLength
                                        * Referenced by: '<S2>/Delay'
                                        */
  1                                    /* Computed Parameter: MedianFilter_PadValue
                                        * Referenced by: '<S3>/Median Filter'
                                        */
};

/* Constant parameters (auto storage) */
const ConstParam_opitcal_flow opitcal_flow_ConstP = {
  /* Expression: nhood
   * Referenced by: '<S3>/Closing'
   */
  { 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 }
};
