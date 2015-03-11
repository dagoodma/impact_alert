#define _USE_MATH_DEFINES // to get M_PI
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <cmath>
#include <functional>
#include "BallTracker.hpp"
#include "Utility.hpp"

// Intialize CaptureType struct names
const char* CaptureType::names[] = {"camera", "video", "image"};

// List of extensions for image files
const std::string imageFileExtensions[] = {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".dib",
    ".jp2", ".pbm", ".pgm", ".ppm", ".tiff", ".tif"};
std::vector<std::string> imageFileExtensionList(imageFileExtensions, end(imageFileExtensions));

//-------------------------------- Member functions -----------------------------
BallTracker::BallTracker(int deviceNum, bool debug, bool debugVerbose, bool trackBall, int camWidth,
    int camHeight, TrackingParameters *trackingParameters) {
	isRunning = false;
	interfaceIsInitialized = false;
	videoIsPaused = false;
	foundBall = false;
	ballCenter = cv::Point2i(0,0);
	ballRadius = 0;
    this->deviceNum = deviceNum;
    trackingEnabled = trackBall;
    captureProperties["width"] = (float)camWidth;
    captureProperties["height"] = (float)camHeight;
    this->trackingParameters = trackingParameters;
    this->trackingParameters->debug = debugVerbose;
    captureType = CaptureType::camera;
    this->debug = debug;
    this->debugVerbose = debugVerbose;

    // Open the capture
    cap = cv::VideoCapture(this->deviceNum);
    if (!cap.isOpened())
        throw std::runtime_error("failed to capture from device");

    // Set capture properties
    cap.set(cv::CAP_PROP_FRAME_WIDTH, camWidth);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, camHeight);

    // Debug information
    if (debug) {
        std::cout << "Camera: device " << this->deviceNum << std::endl;
        std::cout << "Properties:" << std::endl << "\tWidth: " << captureProperties["width"] << std::endl
			<< "\tHeight: "<< captureProperties["height"] << std::endl;
        std::cout << "\tMax Width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
        std::cout << "\tMax Height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
        /*
		std::cout << "\tMode: " << cap.get(cv::CAP_PROP_MODE) << std::endl;
        std::cout << "\tBrightness: " << cap.get(cv::CAP_PROP_BRIGHTNESS) << std::endl;
        std::cout << "\tContrast: " << cap.get(cv::CAP_PROP_CONTRAST) << std::endl;
        std::cout << "\tSaturation: " << cap.get(cv::CAP_PROP_SATURATION) << std::endl;
        std::cout << "\tHue: " << cap.get(cv::CAP_PROP_HUE) << std::endl;
        std::cout << "\tGain: " << cap.get(cv::CAP_PROP_GAIN) << std::endl;
        std::cout << "\tExposure: " << cap.get(cv::CAP_PROP_EXPOSURE) << std::endl;
		*/

        std::cout << "Ball tracker initialized." << std::endl;
    }
}

BallTracker::BallTracker(std::string fileName, bool debug, bool debugVerbose, bool trackBall,
    bool startVideoPaused, TrackingParameters *trackingParameters) {
	isRunning = false;
	frameNumber = 1;
	interfaceIsInitialized = false;
	videoIsPaused = false;
	foundBall = false;
    this->fileName = fileName;
    trackingEnabled = trackBall;
    videoIsPaused = startVideoPaused;
    this->trackingParameters = trackingParameters;
    this->trackingParameters->debug = debugVerbose;
    captureType = CaptureType::video;
    this->debug = debug;
    this->debugVerbose = debugVerbose;

    // Check if we have an image file using the file extension
    std::string fileNameExt = fileName.substr(fileName.find_last_of(".") + 1);
    if (std::find(imageFileExtensionList.begin(), imageFileExtensionList.end(),
        fileNameExt) != imageFileExtensionList.end()) {
        captureType = CaptureType::image;
    }

    // Open the capture
    cap = cv::VideoCapture(this->fileName);
    if (!cap.isOpened())
		throw std::runtime_error("Failed to capture from file");

    // Get capture properties
    captureProperties["width"] = (float)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    captureProperties["height"] = (float)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    if (isUsingVideo()) {
        captureProperties["fps"] = (float)cap.get(cv::CAP_PROP_FPS);
        captureProperties["frameCount"] = (float)cap.get(cv::CAP_PROP_FRAME_COUNT);
        captureProperties["length"] = captureProperties["frameCount"] / captureProperties["fps"];
    }

    // Debug information
    if (debug) {
        std::cout << CaptureType::names[captureType] << " file: " << this->fileName << std::endl;
        std::cout << "Properties:" << std::endl
			<< "\tWidth: " << captureProperties["width"] << std::endl
			<< "\tHeight: " << captureProperties["height"] << std::endl;
        if (isUsingVideo()) {
            std::cout << "\tFPS: " << captureProperties["fps"] << std::endl;
            std::cout << "\tFrames: " << captureProperties["frameCount"] << std::endl;
            std::cout << "\tLength: " << captureProperties["length"] << std::endl;
        }
        std::cout << "Ball tracker initialized." << std::endl;
    }
}

