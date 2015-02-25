import cv2
import os
import numpy as np


#cap = cv2.VideoCapture(os.path.abspath('../video/soccer_ball4_orig.mov'))
cap = cv2.VideoCapture(os.path.abspath('../video/tennis_ball2.mov'))
frameNum = 900

ret = True
while (ret and cap.get(cv2.CAP_PROP_POS_MSEC) < frameNum):
	ret,frame = cap.read()

hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
dims = 3
#hbins = 30 # hue levels
#sbins = 32 # saturation levels
histSize = [4] * dims
#histSize = (hbins, sbins)
#hueRange = (0, 256)
hueRange = [0, 256] * dims
channels = list(xrange(dims))
#channels = (0, 1) # compute histogram from 0th and 1st channels
#mask = np.zeros(frame.shape, dtype=np.uint8)
print "Frame details: ", type(hsv), hsv.shape, hsv.dtype
hist = cv2.calcHist([hsv], channels=channels, mask=None, histSize=histSize, ranges=hueRange) 
#, hist, False) #, 2, histSize, ranges, true, false)

(minVal, maxVal, minLoc, maxLoc) = cv2.minMaxLoc(hist)

scale = 10
histImg = np.zeros((sbins*scale,hbins*scale,3), dtype=np.uint8)

print type(hist)
print hist.shape

for i in range(0,hbins):
	for j in range(0,sbins):
		pass

    # ret, frame = cap.read()

    # fgmask = fgbg.apply(frame)

    # cv2.imshow('frame',fgmask)
    # k = cv2.waitKey(30) & 0xff
    # if k == 27:
    #     break

cap.release()
cv2.destroyAllWindows()