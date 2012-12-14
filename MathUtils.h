#pragma once
#include <math.h>
#include <jansson.h>

class Vector2
{
public:
	Vector2() : m_x(0), m_y(0) {}
	Vector2(float x, float y) : m_x(x), m_y(y) {}

	float X() const { return m_x; }
	float Y() const { return m_y; }
	void X(float x) { m_x = x; }
	void Y(float y) { m_y = y; }

	float Length() { return sqrt(m_x*m_x + m_y*m_y); }

	void Normalize()
	{
		float len = Length();
		m_x /= len;
		m_y /= len;
	}

	Vector2 operator +(const Vector2& v)
	{
		return Vector2(m_x + v.m_x, m_y + v.m_y);
	}

	Vector2 operator /(float f)
	{
		return Vector2(m_x / f, m_y / f);
	}

	void operator *=(const Vector2& v)
	{
		m_x *= v.m_x;
		m_y *= v.m_y;
	}

	Vector2 operator *(const Vector2& v)
	{
		return Vector2(m_x * v.m_x, m_y * v.m_y);
	}

	friend Vector2 lerp(const Vector2&, const Vector2&, float);
	friend Vector2 lerp4(const Vector2&, const Vector2&, const Vector2&, const Vector2&, float, float);

private:
	float m_x, m_y;
};



json_t* json_vector2(const Vector2& v);
bool json_is_vector2(const json_t *json);
Vector2 json_vector2_value(const json_t *json);


float min(float x1, float x2);
float max(float x1, float x2);
float lerp(float minVal, float maxVal, float term);
float clamp(float val, float minVal, float maxVal);
Vector2 lerp(const Vector2& minVal, const Vector2& maxVal, float term);
Vector2 lerp4(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomRight, const Vector2& bottomLeft, float termX, float termY);