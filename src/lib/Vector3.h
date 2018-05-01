#pragma once

#include <math.h>

class Vector3
{
	public:
		float x, y, z;

		Vector3(void);
		Vector3(float x, float y, float z);
		~Vector3();

		static float GetDotProduct(Vector3 vector1, Vector3 vector2);
		static Vector3 GetCrossProduct(Vector3 vector1, Vector3 vector2);
		float GetMagnitude();
		Vector3 GetNormalization();
};

