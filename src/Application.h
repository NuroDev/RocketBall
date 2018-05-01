#pragma once

#include <windows.h>
#include <d3dcompiler.h>
#include <directxcolors.h>

#include "lib\Camera.h"
#include "lib/DDSTextureLoader.h"
#include "lib\Transform.h"
#include "lib\OBJLoader.h"
#include "../resource.h"

#include <vector>
#include "GameObject.h"

using namespace DirectX;

struct SurfaceInfo
{
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 SpecularMtrl;
};

struct Light
{
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 SpecularLight;

	float SpecularPower;
	Vector3 LightVecW;
};

struct ConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;

	SurfaceInfo surface;
	Light light;

	Vector3 EyePosW;
	float HasTexture;
};

class Application
{
	public:
		Application();
		~Application();

		HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

		bool HandleKeyboard(MSG msg);

		void Update();
		void Draw();

	private:
		HINSTANCE _hInst;
		HWND _hWnd;
		D3D_DRIVER_TYPE _driverType;
		D3D_FEATURE_LEVEL _featureLevel;
		ID3D11Device* _pd3dDevice;
		ID3D11DeviceContext* _pImmediateContext;
		IDXGISwapChain* _pSwapChain;
		ID3D11RenderTargetView* _pRenderTargetView;
		ID3D11VertexShader* _pVertexShader;
		ID3D11PixelShader* _pPixelShader;
		ID3D11InputLayout* _pVertexLayout;

		ID3D11Buffer* _pVertexBuffer;
		ID3D11Buffer* _pIndexBuffer;

		ID3D11Buffer* _pConstantBuffer;

		Light _basicLight;

		vector<GameObject*> _gameObjects;

		Camera* _pCamera;
		float _cameraOrbitRadius = 7.0f;
		float _cameraOrbitRadiusMin = 2.0f;
		float _cameraOrbitRadiusMax = 50.0f;
		float _cameraOrbitAngleXZ = -90.0f;
		float _cameraSpeed = 2.0f;

		MeshData _objGround;
		MeshData _objSkyBox;
		MeshData _objCar;

		ID3D11DepthStencilView* _pDepthStencilView;
		ID3D11Texture2D* _pDepthStencilBuffer;

		ID3D11RasterizerState* _pWireFrame;
		ID3D11RasterizerState* _pSolidObject;
		bool _isWireframe;
		bool _isTransparent;
		int _selectedObject;

		ID3D11SamplerState* _pSamplerLinear = nullptr;

		ID3D11ShaderResourceView* _pTextureSkyBoxRV = nullptr;
		ID3D11ShaderResourceView* _pTextureCarRV = nullptr;
		ID3D11ShaderResourceView* _pTextureCarSpecularRV = nullptr;
		ID3D11ShaderResourceView* _pTextureGroundRV = nullptr;
		ID3D11ShaderResourceView* _pTextureGroundSpecularRV = nullptr;

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

		ID3D11BlendState* _pTransparency;

		HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
		HRESULT InitDevice();
		void Cleanup();
		HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
		HRESULT InitShadersAndInputLayout();
		Geometry InitGeometry(ID3D11Buffer* indexBuffer, ID3D11Buffer* vertexBuffer, int numIndices, int numVertexBufferOffset);
		Material InitMaterial(Vector3 ambient, Vector3 diffuse, Vector3 specular, float specularPower);
		void InitGameObject(GameObject* obj, Vector3 pos, Vector3 scale, Vector3 rotation);
		
		UINT _WindowHeight;
		UINT _WindowWidth;

		UINT _renderHeight = 1080;
		UINT _renderWidth = 1920;
};

