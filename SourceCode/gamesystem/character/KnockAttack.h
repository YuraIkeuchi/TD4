#pragma once
#include "InterAttack.h"
#include"IKETexture.h"
class KnockAttack :
    public InterAttack
{
public:
    void Init() override;

    void Upda() override;

    void Draw(DirectXCommon* dxCommon) override;

    void SpriteDraw() override;
private:
    bool KnockF=false;
    //CSV—p
    float KnockVal;
    float KnockAreaRad;

    std::unique_ptr<IKETexture>KnockImpTex;
    std::unique_ptr<IKETexture>KnockImpTex2;

    XMFLOAT3 TexScl, TexScl2;
    float TexAlpha, TexAlpha2;

    std::unique_ptr<IKEObject3d>darksutopon;
    XMFLOAT3 stopos;
    XMFLOAT3 strot;
    float impactMoving;
    float SutoponCircleAngle;


    float GuardEaseT;
    bool GuardStart = false;
    int GuardTime;

    float JFrame;

    float DownMoveVal;

    float ReturnEaseT;
    float OldPosY;
    void ImpactAction();
public:
    inline void setKnockF(bool f) { KnockF = f; TexAlpha = 1.f; TexAlpha2 = 1.f; }
    bool GetKnockF() { return KnockF; }

};

