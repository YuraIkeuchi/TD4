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
    //モデル、攻撃予測の範囲
    static constexpr int BulSize = 3;
    std::array<std::unique_ptr<IKEObject3d>, BulSize>ShotObj;
	std::array<std::unique_ptr<IKESprite>, 4>ShotArea;
    std::array<float, 4>AreaAngle={};
    std::array<XMFLOAT3, BulSize>BulPos;
    std::array<XMFLOAT3, BulSize>BulRot;
    std::array<float, BulSize>BulAlpha;

    std::array<XMVECTOR, BulSize>move;
    std::array<XMMATRIX, BulSize>matRot;

    //フェーズ
    enum Phase
    {
        NON,
        SHOT,
        END
    }_phase=Phase::NON;

    static void (ShotAttack::* stateTable[])();

    void Phase_Idle();

    void Phase_Shot();

    void Phase_End();


    void RottoPlayer();

public:
    //Phase GetPhase();
};

