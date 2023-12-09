#include "EditorHelperController.h"

EditorHelperControllerPool EditorHelperControllerPool::instance;

void EditorHelperController::ControlSwitch_MouseLeftDown(WPARAM& key)
{
	if (!bIsSelected)
		return;

}

void EditorHelperController::ControlSwitch_MouseLeftUp(WPARAM& key)
{
	if (!bIsSelected)
		return;

	bIsRotate = false;
	bIsGrab = false;

	bIsAxisLockX = false;
	bIsAxisLockY = false;
	bIsAxisLockZ = false;

	MouseOriginPoint = POINT(0, 0);

	// 변형 적용
	// Pass
}

void EditorHelperController::ControlSwitch_MouseRightDown(WPARAM& key)
{
	if (!bIsSelected)
		return;

}

void EditorHelperController::ControlSwitch_MouseRightUp(WPARAM& key)
{
	if (!bIsSelected)
		return;

	bIsRotate = false;
	bIsGrab = false;

	bIsAxisLockX = false;
	bIsAxisLockY = false;
	bIsAxisLockZ = false;

	MouseOriginPoint = POINT(0, 0);

	// 변형 적용 취소
    MemoryPool::getInstance().pickedInstance->SetPosition(
		PreviousInstance.Position[0],
		PreviousInstance.Position[1],
		PreviousInstance.Position[2]
	);

    MemoryPool::getInstance().pickedInstance->SetRotation(
		PreviousInstance.Rotation[0],
		PreviousInstance.Rotation[1],
		PreviousInstance.Rotation[2]
	);

    MemoryPool::getInstance().pickedInstance->SetScale(
		PreviousInstance.Scale[0],
		PreviousInstance.Scale[1],
		PreviousInstance.Scale[2]
	);

}

void EditorHelperController::ControlSwitch_KeyDown(WPARAM& key)
{
	if (!bIsSelected)
		return;

}

void EditorHelperController::ControlSwitch_KeyUp(WPARAM& key)
{
	if (!bIsSelected)
		return;

	if (key == 'G')
	{
		bIsRotate = false;
		bIsGrab ^= true;

		MouseOriginPoint = Controller::getInstance().GetCurrentMousePosition();

		// 기존 인스턴스 값을 저장
        MemoryPool::getInstance().pickedInstance->GetLocalPosition(
			PreviousInstance.Position[0],
			PreviousInstance.Position[1],
			PreviousInstance.Position[2]
		);

        MemoryPool::getInstance().pickedInstance->GetLocalPosition(
			PreviousInstance.Rotation[0],
			PreviousInstance.Rotation[1],
			PreviousInstance.Rotation[2]
		);


        MemoryPool::getInstance().pickedInstance->GetLocalScale(
			PreviousInstance.Scale[0],
			PreviousInstance.Scale[1],
			PreviousInstance.Scale[2]
		);

	}
	else if (key == 'R')
	{
		bIsGrab = false;
		bIsRotate ^= true;

		MouseOriginPoint = Controller::getInstance().GetCurrentMousePosition();

		// 기존 인스턴스 값을 저장
        MemoryPool::getInstance().pickedInstance->GetLocalPosition(
			PreviousInstance.Position[0],
			PreviousInstance.Position[1],
			PreviousInstance.Position[2]
		);

        MemoryPool::getInstance().pickedInstance->GetLocalRotation(
			PreviousInstance.Rotation[0],
			PreviousInstance.Rotation[1],
			PreviousInstance.Rotation[2]
		);


        MemoryPool::getInstance().pickedInstance->GetLocalScale(
			PreviousInstance.Scale[0],
			PreviousInstance.Scale[1],
			PreviousInstance.Scale[2]
		);

	}
	else if ((bIsGrab + bIsRotate))
	{
		if (key == 'X')
		{
			bIsAxisLockX ^= true;
			bIsAxisLockY = false;
			bIsAxisLockZ = false;
		}
		else if (key == 'Y')
		{
			bIsAxisLockX = false;
			bIsAxisLockY ^= true;
			bIsAxisLockZ = false;
		}
		else if (key == 'Z')
		{
			bIsAxisLockX = false;
			bIsAxisLockY = false;
			bIsAxisLockZ ^= true;
		}
	}
}

