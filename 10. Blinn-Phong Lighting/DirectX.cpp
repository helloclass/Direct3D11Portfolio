#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include "DirectX.h"
#include "stb_image.h"

UINT DirectXHelper::RTVBackBufferOffset = 0;
UINT DirectXHelper::DSVBackBufferOffset = 0;

DWORD WINAPI AnimationThread(PVOID arg)
{
    std::unordered_map<std::string, ObjectData*>::iterator iter, end;
    UINT submeshIDX = 0;
    UINT clusterIDX = 0;

    int currentFrame = 0;

    FbxSkin* lSkinDeformer;

    UINT lClusterCount = 0;
    FbxCluster* lCluster;

    fbxsdk::FbxAMatrix lVertexTransformMatrix;
    int lVertexIndexCount;

    int* mIndices       = nullptr;
    double* mWeights    = nullptr;

    int i(0), j(0), k(0);
    UINT VertSize;

    fbxsdk::FbxVector4* vertices;
    std::vector<fbxsdk::FbxAMatrix> lClusterDeformation;
    std::vector<double>             lWeightAccumulation;

    fbxsdk::FbxVector4 result;

    Sequence*   assetSequence;
    Montage*    assetMontage;
    BlendSpace* assetBlendSpace;

    while (true)
    {
        if (CreateWindows::getInstance().isPaused)
            continue;

        iter = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.begin();
        end = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.end();

        // Lock and Wait Until Mutex has unlock!
        WaitForSingleObject(hAnimThreadWriteEvent, INFINITE);

        while (iter != end)
        {
            if (!(iter->second))
            {
                iter++;
                continue;
            }

            // 애니메이션을 업데이트 한다.
            if (iter->second->objType == ObjectType::FBX_SKINNED_TYPE)
            {
                if (iter->second->CurrentAsset->mType == AnimationAsset::AnimationAssetType::SEQUENCE)
                {
                    assetSequence = (Sequence*)iter->second->GetAsset();

                    currentFrame = assetSequence->mFrame;
                    assetSequence->mAnimBuffer.resize(iter->second->SubmeshCount);

                    for (submeshIDX = 0; submeshIDX < iter->second->SubmeshCount; submeshIDX++)
                    {
                        VertSize = iter->second->meshDatas[submeshIDX].Vertices.size();

                        assetSequence->mAnimBuffer[submeshIDX].clear();
                        assetSequence->mAnimBuffer[submeshIDX].resize(VertSize);
                        lClusterDeformation.clear();
                        lClusterDeformation.resize(VertSize);
                        lWeightAccumulation.clear();
                        lWeightAccumulation.resize(VertSize);

                        vertices = iter->second->meshFBXDatas[submeshIDX]->GetControlPoints();
                        lSkinDeformer =
                            (FbxSkin*)iter->second->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);
                        lClusterCount =
                            lSkinDeformer->GetClusterCount();



                        for (clusterIDX = 0; clusterIDX < lClusterCount; clusterIDX++)
                        {
                            lCluster = lSkinDeformer->GetCluster(clusterIDX);

                            assetSequence->GetAnimationFBXMatrix(submeshIDX, clusterIDX, lVertexTransformMatrix);

                            mIndices = lCluster->GetControlPointIndices();
                            lVertexIndexCount = lCluster->GetControlPointIndicesCount();

                            mWeights = lCluster->GetControlPointWeights();

                            for (k = 0; k < lVertexIndexCount; k++)
                            {
                                for (i = 0; i < 4; i++)
                                    for (j = 0; j < 4; j++)
                                        lClusterDeformation[mIndices[k]][i][j] +=
                                        lVertexTransformMatrix[i][j] * mWeights[k];

                                lWeightAccumulation[mIndices[k]] += mWeights[k];
                            }

                        } // ClusterCount

                        for (i = 0; i < VertSize; i++)
                        {
                            result = lClusterDeformation[i].MultT(vertices[i]);
                            result = result / lWeightAccumulation[i];

                            memcpy(
                                &assetSequence->mAnimBuffer[submeshIDX][i],
                                &iter->second->meshDatas[submeshIDX].Vertices[i],
                                sizeof(Vertex)
                            );

                            assetSequence->mAnimBuffer[submeshIDX][i].Position[0] =
                                result.mData[0];
                            assetSequence->mAnimBuffer[submeshIDX][i].Position[1] =
                                result.mData[1];
                            assetSequence->mAnimBuffer[submeshIDX][i].Position[2] =
                                result.mData[2];
                        }

                        int indexSize = iter->second->meshDatas[submeshIDX].Indices32.size();
                        Vertex* vertices = assetSequence->mAnimBuffer[submeshIDX].data();
                        uint32* indices = iter->second->meshDatas[submeshIDX].Indices32.data();
                        float3 V1, V2, V3;

                        for (int i = 0; i < indexSize - 2; i += 3)
                        {
                            V1.x = vertices[indices[i]].Position[0] -
                                vertices[indices[i + 1]].Position[0];
                            V1.y = vertices[indices[i]].Position[1] -
                                vertices[indices[i + 1]].Position[1];
                            V1.z = vertices[indices[i]].Position[2] -
                                vertices[indices[i + 1]].Position[2];

                            V2.x = vertices[indices[i]].Position[0] -
                                vertices[indices[i + 2]].Position[0];
                            V2.y = vertices[indices[i]].Position[1] -
                                vertices[indices[i + 2]].Position[1];
                            V2.z = vertices[indices[i]].Position[2] -
                                vertices[indices[i + 2]].Position[2];

                            V3 = cross(V1, V2);
                            V3 = normalise(V3);

                            vertices[indices[i]].Normal[0] = V3.x;
                            vertices[indices[i]].Normal[1] = V3.y;
                            vertices[indices[i]].Normal[2] = V3.z;

                            vertices[indices[i + 1]].Normal[0] = V3.x;
                            vertices[indices[i + 1]].Normal[1] = V3.y;
                            vertices[indices[i + 1]].Normal[2] = V3.z;

                            vertices[indices[i + 2]].Normal[0] = V3.x;
                            vertices[indices[i + 2]].Normal[1] = V3.y;
                            vertices[indices[i + 2]].Normal[2] = V3.z;
                        }


                    } // SubmeshCount

                    // 다음 프레임
                    assetSequence->mFrame =
                        (assetSequence->mFrame + 1) < (UINT)assetSequence->mFrameLength ?
                        (assetSequence->mFrame + 1) : 0;
                }
                else if (iter->second->CurrentAsset->mType == AnimationAsset::AnimationAssetType::MONTAGE)
                {
                    assetMontage = (Montage*)iter->second->GetAsset();

                    currentFrame = assetMontage->mFrame;
                    assetMontage->mAnimBuffer.resize(iter->second->SubmeshCount);

                    for (submeshIDX = 0; submeshIDX < iter->second->SubmeshCount; submeshIDX++)
                    {
                        VertSize = iter->second->meshDatas[submeshIDX].Vertices.size();

                        assetMontage->mAnimBuffer[submeshIDX].clear();
                        assetMontage->mAnimBuffer[submeshIDX].resize(VertSize);
                        lClusterDeformation.clear();
                        lClusterDeformation.resize(VertSize);
                        lWeightAccumulation.clear();
                        lWeightAccumulation.resize(VertSize);

                        vertices = iter->second->meshFBXDatas[submeshIDX]->GetControlPoints();
                        lSkinDeformer =
                            (FbxSkin*)iter->second->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);
                        lClusterCount =
                            lSkinDeformer->GetClusterCount();



                        for (clusterIDX = 0; clusterIDX < lClusterCount; clusterIDX++)
                        {
                            lCluster = lSkinDeformer->GetCluster(clusterIDX);

                            assetMontage->GetAnimationFBXMatrix(submeshIDX, clusterIDX, lVertexTransformMatrix);

                            mIndices = lCluster->GetControlPointIndices();
                            lVertexIndexCount = lCluster->GetControlPointIndicesCount();

                            mWeights = lCluster->GetControlPointWeights();

                            for (k = 0; k < lVertexIndexCount; k++)
                            {
                                for (i = 0; i < 4; i++)
                                    for (j = 0; j < 4; j++)
                                        lClusterDeformation[mIndices[k]][i][j] +=
                                        lVertexTransformMatrix[i][j] * mWeights[k];

                                lWeightAccumulation[mIndices[k]] += mWeights[k];
                            }

                        } // ClusterCount

                        for (i = 0; i < VertSize; i++)
                        {
                            result = lClusterDeformation[i].MultT(vertices[i]);
                            result = result / lWeightAccumulation[i];

                            memcpy(
                                &assetMontage->mAnimBuffer[submeshIDX][i],
                                &iter->second->meshDatas[submeshIDX].Vertices[i],
                                sizeof(Vertex)
                            );

                            assetMontage->mAnimBuffer[submeshIDX][i].Position[0] =
                                result.mData[0];
                            assetMontage->mAnimBuffer[submeshIDX][i].Position[1] =
                                result.mData[1];
                            assetMontage->mAnimBuffer[submeshIDX][i].Position[2] =
                                result.mData[2];
                        }

                        int indexSize = iter->second->meshDatas[submeshIDX].Indices32.size();
                        Vertex* vertices = assetMontage->mAnimBuffer[submeshIDX].data();
                        uint32* indices = iter->second->meshDatas[submeshIDX].Indices32.data();
                        float3 V1, V2, V3;

                        for (int i = 0; i < indexSize - 2; i += 3)
                        {
                            V1.x = vertices[indices[i]].Position[0] -
                                vertices[indices[i + 1]].Position[0];
                            V1.y = vertices[indices[i]].Position[1] -
                                vertices[indices[i + 1]].Position[1];
                            V1.z = vertices[indices[i]].Position[2] -
                                vertices[indices[i + 1]].Position[2];

                            V2.x = vertices[indices[i]].Position[0] -
                                vertices[indices[i + 2]].Position[0];
                            V2.y = vertices[indices[i]].Position[1] -
                                vertices[indices[i + 2]].Position[1];
                            V2.z = vertices[indices[i]].Position[2] -
                                vertices[indices[i + 2]].Position[2];

                            V3 = cross(V1, V2);
                            V3 = normalise(V3);

                            vertices[indices[i]].Normal[0] = V3.x;
                            vertices[indices[i]].Normal[1] = V3.y;
                            vertices[indices[i]].Normal[2] = V3.z;

                            vertices[indices[i + 1]].Normal[0] = V3.x;
                            vertices[indices[i + 1]].Normal[1] = V3.y;
                            vertices[indices[i + 1]].Normal[2] = V3.z;

                            vertices[indices[i + 2]].Normal[0] = V3.x;
                            vertices[indices[i + 2]].Normal[1] = V3.y;
                            vertices[indices[i + 2]].Normal[2] = V3.z;
                        }


                    } // SubmeshCount

                    // 다음 프레임
                    assetMontage->mFrame =
                        ((assetMontage->mFrame + 1) < (UINT)assetMontage->mFrameLength) ?
                            assetMontage->mFrame + 1 : 0;
                }
                // Type is BlendSpace
                else
                {
                    assetBlendSpace = (BlendSpace*)iter->second->GetAsset();

                    currentFrame = assetBlendSpace->mFrame;
                    assetBlendSpace->mAnimBuffer.resize(iter->second->SubmeshCount);

                    for (submeshIDX = 0; submeshIDX < iter->second->SubmeshCount; submeshIDX++)
                    {
                        VertSize = iter->second->meshDatas[submeshIDX].Vertices.size();

                        assetBlendSpace->mAnimBuffer[submeshIDX].clear();
                        assetBlendSpace->mAnimBuffer[submeshIDX].resize(VertSize);
                        lClusterDeformation.clear();
                        lClusterDeformation.resize(VertSize);
                        lWeightAccumulation.clear();
                        lWeightAccumulation.resize(VertSize);

                        vertices = iter->second->meshFBXDatas[submeshIDX]->GetControlPoints();
                        lSkinDeformer =
                            (FbxSkin*)iter->second->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);
                        lClusterCount =
                            lSkinDeformer->GetClusterCount();



                        for (clusterIDX = 0; clusterIDX < lClusterCount; clusterIDX++)
                        {
                            lCluster = lSkinDeformer->GetCluster(clusterIDX);

                            assetBlendSpace->GetAnimationFBXMatrix(submeshIDX, clusterIDX, lVertexTransformMatrix);

                            mIndices = lCluster->GetControlPointIndices();
                            lVertexIndexCount = lCluster->GetControlPointIndicesCount();

                            mWeights = lCluster->GetControlPointWeights();

                            for (k = 0; k < lVertexIndexCount; k++)
                            {
                                for (i = 0; i < 4; i++)
                                    for (j = 0; j < 4; j++)
                                        lClusterDeformation[mIndices[k]][i][j] +=
                                        lVertexTransformMatrix[i][j] * mWeights[k];

                                lWeightAccumulation[mIndices[k]] += mWeights[k];
                            }

                        } // ClusterCount

                        for (i = 0; i < VertSize; i++)
                        {
                            result = lClusterDeformation[i].MultT(vertices[i]);
                            result = result / lWeightAccumulation[i];

                            memcpy(
                                &assetBlendSpace->mAnimBuffer[submeshIDX][i],
                                &iter->second->meshDatas[submeshIDX].Vertices[i],
                                sizeof(Vertex)
                            );

                            assetBlendSpace->mAnimBuffer[submeshIDX][i].Position[0] =
                                result.mData[0];
                            assetBlendSpace->mAnimBuffer[submeshIDX][i].Position[1] =
                                result.mData[1];
                            assetBlendSpace->mAnimBuffer[submeshIDX][i].Position[2] =
                                result.mData[2];
                        }

                        int indexSize = iter->second->meshDatas[submeshIDX].Indices32.size();
                        Vertex* vertices = assetBlendSpace->mAnimBuffer[submeshIDX].data();
                        uint32* indices = iter->second->meshDatas[submeshIDX].Indices32.data();
                        float3 V1, V2, V3;

                        for (int i = 0; i < indexSize - 2; i += 3)
                        {
                            V1.x = vertices[indices[i]].Position[0] -
                                vertices[indices[i + 1]].Position[0];
                            V1.y = vertices[indices[i]].Position[1] -
                                vertices[indices[i + 1]].Position[1];
                            V1.z = vertices[indices[i]].Position[2] -
                                vertices[indices[i + 1]].Position[2];

                            V2.x = vertices[indices[i]].Position[0] -
                                vertices[indices[i + 2]].Position[0];
                            V2.y = vertices[indices[i]].Position[1] -
                                vertices[indices[i + 2]].Position[1];
                            V2.z = vertices[indices[i]].Position[2] -
                                vertices[indices[i + 2]].Position[2];

                            V3 = cross(V1, V2);
                            V3 = normalise(V3);

                            vertices[indices[i]].Normal[0] = V3.x;
                            vertices[indices[i]].Normal[1] = V3.y;
                            vertices[indices[i]].Normal[2] = V3.z;

                            vertices[indices[i + 1]].Normal[0] = V3.x;
                            vertices[indices[i + 1]].Normal[1] = V3.y;
                            vertices[indices[i + 1]].Normal[2] = V3.z;

                            vertices[indices[i + 2]].Normal[0] = V3.x;
                            vertices[indices[i + 2]].Normal[1] = V3.y;
                            vertices[indices[i + 2]].Normal[2] = V3.z;
                        }


                    } // SubmeshCount

                    // 다음 프레임
                    assetBlendSpace->mFrame =
                        ((assetBlendSpace->mFrame + 1) < (UINT)assetBlendSpace->mFrameLength) ?
                        (assetBlendSpace->mFrame + 1) : 0;
                }
            }

            iter++;
        }

        SetEvent(hAnimThreadReadEvent);
        ResetEvent(hAnimThreadWriteEvent);
    }
}

