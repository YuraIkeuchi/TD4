#include "KnockAttack.h"

#include "Collision.h"
#include "Easing.h"
#include "Helper.h"
#include "ImageManager.h"
#include "Player.h"

void KnockAttack::Init()
{
	darksutopon.reset(new IKEObject3d());
	darksutopon->Initialize();
	darksutopon->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Sutopon));

	KnockImpTex.reset(IKETexture::Create(ImageManager::IMPACT2, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	KnockImpTex->TextureCreate();
	KnockImpTex2.reset(IKETexture::Create(ImageManager::IMPACT, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	KnockImpTex2->TextureCreate();
}

void KnockAttack::Upda()
{
	ImpactAction();

	SutoponCircleAngle++;
	stopos.x = boss->GetPosition().x + sinf(SutoponCircleAngle * (PI / 180.0f)) * 10.0f;
	stopos.z = boss->GetPosition().z + cosf(SutoponCircleAngle * (PI / 180.0f)) * 10.0f;

	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = {l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { strot.x,strot.y,strot.z };

	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	float RottoPlayer;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	strot.y = RottoPlayer * 60.f + 270.f;
	darksutopon->SetScale({ 1,1,1 });
	darksutopon->SetColor({ 0.9f,0.2f,0.7f,0.7f });
	darksutopon->SetPosition(stopos);
	darksutopon->SetRotation(strot);
	darksutopon->Update();

	KnockImpTex->Update();
	KnockImpTex->SetPosition({ stopos.x,boss->GetPosition().y + 5.f,stopos.z });
	KnockImpTex->SetScale({TexScl});
	KnockImpTex->SetRotation({ 90,0,0 });
	KnockImpTex->SetColor({ 1,1,1,TexAlpha });

	KnockImpTex2->SetPosition({ stopos.x,boss->GetPosition().y + 5.f,stopos.z });
	KnockImpTex2->SetScale(TexScl2);
	KnockImpTex2->SetRotation({ 90,0,0 });
	KnockImpTex2->SetColor({ 1,1,1,TexAlpha2 });
	KnockImpTex2->Update();
}

void KnockAttack::Draw(DirectXCommon* dxCommon)
{
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	KnockImpTex->Draw();
	KnockImpTex2->Draw();
	IKETexture::PostDraw();

	IKEObject3d::PreDraw();
	darksutopon->Draw();
	IKEObject3d::PostDraw();

}

void KnockAttack::ImpactAction()
{
	constexpr float AddTexScling = 0.08f;
	//ドッスン挙動
	//落下の緩急
	constexpr float Distortion = 3.f;
	//地面の高さ
	constexpr float GroundY = 10.f;
	//ジャンプ高さ
	constexpr float Height = 10.f;

	if(KnockF)
	{
		JFrame += 1.f / 60.f;
		stopos.y = GroundY+ (1.0f - pow(1.0f - sin(PI * JFrame), Distortion)) * Height;
		ReturnEaseT = 0.f;
		//SUB Alpha-Scling
		if (JFrame >= 1.f) {
			TexAlpha -= 0.02f;
			TexScl.x += AddTexScling; TexScl.y += AddTexScling;
			if (TexScl.x > 1.5f || TexScl.z > 1.5f)
			{
				TexAlpha2 -= 0.02f;
				TexScl2.x += AddTexScling; TexScl2.y += AddTexScling;
			}
		}
		
		//knock
		if(Collision::GetLength(boss->GetPosition(),Player::GetInstance()->GetPosition())<15.f)
			Player::GetInstance()->PlayerHit(m_Position);
	}
	else
	{
		Helper::GetInstance()->FrameCheck(ReturnEaseT, 0.04f);
		Ease(In, Quad, ReturnEaseT, 10.f, boss->GetPosition().y + 10.f);
		//reset
		TexScl = { 0.f,0.f,0.f };
		TexScl2 = { 0.f,0.f,0.f };

		stopos.y = boss->GetPosition().y + 10.f;
		JFrame = 0.f;
		TexAlpha = 0.f;
		TexAlpha2 = 0.f;
	}
	//off Flag
	if (TexAlpha2 <= 0.f)
		KnockF = false;

	Helper::GetInstance()->Clamp(TexAlpha, 0.f, 1.f);
	Helper::GetInstance()->Clamp(TexAlpha2, 0.f, 1.f);
	Helper::GetInstance()->Clamp(JFrame, 0.f, 1.f);
	//Helper::GetInstance()->Clamp(JFrame, 0.f, 1.f);
}

void KnockAttack::SpriteDraw()
{
	
}



