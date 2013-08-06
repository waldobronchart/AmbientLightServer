#pragma once

#include <stdint.h>

#include <opencv2/core/core.hpp>
using namespace cv;

#include "Logging.h"
#include "MathUtils.h"
#include "TrapezoidSampler.h"
#include "HardwareConfig.h"

#include <boost/timer/timer.hpp>
#include <boost/thread.hpp>
using boost::timer::cpu_timer;

// todo: ability to disconnect and reconnect camera
class CameraController
{
public:
	static CameraController* Instance;

	CameraController()
	{
		m_videoCapture.open(0);

		m_videoCapture.set(CV_CAP_PROP_FPS, CAM_CAPTURE_FPS);
		m_videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, CAM_CAPTURE_SIZE_WIDTH);
		m_videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_CAPTURE_SIZE_HEIGHT);

		boost::this_thread::sleep(boost::posix_time::milliseconds(2000));

		m_videoCapture.set(CV_CAP_PROP_EXPOSURE, -6); // range is [-8:-2] for Logitech C250

		Instance = this;
	}

	~CameraController()
	{
		m_videoCapture.release();
	}

	void CaptureFrame()
	{
		if (!m_videoCapture.isOpened())
		{
			m_videoCapture.open(0);
			return;
		}

		m_videoCapture >> m_image;
	}

	const Mat& Frame()
	{
		return m_image;
	}

	void UpdateSettings(uint8_t saturation, uint8_t brightness, uint8_t contrast, uint8_t gain)
	{
		if (!m_videoCapture.isOpened())
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

		m_videoCapture.set(CV_CAP_PROP_SATURATION, cvSaturation);
		m_videoCapture.set(CV_CAP_PROP_BRIGHTNESS, cvBrightness);
		m_videoCapture.set(CV_CAP_PROP_CONTRAST, cvContrast);
		m_videoCapture.set(CV_CAP_PROP_GAIN, cvGain);

		//m_videoCapture.set(CV_CAP_PROP_EXPOSURE, -6);
	}

private:
	VideoCapture m_videoCapture;
	Mat m_image;
};