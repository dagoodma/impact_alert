import numpy as np
import cv2
import os

# Doesn't work in OpenCV 3.0-beta

#cap = cv2.VideoCapture(os.path.abspath('../video/soccer_ball4_orig.mov'))
cap = cv2.VideoCapture(os.path.abspath('../video/tennis_ball2.mov'))

fgbg = cv2.createBackgroundSubtractorMOG(200,5,0.7,0.1)
# params: int history=200, int nmixtures=5, double backgroundRatio=0.7, double noiseSigma=0 (automatic));

while(1):
    ret, frame = cap.read()

    fgmask = fgbg.apply(frame)

    cv2.imshow('frame',fgmask)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()