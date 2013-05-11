#pragma once

#include <stdint.h>
#include <jansson.h>
#include "MathUtils.h"

struct Color
{
	Color() : m_r(0), m_g(0), m_b(0) {}
	Color(float r, float g, float b) : m_r(clamp(0, 1, r)), m_g(clamp(0, 1, g)), m_b(clamp(0, 1, b)) {}
	
	void R(float r) { m_r = clamp(0, 1, r); }
	void G(float g) { m_g = clamp(0, 1, g); }
	void B(float b) { m_b = clamp(0, 1, b); }

	float R() const { return m_r; }
	float G() const { return m_g; }
	float B() const { return m_b; }

	uint8_t ByteR() const { return (uint8_t)(m_r * 255); }
	uint8_t ByteG() const { return (uint8_t)(m_g * 255); }
	uint8_t ByteB() const { return (uint8_t)(m_b * 255); }

private:
	float m_r;
	float m_g;
	float m_b;
};

Color lerpColor(const Color& c1, const Color& c2, float val);

json_t* json_color(const Color& c);
bool json_is_color(const json_t *json);
Color json_color_value(const json_t *json);