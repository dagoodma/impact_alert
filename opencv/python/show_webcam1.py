import numpy as np
import cv2
import os
from functools import partial

DEBUG = True

def runWebcamTest():
	# Find input video
	cap = cv2.VideoCapture(0)
	if (not cap.isOpened()):
		raise 'Failed to open webcam connection.'

	width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
	height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
	if DEBUG:
		print('Video properties:\n\tWidth: {}px\n\tHeight: {}px'.format(width, height))
		print '\tMode: ', cap.get(cv2.CAP_PROP_MODE)
		print '\tBrightness: ', cap.get(cv2.CAP_PROP_BRIGHTNESS)
		print '\tContrast: ', cap.get(cv2.CAP_PROP_CONTRAST)
		print '\tSatur: ', cap.get(cv2.CAP_PROP_SATURATION)
		print '\tHue: ', cap.get(cv2.CAP_PROP_HUE)
		print '\tGain: ', cap.get(cv2.CAP_PROP_HUE)
		print '\tExposure: ', cap.get(cv2.CAP_PROP_EXPOSURE)

	# Camera setup
	cap.set(cv2.CAP_PROP_FRAME_WIDTH,640);
	cap.set(cv2.CAP_PROP_FRAME_HEIGHT,480);
	width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
	height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

	# Read from webcam
	error = None
	try:
		# Video statistics (OpenCV 3.0-beta)
		if DEBUG:
			print('Video properties:\n\tWidth: {}px\n\tHeight: {}px'.format(width, height))


		# Create windows
		cv2.namedWindow('Camera', cv2.WINDOW_AUTOSIZE)
		cv2.namedWindow('HSV', cv2.WINDOW_AUTOSIZE)

		# Main Loop
		frame_i = 0
		while(cap.isOpened()):
			ret, frame = cap.read()
			if not ret:
				break

			# Convert color space 
			hsvFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

			# Show the videos
			cv2.imshow('Camera',frame)
			cv2.imshow('HSV',hsvFrame)

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
   
    runWebcamTest()