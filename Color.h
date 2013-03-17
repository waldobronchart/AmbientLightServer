#pragma once

#include <stdint.h>
#include <jansson.h>

struct Color
{
	Color() : R(0), G(0), B(0) {}
	Color(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b) {}

	uint8_t R;
	uint8_t G;
	uint8_t B;
};

Color lerpColor(const Color& c1, const Color& c2, float val);

json_t* json_color(const Color& c);
bool json_is_color(const json_t *json);
Color json_color_value(const json_t *json);