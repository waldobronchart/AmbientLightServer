#pragma once

#include <vector>
#include <opencv/cv.h>

#include "MathUtils.h"
#include "Color.h"
#include "HardwareConfig.h"

/// Single area for each LED
struct TrapezoidSampleArea
{
	TrapezoidSampleArea() {}
	TrapezoidSampleArea(int gridX, int gridY) : GridX(gridX), GridY(gridY) {}

	Vector2 Center()
	{
		return (TopLeft + TopRight + BottomRight + BottomLeft) / 4.0f;
	}
	
	int GridX, GridY;
	Vector2 TopLeft;
	Vector2 TopRight;
	Vector2 BottomRight;
	Vector2 BottomLeft;
};

class TrapezoidSampler
{
public:
	static TrapezoidSampler* Instance;

	TrapezoidSampler() : m_rows(NUM_LEDS_HORIZONTAL), m_cols(NUM_LEDS_VERTICAL) {
		Instance = this;
		SetSize(m_rows, m_cols);
	}
	~TrapezoidSampler() {}

	void SetSize(short rows, short cols);
	void UpdatePoints(Vector2 topLeft, Vector2 topRight, Vector2 bottomRight, Vector2 bottomLeft);
	Color* SampleFromImage(const IplImage* frame);
	int NumLeds() { return m_rows * 2 + m_cols * 2; }
	
private:
	void UpdateSamplerAreas();

	Vector2 m_topLeft;
	Vector2 m_topRight;
	Vector2 m_bottomRight;
	Vector2 m_bottomLeft;

	short m_rows;
	short m_cols;

	std::vector<TrapezoidSampleArea> m_sampleAreas;
};