BallTracker::~BallTracker() {
    if (cap.isOpened())
        cap.release();

    cv::destroyAllWindows();
}

bool BallTracker::isUsingVideo() {
    return captureType == CaptureType::video;
}

bool BallTracker::isUsingImage() {
    return captureType == CaptureType::image;
}

bool BallTracker::isUsingCamera() {
    return captureType == CaptureType::camera;
}

int BallTracker::getWidth() {
	return (int)captureProperties["width"];
}

int BallTracker::getHeight() {
	return (int)captureProperties["height"];
}


void BallTracker::run() {
	if (! cap.isOpened())
		return;

	if (! interfaceIsInitialized)
		initializeInterface();

	bool isRunning = true;
	bool wantNextFrame = true; // want a new frame
	bool haveNewFrame = false; // new frame read
	int result;
	cv::Mat frame(getHeight(),getWidth(), CV_8UC3), 
			frameFiltered(getHeight(),getWidth(), CV_8UC3), 
			frameBall(getHeight(), getWidth(), CV_8UC3);

	// Only load images once
	if(isUsingImage()) {
		result = cap.read(frame);
		haveNewFrame = true;
		cap.release();
	}
	
	backgroundSubtractor = cv::createBackgroundSubtractorMOG2();

	// Main loop
	while (isRunning) {
		// Read the frame if not using an single image
		if (!isUsingImage()) {
			if (videoIsPaused && !wantNextFrame) {
				// do nothing
			}
			else {
				result = cap.read(frame);
				wantNextFrame = false;
				haveNewFrame = true;
			}
		}
		frame.copyTo(frameBall);

		// Process frame by filtering and then detecting the ball.
		filterFrame(frame, frameFiltered);
		if (trackingEnabled) {
			foundBall = detectBall(frameFiltered, ballCenter, ballRadius);

			if (foundBall) {
				drawBall(frameBall,ballCenter,ballRadius);
			}
		}

		// Frame info shows in bottom corner of video.
		if (isUsingVideo()) {
			double textScale = 0.8f;
			int textThickness = 1;
			cv::Scalar textColor(255,160,160);
			char *pausedStr = "";
			if (videoIsPaused)
				pausedStr = " (paused)";
			sprintf(tempBuf, "Frame %u of %u%s.", frameNumber, (int)captureProperties["frameCount"], pausedStr);
			cv::putText(frameBall, std::string(tempBuf),cv::Point(0,(int)captureProperties["height"]-10),
				cv::FONT_HERSHEY_PLAIN, textScale, textColor, textThickness);
		}

		// Debug info prints into console
		if (debug && (trackingParameters->wasChanged() || haveNewFrame)) {
			tempBuf[0] = '.'; tempBuf[1] = '\0';

			if (isUsingVideo())
				sprintf(tempBuf, " at frame %u of %u.", frameNumber, (unsigned int)captureProperties["frameCount"]);

			if (foundBall)
				std::cout << "Found ball at " << ballCenter.x << ", " << ballCenter.y << " with radius "
					<< ballRadius << tempBuf << std::endl;
			else
				std::cout << "Did not find ball" << tempBuf << std::endl;
		}

		// Show the frames
		cv::imshow("Camera", frameBall);
		cv::imshow("Filtered", frameFiltered);

		// Get key presses
		int key = cv::waitKey(1) & 0xFF;
		if (key == 'q') // quits
			break;
		else if (key == 'n') // advances to next frame when paused
			wantNextFrame = true;
		else if (key == 'p')
			trackingParameters->printParameters(); // pause
		else if (key == 13) // enter - pauses a video or camera
			videoIsPaused = videoIsPaused ^ 1;

		// Advancing to next frame
		if (isUsingVideo() && !videoIsPaused)
			wantNextFrame = true;
		if (!isUsingImage() && wantNextFrame)
			frameNumber++;

		// Makes a video loop by releasing and opening a new capture.
		if (isUsingVideo() && frameNumber >= (int)captureProperties["frameCount"]) {
			frameNumber = 1;
			cap.release();
			cap = cv::VideoCapture(fileName);
		}
		haveNewFrame = false;
	} // while (isRunning())

	if (debug) {
		trackingParameters->printParameters();
		std::cout << "Ball tracker finished." << std::endl;
	}
}

