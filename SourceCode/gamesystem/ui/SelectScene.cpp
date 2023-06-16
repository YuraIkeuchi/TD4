#include "SelectScene.h"
#include"Helper.h"
#include "ImageManager.h"
#include "SceneManager.h"


SelectScene* SelectScene::GetIns()
{
	static SelectScene ins;
	return &ins;
}

void SelectScene::Init()
{
	Pedestal.reset(new IKEObject3d());
	Pedestal->Initialize();
	Pedestal->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Pedestal));
	Pedestal->SetScale({ 300,20,300 });

	BackSkyDome.reset(new IKEObject3d());
	BackSkyDome->Initialize();
	BackSkyDome->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Pedestal));
	BackSkyDome->SetScale({ 100.f,100.f,100.f });


	ButtonNav_RBLB[0]= IKESprite::Create(ImageManager::BOX, { 0,0 });
	ButtonNav_RBLB[1]=IKESprite::Create(ImageManager::BOX, { 0,0 });

	//今はいたポリ
	StageObj[FIRST].reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[SECOND].reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[THIRD].reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[FOUR].reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[FIVE].reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[SIX].reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[SEVEN].reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));

	//ポストエフェクト用
	BossIcon[FIRST]=IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[SECOND] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[THIRD] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[FOUR] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[FIVE] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[SIX] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[SEVEN] = IKESprite::Create(ImageManager::BOX, { 0,0 });

	constexpr float PosRad = 40.f;
	for (auto i = 0; i < ObjNum; i++)
	{
		StageObj[i]->TextureCreate();
		StageObjRotAngle[i] = static_cast<float>(i) * (360.f / static_cast<float>(ObjNum));
		//位置の初期化
		StageObjPos[i].x = Pedestal->GetPosition().x + sinf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].z = Pedestal->GetPosition().z + cosf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;

		//BossIcon.
	}
}

void SelectScene::Upda()
{
	BackSkyDome->Update();
	Pedestal->Update();
	
	for (auto i = 0; i < ObjNum; i++)
	{
		StageObj[i]->SetScale({ 5.f,5.f,5.f });
		StageObj[i]->SetPosition(StageObjPos[i]);
		StageObj[i]->Update();
	}
}

void SelectScene::Draw_Obj(DirectXCommon*dxcomn)
{
	size_t t = ObjNum;

	IKEObject3d::PreDraw();
	BackSkyDome->Draw();
	for(auto i=0;i<ObjNum;i++)
	{
		
	}
	Pedestal->Draw();
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxcomn,1);
	for(auto i=0;i<ObjNum;i++)
	StageObj[i]->Draw();
}

void SelectScene::Draw_Sprite()
{

	ButtonNav_RBLB[0]->Draw();
	ButtonNav_RBLB[1]->Draw();

	size_t t = ObjNum;
	for(auto i=0;i<t;i++)
	{
		BossIcon[i]->Draw();
	}
}

void SelectScene::ResetParam()
{
	
}

void SelectScene::SetStage(bool judg, string sceneName)
{
	if (judg)SceneManager::GetInstance()->ChangeScene(sceneName);

}


void SelectScene::RotPedestal()
{
	if(Input::GetInstance()->TriggerButton(Input::RB))
	{
		
	}

	if(Input::GetInstance()->TriggerButton(Input::LB))
	{
		
	}
}

