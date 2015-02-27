import numpy as np
import cv2
import os
from functools import partial
import collections
import copy

DEBUG = True
DEBUG_VERBOSE = True


"""
Static Video capture type. Can be a camera, a video, or an image.
"""
class CaptureType:
	Camera, Video, Image = range(3)

	@staticmethod
	def toString(type):
		if type == CaptureType.Camera:
			return 'Camera'
		elif type == CaptureType.Video:
			return 'Video'
		elif type == CaptureType.Image:
			return 'Image'
		else:
			return 'Unknown'


"""
Tracking parameters for BallTracker.
"""
class TrackingParameters:
	def __init__(self, hueLower=27, hueUpper=42, satLower=58, satUpper=256,
		valLower=44, valUpper=256, filterIterations=3, filterSigma=2,
		houghDp=1, houghThreshUpper=114, houghThreshLower=16, 
		houghMinRadius=3, houghMaxRadius=140):
		# Parameters
		self.HueLower = hueLower
		self.HueUpper = hueUpper
		self.SatLower = satLower
		self.SatUpper = satUpper
		self.ValLower = valLower
		self.ValUpper = valUpper
		self.FilterIterations = filterIterations
		self.FilterSigma = filterSigma
		self.HoughDP = houghDp
		self.HoughThreshUpper = houghThreshUpper
		self.HoughThreshLower = houghThreshLower
		self.HoughMinRadius = houghMinRadius
		self.HoughMaxRadius = houghMaxRadius
		# Other attributes
		self.ParameterChanged = False

	"""
	Event called when parameter is changed.
	"""
	def changeParameter(self,val, parameter):
		setattr(self,parameter,val)
		self.ParameterChanged = True
		if (DEBUG and DEBUG_VERBOSE):
			print parameter, ': ', val


	"""
	Prints all parameters.
	"""
	def printParameters(self):
		print "Tracking parameters:"
		paramDict = vars(self)
		oParamDict = collections.OrderedDict(sorted(paramDict.items()))
		houghParamDict = collections.OrderedDict()

		#print paramDict
		# Print 
		for k, v in oParamDict.items():
			if k.startswith('Hough'):
				houghParamDict[k] = v
			else:
				print "\t{}: {}".format(k,v)
		# Hough params
		for k, v in houghParamDict.items():
			print "\t{}: {}".format(k,v)

