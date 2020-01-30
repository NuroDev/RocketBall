#include "Application.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool Application::HandleKeyboard(MSG msg)
{
	Vector3 cameraPosition = _pCamera->GetPosition();

	switch (msg.wParam)
	{
	case VK_UP:
		_cameraOrbitRadius = max(_cameraOrbitRadiusMin, _cameraOrbitRadius - (_cameraSpeed * 0.2f));
		return true;
		break;

	case VK_DOWN:
		_cameraOrbitRadius = min(_cameraOrbitRadiusMax, _cameraOrbitRadius + (_cameraSpeed * 0.2f));
		return true;
		break;

	case VK_RIGHT:
		_cameraOrbitAngleXZ -= _cameraSpeed;
		return true;
		break;

	case VK_LEFT:
		_cameraOrbitAngleXZ += _cameraSpeed;
		return true;
		break;

	case VK_ESCAPE:
		exit(0);
		break;
	}

	return false;
}

Application::Application()
{
	_lightDirection = XMFLOAT3(0.0f, 0.0f, -1.0f);
	_diffuseMaterial = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_diffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_ambientMaterial = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	_ambientLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	_specularMaterial = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	_specularLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_specularPower = float(15.0f);
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hr;

	hr = InitWindow(hInstance, nCmdShow);
	if (FAILED(hr))
		return hr;

	RECT rc;
	GetClientRect(_hWnd, &rc);
	_WindowWidth = rc.right - rc.left;
	_WindowHeight = rc.bottom - rc.top;

	hr = InitDevice();
	if (FAILED(hr))
	{
		Release();
		return hr;
	}

	// Load models
	_objGround = OBJLoader::Load("assets/models/Ground.obj", _pd3dDevice, false);
	_objSkyBox = OBJLoader::Load("assets/models/Skybox.obj", _pd3dDevice);
	_objCar = OBJLoader::Load("assets/models/Car.obj", _pd3dDevice);

	// Load textures
	CreateDDSTextureFromFile(_pd3dDevice, L"assets/textures/Skybox_COLOR.dds", nullptr, &_pTextureSkyBoxRV);
	CreateDDSTextureFromFile(_pd3dDevice, L"assets/textures/Car.dds", nullptr, &_pTextureCarRV);
	CreateDDSTextureFromFile(_pd3dDevice, L"assets/textures/Car.dds", nullptr, &_pTextureCarSpecularRV);
	CreateDDSTextureFromFile(_pd3dDevice, L"assets/textures/asphalt_COLOR.dds", nullptr, &_pTextureGroundRV);
	CreateDDSTextureFromFile(_pd3dDevice, L"assets/textures/asphalt_SPEC.dds", nullptr, &_pTextureGroundSpecularRV);

	// Initialize lighting
	_basicLight.AmbientLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_basicLight.DiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_basicLight.SpecularLight = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	_basicLight.SpecularPower = 20.0f;
	_basicLight.LightVecW = Vector3(0.0f, 1.0f, -1.0f);

	// Initialize camera
	Vector3 eye = Vector3(0.0f, 2.0f, -10.0f);
	Vector3 at = Vector3(0.0f, 2.0f, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

	_pCamera = new Camera(eye, at, up, (float)_renderWidth, (float)_renderHeight, 0.01f, 200.0f);

	// Initialize gameobjects(geometry, materials, gameObjects)
	Material shinyMaterial = InitMaterial(Vector3(0.3f, 0.3f, 0.3f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.5f, 0.5f, 0.5f), 10.0f);
	Material matteMaterial = InitMaterial(Vector3(0.1f, 0.1f, 0.1f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), 0.0f);

	// Ground plane object
	Geometry planeGeometry = InitGeometry(_objGround.IndexBuffer, _objGround.VertexBuffer, _objGround.IndexCount, _objGround.VBOffset);
	Appearance *pFloorAppearance = new Appearance(planeGeometry, matteMaterial);
	pFloorAppearance->SetTextureRV(_pTextureGroundRV);
	pFloorAppearance->SetSpecularRV(_pTextureGroundSpecularRV);
	GameObject *pFloorGameObject = new GameObject("Floor", *pFloorAppearance, true, 1.0f);
	InitGameObject(pFloorGameObject, Vector3(0.0f, 0.0f, 0.0f), Vector3(5.0f, 5.0f, 5.0f), Vector3(0.0f, 0.0f, 0.0f));

	// Car object
	Geometry carGeometry = InitGeometry(_objCar.IndexBuffer, _objCar.VertexBuffer, _objCar.IndexCount, _objCar.VBOffset);
	Appearance *pCarAppearance = new Appearance(carGeometry, shinyMaterial);
	pCarAppearance->SetTextureRV(_pTextureCarRV);
	pCarAppearance->SetSpecularRV(_pTextureCarSpecularRV);
	GameObject *pCarGameObject = new GameObject("Car", *pCarAppearance, true, 1.0f);
	InitGameObject(pCarGameObject, Vector3(0.0f, 7.5f, 10.0f), Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 135.0f, 0.0f));

	pCarGameObject->GetParticleModel()->SetMass(50.0f);
	pCarGameObject->GetParticleModel()->SetGravity(12.0f);

	// Sphere object
	Geometry sphereGeometry = InitGeometry(_objSkyBox.IndexBuffer, _objSkyBox.VertexBuffer, _objSkyBox.IndexCount, _objSkyBox.VBOffset);
	Appearance *pSphereAppearance = new Appearance(sphereGeometry, shinyMaterial);
	pSphereAppearance->SetTextureRV(_pTextureSkyBoxRV);
	GameObject *pSphereGameObject = new GameObject("Sphere", *pSphereAppearance, false, 1.0f);
	InitGameObject(pSphereGameObject, Vector3(-3.0f, 5.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));

	pSphereGameObject->GetParticleModel()->SetMass(5.0f);
	pSphereGameObject->GetParticleModel()->SetGravity(5.0f);
	pSphereGameObject->GetParticleModel()->SetRadius(1.0f);

	// Moon Sphere object
	Geometry moonSphereGeometry = InitGeometry(_objSkyBox.IndexBuffer, _objSkyBox.VertexBuffer, _objSkyBox.IndexCount, _objSkyBox.VBOffset);
	Appearance *pMoonSphereAppearance = new Appearance(moonSphereGeometry, shinyMaterial);
	pMoonSphereAppearance->SetTextureRV(_pTextureSkyBoxRV);
	GameObject *pMoonSphereGameObject = new GameObject("MoonSphere", *pMoonSphereAppearance, false, 1.0f);
	InitGameObject(pMoonSphereGameObject, Vector3(3.0f, 10.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));

	pSphereGameObject->GetParticleModel()->SetMass(50.0f);
	pSphereGameObject->GetParticleModel()->SetGravity(0.0f);
	pSphereGameObject->GetParticleModel()->SetRadius(1.0f);

	return hr;
}

