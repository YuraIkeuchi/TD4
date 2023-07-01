#pragma once
#include "InterAttack.h"
#include"IKESprite.h"
class ShotAttack :
    public InterAttack
{
public:
    void Init() override;

    void Upda() override;

    void Draw() override;

    void SpriteDraw() override;
private:
    //���f���A�U���\���͈̔�
    static constexpr int BulSize = 3;
    std::array<std::unique_ptr<IKEObject3d>, BulSize>ShotObj;
	std::array<std::unique_ptr<IKESprite>, 4>ShotArea;
    std::array<float, 4>AreaAngle={};

    //�t�F�[�Y
    enum class Phase
    {
        NON,
        SHOT,
        END
    }_phase=Phase::NON;

    static void (ShotAttack::* stateTable[])();

    void Phase_Idle();

    void Phase_Shot();

    void Phase_End();

public:
    //Phase GetPhase();
};

