#pragma once

#include <directxmath.h>
#include <string>
#include "lib\Vector3.h"
#include "lib\ParticleModel.h"
#include "lib\Appearance.h"

using namespace DirectX;
using namespace std;

class GameObject
{
	public:
		GameObject(string type, Appearance& appearance, bool isLaminar, float radius);
		~GameObject();

		string GetType() const { return _type; }

		Appearance& GetAppearance() const { return _rAppearance; }
		Transform* GetTransform() const { return _pTransform; }
		ParticleModel* GetParticleModel() const { return _pParticleModel; }

		XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

		void SetParent(GameObject* parent) { _pParent = parent; }

		void Update(float deltaTime);
		void Draw(ID3D11DeviceContext* pImmediateContext);

	private:
		string _type;

		XMFLOAT4X4 _world;

		Appearance& _rAppearance;
		Transform* _pTransform;
		ParticleModel* _pParticleModel;

		GameObject* _pParent;
};

