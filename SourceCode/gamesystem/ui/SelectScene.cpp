#include "SelectScene.h"
#include "Helper.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include "Easing.h"
#include "ParticleEmitter.h"

SelectScene* SelectScene::GetIns() {
	static SelectScene ins;
	return &ins;
}

void SelectScene::ResetParama() {
	ButtonNav_Challenge_Cancel[0]->SetPosition({ 400,650 });
	ButtonNav_Challenge_Cancel[1]->SetPosition({ 200,650 });
	ButtonNav_Pos[0] = { 700,500 };
	ButtonNav_Pos[1] = { 500,500 };
	for (auto i = 0; i < 2; i++) {
		ButtonNav_Challenge_Cancel[i]->SetAnchorPoint({ 0.5f,0.5f });
		ButtonNav_Challenge_CancelScl[i] = { 200,150 };
		ButtonNav_Challenge_CancelColAlpha[i] = 1.f;
	}

	constexpr float PosRad = 25.f;
	for (auto i = 0; i < ObjNum; i++) {
		TipsPosY[i] = -360.f;
		StageObjRotAngle[i] = static_cast<float>(i) * (360.f / static_cast<float>(ObjNum)) + 180.f;
		//位置の初期化
		StageObjPos[i].x = Pedestal->GetPosition().x + sinf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].z = Pedestal->GetPosition().z + cosf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].y = Pedestal->GetPosition().y + 8.f;
		//BossIcon.
		StageObjRot[i].y = 90;
		StageObjs[i]->SetPosition(StageObjPos[i]);
		StageObjs[i]->SetScale({ 1,1,1 });
	}
	StageObjs[FIRST]->SetScale({ 5,5,5 });
	StageObjs[THIRD]->SetScale({ 4,4,4 });
	StageObjs[FOUR]->SetScale({ 3.f,3.f,3.f });
	StageObjs[FOUR]->SetRotation({ 0.0f,0.0f,0.0f });
	StageObjs[FIVE]->SetScale({ 0.2f,0.2f,0.2f });
	TrigerSelect = NOINP;
	CloseF = false;

	closeScl = 6500.f;
	closeRad = 1500.f;


	for (auto i = 0; i < ObjNum; i++) {
		TipsAct[i] = false;
		TipsPosY[i] = -360.f;
	}

	sin = false;
	_stages = Stage::MAX;

	JudgChal = false;
}