DirectXHelper::DirectXHelper()
{
    int Index = 0;

    RTVBackBuffers.resize(PostProcessBackBufferCount);
    DSVBackBuffers.resize(PostProcessBackBufferCount);

    for (Index = 0; Index < RTVBackBuffers.size(); Index++)
    {
        RTVBackBuffers[Index] = new RenderTargetViewBackBuffer;
    }
    for (Index = 0; Index < DSVBackBuffers.size(); Index++)
    {
        DSVBackBuffers[Index] = new DepthStencilViewBackBuffer;
    }
}

DirectXHelper::~DirectXHelper()
{
    int Index = 0;

    for (Index = 0; Index < RTVBackBuffers.size(); Index++)
    {
        delete RTVBackBuffers[Index];
    }
    for (Index = 0; Index < DSVBackBuffers.size(); Index++)
    {
        delete DSVBackBuffers[Index];
    }
}

DWORD DirectXHelper::CreateDeviceAndContext()
{
    ID3D11Device* baseDevice;
    ID3D11DeviceContext* baseDeviceContext;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG_BUILD)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hResult = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,
        0, creationFlags,
        featureLevels, ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION, &baseDevice,
        0, &baseDeviceContext);
    if (FAILED(hResult)) {
        MessageBoxA(0, "D3D11CreateDevice() failed", "Fatal Error", MB_OK);
        return GetLastError();
    }

    // Get 1.1 interface of D3D11 Device and Context
    hResult = baseDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&CreateWindows::getInstance().d3d11Device);
    assert(SUCCEEDED(hResult));
    baseDevice->Release();

    hResult = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&CreateWindows::getInstance().d3d11DeviceContext);
    assert(SUCCEEDED(hResult));
    baseDeviceContext->Release();

#ifdef DEBUG_BUILD
    // Set up debug layer to break on D3D11 errors
    ID3D11Debug* d3dDebug = nullptr;
    d3d11Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug);
    if (d3dDebug)
    {
        ID3D11InfoQueue* d3dInfoQueue = nullptr;
        if (SUCCEEDED(d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
        {
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
            d3dInfoQueue->Release();
        }
        d3dDebug->Release();
    }
#endif

    return (DWORD)0;
}

DWORD DirectXHelper::CreateDXGI()
{
    IDXGIDevice1* dxgiDevice;
    HRESULT hResult = CreateWindows::getInstance().d3d11Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
    assert(SUCCEEDED(hResult));

    IDXGIAdapter* dxgiAdapter;
    hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
    assert(SUCCEEDED(hResult));
    dxgiDevice->Release();

    DXGI_ADAPTER_DESC adapterDesc;
    dxgiAdapter->GetDesc(&adapterDesc);

    OutputDebugStringA("Graphics Device: ");
    OutputDebugStringW(adapterDesc.Description);

    hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&CreateWindows::getInstance().dxgiFactory);
    assert(SUCCEEDED(hResult));
    dxgiAdapter->Release();

    return (DWORD)0;
}

DWORD DirectXHelper::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
    d3d11SwapChainDesc.Width = 0; // use window width
    d3d11SwapChainDesc.Height = 0; // use window height
    d3d11SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    d3d11SwapChainDesc.SampleDesc.Count = 1;
    d3d11SwapChainDesc.SampleDesc.Quality = 0;
    d3d11SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    d3d11SwapChainDesc.BufferCount = 2;
    d3d11SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    d3d11SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    d3d11SwapChainDesc.Flags = 0;

    HRESULT hResult = CreateWindows::getInstance().dxgiFactory->CreateSwapChainForHwnd(
        CreateWindows::getInstance().d3d11Device, 
        *MainHWND, &d3d11SwapChainDesc, 0, 0, &d3d11SwapChain);
    assert(SUCCEEDED(hResult));

    return (DWORD)0;
}

