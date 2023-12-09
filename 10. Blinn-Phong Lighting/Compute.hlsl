cbuffer CB: register(b0)
{
	float4x4	WVPMat;
}

// �Է¹���(����ȭ ����, �б� ����)
StructuredBuffer<float4> Input:register(t0);
// ��¹���(����ȭ ����, �б� ���� ����)
RWStructuredBuffer<float> Result:register(u0);

// ���� �޸�
groupshared float4 shared_data[256];

// ���̴� �Լ�
[numthreads(256, 1, 1)]
void cs_main(
	uint3 Gid	:SV_GroupID,
	uint3 DTid	: SV_DispatchThreadID,
	uint3 GTid	: SV_GroupThreadID,
	uint  GI	: SV_GroupIndex
) {
	float4 test;

	// ������ �б� (256���� �а� �������� 0����)
	if (DTid.x < 256)
		shared_data[GI] = Input[DTid.x];
	else
		shared_data[GI] = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// ����ȭ ���
	GroupMemoryBarrierWithGroupSync();

	//shared_data[GI] += shared_data[GI + 64];
	test = shared_data[GI];
	test = mul(test, WVPMat);

	GroupMemoryBarrierWithGroupSync();

	shared_data[GI] = test;
	shared_data[GI] = float4(1.0f, 1.0f, 1.0f, 1.0f);

	GroupMemoryBarrierWithGroupSync();

	// ������ �׷��� ó�� �����忡�� ����� ����.
	Result[DTid.x] = shared_data[GI].x;
}