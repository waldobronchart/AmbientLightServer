#pragma once

#include <stdint.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Logging.h"
#include "MathUtils.h"
#include "TrapezoidSampler.h"
#include "HardwareConfig.h"

#include <boost/timer/timer.hpp>
using boost::timer::cpu_timer;

// todo: ability to disconnect and reconnect camera
class CameraController
{
public:
	static CameraController* Instance;

	CameraController()
		: m_capture(0), m_frame(0)
	{
		m_capture = cvCaptureFromCAM(CV_CAP_ANY);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_FPS, CAM_CAPTURE_FPS);

		cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH, CAM_CAPTURE_SIZE_WIDTH);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT, CAM_CAPTURE_SIZE_HEIGHT);

		Instance = this;
	}

	~CameraController()
	{
		cvReleaseCapture(&m_capture);
	}

	void CaptureFrame()
	{
		if (m_capture == 0)
			return;

		m_frame = cvQueryFrame(m_capture);
		if (!m_frame)
			return;
	}

	const IplImage* Frame()
	{
		return m_frame;
	}

	void UpdateSettings(uint8_t saturation, uint8_t brightness, uint8_t contrast, uint8_t gain)
	{
		if (m_capture == 0)
			return;

		double cvSaturation = (double)saturation;
		double cvBrightness = (double)brightness;
		double cvContrast = (double)contrast;
		double cvGain = (double)gain;

		// Gotcha! Raspberry pi expects values between 0 and 1
		#if RASPBERRY_PI
		cvSaturation /= 255;
		cvBrightness /= 255;
		cvContrast /= 255;
		cvGain /= 255;
		#endif
		
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_SATURATION, cvSaturation);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_BRIGHTNESS, cvBrightness);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_CONTRAST, cvContrast);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_GAIN, cvGain);

		// exposure doesn't work for me
		//cvSetCaptureProperty(m_capture, CV_CAP_PROP_EXPOSURE, -4);
	}

private:
	CvCapture* m_capture;
	IplImage* m_frame;
};