DWORD DirectXHelper::CreateRenderDepthTargets()
{
    ID3D11Texture2D* d3d11FrameBuffer;
    HRESULT hResult = d3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11FrameBuffer);
    assert(SUCCEEDED(hResult));

    hResult = CreateWindows::getInstance().d3d11Device->CreateRenderTargetView(d3d11FrameBuffer, 0, &renderBufferView);
    assert(SUCCEEDED(hResult));

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
    d3d11FrameBuffer->GetDesc(&depthBufferDesc);

    d3d11FrameBuffer->Release();

    depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    hResult = CreateWindows::getInstance().d3d11Device->CreateTexture2D(&depthBufferDesc, NULL, &depthBuffer);
    assert(SUCCEEDED(hResult));

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

    hResult = CreateWindows::getInstance().d3d11Device->CreateDepthStencilView(depthBuffer, &dsvDesc, &depthBufferView);
    assert(SUCCEEDED(hResult) && "Create DSV Failed");


#pragma region Create Deferred RTV

    // Get window dimensions
    int windowWidth, windowHeight;
    float windowAspectRatio;
    {
        RECT clientRect;
        GetClientRect(*MainHWND, &clientRect);
        windowWidth = clientRect.right - clientRect.left;
        windowHeight = clientRect.bottom - clientRect.top;
        windowAspectRatio = (float)windowWidth / (float)windowHeight;
    }

    D3D11_TEXTURE2D_DESC textureDesc;
    HRESULT result;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


    // RTT 디스크립션을 초기화합니다.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // RTT 디스크립션을 세팅합니다.
    textureDesc.Width = windowWidth;
    textureDesc.Height = windowHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0; 

    // 렌더 타겟 뷰에 대한 디스크립션을 설정합니다.
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    // 셰이더 리소스 뷰에 대한 디스크립션을 설정합니다.
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    for (UINT idx = 0; idx < RTVBackBuffers.size(); idx++)
    {
        // RTT를 생성합니다.
        result = 
            CreateWindows::getInstance().d3d11Device->CreateTexture2D(
                &textureDesc, NULL, &RTVBackBuffers[idx]->m_renderTargetTexture);
        assert(SUCCEEDED(result) && "Create Deferred RenderTexture Failed..");

        // 렌더 타겟 뷰를 생성합니다.
        result = CreateWindows::getInstance().d3d11Device->CreateRenderTargetView(
            RTVBackBuffers[idx]->m_renderTargetTexture,
            &renderTargetViewDesc, 
            &RTVBackBuffers[idx]->deferRenderTargetView);
        assert(SUCCEEDED(result) && "Create Deferred RTV Failed..");

        // 셰이더 리소스 뷰를 생성합니다.
        result = CreateWindows::getInstance().d3d11Device->CreateShaderResourceView(
            RTVBackBuffers[idx]->m_renderTargetTexture,
            &shaderResourceViewDesc,
            &RTVBackBuffers[idx]->m_shaderResourceView);
        assert(SUCCEEDED(result) && "Create SRV Failed..");
    }

#pragma endregion

#pragma region Create Deferred DSV

    depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    uint32_t* initData = new uint32_t[depthBufferDesc.Width * depthBufferDesc.Height];
    ZeroMemory(initData, sizeof(uint32_t) * depthBufferDesc.Width * depthBufferDesc.Height);

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = initData;
    data.SysMemPitch = sizeof(uint32_t) * depthBufferDesc.Width;
    data.SysMemSlicePitch = 0;
    
    dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    for (UINT idx = 0; idx < RTVBackBuffers.size(); idx++)
    {
        hResult = CreateWindows::getInstance().d3d11Device->CreateTexture2D(
            &depthBufferDesc, 
            &data, 
            &DSVBackBuffers[idx]->deferDepthBuffer);
        assert(SUCCEEDED(hResult));

        CreateWindows::getInstance().d3d11Device->CreateDepthStencilView(
            DSVBackBuffers[idx]->deferDepthBuffer,
            &dsvDesc, 
            &DSVBackBuffers[idx]->deferDepthBufferView);

        CreateWindows::getInstance().d3d11Device->CreateShaderResourceView(
            DSVBackBuffers[idx]->deferDepthBuffer,
            &srvDesc, 
            &DSVBackBuffers[idx]->m_depthResourceView);
    }

#pragma endregion

    return (DWORD)0;
}


DWORD DirectXHelper::CreateShaders()
{
    Shader* newShader = nullptr;

    HRESULT hResult;

    ID3DBlob* compileErrors;
    ID3DBlob* psBlob;

    std::string root = "shader\\";
    std::string path;
    std::wstring wpath;

    for (const auto& file : directory_iterator(root))
    {
        path = file.path().string();
        wpath = file.path().wstring();

        newShader = new Shader;

        UINT shaderCompileFlags = 0;
        // Compiling with this flag allows debugging shaders with Visual Studio
#if defined(DEBUG_BUILD)
        shaderCompileFlags |= D3DCOMPILE_DEBUG;
#endif

#pragma region CreateVertexShader

        hResult = D3DCompileFromFile(
            wpath.c_str(),
            nullptr,
            nullptr,
            "vs_main",
            "vs_5_0",
            shaderCompileFlags,
            0,
            &newShader->ShaderCode,
            &compileErrors
        );
        if (FAILED(hResult))
        {
            const char* errorString = NULL;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (compileErrors) {
                errorString = (const char*)compileErrors->GetBufferPointer();
            }
            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        hResult = CreateWindows::getInstance().d3d11Device->CreateVertexShader(
            newShader->ShaderCode->GetBufferPointer(), 
            newShader->ShaderCode->GetBufferSize(), 
            nullptr, 
            &newShader->VertexShader
        );
        assert(SUCCEEDED(hResult));

#pragma endregion

#pragma region CreatePixelShader

        hResult = D3DCompileFromFile(
            wpath.c_str(),
            nullptr,
            nullptr,
            "ps_main",
            "ps_5_0",
            shaderCompileFlags,
            0,
            &psBlob,
            &compileErrors
        );
        if (FAILED(hResult))
        {
            const char* errorString = NULL;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (compileErrors) {
                errorString = (const char*)compileErrors->GetBufferPointer();
            }
            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        hResult = CreateWindows::getInstance().d3d11Device->CreatePixelShader(
            psBlob->GetBufferPointer(), 
            psBlob->GetBufferSize(), 
            nullptr, 
            &newShader->PixelShader
        );
        assert(SUCCEEDED(hResult));
        psBlob->Release();

#pragma endregion

        std::string s = Path2FileName(path.c_str());

        MemoryPool::getInstance().AddShader(
            Path2FileName(path.c_str()),
            newShader
        );

    }

    return (DWORD)0;
}

DWORD DirectXHelper::CreateShadowShader()
{
    UINT shaderCompileFlags = 0;
    // Compiling with this flag allows debugging shaders with Visual Studio
#if defined(DEBUG_BUILD)
    shaderCompileFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pBlobVS = NULL;
    ID3DBlob* compileErrors;

    HRESULT hResult = 
        D3DCompileFromFile(
            L"Compute.hlsl", 
            NULL, 
            NULL,
            "cs_main", 
            "cs_5_0", 
            shaderCompileFlags, 
            0, 
            &pBlobVS,
            &compileErrors
        );
    if (FAILED(hResult))
    {
        const char* errorString = NULL;
        if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
            errorString = "Could not compile shader; file not found";
        else if (compileErrors) {
            errorString = (const char*)compileErrors->GetBufferPointer();
        }
        MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    hResult = CreateWindows::getInstance().d3d11Device->CreateComputeShader(
        pBlobVS->GetBufferPointer(), 
        pBlobVS->GetBufferSize(), 
        nullptr, 
        &CreateWindows::getInstance().shadowShader
    );
    assert(SUCCEEDED(hResult));
    pBlobVS->Release();

    // 1. 컴퓨트 셰이더의 입출력으로 사용될 Struct Buffer를 준비한다. 
    //      (D3D11_BUFFER_DESC::MiscFlags 필드에 D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)를 설정
    // 2. 컴퓨트 셰이더의 출력값을 받기 위해 Unordered Access View를 생성한다.
    //      (D3D11_BUFFER_DESC::BindFlags 필드에 D3D11_BIND_UNOREDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE
    // 3. CPU가 Struct Buffer에 UAV에 접근하기 위해 USAGE_STAGING 필드를 제작

    ID3D11Buffer* g_pCBuffer = NULL;
    ID3D11Buffer* g_pBuffer[2]          = { NULL, NULL };
    ID3D11Buffer* g_pReadBackBuffer     = NULL;

    ID3D11ShaderResourceView* g_pSRV[2] = { NULL, NULL };
    ID3D11UnorderedAccessView* g_pUAV[2] = { NULL, NULL };

    struct cbCalcDataCount {
        float4x4 WVP;
    };
    cbCalcDataCount g_cbCalcDataCount = {
        {   1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        }
    };

    D3D11_BUFFER_DESC Desc = {};
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.ByteWidth = sizeof(g_cbCalcDataCount);
    // CPU가 접근, 쓰기 가능 (MAP을 통해 읽기 쓰기가 가능하도록)
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
        &Desc,
        nullptr,
        &g_pCBuffer
    );
    if (FAILED(hResult))
        throw std::runtime_error("CreateBuffer (CBV)");

    // 테스트 초기화용 데이터 (x, y, z, w)
    const UINT VerticesSize = 4000;

    float4* initData = new float4[VerticesSize];
    // 이후 여기에 Vertex Position 넣으면 됨
    for (int i = 0; i < VerticesSize; ++i)
        initData[i] = {1.0f, 1.0f, 1.0f, 1.0f};

    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = VerticesSize * sizeof(float4);
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    Desc.StructureByteStride = sizeof(float4);

    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = initData;
    subResource.SysMemPitch = 0;
    subResource.SysMemSlicePitch = 0;

    // 입력 메모리 생성
    hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
        &Desc,
        &subResource,
        &g_pBuffer[0]
    );

    delete[] initData;
    if (FAILED(hResult))
        throw std::runtime_error("CreateBuffer (INPUT)");

    // 출력 메모리 생성
    hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
        &Desc,
        NULL,
        &g_pBuffer[1]
    );
    if (FAILED(hResult))
        throw std::runtime_error("CreateBuffer (OUTPUT)");

    ZeroMemory(&Desc, sizeof(Desc));

    // 스태깅 메모리 생성
    Desc.ByteWidth = VerticesSize * sizeof(float);
    Desc.Usage = D3D11_USAGE_STAGING;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    Desc.StructureByteStride = sizeof(float);

    hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
        &Desc,
        NULL,
        &g_pReadBackBuffer
    );

    if (FAILED(hResult))
        throw std::runtime_error("CreateBuffer (Staging)");

    // SRV 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC descSRV = {};
    descSRV.Format = DXGI_FORMAT_UNKNOWN;
    descSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    descSRV.Buffer.ElementWidth = VerticesSize;

    hResult = CreateWindows::getInstance().d3d11Device->CreateShaderResourceView(
        g_pBuffer[0],
        &descSRV,
        &g_pSRV[0]
    );

    hResult = CreateWindows::getInstance().d3d11Device->CreateShaderResourceView(
        g_pBuffer[1],
        &descSRV,
        &g_pSRV[1]
    );

    // UAV 생성
    D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV = {};
    descUAV.Format = DXGI_FORMAT_UNKNOWN;
    descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    descUAV.Buffer.NumElements = VerticesSize;

    hResult = CreateWindows::getInstance().d3d11Device->CreateUnorderedAccessView(
        g_pBuffer[0],
        &descUAV,
        &g_pUAV[0]
    );

    hResult = CreateWindows::getInstance().d3d11Device->CreateUnorderedAccessView(
        g_pBuffer[1],
        &descUAV,
        &g_pUAV[1]
    );

    // CBV 바인딩
    CreateWindows::getInstance().d3d11DeviceContext->CSSetConstantBuffers(0, 1, &g_pCBuffer);
    // 셰이더 설정
    CreateWindows::getInstance().d3d11DeviceContext->CSSetShader(
        CreateWindows::getInstance().shadowShader, NULL, 0);

    // 연산 실행
    unsigned int dataCount = VerticesSize;
    bool flag = false;
    ID3D11ShaderResourceView* pViewNULL = NULL;

    //do {
        // 리소스를 변경한다.
        flag = !flag;

        // UAV를 설정하기 위해
        // 현재 SRV 해제
        CreateWindows::getInstance().d3d11DeviceContext->CSSetShaderResources(0, 1, &pViewNULL);
        // UAV 설정
        CreateWindows::getInstance().d3d11DeviceContext->CSSetUnorderedAccessViews(0, 1, &g_pUAV[flag ? 1 : 0], NULL);
        // SRV 설정
        CreateWindows::getInstance().d3d11DeviceContext->CSSetShaderResources(0, 1, &g_pSRV[flag ? 0 : 1]);

        // 상수 버퍼 갱신
        D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };

        hResult = CreateWindows::getInstance().d3d11DeviceContext->Map(
            g_pCBuffer, 
            0, 
            D3D11_MAP_WRITE_DISCARD, 
            0, 
            &MappedResource
        );

        if (FAILED(hResult))
            throw std::runtime_error("");

        memcpy(MappedResource.pData, &g_cbCalcDataCount, sizeof(g_cbCalcDataCount));
        CreateWindows::getInstance().d3d11DeviceContext->Unmap(g_pCBuffer, 0);

        // 디스패치 할 스레드 그룹 수
        unsigned int threadGroup = (dataCount + 255) / 256;
        // 컴퓨트 셰이더 실행
        CreateWindows::getInstance().d3d11DeviceContext->Dispatch(threadGroup, 1, 1);

    //    // 다음 계산할 데이터 수
    //    dataCount = threadGroup;
    //} while (dataCount > 1);

    // 계산 결과를 리드 백 버퍼 리소스로 다운로드
    
    // 1 Dimension
    /* 
    d3d11DeviceContext->CopyResource(
        g_pReadBackBuffer, 
        g_pBuffer[flag ? 1 : 0]
    );
    */

    // 2 Dimension
    D3D11_BOX box = {
        0, 0, 0, sizeof(float), 1, 1
    };
    CreateWindows::getInstance().d3d11DeviceContext->CopySubresourceRegion(
        g_pReadBackBuffer,
        0, 0, 0, 0,
        g_pBuffer[flag ? 1 : 0],
        0,
        &box
    );

    // 결과를 읽어 들임
    ZeroMemory(&MappedResource, sizeof(MappedResource));

    hResult = CreateWindows::getInstance().d3d11DeviceContext->Map(
        g_pReadBackBuffer,
        0,
        D3D11_MAP_READ,
        0,
        &MappedResource
    );

    if (FAILED(hResult))
        throw std::runtime_error("");

    float* sum = ((float*)MappedResource.pData);
    CreateWindows::getInstance().d3d11DeviceContext->Unmap(g_pReadBackBuffer, 0);

    return (DWORD)0;
}

