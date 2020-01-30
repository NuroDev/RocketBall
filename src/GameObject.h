#pragma once

#include <directxmath.h>
#include <string>

#include "Appearance.h"
#include "ParticleModel.h"
#include "util/Vector3.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Appearance &appearance, bool isLaminar, float radius);
	~GameObject();

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext *pContext);

	string GetType() const { return _type; }

	Appearance &GetAppearance() const { return _rAppearance; }
	Transform *GetTransform() const { return _pTransform; }
	ParticleModel *GetParticleModel() const { return _pParticleModel; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void SetParent(GameObject *parent) { _pParent = parent; }

private:
	string _type;

	XMFLOAT4X4 _world;

	Appearance &_rAppearance;
	GameObject *_pParent = nullptr;
	ParticleModel *_pParticleModel = nullptr;
	Transform *_pTransform = nullptr;
};
