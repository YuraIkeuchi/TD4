#pragma once
#include<memory>
#include<array>
#include"IKEObject3d.h"
#include"InterBoss.h"
#include<DirectXMath.h>

using namespace DirectX;
class InterAttack
{
public:
	virtual void Init()=0;

	virtual void Upda()=0;

	virtual void Draw()=0;

	virtual void SpriteDraw();

public:
	void SetPlayer(InterBoss* boss);
protected:
	InterBoss* boss=nullptr;

	int AttackTimer=0;

	float FollowAngle;


protected:
	void FollowPlayer();
};

