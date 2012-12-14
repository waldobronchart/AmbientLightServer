#pragma once

struct Color
{
	Color() : R(0), G(0), B(0) {}
	Color(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b) {}

	unsigned char R;
	unsigned char G;
	unsigned char B;
};