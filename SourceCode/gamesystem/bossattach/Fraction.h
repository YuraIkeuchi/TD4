#pragma once
#include"InterBullet.h"
#include"BreakEffect.h"
#include"IKEObject3d.h"
#include"IKEModel.h"
#include"InterBullet.h"

class Fraction 
{
private:
    // DirectX::Çè»ó™
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;
    using XMVECTOR = DirectX::XMVECTOR;
    using XMMATRIX = DirectX::XMMATRIX;

public:
    ~Fraction();

    void Init(const XMFLOAT3& BossPos);
    
    void Update(vector<InterBullet*> bullet);

    void Draw(DirectXCommon* dxCommon);

    bool IsDelete()const { return Isdelete; }

    void Obj_Set();

    void Drop();

    void ColPlayer(vector<InterBullet*> bullet);
private:
   
    unique_ptr<IKEObject3d> m_Object;
    XMFLOAT3 boss_pos_{};
    XMFLOAT3 m_Scale{};
    XMFLOAT3 m_Position{};
    XMFLOAT3 m_Rotation{};
    XMFLOAT4 m_Color{ 1.f,1.f,1.f,1.f };


    bool Isdelete = false;

    float m_Radius = 2.f;
};

