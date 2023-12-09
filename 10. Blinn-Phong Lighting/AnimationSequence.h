#pragma once
#include "Item/Bones.h"
#include "CreateWindows.h"

/**
 *
 * @brief KeyBoard Input Action, Axis를 맵핑한다.
 * @details 키보드의 특정 키에 대하여 이름을 부여한다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
enum AnimSequenceKeyAction {

    AnimKeyActionGrab,
    AnimKeyActionRotate,

    AnimKeyAxisLockX,
    AnimKeyAxisLockY,
    AnimKeyAxisLockZ,

    AnimKeyActionCount
};

class AnimSequenceKeyVector {
private:
    static AnimSequenceKeyVector instance;

public:
    static inline AnimSequenceKeyVector& getInstance()
    {
        return instance;
    }

    // KeyInput Vector
    bool MouseBehavior[MouseActionCount];
    // KeyInput Vector
    bool KeyboardBehavior[GameActionCount];
};

class AnimationSequence : public SubWindow
{
private:
    // Make to SingleTone
    static AnimationSequence instance;

private:
    ObjectData* target;
    ObjectData* Bone;
    ObjectData* Pivot;

    UINT currentFrame = 0;

    bool AnimPlay = false;
    bool GlobalPivot = false;

    int maxFrame = 0;
    int frame = 0;

public:
    // 각 애니메이션 기능에 필요한 파라미터 포인터
    float pMontageBlendTime = 0;

    float pBlendSpacePositionY = 0;
    float pBlendSpacePositionX = 0;

    float pCompositePositionY = 0;
    float pCompositePositionX = 0;


public:
    static AnimationSequence& getInstance()
    {
        return instance;
    }

public:
#pragma region Window_Animation_Sequence

    UI_Button KeyButton;
    UI_Button SaveButton;
    UI_ListBox BonesListBox;

#pragma endregion

#pragma region Window_Animation_Montage

    UI_ListBox SequenceListBox;
    UI_ListBox MontageListBox;

    UI_Button CreateMontage;
    UI_Button DestroyMontage;

    UI_ListBox MontageSeqListBox;

    UI_Edit BlendTime;

    UI_Button PushMontage;
    UI_Button PopMontage;

#pragma endregion

#pragma region Window_Animation_BlendSpace

    UI_ListBox SequenceListBox1;
    UI_ListBox BlendSpaceListBox;

    UI_Button CreateBlendSpace;
    UI_Button DestroyBlendSpace;

    UI_ListBox BlendSpaceSeqListBox;

    UI_Edit BlendPositionY;
    UI_Edit BlendPositionX;

    UI_Button PushBlendSpace;
    UI_Button PopBlendSpace;

#pragma endregion

#pragma region Window_Animation_Composite

    UI_ListBox SequenceListBox2;
    UI_ListBox CompositeListBox;

    UI_Button CreateComposite;
    UI_Button DestroyComposite;

    UI_ListBox CompositeSeqListBox;

    UI_Edit CompositePositionY;
    UI_Edit CompositePositionX;

    UI_Button PushComposite;
    UI_Button PopComposite;

#pragma endregion

#pragma region Window_Animation_Morph

    UI_ListBox MorphListBox;

#pragma endregion

public:
    AnimationSequence();
    ~AnimationSequence();

    void Init(std::wstring WindowName);
    void Update();

    inline void SetTarget(ObjectData* data)
    {
        if (!data)
            throw std::runtime_error("");

        std::vector<Tree<Skeleton>*> Items;

        target = data;
        target->BoneHierarchy.GetAllChilds(
            (Tree<Skeleton>*)target->BoneHierarchy.Root,
            Items
        );

        BonesListBox.ResetListBoxMessage();

        // 객체의 뼈대 리스트를 나열
        std::string BoneName;
        for (Tree<Skeleton>* inst : Items)
        {
            BoneName = "";
            //for (int level = 0; level < inst->Level; level++)
            //    BoneName += ">";
            BoneName += inst->Name;
            BonesListBox.AppendListBoxMessage(BoneName);
        }

        maxFrame = target->GetAsset()->mFrameLength;

        // 타겟의 애니메이션 시퀸스 이름 적재
        int sequenceIter = 0;

        SequenceListBox.ResetListBoxMessage();
        SequenceListBox1.ResetListBoxMessage();
        for (Sequence* anim : target->Sequences)
        {
            if (!anim->mName.empty())
            {
                SequenceListBox.AppendListBoxMessage(anim->mName);
                SequenceListBox1.AppendListBoxMessage(anim->mName);
            }
            else
            {
                SequenceListBox.AppendListBoxMessage("Sequence" + std::to_string(sequenceIter));
                SequenceListBox1.AppendListBoxMessage("Sequence" + std::to_string(sequenceIter));
            }

            sequenceIter++;
        }

    }

    inline ObjectData* GetTarget()
    {
        return target;
    }

public:
    void MaterialUpdate(ObjectData* objIter, UINT submeshIDX);
    void AnimationUpdate();

public:
    void AnimationSequenceUpdate();
    void AnimationMontageUpdate();
    void AnimationBlendSpaceUpdate();
    void AnimationMorphUpdate();
};

static int tabPage = 0;

static bool bIsPaused = false;

static UI_ListBox* bIsListBoxUpdate = nullptr;