void SelectScene::Init() {
	Pedestal.reset(new IKEObject3d());
	Pedestal->Initialize();
	Pedestal->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Pedestal));
	Pedestal->SetScale({ 300,20,300 });
	for (auto i = 0; i < ObjNum; i++) {
		StageObjs[i].reset(new IKEObject3d());
		StageObjs[i]->Initialize();
	}
	StageObjs[FIRST]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::MILKCAP_NORMAL));
	StageObjs[SECOND]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Tyuta));
	StageObjs[THIRD]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::KIDO_OBJ));
	StageObjs[FOUR]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Camera));
	StageObjs[FIVE]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::DJ));
	StageObjs[SIX]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Sutopon));
	StageObjs[SEVEN]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::LASTBOSS));

	BackSkyDome.reset(new IKEObject3d());
	BackSkyDome->Initialize();
	BackSkyDome->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Skydome));
	BackSkyDome->SetScale({ 7.f,7.f,7.f });

	ButtonNav_RBLB[0] = IKESprite::Create(ImageManager::RBBUTTON, { 0,0 });
	ButtonNav_RBLB[1] = IKESprite::Create(ImageManager::LBBUTTON, { 0,0 });

	ButtonNav_Challenge_Cancel[0] = IKESprite::Create(ImageManager::Challenge, { 0,0 });
	ButtonNav_Challenge_Cancel[1] = IKESprite::Create(ImageManager::Cancel, { 0,0 });

	ButtonNav_RBLB[0]->SetAnchorPoint({ 0.5f,0.5f });
	ButtonNav_RBLB[1]->SetAnchorPoint({ 0.5f,0.5f });
	ButtonNav_RBLB[0]->SetPosition({ 400,650 });
	ButtonNav_RBLB[1]->SetPosition({ 200,650 });
	ButtonNav_RBLB[0]->SetSize({ 300,300 });
	ButtonNav_RBLB[1]->SetSize({ 300,300 });

	ButtonNav_Challenge_Cancel[0]->SetPosition({ 400,650 });
	ButtonNav_Challenge_Cancel[1]->SetPosition({ 200,650 });
	for (auto i = 0; i < 2; i++) {
		ButtonNav_Challenge_Cancel[i]->SetAnchorPoint({ 0.5f,0.5f });
		ButtonNav_Challenge_CancelScl[i] = { 200,150 };
		ButtonNav_Challenge_CancelColAlpha[i] = 1.f;
	}

	StageObj[FIRST] = IKESprite::Create(ImageManager::tip1, { 0,0 });
	StageObj[SECOND] = IKESprite::Create(ImageManager::tip1, { 0,0 });
	StageObj[THIRD] = IKESprite::Create(ImageManager::tip1, { 0,0 });
	StageObj[FOUR] = IKESprite::Create(ImageManager::tip1, { 0,0 });
	StageObj[FIVE] = IKESprite::Create(ImageManager::tip1, { 0,0 });
	StageObj[SIX] = IKESprite::Create(ImageManager::tip1, { 0,0 });
	StageObj[SEVEN] = IKESprite::Create(ImageManager::tip1, { 0,0 });
	//ポストエフェクト用
	BossIcon[FIRST] = IKESprite::Create(ImageManager::CLOSEMILK, { 0,0 });
	BossIcon[SECOND] = IKESprite::Create(ImageManager::CLOSESYTOPON, { 0,0 });
	BossIcon[THIRD] = IKESprite::Create(ImageManager::CLOSEKIDO, { 0,0 });
	BossIcon[FOUR] = IKESprite::Create(ImageManager::CLOSECAMERA, { 0,0 });
	BossIcon[FIVE] = IKESprite::Create(ImageManager::CLOSEDJ, { 0,0 });
	BossIcon[SIX] = IKESprite::Create(ImageManager::CLOSEDARK, { 0,0 });
	BossIcon[SEVEN] = IKESprite::Create(ImageManager::CLOSEDARK, { 0,0 });

	for (int i = 0; i < MAX; i++) {
		BossIcon[i]->SetAnchorPoint({ 0.5f,0.5f });
		BossIcon[i]->SetSize({ 0,0 });
		BossIcon[i]->SetPosition({ 1280 / 2,720 / 2 });
	}
	ButtonNav_Pos[0] = { 700,500 };
	ButtonNav_Pos[1] = { 500,500 };
	constexpr float PosRad = 25.f;
	for (auto i = 0; i < ObjNum; i++) {
		TipsPosY[i] = -360.f;
		StageObjRotAngle[i] = static_cast<float>(i) * (360.f / static_cast<float>(ObjNum)) + 180.f;
		//位置の初期化
		StageObjPos[i].x = Pedestal->GetPosition().x + sinf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].z = Pedestal->GetPosition().z + cosf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].y = Pedestal->GetPosition().y + 8.f;
		//BossIcon.
		StageObjRot[i].y = 90;
		StageObjs[i]->SetPosition(StageObjPos[i]);
		StageObjs[i]->SetScale({ 0.0f,0.0f,0.0f });
		m_Scale[i] = { 0.0f,0.0f,0.0f };
		AfterScale[i] = { 1.0f,1.0f,1.0f };
	}
	m_Scale[FIRST] = { 5.0f,5.0f,5.0f };
	AfterScale[FIRST] = { 5.0f,5.0f,5.0f };
	m_Birth[FIRST] = true;
	m_BirthFinish[FIRST] = true;
	AfterScale[THIRD] = { 4.0f,4.0f,4.0f };
	AfterScale[FOUR] = { 3.0f,3.0f,3.0f };
	StageObjs[FOUR]->SetRotation({ 0.0f,90.0f,0.0f });
	AfterScale[FIVE] = { 0.2f,0.2f,0.2f };
	
	//一回開放したら大きさを合わせる
	for (auto i = 0; i < ObjNum; i++) {
		if (m_Birth[i]) {
			m_Scale[i] = AfterScale[i];
		}
		TipsAct[i] = false;
	}
}

