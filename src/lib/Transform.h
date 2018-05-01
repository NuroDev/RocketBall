#pragma once

#include "Vector3.h"

class Transform
{
	public:
		Transform(void);
		~Transform();

		void SetPosition(Vector3 position);
		void SetPosition(float x, float y, float z);

		void SetRotation(Vector3 rotation);
		void SetRotation(float x, float y, float z);

		void SetScale(Vector3 scale);
		void SetScale(float x, float y, float z);

		Vector3 GetPosition();
		Vector3 GetRotation();
		Vector3 GetScale();

	private:
		Vector3 _position;
		Vector3 _rotation;
		Vector3 _scale;
};

