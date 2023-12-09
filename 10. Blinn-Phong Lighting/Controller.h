#pragma once
#include "Util.h"

class Controller
{
private:
	static Controller instance;

	/**
	*
	* 마우스 포인터 값은 윈도우마다 특색(사이즈, 시작 끝 지점)이 달라짐
	* 그렇기 때문에 Camera 내에서 마우스 포인터를 구하기 위해서는
	* 현재 보고있는 윈도우의 MAIN_HWND가 무엇인지를 알아야함.
	*
	*/
	HWND* MAIN_SCREEN_HWMD = nullptr;

public:
	// Mouse Delta Point Param
	POINT PreviousPoint;
	POINT DeltaPoint;

public:
	static Controller& getInstance()
	{
		return instance;
	}

	void SetWindow(const HWND* hwnd);
	void Update();

public:
	inline POINT GetCurrentMousePosition()
	{
		POINT ScreenPoint;

		GetCursorPos(&ScreenPoint);
		ScreenToClient(*MAIN_SCREEN_HWMD, &ScreenPoint);

		return ScreenPoint;
	}

	inline POINT GetMouseDeltaPoint()
	{
		return DeltaPoint;
	}

	inline float GetDistanceWith(POINT& origin)
	{
		float Distance = 0.0f;
		POINT ScreenPoint;

		GetCursorPos(&ScreenPoint);
		ScreenToClient(*MAIN_SCREEN_HWMD, &ScreenPoint);

		Distance = (float)(ScreenPoint.x - origin.x) + (ScreenPoint.y - origin.y);

		return Distance;
	}
};