DWORD DirectXHelper::CreateInputDescriptor()
{
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORM", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "INSTANCE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        { "INSTANCE", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        { "INSTANCE", 2, DXGI_FORMAT_R32G32B32_FLOAT, 1, 24, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    };

    int iNumElements = sizeof(inputElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);

    HRESULT hResult;

    std::unordered_map<std::string, Shader*>::iterator iter;
    std::unordered_map<std::string, Shader*>::iterator end;

    iter   = MemoryPool::getInstance()._GLOBAL_SHADER_POOL.begin();
    end = MemoryPool::getInstance()._GLOBAL_SHADER_POOL.end();

    while(iter != end)
    {
        hResult = CreateWindows::getInstance().d3d11Device->CreateInputLayout(
            inputElementDesc,
            iNumElements,
            iter->second->ShaderCode->GetBufferPointer(),
            iter->second->ShaderCode->GetBufferSize(),
            &CreateWindows::getInstance().blinnPhongInputLayout
        );
        assert(SUCCEEDED(hResult));
        iter->second->ShaderCode->Release();

        iter++;
    }

    return (DWORD)0;
}

DWORD DirectXHelper::CreateSampler()
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    CreateWindows::getInstance().d3d11Device->CreateSamplerState(
        &samplerDesc, &CreateWindows::getInstance().samplerState);

    return (DWORD)0;
}

DWORD DirectXHelper::LoadObject()
{
    std::unordered_map<std::string, ObjectData*>::iterator iter =
        MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.begin();

    UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;

    UINT submeshIDX = 0;

    UINT VerticesOffsetStack = 0;
    UINT IndicesOffsetStack = 0;

    D3D11_BUFFER_DESC instanceBufferDesc = {};
    D3D11_SUBRESOURCE_DATA instanceSubresourceData = {};
    std::vector<Instance> instBufferDatas;

    HRESULT hResult;

    while (iter != MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.end())
    {
        VerticesOffsetStack = 0;
        IndicesOffsetStack = 0;
        
        instanceBufferDesc = {};

        instanceBufferDesc.ByteWidth =      64 * sizeof(Instance);
        instanceBufferDesc.Usage =          D3D11_USAGE_DYNAMIC;
        instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        instanceBufferDesc.BindFlags =      D3D11_BIND_VERTEX_BUFFER;

        iter->second->transform->Name = iter->first;
        listbox->AppendListBoxMessage(iter->first + "_0");

        // 생성한 오브젝트는 "오브젝트 이름 _0 (첫 번째 인스턴스)" 이름으로
        // InstanceBuffer에 생성이 되어집니다.
        iter->second->transform->InstanceBuffer.insert(
            std::pair<std::string, Transform::transform*>(
                std::string(iter->first + "_0"),
                new Transform::transform()
            )
        );

        for (const std::pair<std::string, Transform::transform*>& inst : iter->second->transform->InstanceBuffer)
        {
            instBufferDatas.push_back(*inst.second->Matrix);
        }

        instanceSubresourceData = { instBufferDatas.data() };

        hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
            &instanceBufferDesc,
            &instanceSubresourceData,
            &iter->second->InstanceBuffer);
        assert(SUCCEEDED(hResult));

        for (submeshIDX = 0; submeshIDX < iter->second->meshDatas.size(); submeshIDX++)
        {
            iter->second->Stride            = sizeof(Vertex);
            iter->second->VerticesOffset    = 0;
            iter->second->IndicesOffset     = 0;
            iter->second->NumIndices        = iter->second->meshDatas[submeshIDX].Indices32.size();

#pragma region 버텍스, 인덱스 버퍼 생성

            D3D11_BUFFER_DESC vertexBufferDesc = {};
            vertexBufferDesc.ByteWidth = 
                iter->second->meshDatas[submeshIDX].Vertices.size() * sizeof(Vertex);
            // DYNAMIC => CPU에서 접근 가능
            vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            D3D11_SUBRESOURCE_DATA vertexSubresourceData = { 
                iter->second->meshDatas[submeshIDX].Vertices.data() };

            hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
                &vertexBufferDesc,
                &vertexSubresourceData,
                &iter->second->VertexBuffer[submeshIDX]);

            assert(SUCCEEDED(hResult));

            D3D11_BUFFER_DESC indexBufferDesc = {};
            indexBufferDesc.ByteWidth = iter->second->meshDatas[submeshIDX].Indices32.size() * sizeof(uint32_t);
            indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

            D3D11_SUBRESOURCE_DATA indexSubresourceData = { 
                iter->second->meshDatas[submeshIDX].Indices32.data() };

            hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
                &indexBufferDesc,
                &indexSubresourceData,
                &iter->second->IndexBuffer[submeshIDX]);
            assert(SUCCEEDED(hResult));

            VerticesOffsetStack += iter->second->meshDatas[submeshIDX].Vertices.size();
            IndicesOffsetStack  += iter->second->meshDatas[submeshIDX].Indices32.size();

#pragma endregion

