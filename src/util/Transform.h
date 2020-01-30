#pragma once

#include "Vector3.h"

class Transform
{
public:
	Transform(void)
	{
		_scale = Vector3(1.0f, 1.0f, 1.0f);
	};

	void SetPosition(Vector3 position) { _position = position; }
	void SetPosition(float x, float y, float z) { _position = Vector3(x, y, z); }

	void SetRotation(Vector3 rotation) { _rotation = rotation; }
	void SetRotation(float x, float y, float z) { _rotation = Vector3(x, y, z); }

	void SetScale(Vector3 scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale = Vector3(x, y, z); }

	Vector3 GetPosition() { return _position; }
	Vector3 GetRotation() { return _rotation; }
	Vector3 GetScale() { return _scale; }

private:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;
};
