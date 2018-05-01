#include "Vector3.h"

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::~Vector3()
{

}

float Vector3::GetDotProduct(Vector3 vector1, Vector3 vector2)
{
	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.x * vector2.z);
}

Vector3 Vector3::GetCrossProduct(Vector3 vector1, Vector3 vector2)
{
	return Vector3((vector1.y * vector2.z) - (vector1.z * vector2.y), (vector1.z * vector2.x) - (vector1.x * vector2.z), (vector1.z * vector2.y) - (vector1.y * vector2.x));
}

float Vector3::GetMagnitude()
{
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::GetNormalization()
{
	return Vector3(( x / GetMagnitude() ),( y / GetMagnitude() ),( z / GetMagnitude() ));
}