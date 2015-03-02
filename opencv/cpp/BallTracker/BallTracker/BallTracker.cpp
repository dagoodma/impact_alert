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
        std::cout << "\tMode: " << cap.get(cv::CAP_PROP_MODE) << std::endl;
        std::cout << "\tBrightness: " << cap.get(cv::CAP_PROP_BRIGHTNESS) << std::endl;
        std::cout << "\tContrast: " << cap.get(cv::CAP_PROP_CONTRAST) << std::endl;
        std::cout << "\tSaturation: " << cap.get(cv::CAP_PROP_SATURATION) << std::endl;
        std::cout << "\tHue: " << cap.get(cv::CAP_PROP_HUE) << std::endl;
        std::cout << "\tGain: " << cap.get(cv::CAP_PROP_GAIN) << std::endl;
        std::cout << "\tExposure: " << cap.get(cv::CAP_PROP_EXPOSURE) << std::endl;

        std::cout << "Ball tracker initialized." << std::endl;
    }
}

BallTracker::BallTracker(std::string fileName, bool debug, bool debugVerbose, bool trackBall,
    bool startVideoPaused, TrackingParameters *trackingParameters) {
	isRunning = false;
	frameNumber = 0;
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
	
#ifdef USE_BACKGROUND_FILTER
	backgroundSubtractor = cv::createBackgroundSubtractorMOG2();
#endif

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

		// Process frame
		filterFrame(&frame, &frameFiltered);
		if (trackingEnabled) {
			foundBall = detectBall(&frameFiltered, &ballCenter, &ballRadius);

			if (foundBall) {
				drawBall(&frameBall,ballCenter,ballRadius);
			}
		}

		// Frame info
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

		// Debug info
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
		if (key == 'q')
			break;
		else if (key == 'n')
			wantNextFrame = true;
		else if (key == 'p')
			trackingParameters->printParameters();
		else if (key == 13)
			videoIsPaused = videoIsPaused ^ 1;

		// Video advancing and looping
		if (isUsingVideo() && !videoIsPaused)
			wantNextFrame = true;
		if (isUsingVideo() && wantNextFrame)
			frameNumber++;
		if (frameNumber >= (int)captureProperties["frameCount"]) {
			frameNumber = 1;
			cap.release();
			cap = cv::VideoCapture(fileName);
		}
		haveNewFrame = false;
	}

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
	createTrackbar("hueLower", "Controls", DEFAULT_FILTER_HSV_MAX);
	createTrackbar("hueUpper", "Controls", DEFAULT_FILTER_HSV_MAX);
	createTrackbar("satLower", "Controls", DEFAULT_FILTER_HSV_MAX);
	createTrackbar("valLower", "Controls", DEFAULT_FILTER_HSV_MAX);
	createTrackbar("filterIterations", "Controls", DEFAULT_FILTER_ITERATIONS_MAX);
	createTrackbar("filterSigma", "Controls", DEFAULT_FILTER_SIGMA_MAX);

	// Frame window trackbars
	createTrackbar("houghDP", "Controls", DEFAULT_HOUGH_DP_MAX);
	createTrackbar("houghThreshUpper", "Controls", DEFAULT_HOUGH_THRESH_MAX);
	createTrackbar("houghThreshLower", "Controls", DEFAULT_HOUGH_THRESH_MAX);
	createTrackbar("houghMinRadius", "Controls", DEFAULT_HOUGH_RADIUS_MAX);
	createTrackbar("houghMaxRadius", "Controls", DEFAULT_HOUGH_RADIUS_MAX);
}

void BallTracker::createTrackbar(const char* parameterName, const char* windowName, int maxValue) {
	tempInt = trackingParameters->getParameter(parameterName);
	cv::createTrackbar(parameterName, windowName, &tempInt, maxValue, sliderCallback,
		createSliderCallbackData(trackingParameters, parameterName));
}

