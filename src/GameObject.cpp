#include "GameObject.h"

GameObject::GameObject(string type, Appearance &appearance, bool isLaminar, float radius) : _rAppearance(appearance)
{
	_pTransform = new Transform();
	_pParticleModel = new ParticleModel(_pTransform, 1.0f, 1.0f, 1.0f, isLaminar, radius);
}

GameObject::~GameObject()
{

	if (_pTransform) {
		delete _pTransform;
		_pTransform = nullptr;
	}

	if (_pParent) {
		delete _pParent;
		_pParent = nullptr;
	}

	if (_pParticleModel) {
		delete _pParticleModel;
		_pParticleModel = nullptr;
	}
}

void GameObject::Update(float deltaTime)
{
	XMMATRIX scale = XMMatrixScaling(_pTransform->GetScale().x, _pTransform->GetScale().y, _pTransform->GetScale().z);
	XMMATRIX rotation = XMMatrixRotationX(_pTransform->GetRotation().x) * XMMatrixRotationY(_pTransform->GetRotation().y) * XMMatrixRotationZ(_pTransform->GetRotation().z);
	XMMATRIX translation = XMMatrixTranslation(_pTransform->GetPosition().x, _pTransform->GetPosition().y, _pTransform->GetPosition().z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);

	if (_pParent != nullptr)
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _pParent->GetWorldMatrix());

	_pParticleModel->Update(deltaTime);
}

void GameObject::Draw(ID3D11DeviceContext *pContext)
{
	_rAppearance.Draw(pContext);
}
