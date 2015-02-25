import numpy as np
import cv2
import os
from functools import partial

DEBUG = True

# Parameters
parameters = {	'hue_lower': 60,
				'hue_upper': 75,
				'sat_lower': 100,
				'sat_upper': 256,
				'val_lower': 127,
				'val_upper': 256,
				'filter_iterations': 3,
				'filter_sigma': 9,
				'hough_dp': 1,
				'hough_threshupper': 400,
				'hough_threshlower': 10,
				'hough_minradius': 20,
				'hough_maxradius': 400}


def sliderCallback(val, event):
	print event, ': ', val


def runBallDetection(videoFile):
	# Find input video
	videoFile = os.path.abspath(videoFile)
	if not os.path.isfile(videoFile):
		raise 'Failed to find input video: {}'.format(videoFile)
	if DEBUG:
		print 'Input video: {}'.format(videoFile)

	# Initialization

	# Open video
	cap = cv2.VideoCapture(videoFile)
	error = None
	try:
		# Video statistics (OpenCV 3.0-beta)
		width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
		height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
		fps = cap.get(cv2.CAP_PROP_FPS)
		frames = cap.get(cv2.CAP_PROP_FRAME_COUNT)
		length = frames / fps

		# Create windows
		cv2.namedWindow('Camera', cv2.WINDOW_AUTOSIZE)
		cv2.namedWindow('HSV', cv2.WINDOW_AUTOSIZE)
		cv2.namedWindow('Thresholded', cv2.WINDOW_AUTOSIZE)

		# Thresholded trackbars
		cv2.createTrackbar('hue_lower','Thresholded',parameters['hue_lower'],255,partial(sliderCallback, event='hue_lower'))
		cv2.createTrackbar('hue_upper','Thresholded',parameters['hue_upper'],255,partial(sliderCallback, event='hue_upper'))
		cv2.createTrackbar('sat_lower','Thresholded',parameters['sat_lower'],255,partial(sliderCallback, event='sat_lower'))
		cv2.createTrackbar('val_lower','Thresholded',parameters['val_lower'],255,partial(sliderCallback, event='val_lower'))
		cv2.createTrackbar('filter_iterations','Thresholded',parameters['filter_iterations'],255,partial(sliderCallback, event='filter_iterations'))
		cv2.createTrackbar('filter_sigma','Thresholded',parameters['filter_sigma'],255,partial(sliderCallback, event='filter_sigma'))

		# Hough circle trackbars
		cv2.createTrackbar('hough_dp','Camera',parameters['hough_dp'],16,partial(sliderCallback, event='hough_dp'))
		cv2.createTrackbar('hough_threshupper','Camera',parameters['hough_threshupper'],1000,partial(sliderCallback, event='hough_threshupper'))
		cv2.createTrackbar('hough_threshlower','Camera',parameters['hough_threshlower'],400,partial(sliderCallback, event='hough_threshlower'))
		cv2.createTrackbar('hough_minradius','Camera',parameters['hough_minradius'],min(height,width),partial(sliderCallback, event='hough_minradius'))
		cv2.createTrackbar('hough_maxradius','Camera',parameters['hough_maxradius'],max(height,width),partial(sliderCallback, event='hough_maxradius'))

		if DEBUG:
			print('Video properties:\n\tWidth: {}px\n\tHeight: {}px\n\tFPS: {}\n\tLength: {}s'.format(
				width, height, fps, length))

		# Main Loop
		frame_i = 0
		while(True): #cap.isOpened()):
			ret, frame = cap.read()
			frame_i += 1
			if frame_i == cap.get(cv2.CAP_PROP_FRAME_COUNT):
				frame_i = 0
				cap.release()
				cap = cv2.VideoCapture(videoFile)

			# Convert color space 
			hsvFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
			hsvRangeLower = (cv2.getTrackbarPos('hue_lower','Thresholded'),
							cv2.getTrackbarPos('sat_lower','Thresholded'),
							cv2.getTrackbarPos('val_lower','Thresholded'),
							0)
			hsvRangeUpper = (cv2.getTrackbarPos('hue_upper','Thresholded'),
							parameters['sat_upper'],
							parameters['val_upper'],
							0)
			#hsvRangeLower = (parameters['hue_lower'],parameters['sat_lower'],parameters['val_lower'],0)
			#hsvRangeUpper = (parameters['hue_upper'],parameters['sat_upper'],parameters['val_upper'],0)
			threshFrame = cv2.inRange(frame, lowerb=hsvRangeLower, upperb=hsvRangeUpper)
			# if (DEBUG and np.count_nonzero(threshFrame) > 0):
			# 	#print np.nonzero(threshFrame)
			# 	print "Found a non-zero threshFrame at: {}".format(cap.get(cv2.CAP_PROP_POS_MSEC))

			# Filtering
			threshFrame = cv2.dilate(threshFrame, kernel=None,
				iterations=cv2.getTrackbarPos('filter_iterations','Thresholded'))
			threshFrame = cv2.GaussianBlur(threshFrame, ksize=(0, 0),
				sigmaX=max(cv2.getTrackbarPos('filter_sigma','Thresholded'),1),
				sigmaY=max(cv2.getTrackbarPos('filter_sigma','Thresholded'),1))
			threshFrame = cv2.erode(threshFrame, kernel=None,
				iterations=cv2.getTrackbarPos('filter_iterations','Thresholded'))

			# Find circles
			circles = cv2.HoughCircles(threshFrame,
				method=cv2.HOUGH_GRADIENT,
				dp=cv2.getTrackbarPos('hough_dp','Camera'),
				minDist=50,
				param1=max(cv2.getTrackbarPos('hough_threshupper','Camera'),1),
				param2=max(cv2.getTrackbarPos('hough_threshlower','Camera'),1),
				minRadius=cv2.getTrackbarPos('hough_minradius','Camera'),
				maxRadius=cv2.getTrackbarPos('hough_maxradius','Camera'))

			# Detect ball by finding largest circle
			maxRadius = 0
			(x, y) = (0, 0)
			found = False
			if circles is not None:
				circles = circles[0,:,:]
				total = circles.shape[0]
				#if (DEBUG):
				#	print "Found {} Hough circles".format(total)
				for i in range(0, total):
					c = np.transpose(circles[i,:])
					if (c[2] > maxRadius):
						maxRadius = c[2]
						x = c[0]
						y = c[1]
						found = True

			# Draw the ball
			if (found):
				if (DEBUG):
					print "Found ball at ({}, {}), with radius {}".format(x,y, maxRadius)
				cv2.circle(frame, (x,y), radius=3, color=(0,255,0), thickness=-1, lineType=cv2.LINE_8, shift=0)
				cv2.circle(frame, (x,y), radius=maxRadius, color=(0,0,255), thickness=3, lineType=cv2.LINE_8, shift=0)

			# Show the videos
			cv2.imshow('Camera',frame)
			cv2.imshow('HSV',hsvFrame)
			cv2.imshow('Thresholded', threshFrame)
			if cv2.waitKey(1) & 0xFF == ord('q'):
				break

	except Exception as e:
		error = e

	cap.release()
	cv2.destroyAllWindows()

	if error: 
		raise



## *** Entry Point ***
if __name__ == "__main__":
    from optparse import OptionParser

    parser = OptionParser("%prog [options] <inputVideoFile>")
    #parser.add_option("-o", "--output", dest="output", default=None, help="output video")
    (opts, args) = parser.parse_args()
    args = ['../video/tennis_ball2-cut.mov']
    if len(args) < 1:
    	parser.error("You must give an input video file.")

    runBallDetection(args[0])