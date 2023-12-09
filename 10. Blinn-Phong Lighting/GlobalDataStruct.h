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
// 새로운 윈도우나 서브 윈도우에서 독립적인 키 벡터가 필요한 경우
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
