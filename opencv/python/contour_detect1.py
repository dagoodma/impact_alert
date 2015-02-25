import numpy as np
import cv2
import os


def imageInfo(img, name):
	print "Image (\'{}\') details:".format(name)
	dim = img.shape
	print "\tdim:", dim
	print "\tM:{} x N:{}, type:{},{}".format(dim[0],dim[1],type(img),img.dtype)


#cap = cv2.VideoCapture(os.path.abspath('../video/soccer_ball4_orig.mov'))
cap = cv2.VideoCapture(os.path.abspath('../video/tennis_ball2.mov'))

#fgbg = cv2.createBackgroundSubtractorMOG2(1000,16,True) #history=400, varThreshold=16, bShadowDetection=False 

while(1):
	ret, frame = cap.read()
	if (not ret):
		break

		
	# grayFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	# greenFrame = frame[:,:,1]
	# imageInfo(grayFrame,'Gray')
	# imageInfo(greenFrame,'Green')

	# imageInfo(frame,'Original')
	# #(height, width, channels) = frame.shape
	# #frameChannels = np.zeros((height,width,3), dtype=np.uint8)
	# #cv2.split(frame,frameChannels)
	# # Find contours
	# #newFrame[:,:,0] = np.zeros_like(newFrame[:,:,0])
	# #newFrame[:,:,2] = np.zeros_like(newFrame[:,:,0])

	# ret,thresh = cv2.threshold(grayFrame,127,255,0)
	# imageInfo(thresh,'Thresh')
	
	# #...
	# (img, contours,hierarchy) = cv2.findContours(thresh, 1, 2)
	# cv2.drawContours(frame,contours,-1,(0,255,0),3)
	# #if (type(newFrame) == type(None)):
	# #	newFrame = frame
	# #fgmask = fgbg.apply(frame)
	# # Show image
	# #print newFrame
	cv2.imshow('frame',thresh)
	k = cv2.waitKey(30) & 0xff
	if k == ord('q'):
	    break

cap.release()
cv2.destroyAllWindows()