void SelectScene::Upda() {
	constexpr float PosRad = 25.f;
	//背景
	SkydomeRotY += 0.5f;
	BackSkyDome->SetRotation({ 0,SkydomeRotY,0 });
	BackSkyDome->Update();
	//土台
	Pedestal->SetScale({ 15.f,15.f,15.f });
	Pedestal->Update();


	CloseIconView(CloseF);
	Helper::GetInstance()->Clamp(closeScl, 0.f, 12500.f);
	Helper::GetInstance()->Clamp(closeRad, 0.f, 1500.f);
	RotPedestal();

	for (int i = 0; i < MAX; i++) {
		if (IconColor[i] < 1.f) { continue; }
		if (Input::GetInstance()->TriggerButton(Input::B) && (m_Birth[i])) {
			TipsAct[i] = true;
		}
	}

	ChangeEffect("FIRSTSTAGE", Stage::FIRST, FIRST);
	ChangeEffect("SECONDSTAGE", Stage::SECOND, SECOND);
	ChangeEffect("FOURTHSTAGE", Stage::FOUR, FOUR);
	ChangeEffect("THIRDSTAGE", Stage::THIRD, THIRD);
	ChangeEffect("FIVESTAGE", Stage::SIX, SIX);
	ChangeEffect("SIXSTAGE", Stage::FIVE, FIVE);
	ChangeEffect("SEVENSTAGE", Stage::SEVEN, SEVEN);

	for (int i = 0; i < MAX; i++) {
		if (closeScl >= 10000.f) {
			BossIcon[i]->SetAnchorPoint({ 0.5f,0.5f });
			BossIcon[i]->SetSize({ 0,0 });
			BossIcon[i]->SetPosition({ 1280 / 2,720 / 2 });
			_stages = Stage::MAX;
			sin = false;
		}

		if (_stages != Stage::MAX) {
			TipsPosY[i] -= 40.f;
			BossIcon[i]->SetAnchorPoint({ 0.5f,0.5f });
			BossIcon[i]->SetSize({ closeScl,closeScl });
			BossIcon[i]->SetPosition({ 1280 / 2,720 / 2 });
		}
	}


	XMFLOAT3 nowSelpos = {
		Pedestal->GetPosition().x + sinf(180.f * (PI / PI_180)) * PosRad,
		8.f,
		Pedestal->GetPosition().z + cosf(180 * (PI / PI_180)) * PosRad };

	for (auto i = 0; i < ObjNum; i++) {
		if (Collision::GetLength(nowSelpos, StageObjPos[i]) < 5) {
			StageObjRot[i].y++;
			IconColor[i] += 0.05f;
		} else {
			StageObjRot[i].y = 90;
			IconColor[i] -= 0.05f;
		}

		Helper::GetInstance()->Clamp(IconColor[i], 0.3f, 1.f);
	}

	ViewTips();
	for (auto i = 0; i < ObjNum; i++) {
		StageObjPos[i].x = Pedestal->GetPosition().x + sinf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;
		StageObjPos[i].z = Pedestal->GetPosition().z + cosf(StageObjRotAngle[i] * (PI / PI_180)) * PosRad;

		StageObj[i]->SetSize({ 800.f,500.f });
		StageObj[i]->SetPosition({ 640,TipsPosY[i] });
		StageObj[i]->SetAnchorPoint({ 0.5f,0.5f });
		StageObj[i]->SetColor({ 1,1,1,1.f });

		StageObjs[i]->SetColor({ 1,1,1,IconColor[i] });
		StageObjs[i]->SetRotation({ StageObjRot[i] });

		StageObjs[FOUR]->SetRotation({ 0,StageObjRot[FOUR].y,90 });
		StageObjs[i]->SetPosition(StageObjPos[i]);
		StageObjs[i]->SetScale(m_Scale[i]);
		StageObjs[i]->Update();
	}
	bool temp[ObjNum] = {};
	for (auto i = 0; i < TipsAct.size(); i++)
		temp[i] = TipsAct[i];
	if (Helper::GetInstance()->All_OfF(temp, ObjNum)) {
		if (TrigerSelect == NOINP && m_Wide) {
			if (Input::GetInstance()->TriggerButton(Input::RB)) {
				SelIndex++;
				TrigerSelect = RB;
			}

			if (Input::GetInstance()->TriggerButton(Input::LB)) {
				SelIndex--;
				TrigerSelect = LB;
			}
		}
	}

	//セレクトのステート管理
	StateManager();

}