#pragma region BoundBox 버텍스, 인덱스 버퍼 생성

            vertexBufferDesc.ByteWidth =
                iter->second->meshBoundBoxData.Vertices.size() * sizeof(Vertex);
            // DYNAMIC => CPU에서 접근 가능
            vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            vertexSubresourceData = {
                iter->second->meshBoundBoxData.Vertices.data() };

            hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
                &vertexBufferDesc,
                &vertexSubresourceData,
                &iter->second->VertexBoundBoxBuffer
            );

            assert(SUCCEEDED(hResult));

            indexBufferDesc.ByteWidth = iter->second->meshBoundBoxData.Indices32.size() * sizeof(uint32_t);
            indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

            indexSubresourceData = {
                iter->second->meshBoundBoxData.Indices32.data() };

            hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
                &indexBufferDesc,
                &indexSubresourceData,
                &iter->second->IndexBoundBoxBuffer);
            assert(SUCCEEDED(hResult));

            VerticesOffsetStack += iter->second->meshBoundBoxData.Vertices.size();
            IndicesOffsetStack += iter->second->meshBoundBoxData.Indices32.size();

#pragma endregion
        }

        iter++;
    }

    return (DWORD)0;
}

DWORD DirectXHelper::LoadTexture()
{
    // Load Image
    int texWidth, texHeight, texNumChannels;
    int texForceNumChannels = 4;
    unsigned char* testTextureBytes;
    int texBytesPerRow;

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA textureSubresourceData = {};

    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* newShader;
    
    std::set<const char*>::iterator texIter =
        MemoryPool::getInstance().TexturePaths.begin();
    std::set<const wchar_t*>::iterator wtexIter =
        MemoryPool::getInstance().TextureWildPaths.begin();

    std::string TexName;
    std::string WarningWhat;

    for (; texIter != MemoryPool::getInstance().TexturePaths.end(); texIter++)
    {
        TexName     = "";
        WarningWhat = "";

        testTextureBytes = stbi_load(*texIter, &texWidth, &texHeight,
            &texNumChannels, texForceNumChannels);

        if (!testTextureBytes)
        {
            WarningWhat = "Can't find image.(" + std::string(*texIter) + ")";

            //MessageBoxA(*MainHWND, WarningWhat.c_str(), "WARNING", MB_OK);
            continue;
        }

        texBytesPerRow      = 4 * texWidth;

        textureDesc.Width   = texWidth;
        textureDesc.Height  = texHeight;

        textureSubresourceData.pSysMem      = testTextureBytes;
        textureSubresourceData.SysMemPitch  = texBytesPerRow;

        CreateWindows::getInstance().d3d11Device->CreateTexture2D(&textureDesc, &textureSubresourceData, &texture);

        CreateWindows::getInstance().d3d11Device->CreateShaderResourceView(texture, nullptr, &newShader);

        texture->Release();
        free(testTextureBytes);

        TexName = Path2FileName(*texIter);

        MemoryPool::getInstance().AddTexture(TexName, newShader);
        MemoryPool::getInstance().TextureNames.push_back(TexName);
    }

    for (; wtexIter != MemoryPool::getInstance().TextureWildPaths.end(); wtexIter++)
    {
        TexName     = "";
        WarningWhat = "";

        std::wstring ws(*wtexIter);
        std::string  str(ws.begin(), ws.end());

        testTextureBytes = stbi_load(str.c_str(), &texWidth, &texHeight,
            &texNumChannels, texForceNumChannels);
        
        if (!testTextureBytes)
        {
            WarningWhat = "Can't find image.(" + str + ")";

            MessageBoxA(*MainHWND, WarningWhat.c_str(), "WARNING", MB_OK);
            continue;
        }

        texBytesPerRow = 4 * texWidth;

        textureDesc.Width = texWidth;
        textureDesc.Height = texHeight;

        textureSubresourceData.pSysMem = testTextureBytes;
        textureSubresourceData.SysMemPitch = texBytesPerRow;

        CreateWindows::getInstance().d3d11Device->CreateTexture2D(&textureDesc, &textureSubresourceData, &texture);

        CreateWindows::getInstance().d3d11Device->CreateShaderResourceView(texture, nullptr, &newShader);
        texture->Release();

        free(testTextureBytes);

        TexName = Path2FileName(str.c_str());

        MemoryPool::getInstance().AddTexture(TexName, newShader);
        MemoryPool::getInstance().TextureNames.push_back(TexName);
    }

    return (DWORD)0;
}

DWORD DirectXHelper::CreateLightCBV()
{
    D3D11_BUFFER_DESC constantBufferDesc = {};
    // ByteWidth must be a multiple of 16, per the docs
    constantBufferDesc.ByteWidth = sizeof(LightVSConstants) + 0xf & 0xfffffff0;
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hResult;

    for (auto& obj : MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_)
    {
        hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
            &constantBufferDesc, 
            nullptr, 
            &obj.second->LightVSConstantBuffer
        );
        assert(SUCCEEDED(hResult));
    }

    return (DWORD)0;
}

DWORD DirectXHelper::CreateVertexShaderConstantBuffer()
{
    D3D11_BUFFER_DESC ConstantBufferDesc = {};
    // ByteWidth must be a multiple of 16, per the docs
    ConstantBufferDesc.ByteWidth = sizeof(BlinnPhongVSConstants) + 0xf & 0xfffffff0;
    ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_BUFFER_DESC ScreenSizeBufferDesc = {};
    // ByteWidth must be a multiple of 16, per the docs
    ScreenSizeBufferDesc.ByteWidth = sizeof(ScreenSizeVSConstants) + 0xf & 0xfffffff0;
    ScreenSizeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    ScreenSizeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ScreenSizeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hResult;

    for (auto& obj : MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_)
    {
        hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
            &ConstantBufferDesc,
            nullptr, 
            &obj.second->ObjectVSConstantBuffer
        );
        assert(SUCCEEDED(hResult));

        hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
            &ScreenSizeBufferDesc,
            nullptr,
            &obj.second->ScreenSizeVSConstantBuffer
        );
        assert(SUCCEEDED(hResult));
    }

    return (DWORD)0;
}

DWORD DirectXHelper::CreatePixelShaderConstantBuffer()
{
    D3D11_BUFFER_DESC constantBufferDesc = {};
    // ByteWidth must be a multiple of 16, per the docs
    constantBufferDesc.ByteWidth = sizeof(LightData) + 0xf & 0xfffffff0;
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hResult;
    
    for (auto& obj : MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_)
    {
        hResult = CreateWindows::getInstance().d3d11Device->CreateBuffer(
            &constantBufferDesc, 
            nullptr, 
            &obj.second->ObjectPSConstantBuffer
        );
        assert(SUCCEEDED(hResult));
    }

    return (DWORD)0;
}

DWORD DirectXHelper::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = TRUE;

    CreateWindows::getInstance().d3d11Device->CreateRasterizerState(&rasterizerDesc, &CreateWindows::getInstance().rasterizerState);

    return (DWORD)0;
}

DWORD DirectXHelper::CreateQuary()
{
    // To force initial perspectiveMat calculation
    global_windowDidResize = true;

    LARGE_INTEGER perfCount;
    QueryPerformanceCounter(&perfCount);
    GlobalTime::getInstance().startPerfCount = perfCount.QuadPart;

    LARGE_INTEGER perfFreq;
    QueryPerformanceFrequency(&perfFreq);
    GlobalTime::getInstance().perfCounterFrequency = perfFreq.QuadPart;

    return (DWORD)0;
}

DWORD DirectXHelper::CreateDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    CreateWindows::getInstance().d3d11Device->CreateDepthStencilState(&depthStencilDesc, &CreateWindows::getInstance().depthStencilEnableState);

    depthStencilDesc = {};
    depthStencilDesc.DepthEnable = FALSE;

    CreateWindows::getInstance().d3d11Device->CreateDepthStencilState(&depthStencilDesc, &CreateWindows::getInstance().depthStencilDisableState);

    return (DWORD)0;
}

DWORD DirectXHelper::Init(const HWND& mHWND)
{
    this->MainHWND = &mHWND;

    if (!this->MainHWND)
        throw std::runtime_error("HWND IS NULL");

    CreateDeviceAndContext();
    CreateDXGI();
    CreateSwapChain();
    CreateRenderDepthTargets();

    CreateShaders();
    CreateInputDescriptor();

    CreateShadowShader();

    CreateSampler();
    LoadObject();
    LoadTexture();

    CreateLightCBV();
    CreateVertexShaderConstantBuffer();
    CreatePixelShaderConstantBuffer();

    CreateRasterizerState();
    CreateDepthStencilState();

    CreateQuary();

    hAnimThreadReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    WaitForSingleObject(hAnimThreadReadEvent, INFINITE);

    hAnimThreadWriteEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    WaitForSingleObject(hAnimThreadWriteEvent, INFINITE);

    hAnimThread = CreateThread(NULL, 0, AnimationThread, &hAnimThreadParam, 0, &hAnimThreadID);

    return (DWORD)0;
}

DWORD DirectXHelper::Update()
{
    if (CreateWindows::getInstance().isPaused)
        return -1;

#pragma region Thread

    // 애니메이션 쓰레드 
    SetEvent(hAnimThreadWriteEvent);
    WaitForSingleObject(hAnimThreadReadEvent, INFINITE);

#pragma endregion

#pragma region Windows

    // 윈도우 정보 로드
    // 백 버퍼 초기화
    UpdateWindow();

#pragma endregion
    
    UpdateWorldViewProjection();
    UpdateLight();
    MousePickingSetting();

    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (FLOAT)windowWidth, (FLOAT)windowHeight, 0.0f, 1.0f };
    CreateWindows::getInstance().d3d11DeviceContext->RSSetViewports(1, &viewport);

    CreateWindows::getInstance().d3d11DeviceContext->RSSetState(CreateWindows::getInstance().rasterizerState);

#pragma region UpdateObjectResourceBuffer

    UINT SubmeshIDX = 0;
    std::unordered_map<std::string, ObjectData*>::iterator objIter =
        MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.begin();
    std::unordered_map<std::string, ID3D11ShaderResourceView*>::iterator texIter =
        MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_.begin();

