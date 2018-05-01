#include "Appearance.h"

Appearance::Appearance(Geometry geometry, Material material) : _geometry(geometry), _material(material)
{
	_pTextureRV = nullptr;
}

Appearance::~Appearance()
{

}

void Appearance::Draw(ID3D11DeviceContext* pImmediateContext)
{
	// NOTE: Assumes the constant buffers and all other draw initialization has already taken place
	pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.pVertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_geometry.pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}