void SelectScene::Draw_Obj(DirectXCommon* dxcomn) {
	size_t t = ObjNum;

	IKEObject3d::PreDraw();
	BackSkyDome->Draw();
	Pedestal->Draw();
	for (auto i = 0; i < ObjNum; i++) {
		StageObjs[i]->Draw();
	}
	IKEObject3d::PostDraw();

}

void SelectScene::Draw_Sprite() {
	for (auto i = 0; i < ObjNum; i++) {
		if (!BossIcon[i]) { continue; }
		if (_stages != i)continue;
		BossIcon[i]->Draw();
	}
}

void SelectScene::ImGuiDraw() {
	ImGui::Begin("Select");
	ImGui::Text("%d", m_SelectState);
	ImGui::End();
}
void SelectScene::Draw_SpriteBack() {
	if (closeScl <= 0.f) { return; }
	ButtonNav_RBLB[0]->Draw();
	ButtonNav_RBLB[1]->Draw();
	for (auto i = 0; i < ObjNum; i++) {
		StageObj[i]->Draw();
	}
	for (int i = 0; i < MAX; i++) {
		if (TipsPosY[i] < 360.f) { continue; }
		if (JudgChal) {
			size_t cancel = 0, notCancel = 1;
			ButtonNav_Challenge_CancelScl[cancel].x += 5.0f;
			ButtonNav_Challenge_CancelScl[cancel].y += 5.0f;
			ButtonNav_Challenge_CancelColAlpha[cancel] -= 0.02f;
			ButtonNav_Challenge_CancelColAlpha[notCancel] -= 0.04f;
			if (ButtonNav_Challenge_CancelColAlpha[cancel]<0.0f) {
				JudgChal = false;
				for (auto i = 0; i < 2; i++) {
					ButtonNav_Challenge_CancelScl[i] = { 200,150 };
					ButtonNav_Challenge_CancelColAlpha[i] = 1.f;
				}
			}
		}
		for (int i = 0; i < 2; i++) {
			ButtonNav_Challenge_Cancel[i]->SetPosition(ButtonNav_Pos[i]);
			ButtonNav_Challenge_Cancel[i]->SetSize(ButtonNav_Challenge_CancelScl[i]);
			ButtonNav_Challenge_Cancel[i]->SetColor({ 1,1,1,ButtonNav_Challenge_CancelColAlpha[i] });
			ButtonNav_Challenge_Cancel[i]->Draw();
		}
	}
}


void SelectScene::RotPedestal() {
	float vel = static_cast<float>(PI_360 / ObjNum);
	if (TrigerSelect == RB) {
		vel *= 1.0f;
	} else if (TrigerSelect == LB) {
		vel *= -1.0f;
	}
	if (TrigerSelect == NOINP) {
		IconRotAngle_EaseT = 0.f;
		for (auto i = 0; i < ObjNum; i++)
			NowRotAngle[i] = StageObjRotAngle[i];
	} else {
		if (Helper::GetInstance()->FrameCheck(IconRotAngle_EaseT, 1 / 15.0f)) {
			TrigerSelect = NOINP;
		}
		for (auto i = 0; i < ObjNum; i++) {
			StageObjRotAngle[i] = Ease(In, Linear, IconRotAngle_EaseT, StageObjRotAngle[i], NowRotAngle[i] + vel);
		}
	}
}
void SelectScene::CloseIconView(bool closeF) {
	//定数わっしょい　良い方法模索中。。。
	constexpr float texScl = 6500.f;
	constexpr float MinScl = 2000.f;
	constexpr float SubRad = 0.5f;

	if (closeF && !sin) {
		closeScl -= SclingSpeed;
		if (closeScl <= MinScl) {
			SclingSpeed = 55.f;
			closeRad -= SclingSpeed * SubRad;
		} else
			SclingSpeed = 100.f;
	}
	if (sin) {
		if (closeScl >= MinScl)
			SclingSpeed = 180.f;
		else
			SclingSpeed = 100.f;

		closeScl += SclingSpeed;
		closeRad += SclingSpeed * SubRad;
	}
}
void SelectScene::ChangeEffect(std::string name, Stage stage, UINT iconnum) {
	if (_stages != stage) { return; }
	if (sin) { return; }
	CloseF = true;
	if (closeScl <= 0.f) {
		sin = true;
		Player::GetInstance()->MoveStop(false);
		Player::GetInstance()->SetCanShot(true);
		SceneManager::GetInstance()->ChangeScene(name);
		CloseF = false;
	}
}

