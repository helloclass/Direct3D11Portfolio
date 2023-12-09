#pragma once
#include "Util.h"

class Controller
{
private:
	static Controller instance;

	/**
	*
	* ���콺 ������ ���� �����츶�� Ư��(������, ���� �� ����)�� �޶���
	* �׷��� ������ Camera ������ ���콺 �����͸� ���ϱ� ���ؼ���
	* ���� �����ִ� �������� MAIN_HWND�� ���������� �˾ƾ���.
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

