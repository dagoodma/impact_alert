/*
 * opitcal_flow.c
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

/* Block signals (auto storage) */
BlockIO_opitcal_flow opitcal_flow_B;

/* Block states (auto storage) */
D_Work_opitcal_flow opitcal_flow_DWork;

/* Real-time model */
RT_MODEL_opitcal_flow opitcal_flow_M_;
RT_MODEL_opitcal_flow *const opitcal_flow_M = &opitcal_flow_M_;

/* Forward declaration for local functions */
static void opitcal_flow_meshgrid(const real_T x[127], const real_T y[95],
  real_T xx[12065], real_T yy[12065]);
int32_T div_s32_floor(int32_T numerator, int32_T denominator)
{
  int32_T quotient;
  uint32_T absNumerator;
  uint32_T absDenominator;
  uint32_T tempAbsQuotient;
  uint32_T quotientNeedsNegation;
  if (denominator == 0) {
    quotient = numerator >= 0 ? MAX_int32_T : MIN_int32_T;

    /* Divide by zero handler */
  } else {
    absNumerator = (uint32_T)(numerator >= 0 ? numerator : -numerator);
    absDenominator = (uint32_T)(denominator >= 0 ? denominator : -denominator);
    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0) {
        tempAbsQuotient += (uint32_T)1;
      }
    }

    quotient = quotientNeedsNegation ? -(int32_T)tempAbsQuotient : (int32_T)
      tempAbsQuotient;
  }

  return quotient;
}

int32_T div_s32(int32_T numerator, int32_T denominator)
{
  int32_T quotient;
  uint32_T tempAbsQuotient;
  uint32_T quotientNeedsNegation;
  if (denominator == 0) {
    quotient = numerator >= 0 ? MAX_int32_T : MIN_int32_T;

    /* Divide by zero handler */
  } else {
    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = (uint32_T)(numerator >= 0 ? numerator : -numerator) /
      (uint32_T)(denominator >= 0 ? denominator : -denominator);
    quotient = quotientNeedsNegation ? -(int32_T)tempAbsQuotient : (int32_T)
      tempAbsQuotient;
  }

  return quotient;
}

boolean_T PadBConst_ub(const boolean_T u[], const int32_T uStride[], const
  int32_T idx[], const int32_T uStart[], const int32_T uEnd[], const int32_T
  sNumPreEdges, const int32_T sPreEdges[], const int32_T sNumPostEdges, const
  int32_T sPostEdges[], const boolean_T padValue)
{
  boolean_T uOut;
  int32_T edgeNum;
  boolean_T isOutside;

  /* S-Function (svipmdnfilter): '<S3>/Median Filter' */
  /* For boundary sectors: */
  /* -check if index is outside of input bounds ... */
  /*   -if so then return pad extended value */
  /*   -else fetch input data at the index */
  isOutside = FALSE;
  for (edgeNum = 0; edgeNum < sNumPreEdges; edgeNum++) {
    isOutside = (isOutside || (idx[sPreEdges[edgeNum]] <
      uStart[sPreEdges[edgeNum]]));
  }

  for (edgeNum = 0; edgeNum < sNumPostEdges; edgeNum++) {
    isOutside = (isOutside || (idx[sPostEdges[edgeNum]] >
      uEnd[sPostEdges[edgeNum]]));
  }

  if (isOutside) {
    /* return pad value */
    uOut = padValue;
  } else {
    /* linearize index and return input data */
    uOut = u[idx[0U] * uStride[0U] + idx[1U] * uStride[1U]];
  }

  /* End of S-Function (svipmdnfilter): '<S3>/Median Filter' */
  return uOut;
}

