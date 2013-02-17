#ifdef _WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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

class UpdateControl
{
public:
	UpdateControl(asio::io_service& ioService) 
		: m_timer(ioService, boost::posix_time::milliseconds(33))
	{
	}

	void FrameUpdate33ms()
	{
		camControl->CaptureFrame();

		// Update leds
		Color* colorBuffer = sampler->SampleFromImage(camControl->Frame());
		ledControl->UpdateLeds(colorBuffer);

		// todo: you need to delete colorBuffer!

		// Reset timer
		m_timer.expires_at(m_timer.expires_at() + boost::posix_time::milliseconds(33));
		m_timer.async_wait(boost::bind(&UpdateControl::FrameUpdate33ms, this));
	}

private:
	boost::asio::deadline_timer m_timer;
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
		ctrl.FrameUpdate33ms();

		ioService.run();
	}
	catch (std::exception& e)
	{
		LOG_FATAL("Exception: " << e.what() << "\n");
		return 1;
	}
	
	return 0;
}