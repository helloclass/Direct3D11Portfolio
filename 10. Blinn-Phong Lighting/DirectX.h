#pragma once

#ifndef KFBX_DLLINFO
#define KFBX_DLLINFO
#endif // !KFBX_DLLINFO

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include "CreateWindows.h"
#include "ObjLoading.h"

class DirectXHelper {
public:
	struct RenderTargetViewBackBuffer
	{
		// Deferred RTV
		ID3D11Texture2D* m_renderTargetTexture;
		ID3D11RenderTargetView* deferRenderTargetView;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};

	struct DepthStencilViewBackBuffer
	{
		ID3D11Texture2D* deferDepthBuffer;
		ID3D11DepthStencilView* deferDepthBufferView;
		ID3D11ShaderResourceView* m_depthResourceView;
	};

	// Deferred Buffer Vector
	static const UINT PostProcessBackBufferCount = 5;

	static UINT RTVBackBufferOffset;
	static UINT DSVBackBufferOffset;

	std::vector<RenderTargetViewBackBuffer*> RTVBackBuffers;
	std::vector<DepthStencilViewBackBuffer*> DSVBackBuffers;

private:
	const HWND* MainHWND;

	// SwapChain
	IDXGISwapChain1* d3d11SwapChain;
	ID3D11RenderTargetView* renderBufferView;

	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilView* depthBufferView;

	// Create Vertex, Pixel Shader for BoundBox 
	ID3DBlob* BoundBoxVsCode;
	ID3D11VertexShader* BoundBoxVertexShader;
	ID3D11PixelShader* BoundBoxPixelShader;

	float4x4 perspectiveMat = {};
	int windowWidth, windowHeight;

	float4x4 cubeModelViewMat;
	float4x4 inverseModelViewMat;
	float3x3 cubeNormalMat;

	DirectX::XMVECTOR rayOrigin;
	DirectX::XMVECTOR rayDir;

	FLOAT backgroundColor[4] = { 0.1f, 0.2f, 0.6f, 1.0f };

public:
	DirectXHelper();
	~DirectXHelper();

	DWORD Init(const HWND& mHWND);
	DWORD Update();
	DWORD Destroy();

private:
	DWORD CreateDeviceAndContext();
	//  DirectX Graphics Infrastructor support a service about Monitor Information or Controller.
	DWORD CreateDXGI();
	DWORD CreateSwapChain();
	DWORD CreateRenderDepthTargets();

	DWORD CreateShaders();
	DWORD CreateShadowShader();
	DWORD CreateInputDescriptor();

	DWORD CreateSampler();

	DWORD LoadObject();
	DWORD LoadTexture();

	DWORD CreateLightCBV();
	DWORD CreateVertexShaderConstantBuffer();
	DWORD CreatePixelShaderConstantBuffer();

	DWORD CreateRasterizerState();
	DWORD CreateDepthStencilState();

	DWORD CreateQuary();

private:
	void UpdateWindow();
	void UpdateWorldViewProjection();
	void UpdateInstanceBuffer(ObjectData* obj);
	void UpdateAnimationPose(ObjectData* obj, UINT SubmeshIDX);
	// Update vertex shader constant buffer
	void UpdateVertexShderBuffer(ObjectData* obj);
	// Update pixel shader constant buffer
	void UpdatePixelShderBuffer(ObjectData* obj);
	// Update blur shader constant buffer
	void UpdateBlurShderBuffer(ObjectData* obj, bool isWidth);
	void UpdateLight();
	void MousePickingSetting();
	void MousePicking(ObjectData* obj, std::string Name);

	void RecordRTVandDSV(ID3D11RenderTargetView* renderBufferView, ID3D11DepthStencilView* deferDepthBufferView);
	void RecordMaterial(ObjectData* obj, UINT submeshIDX);

	void DrawCollider(ObjectData* obj);
};

static DirectXHelper _DirectXHelper;

static DWORD hAnimThreadID;
static DWORD hAnimThreadParam = 0;
static HANDLE hAnimThread;

static HANDLE hAnimThreadReadEvent;
static HANDLE hAnimThreadWriteEvent;

static fbx::FBXLoader fbxLoader;