"""
Opens a camera, video, or image file and tracks the ball.
"""
class BallTracker:
	"""
	Constructor initializes BallTracker, parameters, and opens the file/device.
	"""
	def __init__(self, deviceOrFile=0, trackBall=True, logFile=None, camWidth=640, camHeight=480, pauseVideoFrames=False):
		self.TrackingEnabled = trackBall
		self.IsRunning = False
		self.InterfaceInitialized = False
		self.VideoWaits = pauseVideoFrames
		# Check whether device or file was given
		if (type(deviceOrFile) is str):
			if (deviceOrFile.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp', '.gif',
				'.dib','.jp2', '.pbm', '.pgm', '.ppm', '.tiff', '.tif'))):
				self.CaptureType = CaptureType.Image
			elif (deviceOrFile.lower().endswith(('.avi','.mov','.mpeg'))):
				self.CaptureType = CaptureType.Video
			else:
				raise Exception('Unknown file extension for file: {}'.format(deviceOrFile))
			self.Input = os.path.abspath(deviceOrFile)
			if (not os.path.isfile(self.Input)):
				raise Exception('Could not find input file: {}'.format(self.Input))
		elif (type(deviceOrFile) in (int, float)):
			self.Input = deviceOrFile
			self.CaptureType = CaptureType.Camera
		else:
			raise 'Unknown input deviceOrFile given'

		# Open input
		self.Cap = cv2.VideoCapture(self.Input)
		if (not self.Cap.isOpened()):
			raise Exception('Could not open capture ({}): {}'.format(
				CaptureType.toString(self.CaptureType), self.Input))
		# Get/set capture information
		if (self.usingCamera()):
			# Set width and height for camera
			self.Cap.set(cv2.CAP_PROP_FRAME_WIDTH,camWidth);
			self.Cap.set(cv2.CAP_PROP_FRAME_HEIGHT,camHeight);
			# Printing info for debug mode
			if (DEBUG):
				print 'Camera: device ', self.Input
				print('Video properties:\n\tWidth: {}px\n\tHeight: {}px'.format(camWidth, camHeight))
				print '\tMax Width: {} px\n\tMax Height: {}px'.format(
					int(self.Cap.get(cv2.CAP_PROP_FRAME_WIDTH)),
					int(self.Cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
				print '\tMode: ', self.Cap.get(cv2.CAP_PROP_MODE)
				print '\tBrightness: ', self.Cap.get(cv2.CAP_PROP_BRIGHTNESS)
				print '\tContrast: ', self.Cap.get(cv2.CAP_PROP_CONTRAST)
				print '\tSatur: ', self.Cap.get(cv2.CAP_PROP_SATURATION)
				print '\tHue: ', self.Cap.get(cv2.CAP_PROP_HUE)
				print '\tGain: ', self.Cap.get(cv2.CAP_PROP_HUE)
				print '\tExposure: ', self.Cap.get(cv2.CAP_PROP_EXPOSURE)
		else:
			# Find width and height for video/image
			typeName = CaptureType.toString(self.CaptureType)
			self.Width = int(self.Cap.get(cv2.CAP_PROP_FRAME_WIDTH))
			self.Height = int(self.Cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
			# Get FPS and frames for video
			if (self.usingVideo()):
				self.FPS = self.Cap.get(cv2.CAP_PROP_FPS)
				self.FrameCount = int(self.Cap.get(cv2.CAP_PROP_FRAME_COUNT))
				self.Length = self.FrameCount / self.FPS
			# Printing debug info
			if (DEBUG):
				print '{} file: {}', typeName, self.Input
				print('{} properties:\n\tWidth: {}px\n\tHeight: {}px'.format(
					typeName, self.Width, self. Height))
				if (self.usingVideo()):
					print '\tFPS: ', self.FPS
					print '\tFrames: ', self.FrameCount
					print '\tLength: ', self.Length

		# Create parameter class
		self.TrackingParameters = TrackingParameters()
		if (DEBUG):
			self.TrackingParameters.printParameters()
			print "Ball tracker initialized."
	# end of def __init__()

	"""
	Returns True if the BallTracker is using a video file.
	"""
	def usingVideo(self):
		return self.CaptureType == CaptureType.Video

	"""
	Returns True if the BallTracker is using an image file.
	"""
	def usingImage(self):
		return self.CaptureType == CaptureType.Image

	"""
	Returns True if the BallTracker is using a camera.
	"""
	def usingCamera(self):
		return self.CaptureType == CaptureType.Camera


	"""
	Initializes BallTracker interface with video windows and sliders for setting parameters.
	"""
	def initializeInterface(self):
		if (self.InterfaceInitialized):
			return

		# Create windows
		cv2.namedWindow('Camera', cv2.WINDOW_AUTOSIZE)
		cv2.namedWindow('Thresholded', cv2.WINDOW_AUTOSIZE)

		# Thresholded trackbars
		cv2.createTrackbar('HueLower','Thresholded',self.TrackingParameters.HueLower,
			255,partial(self.TrackingParameters.changeParameter, parameter='HueLower'))
		cv2.createTrackbar('HueUpper','Thresholded',self.TrackingParameters.HueUpper,
			255,partial(self.TrackingParameters.changeParameter, parameter='HueUpper'))
		cv2.createTrackbar('SatLower','Thresholded',self.TrackingParameters.SatLower,
			255,partial(self.TrackingParameters.changeParameter, parameter='SatLower'))
		cv2.createTrackbar('ValLower','Thresholded',self.TrackingParameters.ValLower,
			255,partial(self.TrackingParameters.changeParameter, parameter='ValLower'))
		cv2.createTrackbar('FilterIterations','Thresholded',self.TrackingParameters.FilterIterations,
			32,partial(self.TrackingParameters.changeParameter, parameter='FilterIterations'))
		cv2.createTrackbar('FilterSigma','Thresholded',self.TrackingParameters.FilterSigma,
			32,partial(self.TrackingParameters.changeParameter, parameter='FilterSigma'))

		# Hough circle trackbars
		minRadiusSliderMax = min(self.Height,self.Width)
		maxRadiusSliderMax = max(self.Height,self.Width)
		cv2.createTrackbar('HoughDP','Camera',self.TrackingParameters.HoughDP,
			16,partial(self.TrackingParameters.changeParameter, parameter='HoughDP'))
		cv2.createTrackbar('HoughThreshUpper','Camera',self.TrackingParameters.HoughThreshUpper,
			500,partial(self.TrackingParameters.changeParameter, parameter='HoughThreshUpper'))
		cv2.createTrackbar('HoughThreshLower','Camera',self.TrackingParameters.HoughThreshLower,
			100,partial(self.TrackingParameters.changeParameter, parameter='HoughThreshLower'))
		cv2.createTrackbar('HoughMinRadius','Camera',self.TrackingParameters.HoughMinRadius,
			minRadiusSliderMax,partial(self.TrackingParameters.changeParameter, parameter='HoughMinRadius'))
		cv2.createTrackbar('HoughMaxRadius','Camera',self.TrackingParameters.HoughMaxRadius,
			maxRadiusSliderMax,partial(self.TrackingParameters.changeParameter, parameter='HoughMaxRadius'))
		self.InterfaceInitialized = True
	# end of def initializeInterface

	"""
	Runs the BallTracker.
	"""
	def run(self):
		if (self.Cap is None or not self.Cap.isOpened()):
			return
		if (not self.InterfaceInitialized):
			self.initializeInterface()
		self.IsRunning = True
		frame_i = 0
		wantNextFrame = True # only used if usingVideo() and VideoWaits
		heldFrame = False
		# Read frame only once for an image
		if (self.usingImage()):
			ret, frame = self.Cap.read()
			origFrame = frame
			self.Cap.release()

		# Main Loop
		while(self.IsRunning):
			if (not self.usingImage()):
				if (self.usingVideo() and self.VideoWaits and not wantNextFrame):
					heldFrame = True
				else:
					ret, frame = self.Cap.read()
					wantNextFrame = False
					heldFrame = False
			# Loop video captures if we're at the last frame
			if (self.usingVideo()):
				if (self.VideoWaits and heldFrame):
					pass
				else:
					frame_i += 1
					if frame_i == self.Cap.get(cv2.CAP_PROP_FRAME_COUNT):
						frame_i = 0
						self.Cap.release()
						self.Cap = cv2.VideoCapture(self.Input)

			threshFrame = self.thresholdFrame(frame)
			found, (x, y), radius = self.detectBall(threshFrame)

			# Draw the ball if it was found
			newFrame = copy.deepcopy(frame)
			if (found):
				self.drawBall(newFrame, (x,y), radius)
				if (DEBUG and DEBUG_VERBOSE and not heldFrame):
					print "Found ball at ({}, {}), with radius {}".format(x,y, radius)
			else:
				if (DEBUG and DEBUG_VERBOSE and not heldFrame):
					ending = '.'
					if (self.usingVideo()):
						ending = ' at frame {} of {}'.format(frame_i, self.FrameCount)
					print "Did not find ball", ending

			# Show the videos
			cv2.imshow('Camera',newFrame)
			cv2.imshow('Thresholded', threshFrame)

			# Make sure capture is still open
			if (not self.usingImage() and not self.Cap.isOpened()):
				break

			# Check for a key press
			key = cv2.waitKey(1) & 0xFF

			if (key == ord('q')):
				break
			if (key == ord('n')):
				wantNextFrame = True
			if (key == ord('p')):
				self.TrackingParameters.printParameters()
		# end of while(self.IsRunning)

		self.IsRunning = False

		if (self.Cap and self.Cap.isOpened()):
			self.Cap.release()
		cv2.destroyAllWindows()
	# end of def run()

	"""
	Draws the ball in the frame at position (x,y) with given radius.
	"""
	def drawBall(self, frame, (x,y), radius, innerRadius=3, innerCircleColor=(0,255,0),
		outerCircleColor=(0,0,255)):
		# Draw inner circle (small dot at center of ball)
		cv2.circle(frame, (x,y),
			radius=innerRadius,
			color=innerCircleColor,
			thickness=-1,
			lineType=cv2.LINE_8,
			shift=0)
		# Draw outer circle
		cv2.circle(frame, (x,y),
			radius=radius,
			color=outerCircleColor,
			thickness=1,
			lineType=cv2.LINE_8,
			shift=0)

	# end of def drawBall()

	"""
	Thresholds each frame and returns the thresholded frame.
	"""
	def thresholdFrame(self, frame):
		# Convert color space 
		hsvFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
		hsvRangeLower = (self.TrackingParameters.HueLower, self.TrackingParameters.SatLower,
			self.TrackingParameters.ValLower, 0)
		hsvRangeUpper = (self.TrackingParameters.HueUpper, self.TrackingParameters.SatUpper,
			self.TrackingParameters.ValUpper, 0)

		threshFrame = cv2.inRange(hsvFrame, lowerb=hsvRangeLower, upperb=hsvRangeUpper)
		# if (DEBUG and np.count_nonzero(threshFrame) > 0):
		# 	#print np.nonzero(threshFrame)
		# 	print "Found a non-zero threshFrame at: {}".format(cap.get(cv2.CAP_PROP_POS_MSEC))

		# Filtering
		threshFrame = cv2.dilate(threshFrame, kernel=None,
			iterations=self.TrackingParameters.FilterIterations)
		threshFrame = cv2.GaussianBlur(threshFrame, ksize=(0, 0),
			sigmaX=max(self.TrackingParameters.FilterSigma,1),
			sigmaY=max(self.TrackingParameters.FilterSigma,1))
		threshFrame = cv2.erode(threshFrame, kernel=None,
			iterations=self.TrackingParameters.FilterIterations)

		return threshFrame

	# end of def thresholdFrame()

	"""
	Processes the given thresholded frame and detects the ball.
	Returns: found, (x,y), radius
	"""
	def detectBall(self, threshFrame):
		# Find circles
		circles = cv2.HoughCircles(threshFrame,
			method=cv2.HOUGH_GRADIENT,
			dp=self.TrackingParameters.HoughDP,
			minDist=50,
			param1=max(self.TrackingParameters.HoughThreshUpper,1),
			param2=max(self.TrackingParameters.HoughThreshLower,1),
			minRadius=self.TrackingParameters.HoughMinRadius,
			maxRadius=self.TrackingParameters.HoughMaxRadius)

		# Detect ball by finding largest circle
		maxRadius = 0
		(x, y) = (0, 0)
		found = False
		if circles is not None:
			circles = circles[0,:,:] # strip off 3rd dim
			total = circles.shape[0] # count circles found
			#if (DEBUG):
			#	print "Found {} Hough circles".format(total)
			# Iterate over each and find the largest circle
			for i in range(0, total):
				c = np.transpose(circles[i,:])
				if (c[2] > maxRadius):
					maxRadius = c[2]
					x = c[0]
					y = c[1]
					found = True

		return found, (x,y), maxRadius

	# end of def detectBall()

# end of class BallTracker()

## *** Entry Point ***
if __name__ == "__main__":
    from optparse import OptionParser

    #parser = OptionParser("%prog [options] <inputVideoFileOrDevice>")
    #parser.add_option("-o", "--output", dest="output", default=None, help="output video")
    #(opts, args) = parser.parse_args()
    args = ['../../../video/tennis_ball2-cut.mov']
    if len(args) < 1:
    	parser.error("You must give an input video file.")

    # Create tracker and run it
    tracker = BallTracker(deviceOrFile=args[0], pauseVideoFrames=True)
    tracker.run()
