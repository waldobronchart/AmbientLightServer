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
		: m_loopTimer(ioService, boost::posix_time::milliseconds(3)), m_colorBuffer(0),
		  m_debugCumulativeDeltaTime(0), m_debugFPS(0)
	{
	}

	void UpdateLoop()
	{
		ptime currentTime = microsec_clock::local_time();
		float deltaTime = (currentTime - m_prevTime).total_microseconds() / 1000000.0f; // to seconds
		if (deltaTime > 1) deltaTime = 0;

		Preferences* prefs = Preferences::Instance;
		if (prefs->FixedColorEnabled)
		{
			ledControl->UpdateLedsFixed(prefs->FixedColor, deltaTime);
		}
		else
		{
			// Capture frame and sample
			camControl->CaptureFrame();
			m_colorBuffer = sampler->SampleFromImage(camControl->Frame());
		
			// Update leds
			if (m_colorBuffer != 0)
			{
				LOG_TRACE("UpdateLeds deltaTime " << deltaTime << "s");
				ledControl->UpdateLeds(m_colorBuffer, deltaTime, (float)prefs->TotalFadeTimeMS);
				delete [] m_colorBuffer;
				m_colorBuffer = 0;
			}
		}

		// Reset timer
		m_loopTimer.expires_at(m_loopTimer.expires_at() + milliseconds(16));
		m_loopTimer.async_wait(boost::bind(&UpdateControl::UpdateLoop, this));
		m_prevTime = currentTime;

		// Debugging: output average deltaTime every second
		m_debugCumulativeDeltaTime += deltaTime;
		if (deltaTime > 0) m_debugFPS++;

		if (m_debugCumulativeDeltaTime >= 1)
		{
			float averageDeltaTimeMS = (m_debugCumulativeDeltaTime / m_debugFPS) * 1000.0f;
			LOG_DEBUG("Average deltaTime: " << averageDeltaTimeMS << "ms (" << m_debugFPS << " FPS)");

			m_debugCumulativeDeltaTime = 0;
			m_debugFPS = 0;
		}
	}

private:
	ptime m_prevTime;

	float m_debugCumulativeDeltaTime;
	int m_debugFPS;

	boost::asio::deadline_timer m_loopTimer;
	Color* m_colorBuffer;
};

int main(int argc, char* argv[])
{
	try
	{
		// Logging setup
		LogUtil::Setup(log4cplus::DEBUG_LOG_LEVEL);

		// Setup controllers
		prefs = new Preferences();
		prefs->Load();

		// LED controller setup
		ledControl = new LEDController();

		// Camera controller setup
		camControl = new CameraController();
		camControl->UpdateSettings(prefs->CamSaturation, prefs->CamBrightness, prefs->CamContrast, prefs->CamGain);

		// Sampler setup
		sampler = new TrapezoidSampler();
		sampler->UpdatePoints(prefs->BoundsTopLeft, prefs->BoundsTopRight, prefs->BoundsBottomRight, prefs->BoundsBottomLeft);

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