#pragma region Render Deferred Shader

    RecordRTVandDSV(
        RTVBackBuffers[0]->deferRenderTargetView,
        DSVBackBuffers[0]->deferDepthBufferView
    );

    for (; objIter != MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.end(); objIter++)
    {
        if (!objIter->second)
            continue;

        SubmeshIDX = 0;

        if (objIter->second->bIsEnableDepthStencil)
            CreateWindows::getInstance().d3d11DeviceContext->OMSetDepthStencilState(
                CreateWindows::getInstance().depthStencilEnableState, 0);
        else 
            CreateWindows::getInstance().d3d11DeviceContext->OMSetDepthStencilState(
                CreateWindows::getInstance().depthStencilDisableState, 0);

        UpdateInstanceBuffer(objIter->second);

        for (; SubmeshIDX < objIter->second->SubmeshCount; SubmeshIDX++)
        {
            UpdateAnimationPose(objIter->second, SubmeshIDX);

            ID3D11Buffer* bufferPointers[2] = {
                    objIter->second->VertexBuffer[SubmeshIDX],
                    objIter->second->InstanceBuffer,
            };

            unsigned int bufferStrides[2] = {
                    objIter->second->Stride,
                    sizeof(Instance),
            };

            unsigned int bufferOffsets[2] = {
                0,
                0,
            };

            CreateWindows::getInstance().d3d11DeviceContext->IASetVertexBuffers(
                0,
                2,
                bufferPointers,
                bufferStrides,
                bufferOffsets
            );
            CreateWindows::getInstance().d3d11DeviceContext->IASetIndexBuffer(
                objIter->second->IndexBuffer[SubmeshIDX],
                DXGI_FORMAT_R32_UINT,
                objIter->second->IndicesOffset
            );

            {

                CreateWindows::getInstance().d3d11DeviceContext->IASetInputLayout(CreateWindows::getInstance().blinnPhongInputLayout);
                CreateWindows::getInstance().d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                Shader* sd =
                    MemoryPool::getInstance()._GLOBAL_SHADER_POOL[
                        objIter->second->ShaderID
                    ];

                CreateWindows::getInstance().d3d11DeviceContext->VSSetShader(
                    sd->VertexShader,
                    nullptr, 0
                );
                CreateWindows::getInstance().d3d11DeviceContext->PSSetShader(
                    sd->PixelShader,
                    nullptr, 0
                );

                RecordMaterial(objIter->second, SubmeshIDX);
                
                UpdateVertexShderBuffer(objIter->second);
                UpdatePixelShderBuffer(objIter->second);

                CreateWindows::getInstance().d3d11DeviceContext->DrawIndexedInstanced(
                    objIter->second->NumIndices,
                    objIter->second->transform->InstanceCount,
                    0,
                    0,
                    0
                );
            }
        } // SubmeshIDX

    } // objIter

#pragma endregion

#pragma region Render Sobel Render

// Screen Render Rect
    objIter = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.find("ScreenBox");

    RecordRTVandDSV(
        RTVBackBuffers[1]->deferRenderTargetView,
        DSVBackBuffers[1]->deferDepthBufferView
    );

    // 2D Screen을 렌더하기 위해 z buffer를 끈다
    CreateWindows::getInstance().d3d11DeviceContext->OMSetDepthStencilState(
        CreateWindows::getInstance().depthStencilDisableState, 0);

    UpdateInstanceBuffer(objIter->second);
    UpdateAnimationPose(objIter->second, 0);

    ID3D11Buffer* bufferPointers2[2] = {
            objIter->second->VertexBuffer[0],
            objIter->second->InstanceBuffer,
    };

    unsigned int bufferStrides2[2] = {
            objIter->second->Stride,
            sizeof(Instance),
    };

    unsigned int bufferOffsets2[2] = {
        0,
        0,
    };

    CreateWindows::getInstance().d3d11DeviceContext->IASetVertexBuffers(
        0,
        2,
        bufferPointers2,
        bufferStrides2,
        bufferOffsets2
    );
    CreateWindows::getInstance().d3d11DeviceContext->IASetIndexBuffer(
        objIter->second->IndexBuffer[0],
        DXGI_FORMAT_R32_UINT,
        objIter->second->IndicesOffset
    );

    CreateWindows::getInstance().d3d11DeviceContext->IASetInputLayout(CreateWindows::getInstance().blinnPhongInputLayout);

    CreateWindows::getInstance().d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CreateWindows::getInstance().d3d11DeviceContext->VSSetShader(
        MemoryPool::getInstance()._GLOBAL_SHADER_POOL["Sobel.hlsl"]->VertexShader,
        nullptr, 0
    );
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShader(
        MemoryPool::getInstance()._GLOBAL_SHADER_POOL["Sobel.hlsl"]->PixelShader,
        nullptr, 0
    );

    // Update Final PostProcess Screen (Deferred Screen)
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(0, 1,
        &RTVBackBuffers[0]->m_shaderResourceView);
    CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(0, 1, &CreateWindows::getInstance().samplerState);

    UpdateVertexShderBuffer(objIter->second);
    UpdatePixelShderBuffer(objIter->second);
    UpdateBlurShderBuffer(objIter->second, true);

    CreateWindows::getInstance().d3d11DeviceContext->DrawIndexedInstanced(
        objIter->second->NumIndices,
        objIter->second->transform->InstanceCount,
        0,
        0,
        0
    );

#pragma endregion

#pragma region Render Blur Render

    // Screen Render Rect
    objIter = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.find("ScreenBox");

    RecordRTVandDSV(
        RTVBackBuffers[2]->deferRenderTargetView,
        DSVBackBuffers[2]->deferDepthBufferView
    );

    // 2D Screen을 렌더하기 위해 z buffer를 끈다
    CreateWindows::getInstance().d3d11DeviceContext->OMSetDepthStencilState(
        CreateWindows::getInstance().depthStencilDisableState, 0);

    UpdateInstanceBuffer(objIter->second);
    UpdateAnimationPose(objIter->second, 0);

    ID3D11Buffer* bufferPointers3[2] = {
            objIter->second->VertexBuffer[0],
            objIter->second->InstanceBuffer,
    };

    unsigned int bufferStrides3[2] = {
            objIter->second->Stride,
            sizeof(Instance),
    };

    unsigned int bufferOffsets3[2] = {
        0,
        0,
    };

    CreateWindows::getInstance().d3d11DeviceContext->IASetVertexBuffers(
        0,
        2,
        bufferPointers3,
        bufferStrides3,
        bufferOffsets3
    );
    CreateWindows::getInstance().d3d11DeviceContext->IASetIndexBuffer(
        objIter->second->IndexBuffer[0],
        DXGI_FORMAT_R32_UINT,
        objIter->second->IndicesOffset
    );

    CreateWindows::getInstance().d3d11DeviceContext->IASetInputLayout(CreateWindows::getInstance().blinnPhongInputLayout);

    CreateWindows::getInstance().d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CreateWindows::getInstance().d3d11DeviceContext->VSSetShader(
        MemoryPool::getInstance()._GLOBAL_SHADER_POOL["Blur.hlsl"]->VertexShader,
        nullptr, 0
    );
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShader(
        MemoryPool::getInstance()._GLOBAL_SHADER_POOL["Blur.hlsl"]->PixelShader,
        nullptr, 0
    );

    // Update Final PostProcess Screen (Deferred Screen)
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(0, 1,
        &RTVBackBuffers[0]->m_shaderResourceView);
    CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(0, 1, &CreateWindows::getInstance().samplerState);

    // Update Final PostProcess Screen (Blur Screen)
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(1, 1,
        &RTVBackBuffers[1]->m_shaderResourceView);
    CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(1, 1, &CreateWindows::getInstance().samplerState);

    // Update Final PostProcess Screen (Deferred Depth Screen)
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(2, 1,
        &DSVBackBuffers[0]->m_depthResourceView);
    CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(2, 1, &CreateWindows::getInstance().samplerState);

    UpdateVertexShderBuffer(objIter->second);
    UpdatePixelShderBuffer(objIter->second);
    UpdateBlurShderBuffer(objIter->second, true);

    CreateWindows::getInstance().d3d11DeviceContext->DrawIndexedInstanced(
        objIter->second->NumIndices,
        objIter->second->transform->InstanceCount,
        0,
        0,
        0
    );

#pragma endregion



#pragma region Render Final PostProcess

    // Screen Render Rect
    objIter = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_.find("ScreenBox");

    RecordRTVandDSV(
        renderBufferView,
        depthBufferView
    );

    // 2D Screen을 렌더하기 위해 z buffer를 끈다
    CreateWindows::getInstance().d3d11DeviceContext->OMSetDepthStencilState(
        CreateWindows::getInstance().depthStencilDisableState, 0);

    UpdateInstanceBuffer(objIter->second);
    UpdateAnimationPose(objIter->second, 0);

    ID3D11Buffer* bufferPointers1[2] = {
            objIter->second->VertexBuffer[0],
            objIter->second->InstanceBuffer,
    };

    unsigned int bufferStrides1[2] = {
            objIter->second->Stride,
            sizeof(Instance),
    };

    unsigned int bufferOffsets1[2] = {
        0,
        0,
    };

    CreateWindows::getInstance().d3d11DeviceContext->IASetVertexBuffers(
        0,
        2,
        bufferPointers1,
        bufferStrides1,
        bufferOffsets1
    );
    CreateWindows::getInstance().d3d11DeviceContext->IASetIndexBuffer(
        objIter->second->IndexBuffer[0],
        DXGI_FORMAT_R32_UINT,
        objIter->second->IndicesOffset
    );

    CreateWindows::getInstance().d3d11DeviceContext->IASetInputLayout(CreateWindows::getInstance().blinnPhongInputLayout);

    CreateWindows::getInstance().d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CreateWindows::getInstance().d3d11DeviceContext->VSSetShader(
        MemoryPool::getInstance()._GLOBAL_SHADER_POOL["RenderScreen.hlsl"]->VertexShader,
        nullptr, 0
    );
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShader(
        MemoryPool::getInstance()._GLOBAL_SHADER_POOL["RenderScreen.hlsl"]->PixelShader,
        nullptr, 0
    );

    // Update Final PostProcess Screen (Deferred Screen)
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(0, 1, 
        &RTVBackBuffers[2]->m_shaderResourceView);
    CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(0, 1, &CreateWindows::getInstance().samplerState);

    UpdateVertexShderBuffer(objIter->second);
    UpdatePixelShderBuffer(objIter->second);

    CreateWindows::getInstance().d3d11DeviceContext->DrawIndexedInstanced(
        objIter->second->NumIndices,
        objIter->second->transform->InstanceCount,
        0,
        0,
        0
    );

