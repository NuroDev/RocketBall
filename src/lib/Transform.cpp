#include "Transform.h"

Transform::Transform()
{
	_scale = Vector3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{

}

void Transform::SetPosition(Vector3 position)
{
	_position = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	_position = Vector3(x, y, z);
}

void Transform::SetRotation(Vector3 rotation)
{
	_rotation = rotation;
}

void Transform::SetRotation(float x, float y, float z)
{
	_rotation = Vector3(x, y, z);
}

void Transform::SetScale(Vector3 scale)
{
	_scale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	_scale = Vector3(x, y, z);
}

Vector3 Transform::GetPosition()
{
	return _position;
}

Vector3 Transform::GetRotation()
{
	return _rotation;
}

Vector3 Transform::GetScale()
{
	return _scale;
}