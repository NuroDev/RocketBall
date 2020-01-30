#include "Appearance.h"

void Appearance::Draw(ID3D11DeviceContext *pContext)
{
	// NOTE: Assumes the constant buffers and all other draw initialization has already taken place
	pContext->IASetVertexBuffers(0, 1, &_geometry.pVertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	pContext->IASetIndexBuffer(_geometry.pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}
