import cv2
#import cv2.cv as cv
import numpy as np
import os

#cap = cv2.VideoCapture(os.path.abspath('../video/soccer_ball4_orig.mov'))
cap = cv2.VideoCapture(os.path.abspath('../video/tennis_ball2.mov'))

ofParams = dict( pyrScale = 0.5,
                 levels = 3,
                 winSize = 15,
                 iterations = 3,
                 polyN = 5,
                 polySigma = 1.5,
                 flags = 0)

ret, frame1 = cap.read()
prvs = cv2.cvtColor(frame1,cv2.COLOR_BGR2GRAY)
hsv = np.zeros_like(frame1)
hsv[...,1] = 255

width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
while(1):
    ret, frame2 = cap.read()
    if not ret:
        break
    next = cv2.cvtColor(frame2,cv2.COLOR_BGR2GRAY)
    flow = cv2.calcOpticalFlowFarneback(prvs,next, None, 0.5, 3, 15, 3, 5, 1.2, 0)
    #flow = cv2.calcOpticalFlowFarneback(prvs,next, ofParams['pyrScale'],
    #    ofParams['levels'], ofParams['winSize'], ofParams['iterations'],
    #    ofParams['polyN'], ofParams['polySigma'], ofParams['flags'])

    mag, ang = cv2.cartToPolar(flow[...,0], flow[...,1])
    hsv[...,0] = ang*180/np.pi/2
    hsv[...,2] = cv2.normalize(mag,None,0,255,cv2.NORM_MINMAX)
    rgb = cv2.cvtColor(hsv,cv2.COLOR_HSV2BGR)

    cv2.imshow('frame2',rgb)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    #k = cv2.waitKey(30) & 0xff
    #if k == 27:
    #    break
    #elif k == ord('s'):
    #    cv2.imwrite('opticalfb.png',frame2)
    #    cv2.imwrite('opticalhsv.png',rgb)
    prvs = next

cap.release()
cv2.destroyAllWindows()