void BallTracker::filterFrame(cv::Mat *src, cv::Mat *dst) {
	// Filter with background subtraction
#ifdef USE_BACKGROUND_FILTER
	cv::Mat frameGray(src->size(), src->type());
	cv::cvtColor(*src,frameGray, cv::COLOR_BGR2BGRA);
	backgroundSubtractor->apply(frameGray,backgroundMask,0);
	*dst = backgroundMask;
#endif

	// Filter with HSV thresholds
#ifdef USE_HSV_FILTER
	cv::Mat frameHsv(src->size(), src->type());
	cv::cvtColor(*src,frameHsv, cv::COLOR_BGR2HSV);
	cv::Scalar hsvRangeLower = cv::Scalar(trackingParameters->getParameter("hueLower"),
		trackingParameters->getParameter("satLower"),
		trackingParameters->getParameter("valLower"), 0);
	cv::Scalar hsvRangeUpper = cv::Scalar(trackingParameters->getParameter("hueUpper"),
		trackingParameters->getParameter("satUpper"),
		trackingParameters->getParameter("valUpper"), 0);

#ifdef USE_BACKGROUND_FILTER // combine filter
	cv::Mat temp;
	cv::inRange(frameHsv, hsvRangeLower, hsvRangeUpper, temp);
	*dst = *dst & temp;
#else
	cv::inRange(frameHsv, hsvRangeLower, hsvRangeUpper, *dst);
#endif
#endif

	// Filter image with dilation, guassian blur, and erosion
#ifdef USE_BLUR_FILTER
	int iterations = trackingParameters->getParameter("filterIterations");
	for (uint8_t i = 0; i < iterations; i++)
		cv::dilate(*dst, *dst, cv::Mat()); 
	cv::GaussianBlur(*dst, *dst, cv::Size(0,0), std::max(trackingParameters->getParameter("filterSigma"),1),
		std::max(trackingParameters->getParameter("filterSigma"),1));
	for (uint8_t i = 0; i < iterations; i++)
		cv::erode(*dst, *dst, cv::Mat()); 
#endif
}

bool BallTracker::detectBall(cv::Mat *frame, cv::Point2i *center, int *radius) {
	std::vector<cv::Vec3f> circles;
	HoughCircles(*frame, circles, cv::HOUGH_GRADIENT, std::max(trackingParameters->getParameter("houghDP"),1),
		trackingParameters->getParameter("houghMinDist"), std::max(trackingParameters->getParameter("houghThreshUpper"),1),
		std::max(trackingParameters->getParameter("houghThreshLower"),1), trackingParameters->getParameter("houghMinRadius"),
		trackingParameters->getParameter("houghMaxRadius"));

	// Find the largest circle
	*radius = 0;
	center->x = 0;
	center->y = 0;
	bool found = false;
	float maxRadius = 0.0;
	float x = 0;
	float y = 0;

	for (size_t i = 0; i < circles.size(); i++) {
		x = circles[i][0];
		y = circles[i][1];
		if (circles[i][2] > maxRadius) {
			maxRadius = circles[i][2];
			found = true;
			center->x = int(x + 0.5f);
			center->y = int(y + 0.5f);
			*radius = int(maxRadius + 0.5f);
		}
	}
	return found;
}


void BallTracker::drawBall(cv::Mat *frame, cv::Point2i center, int outerRadius, int innerRadius,
	const cv::Scalar& innerColor, const cv::Scalar& outerColor, const cv::Scalar& crosshairColor) {
	// Draw circles
	cv::circle(*frame,center,innerRadius,innerColor,-1);
	cv::circle(*frame,center,outerRadius,outerColor,1);

	// Draw crosshairs
	int crosshairSize = (int)((float)outerRadius*2.2f);
	int crosshairThickness = 1;
	int crosshairOffset = crosshairSize/2;
	cv::line(*frame,cv::Point(center.x - crosshairOffset,center.y),
		cv::Point(center.x + crosshairOffset,center.y),
		crosshairColor, crosshairThickness);
	cv::line(*frame, cv::Point(center.x, center.y - crosshairOffset),
		cv::Point(center.x, center.y + crosshairOffset),
		crosshairColor, crosshairThickness);

	// Add text
	char buf[250];
	double textScale = 0.8f;
	int textThickness = 1;
	sprintf(buf,"(x: %u, y:%u), %u px",center.x, center.y, outerRadius);
	cv::putText(*frame, std::string(buf), cv::Point(center.x + outerRadius, center.y + outerRadius),
		cv::FONT_HERSHEY_PLAIN, textScale, crosshairColor, textThickness);
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