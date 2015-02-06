/*
 * opitcal_flow.h
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
#ifndef RTW_HEADER_opitcal_flow_h_
#define RTW_HEADER_opitcal_flow_h_
#ifndef opitcal_flow_COMMON_INCLUDES_
# define opitcal_flow_COMMON_INCLUDES_
#include <float.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "simaqcodegen.h"
#include "vipopticalflow_rt.h"
#include "rt_nonfinite.h"
#endif                                 /* opitcal_flow_COMMON_INCLUDES_ */

#include "opitcal_flow_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
# define rtmGetRTWLogInfo(rtm)         ((rtm)->rtwLogInfo)
#endif

#ifndef rtmCounterLimit
# define rtmCounterLimit(rtm, idx)     ((rtm)->Timing.TaskCounters.cLimit[(idx)])
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmStepTask
# define rtmStepTask(rtm, idx)         ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

#ifndef rtmTaskCounter
# define rtmTaskCounter(rtm, idx)      ((rtm)->Timing.TaskCounters.TID[(idx)])
#endif

/* Block signals (auto storage) */
typedef struct {
  real32_T FromVideoDevice[921600];    /* '<Root>/From Video Device' */
  creal32_T OpticalFlowEstimation[307200];/* '<S2>/Optical Flow Estimation' */
  real32_T ColorSpaceConversion[307200];/* '<Root>/Color Space  Conversion' */
  real32_T DrawShapes[921600];         /* '<S1>/Draw Shapes' */
  real32_T MathFunction[307200];       /* '<S3>/Math Function' */
  boolean_T Closing[307200];           /* '<S3>/Closing' */
  boolean_T MedianFilter[307200];      /* '<S3>/Median Filter' */
  boolean_T RelationalOperator[307200];/* '<S3>/Relational Operator' */
  real32_T vel_Lines[48260];           /* '<S4>/Optical Flow Lines' */
  creal32_T tmp[12065];
} BlockIO_opitcal_flow;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T OpticalFlowEstimation_GRADRT[307200];/* '<S2>/Optical Flow Estimation' */
  boolean_T Closing_ONE_PAD_IMG_DW[317361];/* '<S3>/Closing' */
  real_T FromVideoDevice_FromVideoDevice[137];/* '<Root>/From Video Device' */
  real_T FrameRateDisplay_PrevTime;    /* '<Root>/Frame Rate Display' */
  real_T FrameRateDisplay_TotalTime;   /* '<Root>/Frame Rate Display' */
  real_T FrameRateDisplay_Count;       /* '<Root>/Frame Rate Display' */
  real_T X[12065];                     /* '<S4>/Optical Flow Lines' */
  real_T Y[12065];                     /* '<S4>/Optical Flow Lines' */
  real_T RV[95];                       /* '<S4>/Optical Flow Lines' */
  real_T CV[127];                      /* '<S4>/Optical Flow Lines' */
  void* FromVideoDevice_IMAQObject;    /* '<Root>/From Video Device' */
  real32_T Delay_DSTATE[307200];       /* '<S2>/Delay' */
  real32_T ColorSpaceConversion_DWORK1[921600];/* '<Root>/Color Space  Conversion' */
  real32_T OpticalFlowEstimation_GRADCC[307200];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_GRADRC[307200];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_GRADRR[307200];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_GRADCT[307200];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_MEMC0[480];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_MEMC1[480];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_MEMR0[640];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_MEMR1[640];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_ALPHA[307200];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_VELBUFFC0[480];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_VELBUFFC1[480];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_VELBUFFR0[480];/* '<S2>/Optical Flow Estimation' */
  real32_T OpticalFlowEstimation_VELBUFFR1[480];/* '<S2>/Optical Flow Estimation' */
  real32_T Meanvelocityperframe_AccVal;/* '<S6>/Mean velocity per frame' */
  real32_T Meanvelocityperframeacrosstime_;/* '<S6>/Mean velocity per frame across time' */
  int32_T MedianFilter_bSEnd[2];       /* '<S3>/Median Filter' */
  int32_T MedianFilter_bSPreEdg[2];    /* '<S3>/Median Filter' */
  int32_T MedianFilter_bSPstEdg[2];    /* '<S3>/Median Filter' */
  int32_T MedianFilter_bSStart[2];     /* '<S3>/Median Filter' */
  int32_T MedianFilter_inSEnd[2];      /* '<S3>/Median Filter' */
  int32_T MedianFilter_inSStart[2];    /* '<S3>/Median Filter' */
  int32_T MedianFilter_mLoc[10];       /* '<S3>/Median Filter' */
  int32_T MedianFilter_mWidth[10];     /* '<S3>/Median Filter' */
  int32_T MedianFilter_oSPreEdg[2];    /* '<S3>/Median Filter' */
  int32_T MedianFilter_oSPstEdg[2];    /* '<S3>/Median Filter' */
  int32_T MedianFilter_oSStart[2];     /* '<S3>/Median Filter' */
  int32_T MedianFilter_oSecEnd[2];     /* '<S3>/Median Filter' */
  int32_T MedianFilter_sCnt[2];        /* '<S3>/Median Filter' */
  int32_T Closing_NUMNONZ_DW;          /* '<S3>/Closing' */
  int32_T Closing_STREL_DW;            /* '<S3>/Closing' */
  int32_T Closing_ERODE_OFF_DW[25];    /* '<S3>/Closing' */
  int32_T Closing_DILATE_OFF_DW[25];   /* '<S3>/Closing' */
  uint32_T Meanvelocityperframeacrosstim_l;/* '<S6>/Mean velocity per frame across time' */
  boolean_T MedianFilter_isHgtUpd[2];  /* '<S3>/Median Filter' */
  boolean_T first_time_not_empty;      /* '<S4>/Optical Flow Lines' */
} D_Work_opitcal_flow;