void SelectScene::TipsPosUpda(Stage stage) {
	if (_stages == stage) { return; }
	if (!TipsAct[stage]) { return; }
	if (TipsPosY[stage] < 360.f) { return; }
	if (Input::GetInstance()->TriggerButton(Input::B)) {
		JudgChal = true;
		_stages = stage;
	} else if (Input::GetInstance()->TriggerButton(Input::X)) {
		TipsAct[stage] = false;
	}
}

void SelectScene::ViewTips() {
	constexpr float AddVal = 40.f;
	for (auto i = 0; i < ObjNum; i++) {
		TipsPosUpda((Stage)i);
		if (TipsAct[i])
			TipsPosY[i] += AddVal;
		else
			TipsPosY[i] -= AddVal;

		Helper::GetInstance()->Clamp(TipsPosY[i], -360.f, 360.f);
	}
}

void SelectScene::StateManager() {
	//
	m_Wide = true;
	m_SelectState =SELECT_SECOND;
	//
	//クリア状況に応じてOBJの大きさだったりが違う
	if (m_SelectState == SELECT_FIRST) {		//ここは牛乳のみ

	}
	else if (m_SelectState == SELECT_SECOND) {			//牛乳をクリアしてラスボス以外出現する
		bool temp[ObjNum] = {};
		for (auto i = 0; i < TipsAct.size(); i++)
			temp[i] = TipsAct[i];
		if (Helper::GetInstance()->All_OfF(temp, ObjNum)) {			//画面を閉じた後
			m_BirthTimer++;
			for (auto i = 1; i < ObjNum - 1; i++) {			//ラスボス以外
				m_Birth[i] = true;			//ボスが出現した
			}
			if (m_BirthTimer == 150) {			//　一定フレームに達するとボスが大きくなる
				m_Wide = true;
				m_BirthTimer = {};
			}
			if (m_Wide) {			//ボスを大きくする
				for (auto i = 1; i < ObjNum - 1; i++) {
					m_Scale[i] = { Ease(In,Cubic,0.5f,m_Scale[i].x,AfterScale[i].x),
						Ease(In,Cubic,0.5f,m_Scale[i].y,AfterScale[i].y),
						Ease(In,Cubic,0.5f,m_Scale[i].z,AfterScale[i].z),
					};
					m_BirthFinish[i] = true;
				}
			}
			else {			//大きくなる前はパーティクルを出すようにしている
				BirthParticle();
			}
		}

		if (SceneSave::GetInstance()->AllClear()) {			//ラスボス以外倒したら(いまはちえよしまで)ラスボスが出現する
			m_SelectState = SELECT_LAST;
			m_Wide = false;
			m_BirthTimer = {};
		}
	}
	else {			//ラスボスゾーンの出現
		bool temp[ObjNum] = {};
		for (auto i = 0; i < TipsAct.size(); i++)
			temp[i] = TipsAct[i];
		if (Helper::GetInstance()->All_OfF(temp, ObjNum)) {
			m_BirthTimer++;
			m_Birth[SEVEN] = true;			//ラスボスの出現

			if (m_BirthTimer == 150) {
				m_Wide = true;
				m_BirthTimer = {};
			}

			if (m_Wide) {//ラスボスのOBJを大きくする
				m_Scale[SEVEN] = { Ease(In,Cubic,0.5f,m_Scale[SEVEN].x,AfterScale[SEVEN].x),
					Ease(In,Cubic,0.5f,m_Scale[SEVEN].y,AfterScale[SEVEN].y),
					Ease(In,Cubic,0.5f,m_Scale[SEVEN].z,AfterScale[SEVEN].z),
				};
				m_BirthFinish[SEVEN] = true;
			}
			else {
				BirthParticle();
			}
		}
	}
}

//パーティクル
void SelectScene::BirthParticle() {
	int l_Life[ObjNum];

	for (auto i = 0; i < ObjNum; i++) {
		l_Life[i] = 50;
		if (m_Birth[i] && !m_BirthFinish[i]) {
			ParticleEmitter::GetInstance()->SelectEffect(l_Life[i], StageObjPos[i], 1.0f, 0.0f, { 0.8f,0.5f,0.4f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		}
	}
}