void MdnFlt_M_IBConst_ub_yb_ab_pb_f(const int32_T hLoc[], const int32_T hDims[],
  const boolean_T u[], const int32_T uDims[], boolean_T y[], const int32_T
  yDims[], const int32_T yOrigin[], boolean_T padValue)
{
  int32_T uOrigin[2];
  int32_T uEnd[2];
  int32_T uStride[2];
  int32_T uStride_0;
  int32_T hEnd[2];
  int32_T yEnd[2];
  int32_T mLoc[10];
  int32_T mWidth[10];
  int32_T yPre;
  int32_T yPost;
  int32_T oPreW;
  int32_T oPostW;
  int32_T bPreL;
  boolean_T isSEmpty;
  int32_T sStart[2];
  int32_T sEnd[2];
  int32_T offset[2];
  int32_T idxB[2];
  int32_T idxALin[2];
  int32_T idxBLin[2];
  int32_T hIdxA[2];
  int32_T sStart_0[2];
  int32_T sEnd_0[2];
  int32_T sNumPreEdges;
  int32_T sPreEdges[2];
  int32_T sNumPostEdges;
  int32_T sPostEdges[2];
  int32_T c[2];
  int8_T o[5];
  boolean_T isEnd;
  boolean_T dimIsPre;
  boolean_T dimIsInside;
  int32_T offset_0[2];
  int32_T idxA[2];
  int32_T idxB_0[2];
  int32_T idxBLin_0[2];
  int32_T hIdxA_0[2];
  int32_T hIdxB[2];
  int32_T count;

  /* S-Function (svipmdnfilter): '<S3>/Median Filter' */
  uOrigin[0U] = 0;
  uEnd[0U] = uDims[0U] - 1;
  uStride[0U] = 1;
  uOrigin[1U] = 0;
  uEnd[1U] = uDims[1U] - 1;
  uStride[1U] = uDims[0U];
  hEnd[0U] = hDims[0U] - 1;
  hEnd[1U] = hDims[1U] - 1;
  yEnd[0U] = yDims[0U] - 1;
  yEnd[1U] = yDims[1U] - 1;

  /*  COMPUTE MARGINS BETWEEN INPUT AND OUTPUT       */
  /* =============================================== */
  /*      _________________________________________  */
  /*     |u                                        | */
  /*     |                                         | */
  /*     |         ______________________          | */
  /*     |        |y                     |         | */
  /*     |        |                      |         | */
  /*     |<-yPre->|                      |<-yPost->| */
  /*     |        |                      |         | */
  /*  */
  /*  KERNEL MARIGINS                               */
  /* ============================================== */
  /*      ____________________  */
  /*     |h                   | */
  /*     |        |<--hPost-->| */
  /*     |        |           | */
  /*     |        x(hCenter)  | */
  /*     |<-hPre->|           | */
  /*     |        |           | */
  /*  */
  /*  MARGINS (Valid, Boundary and Outside) definitions: */
  /*  Valid Margin: Processing region where entire kernel stays inside input boundaries. All input data thus considered is 'valid' data */
  /*  Boundary Margin: Processing region where part of kernel stays inside the input boundaries and part of it goes outside input boundaries. */
  /*  Outside Margin: Processing region where entire kernel stays outside the input boundaries and never touches input data. In most situations this region does not need to be processed. Instead output pad values are filled in */
  /* =========================================================== */
  /*   y |        |        |               |         |         | */
  /*     |<-oPre->|<-bPre->|<----valid---->|<-bPost->|<-oPost->| */
  /*     |        |        |               |         |         | */
  /*  */
  /*  All margin locations are stored relative to the output origin */
  /* margins between y and u */
  yPre = -yOrigin[0];
  yPost = (yOrigin[0] + yEnd[0]) - uEnd[0];

  /* margins caused by kernel - defined by center location */
  /* hLoc and hEnd are defined relative to origin */
  sNumPreEdges = -hLoc[0];
  uStride_0 = hLoc[0] + hEnd[0];

  /* margins outside of kernel's influence around input */
  /* note that we subtract h'Post' from y'Pre' */
  sNumPostEdges = yPre - uStride_0;

  /* note that we subtract h'Pre' from y'Post' */
  count = yPost - sNumPreEdges;
  sNumPreEdges += yPre;
  uStride_0 += yPost;

  /* W refers to margin Width */
  /* L refers to margin Location */
  /* outer Pre */
  /* saturate */
  if (sNumPostEdges < 0) {
    oPreW = 0;
  } else if (sNumPostEdges > yDims[0]) {
    oPreW = yDims[0];
  } else {
    oPreW = sNumPostEdges;
  }

  /* boundary Pre */
  /* saturate */
  if (sNumPreEdges < 0) {
    sNumPreEdges = 0;
  } else {
    if (sNumPreEdges > yDims[0]) {
      sNumPreEdges = yDims[0];
    }
  }

  if (sNumPostEdges < 0) {
    sNumPostEdges = 0;
  } else {
    if (sNumPostEdges > yDims[0]) {
      sNumPostEdges = yDims[0];
    }
  }

  yPost = sNumPreEdges - sNumPostEdges;
  bPreL = yOrigin[0] + oPreW;

  /* outer Post */
  /* saturate */
  if (count < 0) {
    oPostW = 0;
  } else if (count > yDims[0]) {
    oPostW = yDims[0];
  } else {
    oPostW = count;
  }

  yPre = ((yOrigin[0] + yEnd[0]) - oPostW) + 1;

  /* boundary Post */
  /* saturate */
  if (uStride_0 < 0) {
    uStride_0 = 0;
  } else {
    if (uStride_0 > yDims[0]) {
      uStride_0 = yDims[0];
    }
  }

  if (count < 0) {
    count = 0;
  } else {
    if (count > yDims[0]) {
      count = yDims[0];
    }
  }

  uStride_0 -= count;
  sNumPreEdges = yPre - uStride_0;
  sNumPostEdges = bPreL + yPost;
  count = sNumPreEdges + uStride_0;
  if (sNumPostEdges - 1 >= sNumPreEdges) {
    /* defines situation where hDims > uDims */
    yPost = sNumPreEdges - bPreL;
    sNumPreEdges = sNumPostEdges;
    uStride_0 = count - sNumPostEdges;
  }

  /* assign to the margin set of mLoc and mWidth */
  mLoc[0] = bPreL + yPost;
  mWidth[0] = (((yDims[0] - oPreW) - oPostW) - yPost) - uStride_0;
  mLoc[2] = bPreL;
  mWidth[2] = yPost;
  mLoc[4] = sNumPreEdges;
  mWidth[4] = uStride_0;
  mLoc[6] = yOrigin[0];
  mWidth[6] = oPreW;
  mLoc[8] = yPre;
  mWidth[8] = oPostW;

  /* margins between y and u */
  yPre = -yOrigin[1];
  yPost = (yOrigin[1] + yEnd[1]) - uEnd[1];

  /* margins caused by kernel - defined by center location */
  /* hLoc and hEnd are defined relative to origin */
  sNumPreEdges = -hLoc[1];
  uStride_0 = hLoc[1] + hEnd[1];

  /* margins outside of kernel's influence around input */
  /* note that we subtract h'Post' from y'Pre' */
  sNumPostEdges = yPre - uStride_0;

  /* note that we subtract h'Pre' from y'Post' */
  count = yPost - sNumPreEdges;
  sNumPreEdges += yPre;
  uStride_0 += yPost;

  /* W refers to margin Width */
  /* L refers to margin Location */
  /* outer Pre */
  /* saturate */
  if (sNumPostEdges < 0) {
    oPreW = 0;
  } else if (sNumPostEdges > yDims[1]) {
    oPreW = yDims[1];
  } else {
    oPreW = sNumPostEdges;
  }

  /* boundary Pre */
  /* saturate */
  if (sNumPreEdges < 0) {
    sNumPreEdges = 0;
  } else {
    if (sNumPreEdges > yDims[1]) {
      sNumPreEdges = yDims[1];
    }
  }

  if (sNumPostEdges < 0) {
    sNumPostEdges = 0;
  } else {
    if (sNumPostEdges > yDims[1]) {
      sNumPostEdges = yDims[1];
    }
  }

  yPost = sNumPreEdges - sNumPostEdges;
  bPreL = yOrigin[1] + oPreW;

  /* outer Post */
  /* saturate */
  if (count < 0) {
    oPostW = 0;
  } else if (count > yDims[1]) {
    oPostW = yDims[1];
  } else {
    oPostW = count;
  }

  yPre = ((yOrigin[1] + yEnd[1]) - oPostW) + 1;

  /* boundary Post */
  /* saturate */
  if (uStride_0 < 0) {
    uStride_0 = 0;
  } else {
    if (uStride_0 > yDims[1]) {
      uStride_0 = yDims[1];
    }
  }

  if (count < 0) {
    count = 0;
  } else {
    if (count > yDims[1]) {
      count = yDims[1];
    }
  }

  uStride_0 -= count;
  sNumPreEdges = yPre - uStride_0;
  sNumPostEdges = bPreL + yPost;
  count = sNumPreEdges + uStride_0;
  if (sNumPostEdges - 1 >= sNumPreEdges) {
    /* defines situation where hDims > uDims */
    yPost = sNumPreEdges - bPreL;
    sNumPreEdges = sNumPostEdges;
    uStride_0 = count - sNumPostEdges;
  }

  /* assign to the margin set of mLoc and mWidth */
  mLoc[1] = bPreL + yPost;
  mWidth[1] = (((yDims[1] - oPreW) - oPostW) - yPost) - uStride_0;
  mLoc[3] = bPreL;
  mWidth[3] = yPost;
  mLoc[5] = sNumPreEdges;
  mWidth[5] = uStride_0;
  mLoc[7] = yOrigin[1];
  mWidth[7] = oPreW;
  mLoc[9] = yPre;
  mWidth[9] = oPostW;

  /*  ////////////////////////////////////////// */
  /*  COMPUTE INSIDE SECTOR                      */
  /*  ////////////////////////////////////////// */
  /*  ========================================== */
  /*  Original arrangement of sectors            */
  /*  ========================================== */
  /*   |   3   |   1   |   0   |   2   |   4   | */
  /*   | OutPre| BndPre| Inside|BndPost|OutPost| */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  3| (3,3) | (3,1) | (3,0) | (3,2) | (3,4) | */
  /*   |       |       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  1| (1,3) | (1,1) | (1,0) | (1,2) | (1,4) | */
  /*   |       |       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*   |       |       | VALID |       |       | */
  /*  0| (0,3) | (0,1) | (0,0) | (0,2) | (0,4) | */
  /*   |       |       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  2| (2,3) | (2,1) | (2,0) | (2,2) | (2,4) | */
  /*   |       |       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  4| (4,3) | (4,1) | (4,0) | (4,2) | (4,4) | */
  /*   |       |       |       |       |       | */
  /*    ---------------------------------------  */
  /*  Sectors rearranged in order from 0 to 4 */
  /*  ========================================== */
  /*   |   0   |   1   |   2   |   3   |   4   | */
  /*   | Inside| BndPre|BndPost| OutPre|OutPost| */
  /*    _______________________________________  */
  /*   |///////|       |       |       |       | */
  /*   |/VALID/|       |       |       |       | */
  /*  0|/(0,0)/| (0,1) | (0,2) | (0,3) | (0,4) | */
  /*   |///////|       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  1| (1,0) | (1,1) | (1,2) | (1,3) | (1,4) | */
  /*   |       |       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  2| (2,0) | (2,1) | (2,2) | (2,3) | (2,4) | */
  /*   |       |       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  3| (3,0) | (3,1) | (3,2) | (3,3) | (3,4) | */
  /*   |       |       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  4| (4,0) | (4,1) | (4,2) | (4,3) | (4,4) | */
  /*   |       |       |       |       |       | */
  /*    ---------------------------------------  */
  /* setup counter for valid sector, that is 0 in all dims */
  /* compute inside sector */
  isSEmpty = ((mWidth[0] <= 0) || (mWidth[1] <= 0));

  /* compute sector if not empty */
  if (!isSEmpty) {
    /* sector start corresponds to margin loc */
    sStart[0] = mLoc[0];

    /* apply offset of margin width for sector end */
    sEnd[0] = (mLoc[0] + mWidth[0]) - 1;

    /* sector start corresponds to margin loc */
    sStart[1] = mLoc[1];

    /* apply offset of margin width for sector end */
    sEnd[1] = (mLoc[1] + mWidth[1]) - 1;
  }

  if (!isSEmpty) {
    /* setup indices for the loops */
    /* Origin stores the origin of Y with respect to U */
    /*  where the origin of Y is at its top left corner */
    /* Compute offset between origin of Y and origin of U(0,0) */
    offset[0U] = -yOrigin[0U];
    offset[1U] = -yOrigin[1U];

    /* sector start and end were computed relative to U */
    /* make them relative to Y by adding offset */
    sStart[0U] += offset[0U];
    sEnd[0U] += offset[0U];
    sStart[1U] += offset[1U];
    sEnd[1U] += offset[1U];

    /* adjust offset for kernel center */
    offset[0U] -= hLoc[0U];
    offset[1U] -= hLoc[1U];

    /* loop kernel over data */
    idxB[1U] = sStart[1U];
    while (idxB[1U] <= sEnd[1U]) {
      idxALin[1U] = (idxB[1U] - offset[1U]) * uDims[0U];
      idxBLin[1U] = yDims[0U] * idxB[1U];
      idxB[0U] = sStart[0U];
      while (idxB[0U] <= sEnd[0U]) {
        idxALin[0U] = (idxB[0U] - offset[0U]) + idxALin[1U];
        idxBLin[0U] = idxBLin[1U] + idxB[0U];
        uStride_0 = idxALin[0U];

        /* loop over kernel and compute median value */
        count = 0;

        /* loop over kernel */
        hIdxA[1U] = 0;
        while (hIdxA[1U] <= hEnd[1U]) {
          hIdxA[0U] = 0;
          while (hIdxA[0U] <= hEnd[0U]) {
            /* count number of trues in the window */
            count += (int32_T)u[uStride_0];
            uStride_0++;
            hIdxA[0U]++;
          }

          uStride_0 = (uStride_0 + uDims[0U]) - hDims[0U];
          hIdxA[1U]++;
        }

        if (idxBLin[0U] >= 0) {
          y[idxBLin[0U]] = ((count << 1) >= hDims[0U] * hDims[1U]);
        }

        idxB[0U]++;
      }

      idxB[1U]++;
    }
  }

  /*  ////////////////////////////////////////// */
  /*  COMPUTE ON-BOUNDARY SECTORS                */
  /*  ////////////////////////////////////////// */
  /* =========================================== */
  /*   |   0   |   1   |   2   |   3   |   4   | */
  /*   | Inside| BndPre|BndPost| OutPre|OutPost| */
  /*    _______________________________________  */
  /*   |       |///////|///////|       |       | */
  /*   | VALID |///////|///////|       |       | */
  /*  0| (0,0) |/(0,1)/|/(0,2)/| (0,3) | (0,4) | */
  /*   |       |///////|///////|       |       | */
  /*    _______________________________________  */
  /*   |///////|///////|///////|       |       | */
  /*  1|/(1,0)/|/(1,1)/|/(1,2)/| (1,3) | (1,4) | */
  /*   |///////|///////|///////|       |       | */
  /*    _______________________________________  */
  /*   |///////|///////|///////|       |       | */
  /*  2|/(2,0)/|/(2,1)/|/(2,2)/| (2,3) | (2,4) | */
  /*   |///////|///////|///////|       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  3| (3,0) | (3,1) | (3,2) | (3,3) | (3,4) | */
  /*   |       |       |       |       |       | */
  /*    _______________________________________  */
  /*   |       |       |       |       |       | */
  /*  4| (4,0) | (4,1) | (4,2) | (4,3) | (4,4) | */
  /*   |       |       |       |       |       | */
  /*    ---------------------------------------  */
  o[0U] = 0;
  o[1U] = 1;
  o[2U] = 2;
  o[3U] = 3;
  o[4U] = 4;
  isEnd = FALSE;
  c[1U] = 0;

  /* if hDims < uDims then the all INSIDE sector will not be pushed into the boundary sectors... hence the counter needs to skip the all INSIDEs sector and start from one sector down. */
  c[0U] = 1;
  while (!isEnd) {
    sNumPreEdges = 0;
    sNumPostEdges = 0;

    /* compute on-boundary sector */
    isSEmpty = ((mWidth[o[c[0U]] << 1] <= 0) || (mWidth[(o[c[1U]] << 1) + 1] <=
      0));

    /* compute sector if not empty */
    if (!isSEmpty) {
      /* compute sector for this dim */
      /* sector start corresponds to margin loc */
      sStart_0[0] = mLoc[o[c[0]] << 1];

      /* apply offset of margin width for sector end */
      sEnd_0[0] = (mLoc[o[c[0]] << 1] + mWidth[o[c[0]] << 1]) - 1;

      /* Inside Sector if 0 in all dimensions */
      dimIsInside = (o[c[0]] == 0);
      dimIsPre = ((o[c[0]] == 3) || (o[c[0]] == 1));
      if (dimIsPre || dimIsInside) {
        sPreEdges[0] = 0;
        sNumPreEdges = 1;
      }

      if ((!(dimIsPre || dimIsInside)) || dimIsInside) {
        sPostEdges[0] = 0;
        sNumPostEdges = 1;
      }

      /* compute sector for this dim */
      /* sector start corresponds to margin loc */
      sStart_0[1] = mLoc[(o[c[1]] << 1) + 1];

      /* apply offset of margin width for sector end */
      sEnd_0[1] = (mLoc[(o[c[1]] << 1) + 1] + mWidth[(o[c[1]] << 1) + 1]) - 1;

      /* Inside Sector if 0 in all dimensions */
      dimIsInside = (o[c[1]] == 0);
      dimIsPre = ((o[c[1]] == 3) || (o[c[1]] == 1));
      if (dimIsPre || dimIsInside) {
        sPreEdges[sNumPreEdges] = 1;
        sNumPreEdges++;
      }

      if ((!(dimIsPre || dimIsInside)) || dimIsInside) {
        sPostEdges[sNumPostEdges] = 1;
        sNumPostEdges++;
      }
    }

    if (!isSEmpty) {
      /* setup indices for the loops */
      /* Origin stores the origin of Y with respect to U */
      /*  where the origin of Y is at its top left corner */
      /* Compute offset between origin of Y and origin of U(0,0) */
      offset_0[0U] = -yOrigin[0U];
      offset_0[1U] = -yOrigin[1U];

      /* sector start and end were computed relative to U */
      /* make them relative to Y by adding offset */
      sStart_0[0U] += offset_0[0U];
      sEnd_0[0U] += offset_0[0U];
      sStart_0[1U] += offset_0[1U];
      sEnd_0[1U] += offset_0[1U];

      /* adjust offset for kernel center */
      offset_0[0U] -= hLoc[0U];
      offset_0[1U] -= hLoc[1U];

      /* loop kernel over data */
      idxB_0[1U] = sStart_0[1U];
      while (idxB_0[1U] <= sEnd_0[1U]) {
        idxA[1U] = idxB_0[1U] - offset_0[1U];
        idxBLin_0[1U] = yDims[0U] * idxB_0[1U];
        idxB_0[0U] = sStart_0[0U];
        while (idxB_0[0U] <= sEnd_0[0U]) {
          idxA[0U] = idxB_0[0U] - offset_0[0U];
          idxBLin_0[0U] = idxBLin_0[1U] + idxB_0[0U];

          /* loop over kernel and compute conv/corr */
          count = 0;

          /* loop over kernel */
          hIdxA_0[1U] = 0;
          while (hIdxA_0[1U] <= hEnd[1U]) {
            hIdxB[1U] = idxA[1U] + hIdxA_0[1U];
            hIdxA_0[0U] = 0;
            while (hIdxA_0[0U] <= hEnd[0U]) {
              hIdxB[0U] = idxA[0U] + hIdxA_0[0U];

              /* count number of trues in the window */
              count += (int32_T)PadBConst_ub(&u[0U], &uStride[0U], &hIdxB[0U],
                &uOrigin[0U], &uEnd[0U], sNumPreEdges, &sPreEdges[0U],
                sNumPostEdges, &sPostEdges[0U], padValue);
              hIdxA_0[0U]++;
            }

            hIdxA_0[1U]++;
          }

          if (idxBLin_0[0U] >= 0) {
            y[idxBLin_0[0U]] = ((count << 1) >= hDims[0U] * hDims[1U]);
          }

          idxB_0[0U]++;
        }

        idxB_0[1U]++;
      }
    }

    /* advance sector counter */
    yPre = 0;
    while (yPre < 2) {
      c[yPre]++;
      if (c[yPre] <= 2) {
        isEnd = FALSE;
        yPre = 2;
      } else {
        c[yPre] = 0;
        isEnd = TRUE;
      }

      yPre++;
    }
  }

  /* End of S-Function (svipmdnfilter): '<S3>/Median Filter' */
}

