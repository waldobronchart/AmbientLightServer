#include "TrapezoidSampler.h"
#include "Logging.h"

TrapezoidSampler* TrapezoidSampler::Instance = 0;

#include <sstream>

void TrapezoidSampler::SetSize(short rows, short cols)
{
	LOG_INFO("TrapezoidSampler.SetSize: Rows = " << rows << " Cols = " << cols);

	m_rows = rows;
	m_cols = cols;

	// Create new list of sample areas
	m_sampleAreas.clear();
	m_sampleAreas.resize(rows*2 + cols*2);
	int i = 0;

	// Top row
	for (int x=0; x<rows; x++)
		m_sampleAreas[i++] = TrapezoidSampleArea(x, 0);

	// Right col
	for (int y=1; y<cols+1; y++)
		m_sampleAreas[i++] = TrapezoidSampleArea(rows-1, y);

	// Bottom row
	for (int x=rows-1; x>=0; x--)
		m_sampleAreas[i++] = TrapezoidSampleArea(x, cols+1);

	// Left col
	for (int y=cols-1; y>=0; y--)
		m_sampleAreas[i++] = TrapezoidSampleArea(0, y);
}

void TrapezoidSampler::UpdatePoints(Vector2 topLeft, Vector2 topRight, Vector2 bottomRight, Vector2 bottomLeft)
{
	LOG_INFO("TrapezoidSampler.UpdatePoints");

	m_topLeft = topLeft;
	m_topRight = topRight;
	m_bottomRight = bottomRight;
	m_bottomLeft = bottomLeft;

	UpdateSamplerAreas();
}

Color* TrapezoidSampler::SampleFromImage(const IplImage* frame)
{
	if (frame == 0)
		return 0;

	if (frame->imageData == 0)
		return 0;

	if (NumLeds() == 0)
	{
		LOG_WARN("TrapezoidSampler.SampleFromImage: NumLeds = 0");
		return 0;
	}

	Vector2 frameSize = Vector2((float)frame->width, (float)frame->height);
	char *frameBuffer = frame->imageData;
	Color *colorBuffer = new Color[NumLeds()];

	/*std::stringstream ss;
	ss << "SAMPLES: ";*/

	int led = 0;
	for (std::vector<TrapezoidSampleArea>::iterator i=m_sampleAreas.begin(); i!=m_sampleAreas.end(); ++i)
	{
		TrapezoidSampleArea area = *i;

		Vector2 samplePoint = area.Center() * frameSize;
		int pixelOffset = (int)(samplePoint.X()) +  ((int)(samplePoint.Y()) * frame->height);
		pixelOffset *= 3;

		// OpenCV is GBR
		Color c = Color(frameBuffer[pixelOffset+2], frameBuffer[pixelOffset+1], frameBuffer[pixelOffset]);
		
		/*ss << "L";
		ss << led;
		ss << "(";
		ss << (int)c.R;
		ss << ",";
		ss << (int)c.G;
		ss << ",";
		ss << (int)c.B;
		ss << ")";*/

		//c = Color(255, 0, 0);

		colorBuffer[led] = c;
		led++;
	}

	//LOG_DEBUG(ss.str().c_str());

	return colorBuffer;
}

void TrapezoidSampler::UpdateSamplerAreas()
{
	LOG_INFO("TrapezoidSampler.UpdateSamplerAreas");

	// Calculate lengths of sides
	float topLen = m_topLeft.Length();
	float rightLen = m_topRight.Length();
	float bottomLen = m_bottomRight.Length();
	float leftLen = m_bottomLeft.Length();

	// Calculate perspective distortion on each side based on lengths
	float topPersp = min(topLen/bottomLen, 1);
	float rightPersp = min(rightLen/leftLen, 1);
	float bottomPersp = min(bottomLen/topLen, 1);
	float leftPersp = min(leftLen/rightLen, 1);

	// Minimum and maximum perspective distortions
    float minSidePersp = min(leftPersp, rightPersp);
    float maxSidePersp = max(leftPersp, rightPersp);
    float minTopPersp = min(topPersp, bottomPersp);
    float maxBottomPersp = max(topPersp, bottomPersp);

	// GOOD! Now we can do some work!
	// Iterate over sample area list and update their regions
	for (std::vector<TrapezoidSampleArea>::iterator i=m_sampleAreas.begin(); i!=m_sampleAreas.end(); ++i)
	{
		TrapezoidSampleArea& area = *i;

		// NormPos is the position normalized to values between 0 and 1 in the Trapezoid
		Vector2 normPos, distortion;

		// TopLeft
		normPos = Vector2(area.GridX/(float)(m_rows), area.GridY/(float)(m_cols+2));
		distortion = Vector2(lerp(minSidePersp, maxSidePersp, normPos.X()), lerp(minTopPersp, maxBottomPersp, normPos.Y()));
		normPos *= distortion;
		area.TopLeft = lerp4(m_topLeft, m_topRight, m_bottomRight, m_bottomLeft, normPos.X(), normPos.Y());

		// TopRight
		normPos = Vector2((area.GridX+1)/(float)(m_rows), area.GridY/(float)(m_cols+2));
		distortion = Vector2(lerp(minSidePersp, maxSidePersp, normPos.X()), lerp(minTopPersp, maxBottomPersp, normPos.Y()));
		normPos *= distortion;
		area.TopRight = lerp4(m_topLeft, m_topRight, m_bottomRight, m_bottomLeft, normPos.X(), normPos.Y());

		// BottomRight
		normPos = Vector2((area.GridX+1)/(float)(m_rows), (area.GridY+1)/(float)(m_cols+2));
		distortion = Vector2(lerp(minSidePersp, maxSidePersp, normPos.X()), lerp(minTopPersp, maxBottomPersp, normPos.Y()));
		normPos *= distortion;
		area.BottomRight = lerp4(m_topLeft, m_topRight, m_bottomRight, m_bottomLeft, normPos.X(), normPos.Y());

		// BottomLeft
		normPos = Vector2(area.GridX/(float)(m_rows), (area.GridY+1)/(float)(m_cols+2));
		distortion = Vector2(lerp(minSidePersp, maxSidePersp, normPos.X()), lerp(minTopPersp, maxBottomPersp, normPos.Y()));
		normPos *= distortion;
		area.BottomLeft = lerp4(m_topLeft, m_topRight, m_bottomRight, m_bottomLeft, normPos.X(), normPos.Y());
	}
}