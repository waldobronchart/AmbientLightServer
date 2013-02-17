#include "Color.h"
#include "MathUtils.h"

Color lerpColor(const Color& c1, const Color& c2, float term)
{
	Color result;
	result.R = (char)lerp(c1.R/255.0f, c2.R/255.0f, term);
	result.G = (char)lerp(c1.G/255.0f, c2.G/255.0f, term);
	result.B = (char)lerp(c1.B/255.0f, c2.B/255.0f, term);
	return result;
}