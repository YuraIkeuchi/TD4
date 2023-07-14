#include "KnockAttack.h"

#include "Collision.h"
#include "Helper.h"
#include "ImageManager.h"
#include "Player.h"

void KnockAttack::Init()
{
	KnockImpTex.reset(IKETexture::Create(ImageManager::IMPACT2, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	KnockImpTex->TextureCreate();
	KnockImpTex2.reset(IKETexture::Create(ImageManager::IMPACT, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	KnockImpTex2->TextureCreate();
}

void KnockAttack::Upda()
{
	ImpactAction();

	KnockImpTex->Update();
	KnockImpTex->SetPosition({ boss->GetPosition().x,boss->GetPosition().y + 5.f,boss->GetPosition().z });
	KnockImpTex->SetScale({TexScl});
	KnockImpTex->SetRotation({ 90,0,0 });
	KnockImpTex->SetColor({ 1,1,1,TexAlpha });

	KnockImpTex2->SetPosition({boss->GetPosition().x,boss->GetPosition().y+5.f,boss->GetPosition().z});
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

}

void KnockAttack::ImpactAction()
{
	constexpr float AddTexScling = 0.08f;

	if(KnockF)
	{
		//SUB Alpha-Scling
		TexAlpha -= 0.02f;
		TexScl.x += AddTexScling; TexScl.y += AddTexScling;
		if(TexScl.x>1.5f||TexScl.z>1.5f)
		{
			TexAlpha2 -= 0.02f;
			TexScl2.x += AddTexScling; TexScl2.y += AddTexScling;
		}
		//knock
		if(Collision::GetLength(boss->GetPosition(),Player::GetInstance()->GetPosition())<15.f)
			Player::GetInstance()->PlayerHit(m_Position);
	}
	else
	{
		//reset
		TexScl = { 0.f,0.f,0.f };
		TexScl2 = { 0.f,0.f,0.f };

		TexAlpha = 0.f;
		TexAlpha2 = 0.f;
	}
	//off Flag
	if (TexAlpha2 <= 0.f)
		KnockF = false;

	Helper::GetInstance()->Clamp(TexAlpha, 0.f, 1.f);
	Helper::GetInstance()->Clamp(TexAlpha2, 0.f, 1.f);

}

void KnockAttack::SpriteDraw()
{
	
}