Geometry Application::InitGeometry(ID3D11Buffer *indexBuffer, ID3D11Buffer *vertexBuffer, int numIndices, int numVertexBufferOffset)
{
	Geometry geometry;
	geometry.pIndexBuffer = indexBuffer;
	geometry.pVertexBuffer = vertexBuffer;
	geometry.numberOfIndices = numIndices;
	geometry.vertexBufferOffset = numVertexBufferOffset;
	geometry.vertexBufferStride = sizeof(SimpleVertex);

	return geometry;
}

Material Application::InitMaterial(Vector3 ambient, Vector3 diffuse, Vector3 specular, float specularPower)
{
	Material material;
	material.ambient = XMFLOAT4(ambient.x, ambient.y, ambient.z, 1.0f);
	material.diffuse = XMFLOAT4(diffuse.x, diffuse.y, diffuse.z, 1.0f);
	material.specular = XMFLOAT4(specular.x, specular.y, specular.z, 1.0f);
	material.specularPower = specularPower;

	return material;
}

void Application::InitGameObject(GameObject *obj, Vector3 pos, Vector3 scale, Vector3 rotation)
{
	obj->GetTransform()->SetPosition(pos.x, pos.y, pos.z);
	obj->GetTransform()->SetScale(scale.x, scale.y, scale.z);
	obj->GetTransform()->SetRotation(XMConvertToRadians(rotation.x), rotation.y, rotation.z);

	_gameObjects.push_back(obj);
}

HRESULT Application::InitShadersAndInputLayout()
{
	HRESULT hr;

	ID3DBlob *pVSBlob = nullptr;
	ID3DBlob *pPSBlob = nullptr;

	// Compile/Create the vertex shader
	hr = CompileShaderFromFile(L"RocketBall.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Compile/Create the pixel shader
	hr = CompileShaderFromFile(L"RocketBall.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
	{
		pPSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = ARRAYSIZE(layout);

	// Create/set the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_pVertexLayout);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	_pImmediateContext->IASetInputLayout(_pVertexLayout);

	return hr;
}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hr;

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"RocketBallClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);

	hr = !RegisterClassEx(&wcex);
	if (FAILED(hr))
		return hr;

	// Create window
	_hInst = hInstance;
	RECT rc = {0, 0, 1280, 720};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"RocketBallClass", L"Rocket Ball", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	hr = !_hWnd;
	if (FAILED(hr))
		return hr;

	ShowWindow(_hWnd, nCmdShow);

	return hr;
}

HRESULT Application::CompileShaderFromFile(WCHAR *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pErrorBlob;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr)
			OutputDebugStringA((char *)pErrorBlob->GetBufferPointer());

		if (pErrorBlob)
			pErrorBlob->Release();

		return hr;
	}

	if (pErrorBlob)
		pErrorBlob->Release();

	return hr;
}