#pragma endregion

    d3d11SwapChain->Present(1, 0);

    return (DWORD)0;
}

DWORD DirectXHelper::Destroy()
{
    return 0;
}


void DirectXHelper::UpdateWindow()
{
    // Get window dimensions
    float windowAspectRatio;
    {
        RECT clientRect;
        GetClientRect(*MainHWND, &clientRect);
        windowWidth = clientRect.right - clientRect.left;
        windowHeight = clientRect.bottom - clientRect.top;
        windowAspectRatio = (float)windowWidth / (float)windowHeight;
    }

    if (global_windowDidResize)
    {
        CreateWindows::getInstance().d3d11DeviceContext->OMSetRenderTargets(0, 0, 0);
        renderBufferView->Release();
        depthBufferView->Release();

        HRESULT res = d3d11SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        assert(SUCCEEDED(res));

        CreateRenderDepthTargets();
        perspectiveMat =
            makePerspectiveMat(
                windowAspectRatio,
                degreesToRadians(60.0f),
                0.1f,
                1000.f
            );

        global_windowDidResize = false;
    }
}

void DirectXHelper::UpdateWorldViewProjection()
{
    cubeModelViewMat =
        CameraPool::getInstance().getCamera().getViewMat();
    inverseModelViewMat =
        CameraPool::getInstance().getCamera().getInvViewMat();
    cubeNormalMat =
        float4x4ToFloat3x3(transpose(inverseModelViewMat));
}

void DirectXHelper::UpdateInstanceBuffer(ObjectData* obj)
{
    std::vector<Instance> instBufferDatas;

    obj->transform->InstanceDirty = false;

    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
    HRESULT res = CreateWindows::getInstance().d3d11DeviceContext->Map(
        obj->InstanceBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedSubresource
    );

    if (obj->objType == ObjectType::FBX_SKINNED_TYPE)
    {
        Instance totalInstance = {};
        Instance parentInstance = {};
        DynamicObjectData* dynamicData = reinterpret_cast<DynamicObjectData*>(obj);

        // 부모 변형 매트릭스를 얻어와서
        // (단 부모 오브젝트 중 Instance Index가 0인 인스턴스를 기준으로 변형 매트릭스를 만듦)
        parentInstance = dynamicData->getParentInstance();

        // 모든 인스턴스에 적용
        for (const std::pair<std::string, Transform::transform*> inst : obj->transform->InstanceBuffer)
        {
            totalInstance.Position[0] = parentInstance.Position[0] + inst.second->Matrix->Position[0];
            totalInstance.Position[1] = parentInstance.Position[1] + inst.second->Matrix->Position[1];
            totalInstance.Position[2] = parentInstance.Position[2] + inst.second->Matrix->Position[2];

            totalInstance.Rotation[0] = parentInstance.Rotation[0] + inst.second->Matrix->Rotation[0];
            totalInstance.Rotation[1] = parentInstance.Rotation[1] + inst.second->Matrix->Rotation[1];
            totalInstance.Rotation[2] = parentInstance.Rotation[2] + inst.second->Matrix->Rotation[2];

            totalInstance.Scale[0] = parentInstance.Scale[0] + inst.second->Matrix->Scale[0];
            totalInstance.Scale[1] = parentInstance.Scale[1] + inst.second->Matrix->Scale[1];
            totalInstance.Scale[2] = parentInstance.Scale[2] + inst.second->Matrix->Scale[2];

            instBufferDatas.push_back(totalInstance);
        }
    }
    else
    {
        for (const std::pair<std::string, Transform::transform*>& inst : obj->transform->InstanceBuffer)
        {
            if (inst.second)
                instBufferDatas.push_back(*inst.second->Matrix);
        }
    }

    memcpy(
        mappedSubresource.pData,
        instBufferDatas.data(),
        sizeof(Instance) * obj->transform->InstanceCount
    );

    CreateWindows::getInstance().d3d11DeviceContext->Unmap(obj->InstanceBuffer, 0);
}

void DirectXHelper::UpdateAnimationPose(ObjectData* obj, UINT SubmeshIDX)
{
    if (obj->objType == ObjectType::FBX_SKINNED_TYPE)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource;

        HRESULT hr = CreateWindows::getInstance().d3d11DeviceContext->Map(
            obj->VertexBuffer[SubmeshIDX],
            0,
            D3D11_MAP_WRITE_DISCARD,
            0,
            &mappedSubresource
        );

        UINT VertSize = obj->meshDatas[SubmeshIDX].Vertices.size() * sizeof(Vertex);

        memcpy(
            mappedSubresource.pData,
            obj->GetAsset()->mAnimBuffer[SubmeshIDX].data(),
            VertSize
        );

        CreateWindows::getInstance().d3d11DeviceContext->Unmap(obj->VertexBuffer[SubmeshIDX], 0);
    }
}

// Update vertex shader constant buffer
void DirectXHelper::UpdateVertexShderBuffer(ObjectData* obj)
{
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
    CreateWindows::getInstance().d3d11DeviceContext->Map(
        obj->ObjectVSConstantBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedSubresource
    );

    BlinnPhongVSConstants* constants =
        (BlinnPhongVSConstants*)(mappedSubresource.pData);

    constants->modelViewProj = cubeModelViewMat * perspectiveMat;
    constants->modelView = cubeModelViewMat;
    constants->normalMatrix = cubeNormalMat;
    constants->cameraPosition = CameraPool::getInstance().getCamera().GetPosition();

    CreateWindows::getInstance().d3d11DeviceContext->Unmap(
        obj->ObjectVSConstantBuffer, 0
    );

    CreateWindows::getInstance().d3d11DeviceContext->VSSetConstantBuffers(
        0, 1, &obj->ObjectVSConstantBuffer
    );
    CreateWindows::getInstance().d3d11DeviceContext->PSSetConstantBuffers(
        0, 1, &obj->ObjectPSConstantBuffer
    );
}

// Update pixel shader constant buffer
void DirectXHelper::UpdatePixelShderBuffer(ObjectData* obj)
{
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
    CreateWindows::getInstance().d3d11DeviceContext->Map(
        obj->ObjectPSConstantBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedSubresource
    );

    LightData* gLightData = &MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_;

    memcpy(
        mappedSubresource.pData,
        gLightData,
        sizeof(LightData)
    );

    CreateWindows::getInstance().d3d11DeviceContext->Unmap(obj->ObjectPSConstantBuffer, 0);
}

void DirectXHelper::UpdateBlurShderBuffer(ObjectData* obj, bool isWidth)
{
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
    CreateWindows::getInstance().d3d11DeviceContext->Map(
        obj->ScreenSizeVSConstantBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedSubresource
    );

    ScreenSizeVSConstants* constants =
        (ScreenSizeVSConstants*)(mappedSubresource.pData);

    if (isWidth)
        constants->screenWidth = windowWidth;
    else
        constants->screenWidth = windowHeight;

    CreateWindows::getInstance().d3d11DeviceContext->Unmap(
        obj->ScreenSizeVSConstantBuffer, 0
    );

    CreateWindows::getInstance().d3d11DeviceContext->VSSetConstantBuffers(
        1, 0, &obj->ScreenSizeVSConstantBuffer
    );
}

void DirectXHelper::UpdateLight()
{
    int dirLightNum =
        MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_.dirLightNum;
    int pointLightNum =
        MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_.pointLightNum;

    //float lightRotation = 
    //    -0.3f * 
    //    (float)(M_PI * GlobalTime::getInstance().currentTimeInSeconds);

    DirectionalLight* dLight;
    for (int i = 0; i < dirLightNum; i++)
    {
        dLight = &MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_.dirLight[i];

        //lightRotation += 0.5f * i; // Add an offset so lights have different phases
    }

    PointLight* pLight;
    for (int i = 0; i < pointLightNum; i++)
    {
        pLight = &MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_.pointLights[i];

        //lightRotation += 0.5f * i; // Add an offset so lights have different phases

        pLight->lightViewMatrix =
            translationMat(pLight->posEye.xyz) *
            CameraPool::getInstance().getCamera().getViewMat();

        //pLight->posEye =
        //    pLight->lightViewMatrix.cols[3];
    }
}

void DirectXHelper::MousePickingSetting()
{
    RECT WindowSize = {};
    POINT MousePos = {};
    GetWindowRect(*MainHWND, &WindowSize);
    GetCursorPos(&MousePos);

    ScreenToClient(*MainHWND, &MousePos);

    float w = (MousePos.x / (float)(WindowSize.right - WindowSize.left)) * 2.0f - 1.0f;
    float h = (MousePos.y / (float)(WindowSize.bottom - WindowSize.top)) * 2.0f - 1.0f;
    float z = 1.0f - pow(w * w + h * h, 0.5);

    float4x4 View = CameraPool::getInstance().getCamera().getViewMat();

    DirectX::XMMATRIX toLocal = {
        View.m[0][0], View.m[1][0], View.m[2][0], View.m[3][0],
        View.m[0][1], View.m[1][1], View.m[2][1], View.m[3][1],
        View.m[0][2], View.m[1][2], View.m[2][2], View.m[3][2],
        View.m[0][3], View.m[1][3], View.m[2][3], View.m[3][3],
    };
    DirectX::XMMATRIX Persp = {
    perspectiveMat.m[0][0], perspectiveMat.m[1][0], perspectiveMat.m[2][0], perspectiveMat.m[3][0],
    perspectiveMat.m[0][1], perspectiveMat.m[1][1], perspectiveMat.m[2][1], perspectiveMat.m[3][1],
    perspectiveMat.m[0][2], perspectiveMat.m[1][2], perspectiveMat.m[2][2], perspectiveMat.m[3][2],
    perspectiveMat.m[0][3], perspectiveMat.m[1][3], perspectiveMat.m[2][3], perspectiveMat.m[3][3],
    };

    DirectX::XMVECTOR transposeMat = DirectX::XMMatrixDeterminant(toLocal);
    toLocal = DirectX::XMMatrixInverse(&transposeMat, toLocal);

    // toLocal = DirectX::XMMatrixMultiply(toLocal, Persp);

    rayOrigin = { 0.0f, 0.0f, 0.0f, 1.0f };
    rayDir = { w + 0.0001f, h + 0.0001f, -z + 0.0001f, 1.0f };

    // World Position
    rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, toLocal);
    rayDir = DirectX::XMVector3TransformNormal(rayDir, toLocal);

    rayDir = DirectX::XMVector3Normalize(rayDir);
}

