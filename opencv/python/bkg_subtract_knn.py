import numpy as np
import cv2
import os

#cap = cv2.VideoCapture(os.path.abspath('../video/soccer_ball4_orig.mov'))
cap = cv2.VideoCapture(os.path.abspath('../video/tennis_ball2.mov'))

fgbg = cv2.createBackgroundSubtractorKNN() #history=1, varThreshold=16, bShadowDetection=False 

while(1):
    ret, frame = cap.read()

    fgmask = fgbg.apply(frame)

    cv2.imshow('frame',fgmask)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()