void BallTracker::initializeInterface() {
	if (interfaceIsInitialized)
		return;

	// Create windows
    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Filtered", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Controls", cv::WINDOW_NORMAL);

	// Filter window trackbars
	createTrackbar("hueLower", "Controls", FILTER_HSV_MAX);
	createTrackbar("hueUpper", "Controls", FILTER_HSV_MAX);
	createTrackbar("satLower", "Controls", FILTER_HSV_MAX);
	createTrackbar("valLower", "Controls", FILTER_HSV_MAX);
	createTrackbar("filterBackgroundOn", "Controls", 1);
	createTrackbar("filterIterations", "Controls",  FILTER_ITERATIONS_MAX);
	createTrackbar("filterKernel", "Controls", FILTER_KERNEL_MAX); // rect = 0, ellipse = 1, cross = 2
	createTrackbar("filterKernelSize", "Controls", FILTER_KERNEL_SIZE_MAX);
	createTrackbar("filterBlurOn", "Controls", 1);
	createTrackbar("filterSigma", "Controls", FILTER_SIGMA_MAX);

	// Hough detection trackbars
	createTrackbar("houghOn", "Controls", 1);
	createTrackbar("houghThreshUpper", "Controls", HOUGH_THRESH_MAX);
	createTrackbar("houghThreshLower", "Controls", HOUGH_THRESH_MAX);
	createTrackbar("houghMinRadius", "Controls", HOUGH_RADIUS_MAX);
	createTrackbar("houghMaxRadius", "Controls", HOUGH_RADIUS_MAX);

	// Contour detection trackbars
	createTrackbar("contourMaxArea", "Controls", CONTOUR_MAX_AREA_MAX);
	createTrackbar("contourMinArea", "Controls", CONTOUR_MIN_AREA_MAX);
}

void BallTracker::createTrackbar(const char* parameterName, const char* windowName, int maxValue) {
	tempInt = trackingParameters->getParameter(parameterName);
	cv::createTrackbar(parameterName, windowName, &tempInt, maxValue, sliderCallback,
		createSliderCallbackData(trackingParameters, parameterName));
}

void BallTracker::filterFrame(cv::Mat &src, cv::Mat &dst) {
	// Filter with background subtraction
	cv::Mat frameGray(src.size(), src.type());
	cv::cvtColor(src,frameGray, cv::COLOR_BGR2BGRA);
	backgroundSubtractor->apply(frameGray,backgroundMask,0);

	// Filter with HSV thresholds
	cv::Mat frameHsv(src.size(), src.type());
	cv::cvtColor(src,frameHsv, cv::COLOR_BGR2HSV);
	cv::Scalar hsvRangeLower = cv::Scalar(trackingParameters->getParameter("hueLower"),
		trackingParameters->getParameter("satLower"),
		trackingParameters->getParameter("valLower"), 0);
	cv::Scalar hsvRangeUpper = cv::Scalar(trackingParameters->getParameter("hueUpper"),
		trackingParameters->getParameter("satUpper"),
		trackingParameters->getParameter("valUpper"), 0);

	cv::inRange(frameHsv, hsvRangeLower, hsvRangeUpper, dst);

	// Combine with background filtering if enabled
	if (trackingParameters->getParameter("filterBackgroundOn"))
		dst = dst & backgroundMask;

	// Filter image with dilation, guassian blur, and erosion
	int iterations = trackingParameters->getParameter("filterIterations");
	int kernelSize = trackingParameters->getParameter("filterKernelSize");

	
	// Gaussian blur if enabled
	if (trackingParameters->getParameter("filterBlurOn"))
		cv::GaussianBlur(dst, dst, cv::Size(0,0), std::max(trackingParameters->getParameter("filterSigma"),1),
			std::max(trackingParameters->getParameter("filterSigma"),1));
	
	// Dilate and Erode
	cv::Mat kernel = cv::getStructuringElement(trackingParameters->getParameter("filterKernel"), cv::Size(kernelSize,kernelSize));
	for (uint8_t i = 0; i < iterations; i++)
		cv::erode(dst, dst, kernel); 
	for (uint8_t i = 0; i < iterations; i++)
		cv::dilate(dst, dst, kernel); 

}