void tmMdnFlt_M_RC_SIB_MConst_ub_yb_ab_pb_f(const int32_T hDims[], const
  boolean_T u[], const int32_T uDims[], boolean_T y[], int32_T yDims[],
  boolean_T padValue)
{
  int32_T hLoc[2];
  int32_T yOrigin[2];
  boolean_T isYEmpty;

  /* S-Function (svipmdnfilter): '<S3>/Median Filter' */
  /* set up kernel related coordinates */
  /* compute center */
  /* hLoc is the location of top left corner relative to the center of kernel. */
  if (hDims[0U] % 2 != 0) {
    hLoc[0U] = -(hDims[0U] >> 1);
  } else {
    hLoc[0U] = 1 - (hDims[0U] >> 1);
  }

  /* compute center */
  /* hLoc is the location of top left corner relative to the center of kernel. */
  if (hDims[1U] % 2 != 0) {
    hLoc[1U] = -(hDims[1U] >> 1);
  } else {
    hLoc[1U] = 1 - (hDims[1U] >> 1);
  }

  /* Region of Support (ROS) definition: A selected region that restricts the input space for processing. */
  /* ====================================================== */
  /*                       ---------------------            */
  /*                      |ROI                  |           */
  /*                      |                     |           */
  /*  ----------------------------------------------------  */
  /* | INPUT(u)           |                     |         | */
  /* |                    |                     |         | */
  /* |   ---------------------------------------------    | */
  /* |  | ROS             |OUTPUT(y)////////////|     |   | */
  /* |  |                 |/////////////////////|     |   | */
  /* |  |                  ---------------------      |   | */
  /* |  |                                             |   | */
  /* The user's output mode choices of 'Valid', 'Same as input' and 'Full' map onto correspending definitions of a rectangular ROS. */
  /* Output range support is computed as an intersection of ROS with Region of Interest (ROI) */
  /* ///////////////////// */
  /* begin ROS computation */
  /* compute ROS based on u, h and output mode */
  /* ROS is SAME AS INPUT */
  /* end ROS computation */
  /* /////////////////// */
  /* ///////////////////////// */
  /* begin y sizes computation */
  /* we need to compute yOrigin and yEnd */
  /* yOrigin definition: Location of center of reference (origin) of output (y) coordintate system with respect to input (u) coordinate system */
  /* y sizes are same as ros sizes because there is no ROI */
  yOrigin[0U] = 0;
  yDims[0U] = uDims[0U];
  isYEmpty = (yDims[0U] <= 0);

  /* y sizes are same as ros sizes because there is no ROI */
  yOrigin[1U] = 0;
  yDims[1U] = uDims[1U];
  isYEmpty = (boolean_T)((yDims[1U] <= 0) | (int32_T)isYEmpty);

  /* make yOrigin same as roiLoc when y is empty */
  if (isYEmpty) {
    yOrigin[0U] = 0;
    yOrigin[1U] = 0;
  }

  /* end y sizes computation */
  /* /////////////////////// */
  MdnFlt_M_IBConst_ub_yb_ab_pb_f(&hLoc[0U], &hDims[0U], &u[0U], &uDims[0U], &y
    [0U], &yDims[0U], &yOrigin[0U], padValue);

  /* End of S-Function (svipmdnfilter): '<S3>/Median Filter' */
}

