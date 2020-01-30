#pragma once

#include "util/Transform.h"

class ParticleModel
{
public:
	ParticleModel(Transform *transform, float mass, float gravity, float dragForce, bool isLaminar, float radius);
	~ParticleModel();

	void Update(float deltaTime);

	// Velocity
	Vector3 GetVelocity() const { return _velocity; };
	void SetVelocity(float x, float y, float z) { _velocity.x = x; _velocity.y = y; _velocity.z = z; };
	void SetVelocity(Vector3 velocity) { _velocity = velocity; };
	void MoveConstVel(float deltaTime);

	// Acceleration
	Vector3 GetAcceleration() const { return _acceleration; };
	void SetAcceleration(float x, float y, float z) { _acceleration.x = x; _acceleration.y = y; _acceleration.z = z; };
	void SetAcceleration(Vector3 acceleration) { _acceleration = acceleration; };
	void MoveConstAccel(float deltaTime);
	void UpdateAccel() { _acceleration.x = _netForce.x / _mass; _acceleration.y = _netForce.y / _mass; _acceleration.z = _netForce.z / _mass; };

	// Gravity
	float GetGravity() const { return _gravity; };
	void SetGravity(float gravity) { _gravity = gravity; };

	// Mass
	float GetMass() const { return _mass; };
	void SetMass(float mass) { _mass = mass; };

	// Weight
	float GetWeight() const { return _weight; };

	// Drag
	void DragForce(Vector3 v, float dragFactor) { _isLaminar ? DragLamFlow(v, dragFactor) : DragTurbFlow(v, dragFactor); };
	void DragLamFlow(Vector3 v, float dragFactor) { _drag.x -= dragFactor * v.x; _drag.y -= dragFactor * v.y; _drag.z -= dragFactor * v.z; };
	void DragTurbFlow(Vector3 v, float dragFactor);

	// Forces
	void AddForce(float x, float y, float z) { _netForce.x += x; _netForce.y += y; _netForce.z += z; };
	void AddForce(Vector3 force) { _netForce.x += force.x; _netForce.y += force.y; _netForce.z += force.z; };
	Vector3 GetBrakeForce() const { return _brakeForce; };
	Vector3 GetThrustForce() const { return _thrustForce; };
	void SetBrakeForce(float x, float y, float z) { _brakeForce.x = x; _brakeForce.y = y; _brakeForce.z = z; };
	void SetBrakeForce(Vector3 brakeForce) { _brakeForce = brakeForce; };
	void SetTrustForce(float x, float y, float z) { _thrustForce.x = x; _thrustForce.y = y; _thrustForce.z = z; };
	void SetTrustForce(Vector3 thrustForce) { _thrustForce = thrustForce; };

	// Radius
	float GetRadius() const { return _radius; };
	void SetRadius(float r) { _radius = r; };

	// Misc
	bool CollisionCheck(Vector3 pos, float radius);
	void MotionInFluid(float deltaTime) { DragForce(_velocity, _dragForce); Update(deltaTime); Move(deltaTime); };
	void Move(float deltaTime);
	void UpdateNetForce(Vector3 force) { _netForce.x += force.x; _netForce.y += force.y; _netForce.z += force.z; };

private:
	Transform *_pTransform = nullptr;

	Vector3 _velocity;
	Vector3 _acceleration;
	Vector3 _drag;
	Vector3 _netForce;
	Vector3 _thrustForce;
	Vector3 _brakeForce;

	float _gravity;
	float _mass;
	float _weight;
	float _dragForce;
	float _radius;

	bool _isLaminar;
};
