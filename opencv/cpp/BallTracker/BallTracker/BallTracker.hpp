#ifndef BALL_TRACKER_H
#define BALL_TRACKER_H

// Include standard libraries
#include <string>

// Include OpenCV libraries
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

// Include local libraries
#include "TrackingParameters.hpp"

// Default camera width and height
#define DEFAULT_CAM_WIDTH   640
#define DEFAULT_CAM_HEIGHT  480

// Filter methods
//#define USE_BACKGROUND_FILTER // Gaussian filter
#define USE_HSV_FILTER	  // HSV range filter
#define USE_MORPH_FILTER	// morphology (erosion and dilation) filter
//#define USE_BLUR_FILTER	      // Gaussian blur
//#define USE_HOUGH_DETECTION // uses HoughCircle detection, or contour detection if not

/**
	Slider callback data contains a pointer to the TrackingParameters instance,
	and the name of parameter that was changed.
 */
struct _sliderCallbackData {
	TrackingParameters *obj;
	const char *parameterName;
};
typedef _sliderCallbackData* SliderCallbackData;

/**
	Handler for slider/trackbar changed events.
	@param	val Value of the new slider value.
	@param	userData Instance of SliderCallbackData.
 */
void sliderCallback(int val, void *userData);


/**
	Constructor for Slider Callback Data struct.
	@param	obj Instance of TrackingParameters.
	@param	name Parameter name.
 */
SliderCallbackData createSliderCallbackData(TrackingParameters *obj, const char *name);

/**
	Video capture type: camera, video, or image. Includes an array
	for converting the Enum type to a char*.
 */
struct CaptureType { 
    enum Enum {camera, video, image};
    static const char *names[];
};

/**
	Class for a BallTracker object. Loads a video or camera feed, and processes and shows
	each frame with run().
 */
class BallTracker {
    public:		
		/**
			Constructor for a camera device.
			@param deviceNum	Number of camera device to open.
			@param debug		Controls printing of debug messages to the console.
			@param debug_verbose Extra debug info to the console.
			@param trackBall	Enables tracking the ball.
			@param camWidth		Width in pixels to set on camera.
			@param camHeight	Height in pixels to set on camera.
			@param trackingParameters	Storage for tracking parameters.
		 */
        BallTracker(int deviceNum=0, bool debug=false, bool debugVerbose=false,
            bool trackBall=true, int camWidth=DEFAULT_CAM_WIDTH,
            int camHeight=DEFAULT_CAM_HEIGHT,
            TrackingParameters *trackingParameters=new TrackingParameters());
		/**
			Constructor for a video or image file.
			@param fileName		File path to video or image to open.
			@param debug		Controls printing of debug messages to the console.
			@param debug_verbose Extra debug info to the console.
			@param trackBall	Enables tracking the ball.
			@param startVideoPaused Starts playback in paused mode.
			@param trackingParameters	Storage for tracking parameters.
		*/
        BallTracker(std::string fileName, bool debug=false, bool debugVerbose=false,
            bool trackBall=true, bool startVideoPaused=false,
            TrackingParameters *trackingParameters=new TrackingParameters());
		/**
			Destructor for BallTracker.
		*/
        ~BallTracker();
		/**
			Returns true if capturing frames of a video.
		*/
        bool isUsingVideo();
		/**
			Returns true if captured frame from an image.
		*/
        bool isUsingImage();
		/**
			Returns true if capturing frames from a camera.
		*/
        bool isUsingCamera();
		/**
			Returns the width of the capture in pixels.
		*/
		int getWidth();
		/**
			Returns the height of the capture in pixels.
		*/
		int getHeight();
		/**
			Runs the BallTracker. 
			
			Captures frame(s) and processes frames. After filtering each frame with filterFrame(),
			it will try to detect the ball with detectBall. Displays the ball and filtered frames.
			Waits for keys from the user:
				'q' - Quits the application.
				'enter' - Pauses a video.
				'n' - Steps through a paused video.
				'p' - Prints current tracking parameters to the console.
		*/
        void run();
    private:
        // Methods
        void initializeInterface();
        void drawBall(cv::Mat &frame, cv::Point2i center, int outerRadius,
            int innerRadius=3, const cv::Scalar& innerColor=cv::Scalar(0,255,0),
            const cv::Scalar& outerColor=cv::Scalar(0,0,255),
			const cv::Scalar& crosshairColor=cv::Scalar(255,160,160));
		/**
			Filters source and saves the result into the destination matrix.

			Filtering techniques are defined by the macro definitions: USE_BACKGROUND_FILTER,
			USE_HSV_FILTER, and USE_BLUR_FILTER. Defining these causes the source frame to
			be filtered by MOG2 background subtraction, HSV threshold range,
			and blurring respectively. USE_BLUR_FILTER cannot be used alone.
			@param src	Source frame to filter.
			@param dst	Destination frame to save filtered result into.
		*/
		void filterFrame(cv::Mat &src, cv::Mat &dst);
		/**
			Detects the ball using Hough Circles.
			@param frame	Frame to look for ball in.
			@param center	Pointer to save center pixel of ball to if found.
			@param radius	Pointer to save radius of detected ball.
			@return true if ball was detected.
		*/
        bool detectBall(cv::Mat &frame, cv::Point2i &center, int &radius);
		/**
			Wrapper for cv::createTrackbar using the _sliderCallbackData structure.
			@param parameterName Slider name and TrackingParameters name
			@param windowName Name of OpenCV window to attach to.
			@param maxValue Maximum value of the settable parameter.
			@todo add minimum value
		*/
		void createTrackbar(const char* parameterName, const char* windowName, int maxValue);
        // Attributes
		int tempInt;
		char tempBuf[250];
        bool debug;
        bool debugVerbose;
        bool trackingEnabled;
        bool isRunning;
		bool foundBall;
		cv::Point2i ballCenter;
		int ballRadius;
        bool interfaceIsInitialized;
        bool videoIsPaused;
        int deviceNum;
		unsigned int frameNumber;
        std::string fileName;
        cv::VideoCapture cap;
        CaptureType::Enum captureType;
        std::map<std::string, float> captureProperties;
        TrackingParameters *trackingParameters;
		cv::Mat erosionKernel;
		cv::Mat dilationKernel;
#ifdef USE_BACKGROUND_FILTER
		cv::Ptr<cv::BackgroundSubtractor> backgroundSubtractor;
		cv::Mat backgroundMask;
#endif
};

#endif