void DirectXHelper::MousePicking(ObjectData* obj, std::string Name)
{
    // 마우스 클릭 시 터치된 오브젝트 출력
    if (GlobalKeyVector::getInstance().global_MouseBehavior[MouseActionLeftDown])
    {
        bool isCatch = false;
        float dist = 0.0f;

        float minDist = 987654321.0f;
        std::string minName = "";

        DirectX::BoundingBox rootBox, bb_buffer;

        rootBox = obj->collider;

        for (const std::pair<std::string, Transform::transform*>& inst : obj->transform->InstanceBuffer)
        {
            if (!inst.second)
                continue;

            bb_buffer = rootBox;
            bb_buffer.Center.x += inst.second->Matrix->Position[0];
            bb_buffer.Center.y += inst.second->Matrix->Position[1];
            bb_buffer.Center.z += inst.second->Matrix->Position[2];

            isCatch =
                bb_buffer.Intersects(
                    rayOrigin, rayDir, dist
                );

            if (isCatch && dist < minDist)
            {
                minDist = dist;
                minName = inst.first;
            }
        }

        if (!minName.empty())
        {
            MemoryPool::getInstance().pickedInstance =
                obj->transform->InstanceBuffer[minName];
            UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;

            listbox->SetTextListBoxMessage(minName);

            HWNDPool::getInstance().TargetObjectName = Name;
            HWNDPool::getInstance().TargetInstanceName = minName;

            HWNDPool::getInstance().PosXEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Position[0];
            HWNDPool::getInstance().PosYEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Position[1];
            HWNDPool::getInstance().PosZEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Position[2];

            HWNDPool::getInstance().RotXEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Rotation[0];
            HWNDPool::getInstance().RotYEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Rotation[1];
            HWNDPool::getInstance().RotZEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Rotation[2];

            HWNDPool::getInstance().ScaleXEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Scale[0];
            HWNDPool::getInstance().ScaleYEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Scale[1];
            HWNDPool::getInstance().ScaleZEdit.data =
                &MemoryPool::getInstance().pickedInstance->Matrix->Scale[2];
        }
    }
}

void DirectXHelper::RecordRTVandDSV(ID3D11RenderTargetView* renderBufferView, ID3D11DepthStencilView* deferDepthBufferView) {
    CreateWindows::getInstance().d3d11DeviceContext->OMSetRenderTargets(1, &renderBufferView, deferDepthBufferView);

    CreateWindows::getInstance().d3d11DeviceContext->ClearRenderTargetView(renderBufferView, backgroundColor);
    CreateWindows::getInstance().d3d11DeviceContext->ClearDepthStencilView(deferDepthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectXHelper::RecordMaterial(ObjectData* obj, UINT submeshIDX)
{
    ID3D11ShaderResourceView* srv = nullptr;

    // 텍스쳐 슬롯을 채울 때 마다 값이 하나씩 누적된다.
    UINT AccumulateSlotIndex = 0;
    // 머테리얼에서 텍스쳐 이터레이터용 변수
    UINT TextureIndex = 0;
    Material* mat = nullptr;

    // 서브메쉬 기준으로 나누어 머테리얼을 수정한다.
    AccumulateSlotIndex = 0;
    TextureIndex = 0;
    mat = &obj->meshDatas[submeshIDX].mMaterial;

    // 베이스 텍스쳐가 없다면 아무 텍스쳐나 입힘
    if (!mat->BaseColorTexutures.size())
    {
        srv =
            MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                MemoryPool::getInstance().TextureNames[0]
            ];

        CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
            AccumulateSlotIndex,
            1,
            &srv
        );
        CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
            AccumulateSlotIndex,
            1,
            &CreateWindows::getInstance().samplerState
        );

        AccumulateSlotIndex++;
    }
    // 베이스 텍스쳐가 있다면
    else
    {
        // 베이스 텍스쳐 개수만큼 집어 넣음
        for (TextureIndex = 0; TextureIndex < mat->BaseColorTexutures.size(); TextureIndex++)
        {
            srv =
                MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                    mat->BaseColorTexutures[TextureIndex]
                ];

            CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                AccumulateSlotIndex,
                1,
                &srv
            );
            CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                AccumulateSlotIndex,
                1,
                &CreateWindows::getInstance().samplerState
            );

            AccumulateSlotIndex++;
        }
    }

    // 노말 텍스쳐가 없다면, 넣지 않음
    // 노말 텍스쳐가 있다면
    if (mat->NormalTexutures.size())
    {
        // 노말 텍스쳐 개수만큼 집어 넣음
        for (TextureIndex = 0; TextureIndex < mat->NormalTexutures.size(); TextureIndex++)
        {
            srv =
                MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                    mat->NormalTexutures[TextureIndex]
                ];

            CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                AccumulateSlotIndex,
                1,
                &srv
            );
            CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                AccumulateSlotIndex,
                1,
                &CreateWindows::getInstance().samplerState
            );

            AccumulateSlotIndex++;
        }
    }

    // 마스크 텍스쳐가 없다면, 넣지 않음
    // 마스크 텍스쳐가 있다면
    if (mat->MaskedTextures.size())
    {
        // 마스크 텍스쳐 개수만큼 집어 넣음
        for (TextureIndex = 0; TextureIndex < mat->MaskedTextures.size(); TextureIndex++)
        {
            srv =
                MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                    mat->MaskedTextures[TextureIndex]
                ];

            CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                AccumulateSlotIndex,
                1,
                &srv
            );
            CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                AccumulateSlotIndex,
                1,
                &CreateWindows::getInstance().samplerState
            );

            AccumulateSlotIndex++;
        }
    }

    // 메탈릭 텍스쳐가 없다면, 넣지 않음
    // 메탈릭 텍스쳐가 있다면
    if (mat->MetallicTextures.size())
    {
        // 마스크 텍스쳐 개수만큼 집어 넣음
        for (TextureIndex = 0; TextureIndex < mat->MetallicTextures.size(); TextureIndex++)
        {
            srv =
                MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                    mat->MetallicTextures[TextureIndex]
                ];

            CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                AccumulateSlotIndex,
                1,
                &srv
            );
            CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                AccumulateSlotIndex,
                1,
                &CreateWindows::getInstance().samplerState
            );

            AccumulateSlotIndex++;
        }
    }

    // 러프니스 텍스쳐가 없다면, 넣지 않음
    // 러프니스 텍스쳐가 있다면
    if (mat->RoughnessTextures.size())
    {
        // 마스크 텍스쳐 개수만큼 집어 넣음
        for (TextureIndex = 0; TextureIndex < mat->RoughnessTextures.size(); TextureIndex++)
        {
            srv =
                MemoryPool::getInstance()._GLOBAL_TEXTURE_POOL_[
                    mat->RoughnessTextures[TextureIndex]
                ];

            CreateWindows::getInstance().d3d11DeviceContext->PSSetShaderResources(
                AccumulateSlotIndex,
                1,
                &srv
            );
            CreateWindows::getInstance().d3d11DeviceContext->PSSetSamplers(
                AccumulateSlotIndex,
                1,
                &CreateWindows::getInstance().samplerState
            );

            AccumulateSlotIndex++;
        }
    }
}

void DirectXHelper::DrawCollider(ObjectData* obj)
{
    // Collider Box 전용 셰이더 제작
    ID3D11Buffer* bufferPointers[2] = {
        obj->VertexBoundBoxBuffer,
        obj->InstanceBuffer
    };

    unsigned int bufferStrides[2] = {
            obj->Stride,
            sizeof(Instance),
    };

    unsigned int bufferOffsets[2] = {
        0,
        0,
    };

    CreateWindows::getInstance().d3d11DeviceContext->IASetVertexBuffers(
        0,
        2,
        bufferPointers,
        bufferStrides,
        bufferOffsets
    );
    CreateWindows::getInstance().d3d11DeviceContext->IASetIndexBuffer(
        obj->IndexBoundBoxBuffer,
        DXGI_FORMAT_R32_UINT,
        obj->IndicesOffset
    );

    CreateWindows::getInstance().d3d11DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

    CreateWindows::getInstance().d3d11DeviceContext->IASetInputLayout(CreateWindows::getInstance().blinnPhongInputLayout);
    CreateWindows::getInstance().d3d11DeviceContext->VSSetShader(
        MemoryPool::getInstance()._GLOBAL_SHADER_POOL["BoundBox.hlsl"]->VertexShader,
        nullptr, 0
    );
    CreateWindows::getInstance().d3d11DeviceContext->PSSetShader(
        MemoryPool::getInstance()._GLOBAL_SHADER_POOL["BoundBox.hlsl"]->PixelShader,
        nullptr, 0
    );

    CreateWindows::getInstance().d3d11DeviceContext->VSSetConstantBuffers(0, 1, &obj->ObjectVSConstantBuffer);
    CreateWindows::getInstance().d3d11DeviceContext->PSSetConstantBuffers(0, 1, &obj->ObjectPSConstantBuffer);

    CreateWindows::getInstance().d3d11DeviceContext->DrawIndexedInstanced(
        obj->NumIndices,
        obj->transform->InstanceCount,
        0,
        0,
        0
    );
}