void EditorHelperController::Update()
{
    if (!bIsSelected)
        return;

    float3 Delta = { 0.0f, 0.0f, 0.0f };

    float3 TargetPosition = {};
    float3 TargetRotation = {};
    float3 TargetScale = {};

    if (bIsGrab)
    {
        ModifyWeight = Controller::getInstance().GetDistanceWith(MouseOriginPoint);

        if (bGlobalPivot)
        {
            if (bIsAxisLockX)
            {
                Delta = float3(1.0f, 0.0f, 0.0f);
            }
            else if (bIsAxisLockY)
            {
                Delta = float3(0.0f, 1.0f, 0.0f);
            }
            else if (bIsAxisLockZ)
            {
                Delta = float3(0.0f, 0.0f, 1.0f);
            }
            else
            {
                Delta = CameraPool::getInstance().getCamera().GetLocalRightVector();
            }
        }
        else
        {
            MemoryPool::getInstance().pickedInstance->GetLocalPosition(
                TargetPosition.x,
                TargetPosition.y,
                TargetPosition.z
            );
            MemoryPool::getInstance().pickedInstance->GetLocalRotation(
                TargetRotation.x,
                TargetRotation.y,
                TargetRotation.z
            );
            MemoryPool::getInstance().pickedInstance->GetLocalScale(
                TargetScale.x,
                TargetScale.y,
                TargetScale.z
            );

            float4x4 TranslationMatrix =
                makeAffineMatrix(
                    TargetPosition,
                    TargetRotation,
                    TargetScale
                );

            if (bIsAxisLockX)
            {
                Delta = {
                    TranslationMatrix.m[0][0],
                    TranslationMatrix.m[1][0],
                    TranslationMatrix.m[2][0],
                };
            }
            else if (bIsAxisLockY)
            {
                Delta = {
                    TranslationMatrix.m[0][1],
                    TranslationMatrix.m[1][1],
                    TranslationMatrix.m[2][1],
                };
            }
            else if (bIsAxisLockZ)
            {
                Delta = {
                    TranslationMatrix.m[0][2],
                    TranslationMatrix.m[1][2],
                    TranslationMatrix.m[2][2],
                };
            }
            else
            {
                Delta = CameraPool::getInstance().getCamera().GetLocalRightVector();
            }
        }

        // Apply Delta on Target Instance Position
        TargetPosition.x = PreviousInstance.Position[0] + Delta.x * ModifyWeight;
        TargetPosition.y = PreviousInstance.Position[1] + Delta.y * ModifyWeight;
        TargetPosition.z = PreviousInstance.Position[2] + Delta.z * ModifyWeight;

        MemoryPool::getInstance().pickedInstance->SetPosition(
            TargetPosition.x,
            TargetPosition.y,
            TargetPosition.z
        );
    }

    else if (bIsRotate)
    {
        ModifyWeight = Controller::getInstance().GetDistanceWith(MouseOriginPoint);
        if (bGlobalPivot)
        {
            if (bIsAxisLockX)
            {
                Delta = float3(1.0f, 0.0f, 0.0f);
                bIsAxisLockX = false;
            }
            else if (bIsAxisLockY)
            {
                Delta = float3(0.0f, 1.0f, 0.0f);
                bIsAxisLockY = false;
            }
            else if (bIsAxisLockZ)
            {
                Delta = float3(0.0f, 0.0f, 1.0f);
                bIsAxisLockZ = false;
            }
        }
        else
        {
            MemoryPool::getInstance().pickedInstance->GetLocalPosition(
                TargetPosition.x,
                TargetPosition.y,
                TargetPosition.z
            );
            MemoryPool::getInstance().pickedInstance->GetLocalRotation(
                TargetRotation.x,
                TargetRotation.y,
                TargetRotation.z
            );
            MemoryPool::getInstance().pickedInstance->GetLocalScale(
                TargetScale.x,
                TargetScale.y,
                TargetScale.z
            );

            float4x4 TranslationMatrix =
                makeAffineMatrix(
                    TargetPosition,
                    TargetRotation,
                    TargetScale
                );

            if (bIsAxisLockX)
            {
                Delta = {
                    TranslationMatrix.m[0][0],
                    TranslationMatrix.m[1][0],
                    TranslationMatrix.m[2][0],
                };
            }
            else if (bIsAxisLockY)
            {
                Delta = {
                    TranslationMatrix.m[0][1],
                    TranslationMatrix.m[1][1],
                    TranslationMatrix.m[2][1],
                };
            }
            else if (bIsAxisLockZ)
            {
                Delta = {
                    TranslationMatrix.m[0][2],
                    TranslationMatrix.m[1][2],
                    TranslationMatrix.m[2][2],
                };
            }
        }

        // Apply Delta on Target Instance Position
        TargetRotation.x = PreviousInstance.Rotation[0] + Delta.x * ModifyWeight;
        TargetRotation.y = PreviousInstance.Rotation[1] + Delta.y * ModifyWeight;
        TargetRotation.z = PreviousInstance.Rotation[2] + Delta.z * ModifyWeight;

        MemoryPool::getInstance().pickedInstance->SetRotation(
            TargetRotation.x,
            TargetRotation.y,
            TargetRotation.z
        );

    }
}