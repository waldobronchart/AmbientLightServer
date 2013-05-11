#include "Color.h"

Color lerpColor(const Color& c1, const Color& c2, float term)
{
	float r = lerp(c1.R(), c2.R(), term);
	float g = lerp(c1.G(), c2.G(), term);
	float b = lerp(c1.B(), c2.B(), term);

	return Color(r, g, b);
}

json_t* json_color(const Color& c)
{
	json_t* root = json_array();

	// R
	json_t* jsonR = json_real(c.R());
	json_array_append(root, jsonR);
	json_decref(jsonR);

	// G
	json_t* jsonG = json_real(c.G());
	json_array_append(root, jsonG);
	json_decref(jsonG);

	// B
	json_t* jsonB = json_real(c.B());
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
		if (!json_is_real(json_array_get(root, i)))
			return false;
	}

	return true;
}

Color json_color_value(const json_t *root)
{
	// Read R
	json_t* jsonR = json_array_get(root, 0);
	float r = (float)json_real_value(jsonR);

	// Read G
	json_t* jsonG = json_array_get(root, 1);
	float g = (float)json_real_value(jsonG);

	// Read B
	json_t* jsonB = json_array_get(root, 2);
	float b = (float)json_real_value(jsonB);

	return Color(r, g, b);
}