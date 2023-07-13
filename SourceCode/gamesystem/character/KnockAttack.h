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

    void ImpactAction();
public:
    void setKnockF(bool f) { KnockF = f; }
    bool GetKnockF() { return KnockF; }

};