/* Function for MATLAB Function: '<S4>/Optical Flow Lines' */
static void opitcal_flow_meshgrid(const real_T x[127], const real_T y[95],
  real_T xx[12065], real_T yy[12065])
{
  int32_T iacol;
  int32_T ia;
  int32_T ib;
  int32_T jcol;
  int32_T itilerow;
  ia = 1;
  ib = 0;
  iacol = 1;
  for (jcol = 0; jcol < 127; jcol++) {
    for (itilerow = 0; itilerow < 95; itilerow++) {
      xx[ib] = x[iacol - 1];
      ia = iacol + 1;
      ib++;
    }

    iacol = ia;
  }

  ia = 0;
  for (ib = 0; ib < 127; ib++) {
    iacol = 0;
    for (jcol = 0; jcol < 95; jcol++) {
      yy[ia] = y[iacol];
      iacol++;
      ia++;
    }
  }
}

/* Model step function for TID0 */
void opitcal_flow_step0(void)          /* Sample time: [0.016666666666666666s, 0.0s] */
{
  /* Matfile logging */
  rt_UpdateTXYLogVars(opitcal_flow_M->rtwLogInfo,
                      (&opitcal_flow_M->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.016666666666666666s, 0.0s] */
    if ((rtmGetTFinal(opitcal_flow_M)!=-1) &&
        !((rtmGetTFinal(opitcal_flow_M)-opitcal_flow_M->Timing.taskTime0) >
          opitcal_flow_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(opitcal_flow_M, "Simulation finished");
    }
  }

  /* Update absolute time */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++opitcal_flow_M->Timing.clockTick0)) {
    ++opitcal_flow_M->Timing.clockTickH0;
  }

  opitcal_flow_M->Timing.taskTime0 = opitcal_flow_M->Timing.clockTick0 *
    opitcal_flow_M->Timing.stepSize0 + opitcal_flow_M->Timing.clockTickH0 *
    opitcal_flow_M->Timing.stepSize0 * 4294967296.0;
}