bool BallTracker::detectBall(cv::Mat &frame, cv::Point2i &center, int &radius) {
	bool found = false;
	radius = 0;
	center.x = 0;
	center.y = 0;

	if (trackingParameters->getParameter("houghOn")) {
		std::vector<cv::Vec3f> circles;
		HoughCircles(frame, circles, cv::HOUGH_GRADIENT, 1,
			trackingParameters->getParameter("houghMinDist"), std::max(trackingParameters->getParameter("houghThreshUpper"),1),
			std::max(trackingParameters->getParameter("houghThreshLower"),1), trackingParameters->getParameter("houghMinRadius"),
			trackingParameters->getParameter("houghMaxRadius"));

		// Find the largest circle
		float maxRadius = 0.0;
		float x = 0;
		float y = 0;

		for (size_t i = 0; i < circles.size(); i++) {
			x = circles[i][0];
			y = circles[i][1];
			if (circles[i][2] > maxRadius) {
				maxRadius = circles[i][2];
				found = true;
				center.x = int(x + 0.5f);
				center.y = int(y + 0.5f);
				radius = int(maxRadius + 0.5f);
			}
		}
	} // hough circle detection
	else {
		// Contour detection
		cv::Mat temp;
		frame.copyTo(temp);
		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;

		cv::findContours(temp,contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

		// Look for largest moment
		double maxArea = 0.0;
		float x = 0.0, y = 0.0;
		int numObjects = hierarchy.size();
		if (numObjects > DEFAULT_CONTOUR_MAX_OBJECTS) {
			if (debug)
				std::cout << "Contour detection found too many objects!" << std::endl;
		}
		else if (numObjects > 0) {
			// Use hierarchy to determine next 
			for (int i = 0; i >= 0; i = hierarchy[i][0]) {
			//for (int i = 0; i < numObjects; i++) {
				cv::Moments moment = cv::moments((cv::Mat)contours[i]);
				double objectArea = moment.m00;
				double objectRadius = sqrt(objectArea/M_PI);
				if (objectArea > trackingParameters->getParameter("contourMinArea")
					&& objectArea < trackingParameters->getParameter("contourMaxArea")
					&& objectArea > maxArea) {
						found = true;
						maxArea = objectArea;
						center.x = (int)(moment.m10/objectArea);
						center.y = (int)(moment.m01/objectArea);
						radius = (int)objectRadius;
				}

			}
		}
	} // contour detection

	return found;
}


void BallTracker::drawBall(cv::Mat &frame, cv::Point2i center, int outerRadius, int innerRadius,
	const cv::Scalar& innerColor, const cv::Scalar& outerColor, const cv::Scalar& crosshairColor) {
	// Draw circles
	cv::circle(frame,center,outerRadius,outerColor,1);

	// Draw crosshairs and inner dot if ball is large enough
	if (outerRadius > 7) { 
		cv::circle(frame,center,innerRadius,innerColor,-1); // draw inner dot
		int crosshairSize = (int)((float)outerRadius*2.2f);
		int crosshairThickness = 1;
		int crosshairOffset = crosshairSize/2;
		cv::line(frame,cv::Point(center.x - crosshairOffset,center.y),
			cv::Point(center.x + crosshairOffset,center.y),
			crosshairColor, crosshairThickness);
		cv::line(frame, cv::Point(center.x, center.y - crosshairOffset),
			cv::Point(center.x, center.y + crosshairOffset),
			crosshairColor, crosshairThickness);
	}
	// Add text next to ball: (x,y, radius px)
	char buf[250];
	double textScale = 0.8f;
	int textThickness = 1;
	cv::Scalar textColor(0,255,0);
	sprintf(buf,"(x: %u, y:%u), %u px",center.x, center.y, outerRadius);
	cv::putText(frame, std::string(buf), cv::Point(center.x + outerRadius, center.y + outerRadius),
		cv::FONT_HERSHEY_PLAIN, textScale, textColor, textThickness);
}

/*----------------------- Non-member Functions ------------------------*/
void sliderCallback(int val, void *userData) {
	SliderCallbackData data = (SliderCallbackData)userData;
	std::string parameterName(data->parameterName);
	data->obj->changeParameter(val, parameterName);
}

SliderCallbackData createSliderCallbackData(TrackingParameters *obj, const char *name) {
	SliderCallbackData data = new _sliderCallbackData();
	data->obj = obj;
	data->parameterName = name;
	return data;
}