#pragma once

#include "EditorHelperController.h"

namespace _BUTTON_FUNC {
    static void Button01()
    {
        MessageBoxA(0, "HELLO", "Fatal Error", MB_OK);
    }

    static void InstantateButton()
    {
        UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;

        int midIDX = -1;
        std::string str = listbox->GetTextListBoxMessage();

        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == '_')
            {
                midIDX = i;
                break;
            }
        }

        if (midIDX == -1)
            return;

        ObjectData* data = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_[str.substr(0, midIDX)];

        UIObjectData* UiObjData                 = nullptr;
        ParticleObjectData* ParticleObjData     = nullptr;
        StaticObjectData* staticObjData         = nullptr;
        KinematicObjectData* kinematicObjData   = nullptr;
        DynamicObjectData* dynamicObjData       = nullptr;

        switch (data->physicsType)
        {
        case PhysicsType::NONE_TYPE:
            UiObjData = static_cast<UIObjectData*>(data);
            UiObjData->Instantiate();
            break;

        case PhysicsType::PARTICLE_TYPE:
            ParticleObjData = static_cast<ParticleObjectData*>(data);
            ParticleObjData->Instantiate();
            break;

        case PhysicsType::STATIC_TYPE:
            staticObjData = static_cast<StaticObjectData*>(data);
            staticObjData->Instantiate();
            break;

        case PhysicsType::KINEMATIC_TYPE:
            kinematicObjData = static_cast<KinematicObjectData*>(data);
            kinematicObjData->Instantiate();
            break;

        case PhysicsType::DYNAMIC_TYPE:
            dynamicObjData = static_cast<DynamicObjectData*>(data);
            dynamicObjData->Instantiate();
            break;

        default:
            throw std::runtime_error("Object Physics Type Error..");
        }
    }

    static void DestroyButton()
    {
        UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
        
        int midIDX          = -1;
        std::string name    = listbox->GetTextListBoxMessage();

        listbox->DeleteListBoxMessage(listbox->Selected);

        for (int i = 0; i < name.size(); i++)
        {
            if (name[i] == '_')
            {
                midIDX = i;
                break;
            }
        }

        if (midIDX == -1)
            return;

        ObjectData* data = MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_[name.substr(0, midIDX)];

        switch (data->physicsType)
        {
        case PhysicsType::NONE_TYPE:
            static_cast<UIObjectData*>(data)->Destroy(name);
            break;

        case PhysicsType::PARTICLE_TYPE:
            static_cast<ParticleObjectData*>(data)->Destroy(name);
            break;

        case PhysicsType::STATIC_TYPE:
            static_cast<StaticObjectData*>(data)->Destroy(name);
            break;

        case PhysicsType::KINEMATIC_TYPE:
            static_cast<KinematicObjectData*>(data)->Destroy(name);
            break;

        case PhysicsType::DYNAMIC_TYPE:
            static_cast<DynamicObjectData*>(data)->Destroy(name);
            break;

        default:
            throw std::runtime_error("Object Physics Type Error..");
        }
    }
}

class CreateWindows {
private:
    static CreateWindows _WINDOW;

public:
    HWND MainHWND;
    HINSTANCE hMainInstance;

    ID3D11Device1* d3d11Device;
    ID3D11DeviceContext1* d3d11DeviceContext;

    ID3D11RasterizerState* rasterizerState;
    ID3D11DepthStencilState* depthStencilEnableState;
    ID3D11DepthStencilState* depthStencilDisableState;

    // Get DXGI Factory (needed to create Swap Chain)
    IDXGIFactory2* dxgiFactory;

    // Create Input Layout for our blinnPhong vertex shader
    ID3D11InputLayout* blinnPhongInputLayout;

    // Compute Shader
    ID3D11ComputeShader* shadowShader;

    // Sampler
    ID3D11SamplerState* samplerState;

    bool isPaused = false;

public:
    CreateWindows();
    ~CreateWindows();

    static inline CreateWindows& getInstance() {
        return _WINDOW;
    }

    void Init(const HINSTANCE& hInst);
    void Update();

public:
    std::vector<SubWindow*> _SubWindows;
    bool isQuit;

public:
    bool    isMoveFlag = false;
    bool    isRotatebleFlag = false;
    bool    isScaleFlag = false;

public:
    const HWND& GetHWND();

public:
    void CreateTabControl();

    HRESULT OnSize(LPARAM lParam);
    BOOL OnNotify(LPARAM lParam);
};

// Create Constant Buffer for our light vertex shader
struct LightVSConstants
{
    float4x4 modelViewProj;
    float4 color;
};

// Create Constant Buffer for our Blinn-Phong vertex shader
struct BlinnPhongVSConstants
{
    float4x4 modelViewProj;
    float4x4 modelView;
    float3x3 normalMatrix;
    float4   cameraPosition;
};

struct ScreenSizeVSConstants
{
    float screenWidth;
    float3 padding;
};