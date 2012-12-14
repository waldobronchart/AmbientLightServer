#pragma once

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Logging.h"
#include "MathUtils.h"
#include "TrapezoidSampler.h"

#include <boost/timer/timer.hpp>
using boost::timer::cpu_timer;

class CameraController
{
public:
	static CameraController* Instance;

	CameraController()
		: m_capture(0), m_frame(0)
	{
		m_capture = cvCaptureFromCAM(CV_CAP_ANY);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_FPS, 30);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH, 160);
		cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT, 120);

		Instance = this;
	}

	~CameraController()
	{
		cvReleaseCapture(&m_capture);
	}

	void CaptureFrame()
	{
		if (!m_capture)
			return;

		m_frame = cvQueryFrame(m_capture);
		if (!m_frame)
			return;
	}

	const IplImage* Frame()
	{
		return m_frame;
	}

private:
	CvCapture* m_capture;
	IplImage* m_frame;
};