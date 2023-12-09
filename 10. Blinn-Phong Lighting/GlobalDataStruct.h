#pragma once
#include "DataStruct.h"

class GlobalTime {
private:
	static GlobalTime gt;

public:
	// Timing
	LONGLONG startPerfCount = 0;
	LONGLONG perfCounterFrequency = 0;

	double currentTimeInSeconds = 0.0;

	float dt;

public:
	static GlobalTime& getInstance() {
		return gt;
	}

	void Update() {
		double previousTimeInSeconds = currentTimeInSeconds;
		LARGE_INTEGER perfCount;
		QueryPerformanceCounter(&perfCount);

		currentTimeInSeconds = (double)(perfCount.QuadPart - startPerfCount) / (double)perfCounterFrequency;
		dt = (float)(currentTimeInSeconds - previousTimeInSeconds);
	}
};
// ���ο� �����쳪 ���� �����쿡�� �������� Ű ���Ͱ� �ʿ��� ���
class GlobalKeyVector {
private:
	static GlobalKeyVector kv;

public:
	static inline GlobalKeyVector& getInstance()
	{
		return kv;
	}

	// KeyInput Vector
	bool global_MouseBehavior[MouseActionCount];
	// KeyInput Vector
	bool global_KeyboardBehavior[GameActionCount];
};
