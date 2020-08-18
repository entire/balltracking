from collections import deque
from imutils.video import VideoStream
import numpy as np
import imutils
import time
import cv2
import click

@click.command()
@click.option('--video', '-v', help='video file path')
@click.option('--buffer', '-b', type=int, default=64, help='max buffer size')
def track(video, buffer):
    lower_green = (16,0,174)
    upper_green = (93,249,251)
    pts = deque(maxlen=buffer)

    if not video:
        vs = VideoStream(src=0).start()
    else:
        vs = cv2.VideoCapture(video)
    
    time.sleep(2.0)

    # start looping
    while True:
        frame = vs.read()

        # convert to hsv
        frame = imutils.resize(frame, width=600)
        blurred = cv2.GaussianBlur(frame, (11,11),0)
        hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

        # mask for green
        mask1 = cv2.inRange(hsv, lower_green, upper_green)
        mask2 = cv2.erode(mask1, None, iterations=3)
        mask3 = cv2.dilate(mask2, None, iterations=2)

        contours1 = cv2.findContours(mask3.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        contours2 = imutils.grab_contours(contours1)
        center = None

        print(contours2)
        # if there was a contour
        if len(contours2) > 0:
            c = max(contours2, key=cv2.contourArea)
            ((x,y), radius) = cv2.minEnclosingCircle(c)
            moment = cv2.moments(c)
            center = (int(moment["m10"]/moment["m00"]), int(moment["m01"]/moment["m00"]))

            if radius > 10:
                cv2.circle(frame, (int(x), int(y)), int(radius), (0,255,255), 2)
                cv2.circle(frame, center, 5, (0,0,255), -1)

        pts.appendleft(center)

        # loop ocver and draw contrail
        for i in range(1, len(pts)):
            if pts[i-1] is None or pts[i] is None:
                continue
            
            thickness = int(np.sqrt(buffer/float(i++1)) * 2.5)
            cv2.line(frame, pts[i-1], pts[i], (0,0,255), thickness)

        # show the frame
        cv2.imshow("Frame", frame)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):
            break

    # clean up 
    if not video:
        vs.stop()
    else:
        vs.release()

    # close all windows
    cv2.destroyAllWindows()

def prepAndConvertToHSV(frame):
    return hsv

if __name__ == '__main__':
    track()