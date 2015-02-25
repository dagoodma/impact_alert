# optical_flow1.py
#
#	Adds optical flow vectors to the given video, and saves	
#
import numpy as np
import cv2
# opencv 2.4.9
# import cv2.cv as cv
import os
import sys
import pprint
pp = pprint.PrettyPrinter(indent=4)

DEBUG = True
# Optical flow options
win_size = (16, 16)

print sys.version_info


def doOpticalFlow(videoIn, videoOut):

	# Find input video
	videoIn = os.path.abspath(videoIn)
	if not os.path.isfile(videoIn):
		raise 'Failed to find input video: {}'.format(videoIn)
	if DEBUG:
		print 'Input video: {}'.format(videoIn)

	# Make sure we can write to output video
	videoOut = os.path.abspath(videoOut)
	if os.path.isfile(videoOut):
		raise 'Output video already exists: {}'.format(videoOut)
	if DEBUG:
		print 'Output video: {}'.format(videoOut)


	# Open and read input 
	error = None
	try:
		vi = cv2.VideoCapture(videoIn)

		# Video statistics (OpenCV 3.0-beta)
		width = int(vi.get(cv2.CAP_PROP_FRAME_WIDTH))
		height = int(vi.get(cv2.CAP_PROP_FRAME_HEIGHT))
		fps = vi.get(cv2.CAP_PROP_FPS)
		frames = vi.get(cv2.CAP_PROP_FRAME_COUNT)

		# openCV 2.4.9
		#width = int(vi.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH))
		#height = int(vi.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT))
		#fps = vi.get(cv2.cv.CV_CAP_PROP_FPS)
		#frames = vi.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT)

		length = frames / fps
		if DEBUG:
			print('Video properties:\n\tWidth: {}px\n\tHeight: {}px\n\tFPS: {}\n\tLength: {}s'.format(
				width, height, fps, length))

		prevFrameGray = None
		# OpenCV 3.0-beta
		velX = np.zeros((height,width), dtype=np.float32) #cv2.Matx(height, width, cv2.CV_32FC1)
		velY = np.zeros((height,width), dtype=np.float32) #cv2.Matx(height, width, cv2.CV_32FC1) 
		# OpenCV 2.4.9
		#velX = cv2.createMat(height, width, cv2.CV_32FC1)
		#velY = cv2.createMat(height, width, cv2.CV_32FC1) 

		while(vi.isOpened()):
			ret, frame = vi.read()
			if not ret:
				break
			frameGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)


			cv2.imshow('frame',frameGray)
			if cv2.waitKey(1) & 0xFF == ord('q'):
				break

			# Do optical flow
			if type(prevFrameGray) != type(None):
				pass
				# opencv 2.4.9
				#cv.CalcOpticalFlowLK(cv.fromarray(prevFrameGray), cv.fromarray(frameGray), win_size, velX, velY)
			else:
				if (DEBUG):
					print "Frame details:"
					dim = frame.shape
					print "\tdim:", dim
					print "\tM:{} x N:{}, type:{},{}".format(dim[0],dim[1],type(frame),frame.dtype)

			prevFrameGray = frameGray

	except Exception as e:
		error = e

	vi.release()
	cv2.destroyAllWindows()

	if error: 
		raise error


## *** Entry Point ***
if __name__ == "__main__":
    from optparse import OptionParser

    parser = OptionParser("%prog [options] <inputVideoFile> <outputVideoFile")
    #parser.add_option("-o", "--output", dest="output", default=None, help="output video")
    (opts, args) = parser.parse_args()
    args = ['../video/tennis_ball2.mov', 'soccer_ball4_orig_test.mov']
    if len(args) < 1:
    	parser.error("You must give an input video file.")
    if len(args) < 2:
    	parser.error("You must give an output video file.")

    doOpticalFlow(args[0], args[1])