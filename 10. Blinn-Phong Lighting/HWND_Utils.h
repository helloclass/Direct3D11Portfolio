#pragma once
#include "Util.h"

class SubWindow;

class UI_Button : public UI_HWND
{
public:
	HWND* Main_WINDOW_HWND;
	unsigned int HWND_BUTTON_ID;

	void(*OnClicked)();

public:
	UI_Button() : 
		UI_HWND(),
		Main_WINDOW_HWND(nullptr),
		HWND_BUTTON_ID(0),
		OnClicked(nullptr)
	{}

	void CreateButton(
		_In_ HWND mainHWND,
		_In_ UINT index,
		_In_ std::string ID,
		_In_ LPCWSTR text,
		_In_ int x,
		_In_ int y,
		_In_ int w,
		_In_ int h,
		_In_ void(*func)());

	unsigned int getHWND_ID() override
	{
		return HWND_BUTTON_ID;
	}

};

class UI_Edit : public UI_HWND
{
public:
	HWND* Main_WINDOW_HWND;
	unsigned int HWND_EDIT_ID;

	// Edit과 실 데이터의 동기화를 위해 실 데이터의 포인터를 저장
	float* data;

public:
	UI_Edit() : 
		UI_HWND(),
		Main_WINDOW_HWND(nullptr),
		HWND_EDIT_ID(0),
		data(nullptr)
	{}

	void CreateEdit(
		_In_ HWND mainHWND,
		_In_ UINT index,
		_In_ std::string ID,
		_In_ int x,
		_In_ int y,
		_In_ int w,
		_In_ int h,
		_In_ float* data_ptr);

	unsigned int getHWND_ID() override
	{
		return HWND_EDIT_ID;
	}

	void SetEditText(std::string Message);
	std::string GetEditText();
};

class UI_ListBox : public UI_HWND
{
public:
	HWND* Main_WINDOW_HWND;
	unsigned int HWND_LIST_BOX_ID;

	int Selected = -1;

public:
	UI_ListBox() : 
		UI_HWND(),
		Main_WINDOW_HWND(nullptr),
		HWND_LIST_BOX_ID(0)
	{}

	void CreateListBox(
		_In_ HWND mainHWND,
		_In_ UINT index,
		_In_ std::string ID,
		_In_ int x,
		_In_ int y,
		_In_ int w,
		_In_ int h
	);

	unsigned int getHWND_ID() override
	{
		return HWND_LIST_BOX_ID;
	}

	void AppendListBoxMessage(std::string Message);
	void SetTextListBoxMessage(std::string Item);
	std::string GetTextListBoxMessage();
	void DeleteListBoxMessage(UINT ItemIndex);
	void ResetListBoxMessage();
};

class UI_Trackbar : public UI_HWND
{
public:
	HWND* Main_WINDOW_HWND;
	unsigned int HWND_TRACKBAR_ID;

public:
	UI_Trackbar() : 
		UI_HWND(),
		Main_WINDOW_HWND(nullptr),
		HWND_TRACKBAR_ID(0)
	{}

	void CreateTrackbar(
		_In_ HWND mainHWND,
		_In_ UINT index,
		_In_ std::string ID,
		_In_ int x,
		_In_ int y,
		_In_ int w,
		_In_ int h,
		_In_ UINT iMin,     // minimum value in trackbar range 
		_In_ UINT iMax,   // maximum value in trackbar range 
		_In_ UINT iSelMin,  // minimum value in trackbar selection 
		_In_ UINT iSelMax, // maximum value in trackbar selection 
		_In_ UINT& ParamPTR
	);

	unsigned int getHWND_ID() override
	{
		return HWND_TRACKBAR_ID;
	}

};

static WNDPROC oldEditProc;

class UIPool {
private:
	static UIPool pool;

public:
	static UIPool& getInstance() {
		return pool;
	}

public:
	std::vector<UI_Button*>			UI_BUTTON_LIST;
	std::vector<UI_Edit*>			UI_EDIT_LIST;
	std::vector<UI_ListBox*>		UI_LIST_BOX_LIST;
	std::vector<UI_Trackbar*>		UI_TRACKBAR_LIST;

};

class SubWindow {
protected:
	HWND hwnd;
	HWND CTL_HWND;
	HWND Tab_CTL_HWND;

	HINSTANCE hInstance;
	HINSTANCE hControlInstance;

	IDXGISwapChain1* d3d11SwapChain;

	ID3D11Texture2D* depthBuffer;

	ID3D11RenderTargetView* renderBufferView;
	ID3D11DepthStencilView* depthBufferView;

	FLOAT backgroundColor[4] = { 1.0f, 0.2f, 0.6f, 1.0f };

public:
	std::array<std::unordered_map<std::string, UI_HWND*>, 6> HWNDs;

public:
	void Init(std::wstring WindowName) {}
	void Update() {}

	inline const HWND& getHWND()
	{
		return hwnd;
	}
	inline const HWND& getCTL_HWND()
	{
		return CTL_HWND;
	}
	inline const HWND& getTab_CTL_HWND()
	{
		return Tab_CTL_HWND;
	}
	inline const HINSTANCE& getHWND_INTANCE()
	{
		return hInstance;
	}
	inline const HINSTANCE& getCTL_HWND_INSTANCE()
	{
		return hControlInstance;
	}
	inline IDXGISwapChain1* getSwapChain()
	{
		return d3d11SwapChain;
	}
	inline ID3D11RenderTargetView* getSwapChainRTV()
	{
		return renderBufferView;
	}

};