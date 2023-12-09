cbuffer CB: register(b0)
{
	float4x4	WVPMat;
}

// 입력버퍼(구조화 버퍼, 읽기 전용)
StructuredBuffer<float4> Input:register(t0);
// 출력버퍼(구조화 버퍼, 읽기 쓰기 가능)
RWStructuredBuffer<float> Result:register(u0);

// 공유 메모리
groupshared float4 shared_data[256];

// 셰이더 함수
[numthreads(256, 1, 1)]
void cs_main(
	uint3 Gid	:SV_GroupID,
	uint3 DTid	: SV_DispatchThreadID,
	uint3 GTid	: SV_GroupThreadID,
	uint  GI	: SV_GroupIndex
) {
	float4 test;

	// 데이터 읽기 (256개만 읽고 나머지는 0으로)
	if (DTid.x < 256)
		shared_data[GI] = Input[DTid.x];
	else
		shared_data[GI] = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// 동기화 계산
	GroupMemoryBarrierWithGroupSync();

	//shared_data[GI] += shared_data[GI + 64];
	test = shared_data[GI];
	test = mul(test, WVPMat);

	GroupMemoryBarrierWithGroupSync();

	shared_data[GI] = test;
	shared_data[GI] = float4(1.0f, 1.0f, 1.0f, 1.0f);

	GroupMemoryBarrierWithGroupSync();

	// 쓰레드 그룹의 처음 스레드에서 결과를 쓴다.
	Result[DTid.x] = shared_data[GI].x;
}