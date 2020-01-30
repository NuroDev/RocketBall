#pragma once

#include <d3dcompiler.h>
#include <directxcolors.h>
#include <vector>
#include <windows.h>

#include "../resource.h"
#include "Camera.h"
#include "GameObject.h"
#include "lib/DDSTextureLoader.h"
#include "lib/OBJLoader.h"
#include "util/Transform.h"

using namespace DirectX;

class Application
{
public:
	Application();
	~Application() { Release(); };

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);

	void Update();
	void Draw();

private:
	Geometry InitGeometry(ID3D11Buffer *indexBuffer, ID3D11Buffer *vertexBuffer, int numIndices, int numVertexBufferOffset);
	HRESULT CompileShaderFromFile(WCHAR *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut);
	HRESULT InitDevice();
	HRESULT InitShadersAndInputLayout();
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	Material InitMaterial(Vector3 ambient, Vector3 diffuse, Vector3 specular, float specularPower);
	void InitGameObject(GameObject *obj, Vector3 pos, Vector3 scale, Vector3 rotation);
	void Release();

	HINSTANCE _hInst;
	HWND _hWnd;
	D3D_DRIVER_TYPE _driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL _featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device *_pd3dDevice = nullptr;
	ID3D11DeviceContext *_pImmediateContext = nullptr;
	IDXGISwapChain *_pSwapChain = nullptr;
	ID3D11RenderTargetView *_pRenderTargetView = nullptr;
	ID3D11VertexShader *_pVertexShader = nullptr;
	ID3D11PixelShader *_pPixelShader = nullptr;
	ID3D11InputLayout *_pVertexLayout = nullptr;

	ID3D11Buffer *_pVertexBuffer = nullptr;
	ID3D11Buffer *_pIndexBuffer = nullptr;

	ID3D11Buffer *_pConstantBuffer = nullptr;

	Light _basicLight;

	vector<GameObject *> _gameObjects;

	Camera *_pCamera;
	float _cameraOrbitRadius = 7.0f;
	float _cameraOrbitRadiusMin = 2.0f;
	float _cameraOrbitRadiusMax = 50.0f;
	float _cameraOrbitAngleXZ = -90.0f;
	float _cameraSpeed = 2.0f;

	MeshData _objGround;
	MeshData _objSkyBox;
	MeshData _objCar;

	ID3D11DepthStencilView *_pDepthStencilView = nullptr;
	ID3D11Texture2D *_pDepthStencilBuffer = nullptr;

	ID3D11RasterizerState *_pWireFrame = nullptr;
	ID3D11RasterizerState *_pSolidObject = nullptr;
	bool _isWireframe = false;
	bool _isTransparent = false;
	int _selectedObject = 1;

	ID3D11SamplerState *_pSamplerLinear = nullptr;

	ID3D11ShaderResourceView *_pTextureSkyBoxRV = nullptr;
	ID3D11ShaderResourceView *_pTextureCarRV = nullptr;
	ID3D11ShaderResourceView *_pTextureCarSpecularRV = nullptr;
	ID3D11ShaderResourceView *_pTextureGroundRV = nullptr;
	ID3D11ShaderResourceView *_pTextureGroundSpecularRV = nullptr;

	D3D11_RASTERIZER_DESC wfdesc;

	XMFLOAT3 _lightDirection;
	XMFLOAT4 _diffuseMaterial;
	XMFLOAT4 _diffuseLight;
	XMFLOAT4 _ambientMaterial;
	XMFLOAT4 _ambientLight;
	XMFLOAT4 _specularMaterial;
	XMFLOAT4 _specularLight;
	XMFLOAT3 _eyePosW;
	float _specularPower;

	ID3D11BlendState *_pTransparency = nullptr;

	UINT _WindowHeight;
	UINT _WindowWidth;

	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;
};