HRESULT Application::InitDevice()
{
	HRESULT hr;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _WindowWidth;
	sd.BufferDesc.Height = _WindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D *pBackBuffer = nullptr;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = _WindowWidth;
	depthStencilDesc.Height = _WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_pDepthStencilBuffer);
	_pd3dDevice->CreateDepthStencilView(_pDepthStencilBuffer, nullptr, &_pDepthStencilView);

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	_pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

	// Set index buffer
	_pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_WindowWidth;
	vp.Height = (FLOAT)_WindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &vp);

	InitShadersAndInputLayout();

	// Set primitive topology
	_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_pWireFrame);

	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_BACK;

	hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_pSolidObject);
	if (FAILED(hr))
		return hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	_pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	_pd3dDevice->CreateBlendState(&blendDesc, &_pTransparency);

	return hr;
}

void Application::Release()
{
	if (_pd3dDevice)
		_pd3dDevice->Release();
	if (_pDepthStencilView)
		_pDepthStencilView->Release();
	if (_pDepthStencilBuffer)
		_pDepthStencilBuffer->Release();
	if (_pImmediateContext)
		_pImmediateContext->ClearState();
	if (_pImmediateContext)
		_pImmediateContext->Release();

	if (_pVertexBuffer)
		_pVertexBuffer->Release();
	if (_pIndexBuffer)
		_pIndexBuffer->Release();
	if (_pVertexLayout)
		_pVertexLayout->Release();
	if (_pVertexShader)
		_pVertexShader->Release();
	if (_pPixelShader)
		_pPixelShader->Release();

	if (_pSamplerLinear)
		_pSamplerLinear->Release();
	if (_pConstantBuffer)
		_pConstantBuffer->Release();
	if (_pRenderTargetView)
		_pRenderTargetView->Release();
	if (_pSwapChain)
		_pSwapChain->Release();

	if (_pWireFrame)
		_pWireFrame->Release();
	if (_pSolidObject)
		_pSolidObject->Release();
	if (_pTransparency)
		_pTransparency->Release();

	if (_pTextureCarRV)
		_pTextureCarRV->Release();
	if (_pTextureCarSpecularRV)
		_pTextureCarSpecularRV->Release();
	if (_pTextureGroundRV)
		_pTextureGroundRV->Release();
	if (_pTextureGroundSpecularRV)
		_pTextureGroundSpecularRV->Release();
	if (_pTextureSkyBoxRV)
		_pTextureSkyBoxRV->Release();

	if (_pCamera)
	{
		delete _pCamera;
		_pCamera = nullptr;
	}

	for (auto gameObject : _gameObjects)
	{
		if (gameObject)
		{
			delete gameObject;
			gameObject = nullptr;
		}
	}
}