/* Constant parameters (auto storage) */
typedef struct {
  /* Expression: nhood
   * Referenced by: '<S3>/Closing'
   */
  boolean_T Closing_NHOOD_RTP[25];
} ConstParam_opitcal_flow;

/* Parameters (auto storage) */
struct Parameters_opitcal_flow_ {
  real_T SFunction_p1;                 /* Expression: scaleFactor
                                        * Referenced by: '<S4>/Optical Flow Lines'
                                        */
  real32_T Delay_InitialCondition;     /* Computed Parameter: Delay_InitialCondition
                                        * Referenced by: '<S2>/Delay'
                                        */
  real32_T OpticalFlowEstimation_LAMBDA_RT;/* Computed Parameter: OpticalFlowEstimation_LAMBDA_RT
                                            * Referenced by: '<S2>/Optical Flow Estimation'
                                            */
  real32_T DrawShapes_RTP_FILLCOLOR[3];/* Computed Parameter: DrawShapes_RTP_FILLCOLOR
                                        * Referenced by: '<S1>/Draw Shapes'
                                        */
  int32_T OpticalFlowEstimation_MAXITER_R;/* Computed Parameter: OpticalFlowEstimation_MAXITER_R
                                           * Referenced by: '<S2>/Optical Flow Estimation'
                                           */
  uint32_T Delay_DelayLength;          /* Computed Parameter: Delay_DelayLength
                                        * Referenced by: '<S2>/Delay'
                                        */
  boolean_T MedianFilter_PadValue;     /* Computed Parameter: MedianFilter_PadValue
                                        * Referenced by: '<S3>/Median Filter'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_opitcal_flow {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T taskTime0;
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    struct {
      uint8_T TID[2];
      uint8_T cLimit[2];
    } TaskCounters;

    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block parameters (auto storage) */
extern Parameters_opitcal_flow opitcal_flow_P;

/* Block signals (auto storage) */
extern BlockIO_opitcal_flow opitcal_flow_B;

/* Block states (auto storage) */
extern D_Work_opitcal_flow opitcal_flow_DWork;

/* Constant parameters (auto storage) */
extern const ConstParam_opitcal_flow opitcal_flow_ConstP;

/* Model entry point functions */
extern void opitcal_flow_initialize(void);
extern void opitcal_flow_step(int_T tid);
extern void opitcal_flow_terminate(void);

/* Real-time Model object */
extern RT_MODEL_opitcal_flow *const opitcal_flow_M;

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
 * '<Root>' : 'opitcal_flow'
 * '<S1>'   : 'opitcal_flow/Display Results'
 * '<S2>'   : 'opitcal_flow/Optical Flow'
 * '<S3>'   : 'opitcal_flow/Thresholding and Region Filtering'
 * '<S4>'   : 'opitcal_flow/Display Results/Optical Flow Lines'
 * '<S5>'   : 'opitcal_flow/Display Results/Optical Flow Lines/Optical Flow Lines'
 * '<S6>'   : 'opitcal_flow/Thresholding and Region Filtering/Velocity Threshold'
 */
#endif                                 /* RTW_HEADER_opitcal_flow_h_ */
