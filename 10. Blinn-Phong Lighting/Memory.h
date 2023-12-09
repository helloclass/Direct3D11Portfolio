#pragma once

#include "HWND_Utils.h"

class HWNDPool {
private:
	static HWNDPool pool;

public:
	static HWNDPool& getInstance() {
		return pool;
	}

public:
	HWND CTL_HWND;
	HWND Tab_CTL_HWND;

	HINSTANCE hControlInstance;

	std::array<std::unordered_map<std::string, UI_HWND*>, 6> HWNDs;

public:
public:
#pragma region A_MENU

	UI_Button HelloButton;
	UI_ListBox ObjectListBox;
	UI_Edit ObjectListButton;
	UI_ListBox TextureListBox;
	UI_Edit TextureListButton;

#pragma endregion

#pragma region B_MENU

	UI_Button InstantateButton;
	UI_Button DestroyButton;
	UI_ListBox WorldListBox;

	std::string TargetObjectName;
	std::string TargetInstanceName;

	UI_Edit PosXEdit;
	UI_Edit PosYEdit;
	UI_Edit PosZEdit;

	UI_Edit RotXEdit;
	UI_Edit RotYEdit;
	UI_Edit RotZEdit;

	UI_Edit ScaleXEdit;
	UI_Edit ScaleYEdit;
	UI_Edit ScaleZEdit;

#pragma endregion
};

class MemoryPool {
private:
	static MemoryPool pool;

public:
	static MemoryPool& getInstance() {
		return pool;
	}

	// Global Texture Paths
	std::set<const char*>       TexturePaths;
	std::set<const wchar_t*>    TextureWildPaths;

	std::vector<std::string>	ObjectNames;
	std::vector<std::string>    TextureNames;

	// Global Pool
	std::unordered_map<std::string, ID3D11ShaderResourceView*>   _GLOBAL_TEXTURE_POOL_;
	std::unordered_map<std::string, ObjectData*> _GLOBAL_OBJECT_POOL_;
	std::unordered_map<std::string, Shader*> _GLOBAL_SHADER_POOL;

	LightData _GLOBAL_LIGHT_POOL_ = {};

	inline void AddTexture(std::string name, ID3D11ShaderResourceView* tex)
	{
		_GLOBAL_TEXTURE_POOL_.insert(std::pair<std::string, ID3D11ShaderResourceView*>(name, tex));

		HWNDPool::getInstance().TextureListBox.AppendListBoxMessage(name);
	}

	inline void AddObject(std::string name, ObjectData* obj)
	{
		_GLOBAL_OBJECT_POOL_.insert(std::pair<std::string, ObjectData*>(name, obj));

		HWNDPool::getInstance().ObjectListBox.AppendListBoxMessage(name);
	}

	inline void AddShader(std::string name, Shader* obj)
	{
		_GLOBAL_SHADER_POOL.insert(std::pair<std::string, Shader*>(name, obj));
	}

	void AddLight()
	{

	}

public:
	Transform::transform* pickedInstance = nullptr;

};

