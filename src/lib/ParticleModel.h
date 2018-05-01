#pragma once

#include "Transform.h"

class ParticleModel
{
	public:
		ParticleModel(Transform* transform, float mass, float gravity, float dragForce, bool isLaminar, float radius);
		~ParticleModel();
		void Update(float deltaTime);
		void Move(float deltaTime);

		Vector3 GetVelocity() const { return _velocity; }
		void SetVelocity(float x, float y, float z) { _velocity.x = x; _velocity.y = y; _velocity.z = z; }
		void SetVelocity(Vector3 velocity) { _velocity = velocity; }
		void MoveConstVel(float deltaTime);

		Vector3 GetAcceleration() const { return _acceleration; }
		void SetAcceleration(float x, float y, float z) { _acceleration.x = x; _acceleration.y = y; _acceleration.z = z; }
		void SetAcceleration(Vector3 acceleration) { _acceleration = acceleration; }
		void MoveConstAccel(float deltaTime);
		void UpdateAccel();

		float GetGravity() const { return _gravity; }
		void SetGravity(float gravity) { _gravity = gravity; }

		float GetMass() const { return _mass; }
		void SetMass(float mass) { _mass = mass; }

		float GetWeight() const { return _weight; }

		void UpdateNetForce(Vector3 force);
		void MotionInFluid(float deltaTime);
		void DragForce(Vector3 v, float dragFactor);
		void DragLamFlow(Vector3 v, float dragFactor);
		void DragTurbFlow(Vector3 v, float dragFactor);
		void AddForce(Vector3 force) { _netForce.x += force.x; _netForce.y += force.y; _netForce.z += force.z; }
		void AddForce(float x, float y, float z) { _netForce.x += x; _netForce.y += y; _netForce.z += z; }

		Vector3 GetThrustForce() const { return _thrustForce; }
		void SetTrustForce(Vector3 thrustForce) { _thrustForce = thrustForce; }
		void SetTrustForce(float x, float y, float z) { _thrustForce.x = x; _thrustForce.y = y; _thrustForce.z = z; }

		Vector3 GetBrakeForce() const { return _brakeForce; }
		void SetBrakeForce(Vector3 brakeForce) { _brakeForce = brakeForce; }
		void SetBrakeForce(float x, float y, float z) { _brakeForce.x = x; _brakeForce.y = y; _brakeForce.z = z; }

		float GetRadius() const { return _radius; }
		void SetRadius(float r) { _radius = r; }

		bool CollisionCheck(Vector3 pos, float radius);

	private:
		Transform* _pTransform;

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

