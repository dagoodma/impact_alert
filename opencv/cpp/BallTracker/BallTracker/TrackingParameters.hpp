// TrackingParameters
#ifndef TRACKING_PARAMETERS_H
#define TRACKING_PARAMETERS_H

// Include standard libraries
#include <string>
#include <map>

// -------------------------- Definitions -------------------------
// Defaults without background subtraction
/*
#define DEFAULT_HUE_LOWER           27
#define DEFAULT_HUE_UPPER           42
#define DEFAULT_SAT_LOWER           58
#define DEFAULT_SAT_UPPER           256
#define DEFAULT_VAL_LOWER           44
#define DEFAULT_VAL_UPPER           256
#define DEFAULT_FILTER_ITERATIONS   3
#define DEFAULT_FILTER_SIGMA        2
#define DEFAULT_HOUGH_DP            1 // TODO: remove this parameter
#define DEFAULT_HOUGH_MIN_DIST		50
#define DEFAULT_HOUGH_THRESH_LOWER  16  
#define DEFAULT_HOUGH_THRESH_UPPER  114
#define DEFAULT_HOUGH_MIN_RADIUS    3
#define DEFAULT_HOUGH_MAX_RADIUS    140
*/
// Default with MOG2, HSV, and blur
#define DEFAULT_HUE_LOWER           4
#define DEFAULT_HUE_UPPER           59
#define DEFAULT_SAT_LOWER           67
#define DEFAULT_SAT_UPPER           256
#define DEFAULT_VAL_LOWER           48
#define DEFAULT_VAL_UPPER           256
#define DEFAULT_FILTER_ITERATIONS   3
#define DEFAULT_FILTER_SIGMA        5
#define DEFAULT_HOUGH_DP            1 // TODO: remove this parameter
#define DEFAULT_HOUGH_MIN_DIST		50
#define DEFAULT_HOUGH_THRESH_LOWER  16  
#define DEFAULT_HOUGH_THRESH_UPPER  114
#define DEFAULT_HOUGH_MIN_RADIUS    3
#define DEFAULT_HOUGH_MAX_RADIUS    140

// Default with HSV, morph, and contour
#define DEFAULT_HUE_LOWER           0
#define DEFAULT_HUE_UPPER           256
#define DEFAULT_SAT_LOWER           89
#define DEFAULT_SAT_UPPER           256
#define DEFAULT_VAL_LOWER           74
#define DEFAULT_VAL_UPPER           256
#define DEFAULT_FILTER_ITERATIONS   2
#define DEFAULT_FILTER_SIGMA        5
#define DEFAULT_HOUGH_DP            1 // TODO: remove this parameter
#define DEFAULT_HOUGH_MIN_DIST		50
#define DEFAULT_HOUGH_THRESH_LOWER  16  
#define DEFAULT_HOUGH_THRESH_UPPER  114
#define DEFAULT_HOUGH_MIN_RADIUS    3
#define DEFAULT_HOUGH_MAX_RADIUS    140
#define DEFAULT_CONTOUR_MIN_AREA	(20*20) // 20px by 20px
#define DEFAULT_CONTOUR_MAX_AREA    (204800) // W*H / 1.5
#define DEFAULT_CONTOUR_MAX_OBJECTS	50

// Maximum parameter values
#define DEFAULT_FILTER_HSV_MAX			256
#define DEFAULT_FILTER_ITERATIONS_MAX	16
#define DEFAULT_FILTER_SIGMA_MAX		16
#define DEFAULT_HOUGH_DP_MAX			8
#define DEFAULT_HOUGH_THRESH_MAX		500
#define DEFAULT_HOUGH_RADIUS_MAX		1024

/**
	Class for TrackingParameters object. Storage for tracking parameters used by BallTracker.
*/
class TrackingParameters
{
    public:
		/**
			Constructor for TrackingParameters.
			@param hueLower Lower hue bound for HSV threshold filtering.
			@param hueUpper Upper hue bound for HSV threshold filtering.
			@param satLower	Lower saturation bound for HSV threshold filtering.
			@param satUpper	Upper saturation bound for HSV threshold filtering.
			@param valLower Lower brightness bound for HSV threshold filter.
			@param valUpper Upper brightness bound for HSV threshold filter.
			@param filterIterations Iterations to perform dilation and erosion.
			@param filterSigma Sigma parameter for Gaussian blur filter.
			@param houghDp
			@param houghThreshLower
			@param houghThreshUpper
			@param houghMinRadius
			@param houghMaxRadius
			@param contourMaxArea
			@param contourMinArea
			@param contourMaxObjects
		*/
        TrackingParameters(bool debug=false, int hueLower=DEFAULT_HUE_LOWER, int hueUpper=DEFAULT_HUE_UPPER,
            int satLower=DEFAULT_SAT_LOWER, int satUpper=DEFAULT_SAT_UPPER,
            int valLower=DEFAULT_VAL_LOWER, int valUpper=DEFAULT_VAL_UPPER,
            int filterIterations=DEFAULT_FILTER_ITERATIONS, int filterSigma=DEFAULT_FILTER_SIGMA,
            int houghDp=DEFAULT_HOUGH_DP, int houghMinDist=DEFAULT_HOUGH_MIN_DIST, 
			int houghThreshLower=DEFAULT_HOUGH_THRESH_LOWER, int houghThreshUpper=DEFAULT_HOUGH_THRESH_UPPER, 
            int houghMinRadius=DEFAULT_HOUGH_MIN_RADIUS, int houghMaxRadius=DEFAULT_HOUGH_MAX_RADIUS,
			int contourMaxArea=DEFAULT_CONTOUR_MAX_AREA, int contourMinArea=DEFAULT_CONTOUR_MIN_AREA,
			int contourMaxObjects=DEFAULT_CONTOUR_MAX_OBJECTS);
        void changeParameter(int value, std::string name);
        void printParameters();
        int getParameter(std::string name);
        bool hasParameter(std::string name);
        bool wasChanged();
        bool debug;
    private:
        std::map<std::string, float> parameters;
        bool parameterChanged;
};


#endif