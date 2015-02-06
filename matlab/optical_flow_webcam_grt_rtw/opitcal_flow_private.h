/*
 * opitcal_flow_private.h
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
#ifndef RTW_HEADER_opitcal_flow_private_h_
#define RTW_HEADER_opitcal_flow_private_h_
#include "rtwtypes.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
# define rtmSetTFinal(rtm, val)        ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               (&(rtm)->Timing.taskTime0)
#endif

#ifndef __RTWTYPES_H__
#error This file requires rtwtypes.h to be included
#else
#ifdef TMWTYPES_PREVIOUSLY_INCLUDED
#error This file requires rtwtypes.h to be included before tmwtypes.h
#else

/* Check for inclusion of an incorrect version of rtwtypes.h */
#ifndef RTWTYPES_ID_C08S16I32L32N32F1
#error This code was generated with a different "rtwtypes.h" than the file included
#endif                                 /* RTWTYPES_ID_C08S16I32L32N32F1 */
#endif                                 /* TMWTYPES_PREVIOUSLY_INCLUDED */
#endif                                 /* __RTWTYPES_H__ */

extern boolean_T PadBConst_ub(const boolean_T u[], const int32_T uStride[],
  const int32_T idx[], const int32_T uStart[], const int32_T uEnd[], const
  int32_T sNumPreEdges, const int32_T sPreEdges[], const int32_T sNumPostEdges,
  const int32_T sPostEdges[], const boolean_T padValue);
extern void MdnFlt_M_IBConst_ub_yb_ab_pb_f(const int32_T hLoc[], const int32_T
  hDims[], const boolean_T u[], const int32_T uDims[], boolean_T y[], const
  int32_T yDims[], const int32_T yOrigin[], boolean_T padValue);
extern void tmMdnFlt_M_RC_SIB_MConst_ub_yb_ab_pb_f(const int32_T hDims[], const
  boolean_T u[], const int32_T uDims[], boolean_T y[], int32_T yDims[],
  boolean_T padValue);

/* Exported functions */
extern int32_T div_s32_floor(int32_T numerator, int32_T denominator);
extern int32_T div_s32(int32_T numerator, int32_T denominator);
extern void opitcal_flow_step0(void);
extern void opitcal_flow_step1(void);

#endif                                 /* RTW_HEADER_opitcal_flow_private_h_ */
