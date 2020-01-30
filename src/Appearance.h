#pragma once

#include "./util/Structures.h"

class Appearance
{
public:
	Appearance(Geometry geometry, Material material) : _geometry(geometry), _material(material){};

	Geometry GetGeometryData() const { return _geometry; }
	Material GetMaterial() const { return _material; }

	void SetTextureRV(ID3D11ShaderResourceView *textureRV) { _pTextureRV = textureRV; }
	ID3D11ShaderResourceView *GetTextureRV() const { return _pTextureRV; }
	bool HasTexture() const { return _pTextureRV ? true : false; }

	void SetSpecularRV(ID3D11ShaderResourceView *specTextureRV) { _pSpecTextureRV = specTextureRV; }
	ID3D11ShaderResourceView *GetSpecTextureRV() const { return _pSpecTextureRV; }

	void Draw(ID3D11DeviceContext *pContext);

private:
	Geometry _geometry;
	Material _material;

	ID3D11ShaderResourceView *_pTextureRV = nullptr;
	ID3D11ShaderResourceView *_pSpecTextureRV = nullptr;
};
