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
	BackSkyDome->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Skydome));
	BackSkyDome->SetScale({ 7.f,7.f,7.f });


	ButtonNav_RBLB[0]= IKESprite::Create(ImageManager::BOX, { 0,0 });
	ButtonNav_RBLB[1]=IKESprite::Create(ImageManager::BOX, { 0,0 });

	//今はいたポリ
	StageObj[FIRST].reset(IKETexture::Create(ImageManager::SELECT_FIRST, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[SECOND].reset(IKETexture::Create(ImageManager::SELECT_SECOND, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[THIRD].reset(IKETexture::Create(ImageManager::SELECT_THIRD, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[FOUR].reset(IKETexture::Create(ImageManager::SELECT_FOUR, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[FIVE].reset(IKETexture::Create(ImageManager::SELECT_FIVE, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[SIX].reset(IKETexture::Create(ImageManager::SELECT_SIX, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));
	StageObj[SEVEN].reset(IKETexture::Create(ImageManager::SELECT_SEVEN, { 0,0,0 }, { 5,5,5 }, { 1,1,1,1 }));

	//ポストエフェクト用
	BossIcon[FIRST]=IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[SECOND] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[THIRD] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[FOUR] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[FIVE] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[SIX] = IKESprite::Create(ImageManager::BOX, { 0,0 });
	BossIcon[SEVEN] = IKESprite::Create(ImageManager::BOX, { 0,0 });

	constexpr float PosRad = 20.f;
	for (auto i = 0; i < ObjNum; i++)
	{
		StageObj[i]->TextureCreate();
		StageObjRotAngle[i] = static_cast<float>(i) * (360.f / static_cast<float>(ObjNum))+180.f;
		//位置の初期化
		StageObjPos[i].x = Pedestal->GetPosition().x + sinf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].z = Pedestal->GetPosition().z + cosf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].y = Pedestal->GetPosition().y + 8.f;
		//BossIcon.
	}
}

void SelectScene::Upda()
{
	constexpr float PosRad = 20.f;

	SkydomeRotY += 2.f;
	BackSkyDome->SetRotation({ 0,SkydomeRotY,0 });
	BackSkyDome->Update();

	Pedestal->SetScale({ 15.f,15.f,15.f });
	Pedestal->Update();

	if (TrigerSelect == NON) {
		if (Input::GetInstance()->TriggerButton(Input::RB)) {
			SelIndex++;
			TrigerSelect = RB;
		}

		if (Input::GetInstance()->TriggerButton(Input::LB)) {
			SelIndex--;
			TrigerSelect = LB;
		}
	}

	RotPedestal();

	if(IconColor[0]>=1.f|| IconColor[1] >= 1.f||IconColor[2]>=1.f|| IconColor[3] >= 1.f
		|| IconColor[4] >= 1.f|| IconColor[5] >= 1.f|| IconColor[6] >= 1.f)
	{
		if (Input::GetInstance()->TriggerButton(Input::B))
			ChangeF = true;
	}


	XMFLOAT3 nowSelpos = { Pedestal->GetPosition().x + sinf(180.f * (PI / PI_180)) * PosRad,
		8.f,Pedestal->GetPosition().z + cosf(180 * (PI / PI_180)) * PosRad };

	for (auto i= 0; i < ObjNum; i++) {
		if(Collision::GetLength(nowSelpos,StageObjPos[i])<5)
			IconColor[i] += 0.05f;
		else
			IconColor[i] -= 0.05f;


		Helper::GetInstance()->Clamp(IconColor[i], 0.3f, 1.f);
	}
	
	for (auto i = 0; i < ObjNum; i++)
	{
		StageObjPos[i].x = Pedestal->GetPosition().x + sinf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].z = Pedestal->GetPosition().z + cosf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		//if (StageObjRotAngle[i] >= 360)
		//	StageObjRotAngle[i] = 0;

		StageObj[i]->SetScale({ 1.f,1.f,5.f });
		StageObj[i]->SetPosition(StageObjPos[i]);
		StageObj[i]->SetColor({ 1,1,1,IconColor[i] });
		StageObj[i]->Update();
	}
}

void SelectScene::Draw_Obj(DirectXCommon*dxcomn)
{
	size_t t = ObjNum;

	IKEObject3d::PreDraw();
	BackSkyDome->Draw();
	Pedestal->Draw();
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxcomn,0);
	for(auto i=0;i<ObjNum;i++)
	StageObj[i]->Draw();
}

void SelectScene::Draw_Sprite()
{

//	ButtonNav_RBLB[0]->Draw();
	//ButtonNav_RBLB[1]->Draw();

	size_t t = ObjNum;
	for(auto i=0;i<t;i++)
	{
		//BossIcon[i]->Draw();
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
	if (TrigerSelect==RB) {
		for (auto i = 0; i < ObjNum; i++) {
			StageObjRotAngle[i] = Ease(In, Linear, IconRotAngle_EaseT, StageObjRotAngle[i], NowRotAngle[i] + static_cast<float>(PI_360 / ObjNum));
		}
	}
	else if(TrigerSelect == LB)
	{
		for (auto i = 0; i < ObjNum; i++) {
			StageObjRotAngle[i] = Ease(In, Linear, IconRotAngle_EaseT, StageObjRotAngle[i], NowRotAngle[i] - static_cast<float>(PI_360 / ObjNum));
		}
	}

	if(TrigerSelect==NON)
	{
		IconRotAngle_EaseT = 0.f;
		for (auto i = 0; i < ObjNum; i++)
			NowRotAngle[i] = StageObjRotAngle[i];
	}
	else
	{
		if (Helper::GetInstance()->FrameCheck(IconRotAngle_EaseT, 0.02f))
		{
			TrigerSelect = NON;
		}
	}
	if(Input::GetInstance()->TriggerButton(Input::LB))
	{
		
	}
}

void SelectScene::SceneChange( SceneChanger* schange)
{
	if (ChangeF) {
		if (IconColor[FIRST] >= 1.f)schange->ChangeScene("FIRSTSTAGE", SceneChanger::NonReverse);
		else if (IconColor[SECOND] >= 1.f)schange->ChangeScene("SECONDSTAGE", SceneChanger::NonReverse);
		ChangeF = false;
	}
}