void Application::Update()
{
	// Set delta time to 1/60(fps)
	float deltaTime = 0.01666666666f;

	// Update time
	static float timeSinceStart = 0.0f;
	static DWORD dwTimeStart = 0;

	DWORD dwTimeCur = GetTickCount();

	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;

	timeSinceStart = (dwTimeCur - dwTimeStart) / 1000.0f;

	// Keyboard controls

	// Select a game object
	if (GetAsyncKeyState('1'))
		_selectedObject = 1;
	if (GetAsyncKeyState('2'))
		_selectedObject = 2;
	if (GetAsyncKeyState('3'))
		_selectedObject = 3;

	// WASD movement
	if (GetAsyncKeyState('W'))
		_gameObjects[_selectedObject]->GetParticleModel()->AddForce(Vector3(0.0f, 0.0f, 20.0f));
	if (GetAsyncKeyState('A'))
		_gameObjects[_selectedObject]->GetParticleModel()->AddForce(Vector3(-20.0f, 0.0f, 0.0f));
	if (GetAsyncKeyState('S'))
		_gameObjects[_selectedObject]->GetParticleModel()->AddForce(Vector3(0.0f, 0.0f, -20.0f));
	if (GetAsyncKeyState('D'))
		_gameObjects[_selectedObject]->GetParticleModel()->AddForce(Vector3(20.0f, 0.0f, 0.0f));

	// Space: Jump
	if (GetAsyncKeyState(VK_SPACE))
		_gameObjects[_selectedObject]->GetParticleModel()->AddForce(Vector3(0.0f, 20.0f, 0.0f));

	// T: Toggle transparency
	if (GetAsyncKeyState('T'))
	{
		_isTransparent = !_isTransparent;
		Sleep(120);
	}

	// Y: Toggle Wireframe
	if (GetAsyncKeyState('Y'))
	{
		_isWireframe = !_isWireframe;
		Sleep(120);
	}

	for (int i = 1; i < _gameObjects.size(); i++) {
		// Q: Stop moving
		if (GetAsyncKeyState('Q'))
		{
			_gameObjects[i]->GetParticleModel()->SetVelocity(0.0f, 0.0f, 0.0f);
			_gameObjects[i]->GetParticleModel()->SetAcceleration(0.0f, 0.0f, 0.0f);
			_gameObjects[i]->GetTransform()->SetPosition(_gameObjects[i]->GetTransform()->GetPosition());
		}

		// R: Reset position to start
		if (GetAsyncKeyState('R'))
		{
			_gameObjects[i]->GetParticleModel()->SetVelocity(0.0f, 0.0f, 0.0f);
			_gameObjects[i]->GetParticleModel()->SetAcceleration(0.0f, 0.0f, 0.0f);
			_gameObjects[i]->GetTransform()->SetPosition(-5.0f + (i * 4), 7.5f, 10.0f);
		}
	}

	// Update camera
	float angleAroundZ = XMConvertToRadians(_cameraOrbitAngleXZ);

	float camX = _cameraOrbitRadius * cos(angleAroundZ);
	float camZ = _cameraOrbitRadius * sin(angleAroundZ);

	Vector3 cameraPos = _pCamera->GetPosition();
	cameraPos.x = camX;
	cameraPos.z = camZ;

	_pCamera->SetPosition(cameraPos);
	_pCamera->Update();

	// Update objects
	for (GameObject *i : _gameObjects)
	{
		i->Update(deltaTime);

		for (GameObject *j : _gameObjects)
		{
			if (i != j)
			{
				if (i->GetParticleModel()->CollisionCheck(j->GetTransform()->GetPosition(), j->GetParticleModel()->GetRadius()))
				{
					i->GetParticleModel()->SetVelocity(-(i->GetParticleModel()->GetVelocity().x), -(i->GetParticleModel()->GetVelocity().y), -(i->GetParticleModel()->GetVelocity().z));
				}
			}
		}
	}

	// Wait 1ms after every update
	Sleep(1);
}

void Application::Draw()
{
	// Create draw constant buffer object
	ConstantBuffer cb;

	// Draw clear buffer
	float ClearColor[4] = {0.125f, 0.125f, 0.125f, 1.0f};
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Draw setup buffer
	_pImmediateContext->IASetInputLayout(_pVertexLayout);

	_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);

	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);

	// Draw render scene
	XMFLOAT4X4 viewAsFloats = _pCamera->GetView();
	XMFLOAT4X4 projectionAsFloats = _pCamera->GetProjection();

	XMMATRIX view = XMLoadFloat4x4(&viewAsFloats);
	XMMATRIX projection = XMLoadFloat4x4(&projectionAsFloats);

	cb.View = XMMatrixTranspose(view);
	cb.Projection = XMMatrixTranspose(projection);

	cb.light = _basicLight;
	cb.EyePosW = _pCamera->GetPosition();

	// Initialize Lighting
	_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);

	float blendFactor[] = {0.75f, 0.75f, 0.75f, 1.0f};

	// Transparency/Wireframe toggle
	if (_isWireframe == true) {
		_pImmediateContext->RSSetState(_pWireFrame);
	} else {
		_pImmediateContext->RSSetState(_pSolidObject);
	}

	if (_isTransparent == true) {
		_pImmediateContext->OMSetBlendState(_pTransparency, blendFactor, 0xffffffff);
	} else {
		_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
	}

	for (auto gameObject : _gameObjects)
	{
		Material material = gameObject->GetAppearance().GetMaterial();

		cb.surface.AmbientMtrl = material.ambient;
		cb.surface.DiffuseMtrl = material.diffuse;
		cb.surface.SpecularMtrl = material.specular;

		cb.World = XMMatrixTranspose(gameObject->GetWorldMatrix());

		if (gameObject->GetAppearance().HasTexture()) {
			ID3D11ShaderResourceView *pTextureRV = gameObject->GetAppearance().GetTextureRV();
			_pImmediateContext->PSSetShaderResources(0, 1, &pTextureRV);
			cb.HasTexture = 1.0f;
		} else {
			cb.HasTexture = 0.0f;
		}

		_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		gameObject->Draw(_pImmediateContext);
	}

	_pSwapChain->Present(0, 0);
}