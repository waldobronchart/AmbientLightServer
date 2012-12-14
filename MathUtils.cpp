#include "MathUtils.h"


json_t* json_vector2(const Vector2& v)
{
	json_t* root = json_array();

	// X
	json_t* jsonX = json_real(v.X());
	json_array_append(root, jsonX);
	json_decref(jsonX);

	// Y
	json_t* jsonY = json_real(v.Y());
	json_array_append(root, jsonY);
	json_decref(jsonY);

	return root;
}

bool json_is_vector2(const json_t *root)
{
	if (!json_is_array(root))
		return false;

	if (json_array_size(root) != 2)
		return false;

	for (int i=0; i<2; ++i)
	{
		if (!json_is_real(json_array_get(root, i)))
			return false;
	}

	return true;
}

Vector2 json_vector2_value(const json_t *root)
{
	Vector2 vec;

	// Read X
	json_t* jsonX = json_array_get(root, 0);
	vec.X(json_real_value(jsonX));

	// Read Y
	json_t* jsonY = json_array_get(root, 1);
	vec.Y(json_real_value(jsonY));

	return vec;
}


float min(float x1, float x2)
{
	return (x1 < x2) ? x1 : x2;
}

float max(float x1, float x2)
{
	return (x1 > x2) ? x1 : x2;
}

float lerp(float minVal, float maxVal, float term)
{
	return (maxVal - minVal) * term + minVal;
}

float clamp(float val, float minVal, float maxVal)
{
	return max(min(val, maxVal), minVal);
}

Vector2 lerp(const Vector2& minVal, const Vector2& maxVal, float term)
{
	Vector2 result;
	result.m_x = lerp(minVal.m_x, maxVal.m_x, term);
	result.m_y = lerp(minVal.m_y, maxVal.m_y, term);
	return result;
}

Vector2 lerp4(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomRight, const Vector2& bottomLeft, float termX, float termY)
{
	Vector2 result;
	
    float topX = lerp(topLeft.m_x, topRight.m_x, termX);
    float bottomX = lerp(bottomLeft.m_x, bottomRight.m_x, termX);
	result.m_x = lerp(topX, bottomX, termY);
	
    float leftY = lerp(topLeft.m_y, bottomLeft.m_y, termY);
    float rightY = lerp(topRight.m_y, bottomRight.m_y, termY);
	result.m_y = lerp(leftY, rightY, termX);

	return result;
}