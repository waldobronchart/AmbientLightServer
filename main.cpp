#ifdef _WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <time.h>
#include <sys/timeb.h>

#include "Logging.h"
#include "Server.h"
#include "LEDController.h"
#include "CameraController.h"
#include "TrapezoidSampler.h"
#include "Preferences.h"
#include "Color.h"

using namespace boost;
using namespace std;
using boost::asio::ip::tcp;

LEDController* ledControl;
CameraController* camControl;
Preferences* prefs;
TrapezoidSampler* sampler;

using namespace boost::posix_time;

class UpdateControl
{
public:
	UpdateControl(asio::io_service& ioService) 
		: m_loopTimer(ioService, boost::posix_time::milliseconds(3)), m_colorBuffer(0)
	{
	}

	void UpdateLoop()
	{
		// Calculate deltatime
		ptime currentTime = microsec_clock::local_time();
		float deltaTime = (currentTime - m_prevTime).total_milliseconds() / 1000.0f;
		
		// Capture frame and sample
		float timeSinceLastCaptureMS = (currentTime - m_lastCaptureTime).total_milliseconds();
		if (timeSinceLastCaptureMS >= 32)
		{
			LOG_DEBUG("  CAPTURE FRAME" << timeSinceLastCaptureMS);
			
			camControl->CaptureFrame();
			m_colorBuffer = sampler->SampleFromImage(camControl->Frame());
			m_lastCaptureTime = currentTime;
		}

		// Update leds and cleanup
		LOG_DEBUG("             UPDATE LEDS" << deltaTime);

		ledControl->UpdateLeds(m_colorBuffer, deltaTime);
		if (m_colorBuffer != 0)
			delete [] m_colorBuffer;

		// Reset timer
		m_loopTimer.expires_at(m_loopTimer.expires_at() + boost::posix_time::milliseconds(4));
		m_loopTimer.async_wait(boost::bind(&UpdateControl::UpdateLoop, this));
		m_prevTime = currentTime;
	}

private:
	ptime m_prevTime;
	ptime m_lastCaptureTime;

	boost::asio::deadline_timer m_loopTimer;
	Color* m_colorBuffer;
};

int main(int argc, char* argv[])
{
	try
	{
		// Logging setup
		LogUtil::Setup(log4cplus::TRACE_LOG_LEVEL);

		// Setup controllers
		prefs = new Preferences();
		prefs->Load();

		// LED controller setup
		ledControl = new LEDController();

		// Camera controller setup
		camControl = new CameraController();

		// Get preferences
		Vector2 topLeft, topRight, bottomRight, bottomLeft;
		prefs->GetBounds(topLeft, topRight, bottomRight, bottomLeft);

		// Sampler setup
		// todo: setsize from preferences
		sampler = new TrapezoidSampler();
		sampler->UpdatePoints(topLeft, topRight, bottomRight, bottomLeft);

		// Port
		unsigned short port = 13555;
		if (argc == 2)
			port = (unsigned short)atoi(argv[1]);
		
		// At least one io service needed
		asio::io_service ioService;

		// Setup server
		LOG_INFO("Server: staring on port " << port);
		tcp::endpoint endpoint(tcp::v4(), port);
		Server server(ioService, endpoint);

		// UpdateControl updates the camera class 30 times a sec
		UpdateControl ctrl(ioService);
		ctrl.UpdateLoop();

		ioService.run();
	}
	catch (std::exception& e)
	{
		LOG_FATAL("Exception: " << e.what() << "\n");
		return 1;
	}
	
	return 0;
}