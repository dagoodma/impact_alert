import numpy as np
import cv2
import os
# Doesn't work! Need opencv 3.0:
# http://stackoverflow.com/questions/21626619/python-opencv-attributeerror-module-object-has-no-attribute-createbackground

cap = cv2.VideoCapture(os.path.abspath('../video/tennis_ball2.mov'))

kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))
fgbg = cv2.createBackgroundSubtractorGMG()

while(1):
    ret, frame = cap.read()

    fgmask = fgbg.apply(frame)
    fgmask = cv2.morphologyEx(fgmask, cv2.MORPH_OPEN, kernel)

    cv2.imshow('frame',fgmask)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()