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

		/*#if RASPBERRY_PI
		saturation
		#endif*/
		
		/*cvSetCaptureProperty(m_capture, CV_CAP_PROP_SATURATION, saturation);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_BRIGHTNESS, brightness);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_CONTRAST, contrast);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_GAIN, gain);*/
		//cvSetCaptureProperty(m_capture, CV_CAP_PROP_EXPOSURE, -4);
	}

private:
	CvCapture* m_capture;
	IplImage* m_frame;
};