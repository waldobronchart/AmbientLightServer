#include "Color.h"
#include "MathUtils.h"

Color lerpColor(const Color& c1, const Color& c2, float term)
{
	Color result;
	result.R = (uint8_t)(lerp(c1.R/255.0f, c2.R/255.0f, term)*255);
	result.G = (uint8_t)(lerp(c1.G/255.0f, c2.G/255.0f, term)*255);
	result.B = (uint8_t)(lerp(c1.B/255.0f, c2.B/255.0f, term)*255);
	return result;
}

json_t* json_color(const Color& c)
{
	json_t* root = json_array();

	// R
	json_t* jsonR = json_integer(c.R);
	json_array_append(root, jsonR);
	json_decref(jsonR);

	// G
	json_t* jsonG = json_integer(c.G);
	json_array_append(root, jsonG);
	json_decref(jsonG);

	// B
	json_t* jsonB = json_integer(c.B);
	json_array_append(root, jsonB);
	json_decref(jsonB);

	return root;
}

bool json_is_color(const json_t *root)
{
	if (!json_is_array(root))
		return false;

	if (json_array_size(root) != 3)
		return false;

	for (int i=0; i<3; ++i)
	{
		if (!json_is_integer(json_array_get(root, i)))
			return false;
	}

	return true;
}

Color json_color_value(const json_t *root)
{
	Color color;

	// Read R
	json_t* jsonR = json_array_get(root, 0);
	color.R = (uint8_t)json_integer_value(jsonR);

	// Read G
	json_t* jsonG = json_array_get(root, 1);
	color.G = (uint8_t)json_integer_value(jsonG);

	// Read B
	json_t* jsonB = json_array_get(root, 2);
	color.B = (uint8_t)json_integer_value(jsonB);

	return color;
}