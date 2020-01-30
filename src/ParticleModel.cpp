#include "ParticleModel.h"

ParticleModel::ParticleModel(Transform *transform, float mass, float gravity, float dragForce, bool isLaminar, float radius)
	: _pTransform(transform), _mass(mass), _gravity(gravity), _dragForce(dragForce), _isLaminar(isLaminar), _radius(radius)
{
	_weight = _mass * _gravity;
	_netForce.x = 0.0f;
	_netForce.y = 0.0f;
	_netForce.z = 0.0f;
}

ParticleModel::~ParticleModel()
{
	if (_pTransform)
		_pTransform = nullptr;
}

void ParticleModel::MoveConstVel(float deltaTime)
{
	Vector3 position = _pTransform->GetPosition();

	position.x += _velocity.x * deltaTime;
	position.y += _velocity.y * deltaTime;
	position.z += _velocity.z * deltaTime;

	_pTransform->SetPosition(position);
}

void ParticleModel::MoveConstAccel(float deltaTime)
{
	Vector3 position = _pTransform->GetPosition();

	position.x += _velocity.x * deltaTime + 0.5f * _velocity.x * deltaTime * deltaTime;
	position.y += _velocity.y * deltaTime + 0.5f * _velocity.y * deltaTime * deltaTime;
	position.z += _velocity.z * deltaTime + 0.5f * _velocity.z * deltaTime * deltaTime;

	_velocity.x += _acceleration.x * deltaTime;
	_velocity.y += _acceleration.y * deltaTime;
	_velocity.z += _acceleration.z * deltaTime;

	SetVelocity(_velocity);
}

void ParticleModel::Update(float deltaTime)
{
	UpdateNetForce(_thrustForce);
	UpdateNetForce(_brakeForce);

	_netForce.y -= _weight + _gravity;

	UpdateAccel();
	Move(deltaTime);
}

void ParticleModel::DragTurbFlow(Vector3 v, float dragForce)
{
	float velMag = v.GetMagnitude();
	Vector3 unitVel = v.GetNormalization();

	float dragMag = dragForce * velMag * velMag;

	_drag.x -= dragMag * unitVel.x;
	_drag.y -= dragMag * unitVel.y;
	_drag.z -= dragMag * unitVel.z;
}

void ParticleModel::Move(float deltaTime)
{
	Vector3 position = _pTransform->GetPosition();

	position.x += _velocity.x * deltaTime + 0.5f * _acceleration.x * deltaTime * deltaTime;
	position.y += _velocity.y * deltaTime + 0.5f * _acceleration.y * deltaTime * deltaTime;
	position.z += _velocity.z * deltaTime + 0.5f * _acceleration.z * deltaTime * deltaTime;

	_velocity.x += _acceleration.x * deltaTime;
	_velocity.y += _acceleration.y * deltaTime;
	_velocity.z += _acceleration.z * deltaTime;

	if (position.y <= 1.0f)
	{
		_velocity.y = -(_velocity.y) * 0.55f;
		position.y = 1.0f;
	}

	_netForce.x = 0.0f;
	_netForce.y = 0.0f;
	_netForce.z = 0.0f;

	_pTransform->SetPosition(position);
}

bool ParticleModel::CollisionCheck(Vector3 pos, float radius)
{
	Vector3 position = _pTransform->GetPosition();
	Vector3 distance = Vector3((position.x - pos.x), (position.y - pos.y), (position.z - pos.z));

	float totalRadius = _radius + radius;
	float totalDistance = distance.GetMagnitude();

	if (totalDistance < totalRadius)
		return true;

	return false;
}