/* Model step function for TID1 */
void opitcal_flow_step1(void)          /* Sample time: [0.033333333333333333s, 0.0s] */
{
  char_T *sErr;
  void *source_R;
  int32_T i;
  int32_T h1Dims[2];
  int32_T uDims[2];
  int32_T yDims[2];
  int32_T idxROI;
  int32_T idxPixCur;
  boolean_T isInBound;
  boolean_T visited;
  boolean_T visited_0;
  boolean_T done;
  real32_T rtb_Meanvelocityperframeacrosst;
  int32_T inIdx;
  int32_T centerM;
  int32_T centerN;
  int32_T n;
  int32_T line_idx;
  int32_T line_idx_0;
  int32_T line_idx_1;
  int32_T line_idx_2;
  int32_T line_idx_3;
  int32_T line_idx_4;
  int32_T line_idx_5;
  creal32_T rtb_OpticalFlowEstimation_0;

  /* S-Function (simaqfvd): '<Root>/From Video Device' */
  sErr = GetErrorBuffer(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U]);
  source_R = (void *)&opitcal_flow_B.FromVideoDevice[0U];
  LibOutputs_FVD(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U],
                 source_R, GetNullPointer(), GetNullPointer(), "single", 921600,
                 1, 0);
  if (*sErr != 0) {
    rtmSetErrorStatus(opitcal_flow_M, sErr);
    rtmSetStopRequested(opitcal_flow_M, 1);
  }

  /* End of S-Function (simaqfvd): '<Root>/From Video Device' */

  /* S-Function (svipcolorconv): '<Root>/Color Space  Conversion' */
  for (i = 0; i < 307200; i++) {
    opitcal_flow_B.ColorSpaceConversion[i] = (opitcal_flow_B.FromVideoDevice
      [307200 + i] * 0.587F + opitcal_flow_B.FromVideoDevice[i] * 0.299F) +
      opitcal_flow_B.FromVideoDevice[614400 + i] * 0.114F;
    if (opitcal_flow_B.ColorSpaceConversion[i] > 1.0F) {
      opitcal_flow_B.ColorSpaceConversion[i] = 1.0F;
    } else {
      if (opitcal_flow_B.ColorSpaceConversion[i] < 0.0F) {
        opitcal_flow_B.ColorSpaceConversion[i] = 0.0F;
      }
    }
  }

  /* End of S-Function (svipcolorconv): '<Root>/Color Space  Conversion' */

  /* Delay: '<S2>/Delay' */
  memcpy(&opitcal_flow_B.MathFunction[0], &opitcal_flow_DWork.Delay_DSTATE[0],
         307200U * sizeof(real32_T));

  /* S-Function (svipopticalflow): '<S2>/Optical Flow Estimation' */
  MWVIP_OpticalFlow_HS_C(&opitcal_flow_B.ColorSpaceConversion[0],
    &opitcal_flow_B.MathFunction[0], &opitcal_flow_B.OpticalFlowEstimation[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_MEMC0[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_MEMC1[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_MEMR0[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_MEMR1[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_GRADCC[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_GRADRC[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_GRADRR[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_GRADCT[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_GRADRT[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_ALPHA[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_VELBUFFC0[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_VELBUFFC1[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_VELBUFFR0[0],
    &opitcal_flow_DWork.OpticalFlowEstimation_VELBUFFR1[0],
    &opitcal_flow_P.OpticalFlowEstimation_LAMBDA_RT, TRUE, FALSE,
    &opitcal_flow_P.OpticalFlowEstimation_MAXITER_R, 0, 480, 640);

  /* Math: '<S3>/Math Function'
   *
   * About '<S3>/Math Function':
   *  Operator: magnitude^2
   */
  for (i = 0; i < 307200; i++) {
    rtb_OpticalFlowEstimation_0 = opitcal_flow_B.OpticalFlowEstimation[i];
    opitcal_flow_B.MathFunction[i] = rtb_OpticalFlowEstimation_0.re *
      rtb_OpticalFlowEstimation_0.re + rtb_OpticalFlowEstimation_0.im *
      rtb_OpticalFlowEstimation_0.im;
  }

  /* End of Math: '<S3>/Math Function' */

  /* S-Function (svipstatfcns): '<S6>/Mean velocity per frame' */
  for (i = 0; i < 307200; i += 307200) {
    for (inIdx = i; inIdx < i + 1; inIdx++) {
      opitcal_flow_DWork.Meanvelocityperframe_AccVal =
        opitcal_flow_B.MathFunction[inIdx];
      centerM = 1;
      for (n = 307198; n >= 0; n += -1) {
        opitcal_flow_DWork.Meanvelocityperframe_AccVal +=
          opitcal_flow_B.MathFunction[inIdx + centerM];
        centerM++;
      }

      rtb_Meanvelocityperframeacrosst =
        opitcal_flow_DWork.Meanvelocityperframe_AccVal / 307200.0F;
    }
  }

  /* End of S-Function (svipstatfcns): '<S6>/Mean velocity per frame' */

  /* S-Function (svipstatfcns): '<S6>/Mean velocity per frame across time' */
  opitcal_flow_DWork.Meanvelocityperframeacrosstim_l++;
  if (opitcal_flow_DWork.Meanvelocityperframeacrosstim_l > 1U) {
    opitcal_flow_DWork.Meanvelocityperframeacrosstime_ +=
      rtb_Meanvelocityperframeacrosst;
    rtb_Meanvelocityperframeacrosst =
      opitcal_flow_DWork.Meanvelocityperframeacrosstime_ / (real32_T)
      opitcal_flow_DWork.Meanvelocityperframeacrosstim_l;
  } else {
    if (opitcal_flow_DWork.Meanvelocityperframeacrosstim_l == 0U) {
      opitcal_flow_DWork.Meanvelocityperframeacrosstim_l = 1U;
    }

    opitcal_flow_DWork.Meanvelocityperframeacrosstime_ =
      rtb_Meanvelocityperframeacrosst;
  }

  /* End of S-Function (svipstatfcns): '<S6>/Mean velocity per frame across time' */

  /* RelationalOperator: '<S3>/Relational Operator' */
  for (i = 0; i < 307200; i++) {
    opitcal_flow_B.RelationalOperator[i] = (opitcal_flow_B.MathFunction[i] >=
      rtb_Meanvelocityperframeacrosst);
  }

  /* End of RelationalOperator: '<S3>/Relational Operator' */

  /* S-Function (svipmdnfilter): '<S3>/Median Filter' */
  h1Dims[0U] = 3;
  h1Dims[1U] = 3;
  uDims[0U] = 480;
  uDims[1U] = 640;
  tmMdnFlt_M_RC_SIB_MConst_ub_yb_ab_pb_f(&h1Dims[0U],
    &opitcal_flow_B.RelationalOperator[0U], &uDims[0U],
    &opitcal_flow_B.MedianFilter[0U], &yDims[0U],
    opitcal_flow_P.MedianFilter_PadValue);

  /* S-Function (svipmorphop): '<S3>/Closing' */
  for (i = 0; i < 317361; i++) {
    opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[i] = FALSE;
  }

  for (inIdx = 0; inIdx < 307200; inIdx++) {
    if (opitcal_flow_B.MedianFilter[inIdx]) {
      centerM = div_s32(inIdx, 480);
      centerM = (inIdx - centerM * 480) + centerM * 489;
      for (i = 0; i < opitcal_flow_DWork.Closing_NUMNONZ_DW; i++) {
        opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[centerM +
          opitcal_flow_DWork.Closing_DILATE_OFF_DW[i]] = TRUE;
      }
    }
  }

  inIdx = 980;
  centerM = 0;
  for (n = 0; n < 640; n++) {
    for (centerN = 0; centerN < 480; centerN++) {
      opitcal_flow_B.Closing[centerM] =
        opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[inIdx];
      centerM++;
      inIdx++;
    }

    inIdx += 9;
  }

  centerM = 0;
  centerN = 0;
  for (i = 0; i < 2; i++) {
    for (inIdx = 0; inIdx < 489; inIdx++) {
      opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[centerM] = TRUE;
      centerM++;
    }
  }

  for (i = 0; i < 640; i++) {
    opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[centerM] = TRUE;
    centerM++;
    opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[centerM] = TRUE;
    centerM++;
    memcpy(&opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[centerM],
           &opitcal_flow_B.Closing[centerN], 480U * sizeof(boolean_T));
    centerM += 480;
    centerN += 480;
    for (inIdx = 0; inIdx < 7; inIdx++) {
      opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[centerM] = TRUE;
      centerM++;
    }
  }

  for (i = 0; i < 7; i++) {
    for (inIdx = 0; inIdx < 489; inIdx++) {
      opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[centerM] = TRUE;
      centerM++;
    }
  }

  inIdx = 0;
  centerM = 0;
  for (n = 0; n < 640; n++) {
    for (centerN = 0; centerN < 480; centerN++) {
      opitcal_flow_B.Closing[centerM] = TRUE;
      i = 0;
      while (i < opitcal_flow_DWork.Closing_NUMNONZ_DW) {
        if (!opitcal_flow_DWork.Closing_ONE_PAD_IMG_DW[inIdx +
            opitcal_flow_DWork.Closing_ERODE_OFF_DW[i]]) {
          opitcal_flow_B.Closing[centerM] = FALSE;
          i = opitcal_flow_DWork.Closing_NUMNONZ_DW;
        }

        i++;
      }

      inIdx++;
      centerM++;
    }

    inIdx += 9;
  }

  /* End of S-Function (svipmorphop): '<S3>/Closing' */

  /* MATLAB Function: '<S4>/Optical Flow Lines' */
  /* MATLAB Function 'Display Results/Optical Flow Lines/Optical Flow Lines': '<S5>:1' */
  /*  Generates the coordinate points of optical flow lines */
  if (!opitcal_flow_DWork.first_time_not_empty) {
    /* '<S5>:1:10' */
    /* '<S5>:1:11' */
    opitcal_flow_DWork.first_time_not_empty = TRUE;

    /*     %% user may change the following three parameters */
    /*     %% */
    /* '<S5>:1:18' */
    for (n = 0; n < 95; n++) {
      opitcal_flow_DWork.RV[n] = 5.0 * (real_T)n + 5.0;
    }

    /* '<S5>:1:19' */
    for (n = 0; n < 127; n++) {
      opitcal_flow_DWork.CV[n] = 5.0 * (real_T)n + 5.0;
    }

    opitcal_flow_meshgrid(opitcal_flow_DWork.CV, opitcal_flow_DWork.RV,
                          opitcal_flow_DWork.Y, opitcal_flow_DWork.X);

    /* '<S5>:1:20' */
  }

  /* '<S5>:1:23' */
  /* '<S5>:1:24' */
  for (n = 0; n < 127; n++) {
    for (idxPixCur = 0; idxPixCur < 95; idxPixCur++) {
      opitcal_flow_B.tmp[idxPixCur + 95 * n].re =
        opitcal_flow_B.OpticalFlowEstimation[(((int32_T)opitcal_flow_DWork.CV[n]
        - 1) * 480 + (int32_T)opitcal_flow_DWork.RV[idxPixCur]) - 1].re *
        (real32_T)opitcal_flow_P.SFunction_p1;
      opitcal_flow_B.tmp[idxPixCur + 95 * n].im =
        opitcal_flow_B.OpticalFlowEstimation[(((int32_T)opitcal_flow_DWork.CV[n]
        - 1) * 480 + (int32_T)opitcal_flow_DWork.RV[idxPixCur]) - 1].im *
        (real32_T)opitcal_flow_P.SFunction_p1;
    }
  }

  /* '<S5>:1:25' */
  for (n = 0; n < 12065; n++) {
    opitcal_flow_B.vel_Lines[n] = (real32_T)opitcal_flow_DWork.Y[n];
  }

  for (n = 0; n < 12065; n++) {
    opitcal_flow_B.vel_Lines[n + 12065] = (real32_T)opitcal_flow_DWork.X[n];
  }

  for (n = 0; n < 12065; n++) {
    opitcal_flow_B.vel_Lines[n + 24130] = (real32_T)opitcal_flow_DWork.Y[n] +
      opitcal_flow_B.tmp[n].re;
  }

  for (n = 0; n < 12065; n++) {
    opitcal_flow_B.vel_Lines[n + 36195] = (real32_T)opitcal_flow_DWork.X[n] +
      opitcal_flow_B.tmp[n].im;
  }

  /* End of MATLAB Function: '<S4>/Optical Flow Lines' */

  /* S-Function (svipdrawshapes): '<S1>/Draw Shapes' */
  /* Copy the image from input to output. */
  memcpy(&opitcal_flow_B.DrawShapes[0], &opitcal_flow_B.FromVideoDevice[0],
         921600U * sizeof(real32_T));

  /* Update view port. */
  for (idxROI = 0; idxROI < 12065; idxROI++) {
    rtb_Meanvelocityperframeacrosst = (real32_T)fabs
      (opitcal_flow_B.vel_Lines[idxROI + 12065]);
    if (rtb_Meanvelocityperframeacrosst < 8.388608E+6F) {
      if (rtb_Meanvelocityperframeacrosst >= 0.5F) {
        rtb_Meanvelocityperframeacrosst = (real32_T)floor
          (opitcal_flow_B.vel_Lines[idxROI + 12065] + 0.5F);
      } else {
        rtb_Meanvelocityperframeacrosst = opitcal_flow_B.vel_Lines[idxROI +
          12065] * 0.0F;
      }
    } else {
      rtb_Meanvelocityperframeacrosst = opitcal_flow_B.vel_Lines[idxROI + 12065];
    }

    line_idx_4 = (int32_T)rtb_Meanvelocityperframeacrosst - 1;
    rtb_Meanvelocityperframeacrosst = (real32_T)fabs
      (opitcal_flow_B.vel_Lines[idxROI]);
    if (rtb_Meanvelocityperframeacrosst < 8.388608E+6F) {
      if (rtb_Meanvelocityperframeacrosst >= 0.5F) {
        rtb_Meanvelocityperframeacrosst = (real32_T)floor
          (opitcal_flow_B.vel_Lines[idxROI] + 0.5F);
      } else {
        rtb_Meanvelocityperframeacrosst = opitcal_flow_B.vel_Lines[idxROI] *
          0.0F;
      }
    } else {
      rtb_Meanvelocityperframeacrosst = opitcal_flow_B.vel_Lines[idxROI];
    }

    line_idx_2 = (int32_T)rtb_Meanvelocityperframeacrosst - 1;
    rtb_Meanvelocityperframeacrosst = (real32_T)fabs
      (opitcal_flow_B.vel_Lines[idxROI + 36195]);
    if (rtb_Meanvelocityperframeacrosst < 8.388608E+6F) {
      if (rtb_Meanvelocityperframeacrosst >= 0.5F) {
        rtb_Meanvelocityperframeacrosst = (real32_T)floor
          (opitcal_flow_B.vel_Lines[idxROI + 36195] + 0.5F);
      } else {
        rtb_Meanvelocityperframeacrosst = opitcal_flow_B.vel_Lines[idxROI +
          36195] * 0.0F;
      }
    } else {
      rtb_Meanvelocityperframeacrosst = opitcal_flow_B.vel_Lines[idxROI + 36195];
    }

    i = (int32_T)rtb_Meanvelocityperframeacrosst - 1;
    rtb_Meanvelocityperframeacrosst = (real32_T)fabs(opitcal_flow_B.vel_Lines
      [24130 + idxROI]);
    if (rtb_Meanvelocityperframeacrosst < 8.388608E+6F) {
      if (rtb_Meanvelocityperframeacrosst >= 0.5F) {
        rtb_Meanvelocityperframeacrosst = (real32_T)floor
          (opitcal_flow_B.vel_Lines[24130 + idxROI] + 0.5F);
      } else {
        rtb_Meanvelocityperframeacrosst = opitcal_flow_B.vel_Lines[24130 +
          idxROI] * 0.0F;
      }
    } else {
      rtb_Meanvelocityperframeacrosst = opitcal_flow_B.vel_Lines[24130 + idxROI];
    }

    line_idx = (int32_T)rtb_Meanvelocityperframeacrosst - 1;
    if ((line_idx_4 != i) || (line_idx_2 != line_idx)) {
      isInBound = FALSE;

      /* Find the visible portion of a line. */
      visited = FALSE;
      visited_0 = FALSE;
      done = FALSE;
      line_idx_5 = line_idx_4;
      line_idx_3 = line_idx_2;
      line_idx_1 = i;
      line_idx_0 = line_idx;
      while (!done) {
        n = 0;
        idxPixCur = 0;

        /* Determine viewport violations. */
        if (line_idx_5 < 0) {
          n = 4;
        } else {
          if (line_idx_5 > 479) {
            n = 8;
          }
        }

        if (line_idx_1 < 0) {
          idxPixCur = 4;
        } else {
          if (line_idx_1 > 479) {
            idxPixCur = 8;
          }
        }

        if (line_idx_3 < 0) {
          n = (int32_T)((uint32_T)n | 1U);
        } else {
          if (line_idx_3 > 639) {
            n = (int32_T)((uint32_T)n | 2U);
          }
        }

        if (line_idx_0 < 0) {
          idxPixCur = (int32_T)((uint32_T)idxPixCur | 1U);
        } else {
          if (line_idx_0 > 639) {
            idxPixCur = (int32_T)((uint32_T)idxPixCur | 2U);
          }
        }

        if (!(((uint32_T)n | (uint32_T)idxPixCur) != 0U)) {
          /* Line falls completely within bounds. */
          done = TRUE;
          isInBound = TRUE;
        } else if (((uint32_T)n & (uint32_T)idxPixCur) != 0U) {
          /* Line falls completely out of bounds. */
          done = TRUE;
          isInBound = FALSE;
        } else if ((uint32_T)n != 0U) {
          /* Clip 1st point; if it's in-bounds, clip 2nd point. */
          if (visited) {
            line_idx_5 = line_idx_4;
            line_idx_3 = line_idx_2;
          }

          centerM = line_idx_1 - line_idx_5;
          centerN = line_idx_0 - line_idx_3;
          if (((uint32_T)n & 4U) != 0U) {
            /* Violated RMin. */
            inIdx = -line_idx_5 * centerN;
            if (((inIdx >= 0) && (centerM >= 0)) || ((inIdx < 0) && (centerM < 0)))
            {
              line_idx_3 += (div_s32_floor(inIdx << 1, centerM) + 1) >> 1;
            } else {
              line_idx_3 -= (div_s32_floor(-inIdx << 1, centerM) + 1) >> 1;
            }

            line_idx_5 = 0;
            visited = TRUE;
          } else if (((uint32_T)n & 8U) != 0U) {
            /* Violated RMax. */
            inIdx = (479 - line_idx_5) * centerN;
            if (((inIdx >= 0) && (centerM >= 0)) || ((inIdx < 0) && (centerM < 0)))
            {
              line_idx_3 += (div_s32_floor(inIdx << 1, centerM) + 1) >> 1;
            } else {
              line_idx_3 -= (div_s32_floor(-inIdx << 1, centerM) + 1) >> 1;
            }

            line_idx_5 = 479;
            visited = TRUE;
          } else if (((uint32_T)n & 1U) != 0U) {
            /* Violated CMin. */
            inIdx = -line_idx_3 * centerM;
            if (((inIdx >= 0) && (centerN >= 0)) || ((inIdx < 0) && (centerN < 0)))
            {
              line_idx_5 += (div_s32_floor(inIdx << 1, centerN) + 1) >> 1;
            } else {
              line_idx_5 -= (div_s32_floor(-inIdx << 1, centerN) + 1) >> 1;
            }

            line_idx_3 = 0;
            visited = TRUE;
          } else {
            /* Violated CMax. */
            inIdx = (639 - line_idx_3) * centerM;
            if (((inIdx >= 0) && (centerN >= 0)) || ((inIdx < 0) && (centerN < 0)))
            {
              line_idx_5 += (div_s32_floor(inIdx << 1, centerN) + 1) >> 1;
            } else {
              line_idx_5 -= (div_s32_floor(-inIdx << 1, centerN) + 1) >> 1;
            }

            line_idx_3 = 639;
            visited = TRUE;
          }
        } else {
          /* Clip the 2nd point. */
          if (visited_0) {
            line_idx_1 = i;
            line_idx_0 = line_idx;
          }

          centerM = line_idx_1 - line_idx_5;
          centerN = line_idx_0 - line_idx_3;
          if (((uint32_T)idxPixCur & 4U) != 0U) {
            /* Violated RMin. */
            inIdx = -line_idx_1 * centerN;
            if (((inIdx >= 0) && (centerM >= 0)) || ((inIdx < 0) && (centerM < 0)))
            {
              line_idx_0 += (div_s32_floor(inIdx << 1, centerM) + 1) >> 1;
            } else {
              line_idx_0 -= (div_s32_floor(-inIdx << 1, centerM) + 1) >> 1;
            }

            line_idx_1 = 0;
            visited_0 = TRUE;
          } else if (((uint32_T)idxPixCur & 8U) != 0U) {
            /* Violated RMax. */
            inIdx = (479 - line_idx_1) * centerN;
            if (((inIdx >= 0) && (centerM >= 0)) || ((inIdx < 0) && (centerM < 0)))
            {
              line_idx_0 += (div_s32_floor(inIdx << 1, centerM) + 1) >> 1;
            } else {
              line_idx_0 -= (div_s32_floor(-inIdx << 1, centerM) + 1) >> 1;
            }

            line_idx_1 = 479;
            visited_0 = TRUE;
          } else if (((uint32_T)idxPixCur & 1U) != 0U) {
            /* Violated CMin. */
            inIdx = -line_idx_0 * centerM;
            if (((inIdx >= 0) && (centerN >= 0)) || ((inIdx < 0) && (centerN < 0)))
            {
              line_idx_1 += (div_s32_floor(inIdx << 1, centerN) + 1) >> 1;
            } else {
              line_idx_1 -= (div_s32_floor(-inIdx << 1, centerN) + 1) >> 1;
            }

            line_idx_0 = 0;
            visited_0 = TRUE;
          } else {
            /* Violated CMax. */
            inIdx = (639 - line_idx_0) * centerM;
            if (((inIdx >= 0) && (centerN >= 0)) || ((inIdx < 0) && (centerN < 0)))
            {
              line_idx_1 += (div_s32_floor(inIdx << 1, centerN) + 1) >> 1;
            } else {
              line_idx_1 -= (div_s32_floor(-inIdx << 1, centerN) + 1) >> 1;
            }

            line_idx_0 = 639;
            visited_0 = TRUE;
          }
        }
      }

      if (isInBound) {
        /* Draw a line using Bresenham algorithm. */
        /* Initialize the Bresenham algorithm. */
        if (line_idx_1 >= line_idx_5) {
          i = line_idx_1 - line_idx_5;
        } else {
          i = line_idx_5 - line_idx_1;
        }

        if (line_idx_0 >= line_idx_3) {
          line_idx = line_idx_0 - line_idx_3;
        } else {
          line_idx = line_idx_3 - line_idx_0;
        }

        if (i > line_idx) {
          line_idx_2 = 1;
          centerN = 480;
        } else {
          line_idx_2 = 480;
          centerN = 1;
          centerM = line_idx_5;
          line_idx_5 = line_idx_3;
          line_idx_3 = centerM;
          centerM = line_idx_1;
          line_idx_1 = line_idx_0;
          line_idx_0 = centerM;
        }

        if (line_idx_5 > line_idx_1) {
          centerM = line_idx_5;
          line_idx_5 = line_idx_1;
          line_idx_1 = centerM;
          centerM = line_idx_3;
          line_idx_3 = line_idx_0;
          line_idx_0 = centerM;
        }

        i = line_idx_1 - line_idx_5;
        if (line_idx_3 <= line_idx_0) {
          centerM = 1;
          n = line_idx_0 - line_idx_3;
        } else {
          centerM = -1;
          n = line_idx_3 - line_idx_0;
        }

        line_idx_4 = -((i + 1) >> 1);
        idxPixCur = line_idx_5 * line_idx_2 + line_idx_3 * centerN;
        centerM = centerM * centerN + line_idx_2;
        isInBound = (line_idx_5 <= line_idx_1);
        while (isInBound) {
          opitcal_flow_B.DrawShapes[idxPixCur] =
            opitcal_flow_P.DrawShapes_RTP_FILLCOLOR[0];
          inIdx = idxPixCur + 307200;
          opitcal_flow_B.DrawShapes[inIdx] =
            opitcal_flow_P.DrawShapes_RTP_FILLCOLOR[1];
          inIdx += 307200;
          opitcal_flow_B.DrawShapes[inIdx] =
            opitcal_flow_P.DrawShapes_RTP_FILLCOLOR[2];

          /* Compute the next location using Bresenham algorithm. */
          /* Move to the next pixel location. */
          line_idx_4 += n;
          if (line_idx_4 >= 0) {
            line_idx_4 -= i;
            idxPixCur += centerM;
          } else {
            idxPixCur += line_idx_2;
          }

          line_idx_5++;
          isInBound = (line_idx_5 <= line_idx_1);
        }
      }
    }
  }

  /* End of S-Function (svipdrawshapes): '<S1>/Draw Shapes' */

  /* Update for Delay: '<S2>/Delay' */
  memcpy(&opitcal_flow_DWork.Delay_DSTATE[0],
         &opitcal_flow_B.ColorSpaceConversion[0], 307200U * sizeof(real32_T));
}

/* Model step wrapper function for compatibility with a static main program */
void opitcal_flow_step(int_T tid)
{
  switch (tid) {
   case 0 :
    opitcal_flow_step0();
    break;

   case 1 :
    opitcal_flow_step1();
    break;

   default :
    break;
  }
}

/* Model initialize function */
void opitcal_flow_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)opitcal_flow_M, 0,
                sizeof(RT_MODEL_opitcal_flow));
  (opitcal_flow_M)->Timing.TaskCounters.cLimit[0] = 1;
  (opitcal_flow_M)->Timing.TaskCounters.cLimit[1] = 2;
  rtmSetTFinal(opitcal_flow_M, 10.0);
  opitcal_flow_M->Timing.stepSize0 = 0.016666666666666666;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    opitcal_flow_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(opitcal_flow_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(opitcal_flow_M->rtwLogInfo, (NULL));
    rtliSetLogT(opitcal_flow_M->rtwLogInfo, "tout");
    rtliSetLogX(opitcal_flow_M->rtwLogInfo, "");
    rtliSetLogXFinal(opitcal_flow_M->rtwLogInfo, "");
    rtliSetSigLog(opitcal_flow_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(opitcal_flow_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(opitcal_flow_M->rtwLogInfo, 0);
    rtliSetLogMaxRows(opitcal_flow_M->rtwLogInfo, 1000);
    rtliSetLogDecimation(opitcal_flow_M->rtwLogInfo, 1);
    rtliSetLogY(opitcal_flow_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(opitcal_flow_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(opitcal_flow_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &opitcal_flow_B), 0,
                sizeof(BlockIO_opitcal_flow));

  /* states (dwork) */
  (void) memset((void *)&opitcal_flow_DWork, 0,
                sizeof(D_Work_opitcal_flow));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(opitcal_flow_M->rtwLogInfo, 0.0, rtmGetTFinal
                                   (opitcal_flow_M),
    opitcal_flow_M->Timing.stepSize0, (&rtmGetErrorStatus(opitcal_flow_M)));

  {
    char_T *sErr;
    char * deviceInfo[7];
    char * triggerInfo[3];
    char * colorSpaceInfo[2];
    int32_T roiArray[4];
    char * propNames;
    int32_T propLengths;
    void *propValues;
    char * featureNames[25];
    void *featureValues[25];
    int32_T featureTypes[25];
    real_T currentFeatureRef;
    real_T currentFeatureRef_0;
    real_T currentFeatureRef_1;
    real_T currentFeatureRef_2;
    real_T currentFeatureRef_3;
    real_T currentFeatureRef_4;
    real_T currentFeatureRef_5;
    real_T currentFeatureRef_6;
    real_T currentFeatureRef_7;
    real_T currentFeatureRef_8;
    real_T currentFeatureRef_9;
    real_T currentFeatureRef_a;
    real_T currentFeatureRef_b;
    real_T currentFeatureRef_c;
    real_T currentFeatureRef_d;
    real_T currentFeatureRef_e;
    real_T currentFeatureRef_f;
    real_T currentFeatureRef_g;
    real_T currentFeatureRef_h;
    real_T currentFeatureRef_i;
    real_T currentFeatureRef_j;
    real_T currentFeatureRef_k;
    real_T currentFeatureRef_l;
    real_T currentFeatureRef_m;
    real_T currentFeatureRef_n;
    int32_T idxNHood;
    int32_T idxOffsets;
    int32_T curNumNonZ;
    int32_T n;
    int32_T m;

    /* Start for S-Function (simaqfvd): '<Root>/From Video Device' */
    sErr = GetErrorBuffer(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U]);
    CreateHostLibrary("libmwimaqmex.dylib",
                      &opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U]);
    if (*sErr != 0) {
      CreateHostLibrary("libmwimaqmex.dylib",
                        &opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U]);
    }

    if (*sErr == 0) {
      deviceInfo[0U] = "macvideo";
      deviceInfo[1U] =
        "/Applications/MATLAB_R2012b.app/toolbox/imaq/imaqadaptors/maci64/mwmacvideoimaq.dylib";
      deviceInfo[2U] = "Built-in iSight";
      deviceInfo[3U] = "YCbCr422_640x480";
      deviceInfo[4U] = "default";
      deviceInfo[5U] =
        "/Applications/MATLAB_R2012b.app/toolbox/imaq/imaq/private/imaqmex.imdf";
      deviceInfo[6U] =
        "/Applications/MATLAB_R2012b.app/toolbox/imaq/imaqadaptors/maci64/mwmacvideoimaq.imdf";
      triggerInfo[0U] = "manual";
      triggerInfo[1U] = "none";
      triggerInfo[2U] = "none";
      colorSpaceInfo[0U] = "rgb";
      colorSpaceInfo[1U] = "grbg";
      roiArray[0U] = 0;
      roiArray[1U] = 0;
      roiArray[2U] = 640;
      roiArray[3U] = 480;
      propNames = "Tag";
      propLengths = 0;
      propValues = (void *)"";
      featureNames[0U] = "-debug";
      featureTypes[0U] = 1;
      currentFeatureRef = 0.0;
      featureValues[0U] = (void *)&currentFeatureRef;
      featureNames[1U] = "-debuggentlacquisition";
      featureTypes[1U] = 1;
      currentFeatureRef_0 = 0.0;
      featureValues[1U] = (void *)&currentFeatureRef_0;
      featureNames[2U] = "-debuggentldiscovery";
      featureTypes[2U] = 1;
      currentFeatureRef_1 = 0.0;
      featureValues[2U] = (void *)&currentFeatureRef_1;
      featureNames[3U] = "-debuggentloperation";
      featureTypes[3U] = 1;
      currentFeatureRef_2 = 0.0;
      featureValues[3U] = (void *)&currentFeatureRef_2;
      featureNames[4U] = "-debuggigeconnect";
      featureTypes[4U] = 1;
      currentFeatureRef_3 = 0.0;
      featureValues[4U] = (void *)&currentFeatureRef_3;
      featureNames[5U] = "-debuggigediscovery";
      featureTypes[5U] = 1;
      currentFeatureRef_4 = 0.0;
      featureValues[5U] = (void *)&currentFeatureRef_4;
      featureNames[6U] = "-debuggigeframeassembly";
      featureTypes[6U] = 1;
      currentFeatureRef_5 = 0.0;
      featureValues[6U] = (void *)&currentFeatureRef_5;
      featureNames[7U] = "-debuggigegvspreception";
      featureTypes[7U] = 1;
      currentFeatureRef_6 = 0.0;
      featureValues[7U] = (void *)&currentFeatureRef_6;
      featureNames[8U] = "-debuggigeopen";
      featureTypes[8U] = 1;
      currentFeatureRef_7 = 0.0;
      featureValues[8U] = (void *)&currentFeatureRef_7;
      featureNames[9U] = "-debuggigepacketresend";
      featureTypes[9U] = 1;
      currentFeatureRef_8 = 0.0;
      featureValues[9U] = (void *)&currentFeatureRef_8;
      featureNames[10U] = "-genicamcommandsavailable";
      featureTypes[10U] = 1;
      currentFeatureRef_9 = 0.0;
      featureValues[10U] = (void *)&currentFeatureRef_9;
      featureNames[11U] = "-gigedisableforceip";
      featureTypes[11U] = 1;
      currentFeatureRef_a = 0.0;
      featureValues[11U] = (void *)&currentFeatureRef_a;
      featureNames[12U] = "-gigedisablepacketresend";
      featureTypes[12U] = 1;
      currentFeatureRef_b = 0.0;
      featureValues[12U] = (void *)&currentFeatureRef_b;
      featureNames[13U] = "-logallevents";
      featureTypes[13U] = 1;
      currentFeatureRef_c = 0.0;
      featureValues[13U] = (void *)&currentFeatureRef_c;
      featureNames[14U] = "-previewfullbitdepth";
      featureTypes[14U] = 1;
      currentFeatureRef_d = 0.0;
      featureValues[14U] = (void *)&currentFeatureRef_d;
      featureNames[15U] = "-slowpreview";
      featureTypes[15U] = 1;
      currentFeatureRef_e = 0.0;
      featureValues[15U] = (void *)&currentFeatureRef_e;
      featureNames[16U] = "-usedcamlittleendian";
      featureTypes[16U] = 1;
      currentFeatureRef_f = 0.0;
      featureValues[16U] = (void *)&currentFeatureRef_f;
      featureNames[17U] = "-useobsoletepreview";
      featureTypes[17U] = 1;
      currentFeatureRef_g = 0.0;
      featureValues[17U] = (void *)&currentFeatureRef_g;
      featureNames[18U] = "-vfw";
      featureTypes[18U] = 1;
      currentFeatureRef_h = 1.0;
      featureValues[18U] = (void *)&currentFeatureRef_h;
      featureNames[19U] = "-gigecommandpacketretries";
      featureTypes[19U] = 0;
      currentFeatureRef_i = 1.0;
      featureValues[19U] = (void *)&currentFeatureRef_i;
      featureNames[20U] = "-gigeheartbeattimeout";
      featureTypes[20U] = 0;
      currentFeatureRef_j = 1000.0;
      featureValues[20U] = (void *)&currentFeatureRef_j;
      featureNames[21U] = "-gigepacketacktimeout";
      featureTypes[21U] = 0;
      currentFeatureRef_k = 500.0;
      featureValues[21U] = (void *)&currentFeatureRef_k;
      featureNames[22U] = "-macvideoframegrabduringdevicediscoverytimeout";
      featureTypes[22U] = 0;
      currentFeatureRef_l = 10000.0;
      featureValues[22U] = (void *)&currentFeatureRef_l;
      featureNames[23U] = "-pointgreystartdelay";
      featureTypes[23U] = 0;
      currentFeatureRef_m = 700.0;
      featureValues[23U] = (void *)&currentFeatureRef_m;
      featureNames[24U] = "-pointgreystopdelay";
      featureTypes[24U] = 0;
      currentFeatureRef_n = 850.0;
      featureValues[24U] = (void *)&currentFeatureRef_n;
      LibCreate_FVD(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U],
                    &deviceInfo[0U], 1, &triggerInfo[0U], (rtInf), 1.0,
                    &roiArray[0U], &colorSpaceInfo[0U], 1, &propNames,
                    &propLengths, &propValues, 25, &featureNames[0U],
                    &featureTypes[0U], &featureValues[0U]);
      LibStart(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U]);
    }

    if (*sErr != 0) {
      rtmSetErrorStatus(opitcal_flow_M, sErr);
      rtmSetStopRequested(opitcal_flow_M, 1);
    }

    /* End of Start for S-Function (simaqfvd): '<Root>/From Video Device' */

    /* Start for S-Function (svipmorphop): '<S3>/Closing' */
    idxNHood = 0;
    idxOffsets = 0;
    curNumNonZ = 0;
    for (n = 0; n < 5; n++) {
      for (m = 0; m < 5; m++) {
        if (opitcal_flow_ConstP.Closing_NHOOD_RTP[idxNHood]) {
          opitcal_flow_DWork.Closing_ERODE_OFF_DW[idxOffsets] = n * 489 + m;
          curNumNonZ++;
          idxOffsets++;
        }

        idxNHood++;
      }
    }

    opitcal_flow_DWork.Closing_NUMNONZ_DW = curNumNonZ;
    idxNHood = 0;
    idxOffsets = 0;
    curNumNonZ = 0;
    for (n = 0; n < 5; n++) {
      for (m = 0; m < 5; m++) {
        if (opitcal_flow_ConstP.Closing_NHOOD_RTP[idxNHood]) {
          opitcal_flow_DWork.Closing_DILATE_OFF_DW[idxOffsets] = n * 489 + m;
          curNumNonZ++;
          idxOffsets++;
        }

        idxNHood++;
      }
    }

    opitcal_flow_DWork.Closing_NUMNONZ_DW = curNumNonZ;

    /* End of Start for S-Function (svipmorphop): '<S3>/Closing' */
  }

  {
    int32_T i;

    /* InitializeConditions for Delay: '<S2>/Delay' */
    for (i = 0; i < 307200; i++) {
      opitcal_flow_DWork.Delay_DSTATE[i] = opitcal_flow_P.Delay_InitialCondition;
    }

    /* End of InitializeConditions for Delay: '<S2>/Delay' */

    /* InitializeConditions for S-Function (svipstatfcns): '<S6>/Mean velocity per frame across time' */
    opitcal_flow_DWork.Meanvelocityperframeacrosstim_l = 0U;
    opitcal_flow_DWork.Meanvelocityperframeacrosstime_ = 0.0F;

    /* InitializeConditions for MATLAB Function: '<S4>/Optical Flow Lines' */
    opitcal_flow_DWork.first_time_not_empty = FALSE;
  }
}

/* Model terminate function */
void opitcal_flow_terminate(void)
{
  char_T *sErr;

  /* Terminate for S-Function (simaqfvd): '<Root>/From Video Device' */
  sErr = GetErrorBuffer(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U]);
  LibTerminate(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(opitcal_flow_M, sErr);
    rtmSetStopRequested(opitcal_flow_M, 1);
  }

  LibDestroy(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U], 0);
  DestroyHostLibrary(&opitcal_flow_DWork.FromVideoDevice_FromVideoDevice[0U]);

  /* End of Terminate for S-Function (simaqfvd): '<Root>/From Video Device' */
}
