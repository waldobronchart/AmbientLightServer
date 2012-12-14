import cv
import cv2

cv.NamedWindow("w1", cv.CV_WINDOW_AUTOSIZE)
capture = cv.CaptureFromCAM(0)

cv.SetCaptureProperty(capture, cv.CV_CAP_PROP_FRAME_WIDTH, 640)
cv.SetCaptureProperty(capture, cv.CV_CAP_PROP_FRAME_HEIGHT, 480)

print cv2.__version__

while True:
    print 'FRAME! ',
    frame = cv.QueryFrame(capture)
    cv.ShowImage("w1", frame)
    c = cv.WaitKey(33)
    if c == 113:
        exit(0)

#2.4.9  library linux
#2.4.2  python win
#rev 4557