#include "SevenStageActor.h"
#include"Easing.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "BackObj.h"
#include "Menu.h"
#include "SelectScene.h"
#include "Helper.h"
//������
void SevenStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	//�I�[�f�B�I
	Audio::GetInstance()->LoopWave(AUDIO_BATTLE, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);

	//�|�X�g�G�t�F�N�g
	PlayPostEffect = true;
	//�p�[�e�B�N���S�폜
	ParticleEmitter::GetInstance()->AllDelete();

	//�e�N���X
	Player::GetInstance()->InitState({ 0.0f,5.0f,-5.0f });

	backScreen_ = IKESprite::Create(ImageManager::PLAY, { 0,0 });
	backScreen_->SetSize({ 1280.0f,720.0f });
	//�V�[���`�F���W���[
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	enemymanager = std::make_unique<EnemyManager>("SEVENSTAGE");
	//enemymanager->Initialize(dxCommon);
	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::TALK_FIRST);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("SEVENSTAGE");
	camerawork->SplineSet();
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();

	SelectScene::GetIns()->Init();
	Menu::GetIns()->Init();
	ui->SetBoss(enemymanager->GetBoss());
	BackObj::GetInstance()->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->LightSet(lightgroup);
	LoadStageObj::SetEnemyManager(enemymanager.get());

	m_SceneState = SceneState::IntroState;

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);

	//�ۉe�̂��߂̂��
	lightgroup->SetDirLightActive(0, false);
	lightgroup->SetDirLightActive(1, false);
	lightgroup->SetDirLightActive(2, false);
	for (int i = 0; i < SPOT_NUM; i++) {
		lightgroup->SetSpotLightActive(i, true);
	}

	spotLightPos[0] = { 30,  10, 60 };
	spotLightPos[1] = { 30,  10,  0 };
	spotLightPos[2] = { -30, 10, 60 };
	spotLightPos[3] = { -30, 10,  0 };

	spotLightDir[0] = { 0, -1, 0 };
	spotLightDir[1] = { 0, -1, 0 };
	spotLightDir[2] = { 0, -1, 0 };
	spotLightDir[3] = { 0, -1, 0 };

	spotLightColor[0] = { 1, 1, 1 };
	spotLightColor[1] = { 1, 1, 1 };
	spotLightColor[2] = { 1, 1, 1 };
	spotLightColor[3] = { 1, 1, 1 };
}
//�X�V
void SevenStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//�֐��|�C���^�ŏ�ԊǗ�
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	sceneChanger_->Update();

	//�v���C���[
	if (enemymanager->BossDestroy() && camerawork->GetFeedEnd()) {
		SceneSave::GetInstance()->SetClearFlag(kSevenStage, true);
		lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.0f, Player::GetInstance()->GetPosition().z }));
		lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	}
	else {//�{�X���j���[�r�[�̌�͊ۉe����
		lightgroup->SetCircleShadowActive(0, false);
	}

	//�{�X
	lightgroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemymanager->GetBoss()->GetPosition().x, 	0.0f, 	enemymanager->GetBoss()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));

	lightgroup->Update();
	/*if (SelectScene::GetIns()->GetCloseScl() < 10000.f)
		SelectScene::GetIns()->Upda();

	if (Input::GetInstance()->TriggerButton(Input::Y)) {
		SelectScene::GetIns()->ResetParama();
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}*/


	//�X�|�b�g���C�g�̓���
	MoveSpotLight();
	if (_AppState == APP_END) {
		//�ۉe�̂��߂̂��
		lightgroup->SetDirLightActive(0, true);
		lightgroup->SetDirLightActive(1, true);
		lightgroup->SetDirLightActive(2, true);
		for (int i = 0; i < SPOT_NUM; i++) {
			lightgroup->SetSpotLightActive(i, false);
		}
	}

	///�X�|�b�g���C�g
	for (int i = 0; i < SPOT_NUM; i++) {
		lightgroup->SetSpotLightDir(i, XMVECTOR({ spotLightDir[i].x,spotLightDir[i].y,spotLightDir[i].z,0 }));
		lightgroup->SetSpotLightPos(i, spotLightPos[i]);
		lightgroup->SetSpotLightColor(i, spotLightColor[i]);
		lightgroup->SetSpotLightAtten(i, XMFLOAT3(spotLightAtten));
		lightgroup->SetSpotLightFactorAngle(i, XMFLOAT2(spotLightFactorAngle));
	}
	lightgroup->Update();
	Menu::GetIns()->Upda();
	postEffect->SetCloseRad(SelectScene::GetIns()->GetCloseIconRad());
}
//�`��
void SevenStageActor::Draw(DirectXCommon* dxCommon) {
	//�`����@
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		postEffect->ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//���
void SevenStageActor::Finalize() {
}
//���̕`��
void SevenStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	backScreen_->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	//�p�[�e�B�N���`��
	if (!camerawork->GetFeedEnd() && m_SceneState == SceneState::MainState) {
		if (!enemymanager->BossDestroy()) {
			ParticleEmitter::GetInstance()->BackDrawAll();
		}
	}
	////�e�N���X�̕`��
	if (!camerawork->GetFeedEnd()) {
		Player::GetInstance()->Draw(dxCommon);
		loadobj->Draw(dxCommon);
	}
	enemymanager->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//�|�X�g�G�t�F�N�g��������Ȃ�
void SevenStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//�p�[�e�B�N���`��
	if (!camerawork->GetFeedEnd() && m_SceneState == SceneState::MainState) {
		ParticleEmitter::GetInstance()->FlontDrawAll();
	}

	ParticleEmitter::GetInstance()->DeathDrawAll();
	//���S�ɑO�ɏ����X�v���C�g
	IKESprite::PreDraw();
	if (m_SceneState == SceneState::MainState && !camerawork->GetFeedEnd()) {
		ui->Draw();
	}
	if (m_SceneState == SceneState::IntroState) {
		text_->SpriteDraw(dxCommon);
	}
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	//blackwindow->Draw();
	Menu::GetIns()->Draw();
	camerawork->feedDraw();
	//SelectScene::GetIns()->Draw_Sprite();
	IKESprite::PostDraw();
}
//IMGui�̕`��
void SevenStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	enemymanager->ImGuiDraw();
	Player::GetInstance()->ImGuiDraw();
}
//�o��V�[��
void SevenStageActor::IntroUpdate(DebugCamera* camera) {

	//���o�X�L�b�v
	if (Input::GetInstance()->TriggerButton(Input::A)) {
		camerawork->SetCameraSkip(true);
	}

	if (camerawork->GetAppearEndF()) {
		_AppState = APP_END;
		m_SceneState = SceneState::MainState;
		camerawork->SetCameraState(CAMERA_NORMAL);
		enemymanager->SkipInitialize();
	}

	//�e�N���X�X�V
	BackObj::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	Player::GetInstance()->AppearUpdate();
	enemymanager->AppearUpdate();

	camerawork->Update(camera);

	m_AppTimer++;
	if (m_AppTimer == 400) {
		_AppState = APP_NOTICE;
	}
	else if (m_AppTimer == 580) {
		_AppState = APP_VANISH;
	}

	//�e�L�X�g�֌W
	text_->Display();
	if (m_AppTimer == 1) {
		text_->SelectText(TextManager::TALK_FIRST);
	}
	else if (m_AppTimer == 150) {
		text_->SelectText(TextManager::TALK_SECOND);
	}
	else if (m_AppTimer == 300) {
		text_->SelectText(TextManager::TALK_THIRD);
		text_->ChangeColor(0, { 1.0f,0.0f,0.0f,1.0f });
	}
	else if (m_AppTimer == 400) {
		text_->SelectText(TextManager::TALK_FOURTH);
		for (int i = 0; i < 3; i++) {
			text_->ChangeColor(i, { 1.0f,1.0f,0.0f,1.0f });
		}
	}
	else if (m_AppTimer == 500) {
		text_->SelectText(TextManager::TALK_FIVE);
	}
}
//�o�g���V�[��
void SevenStageActor::MainUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
	ui->Update();
	//�J�������[�N�̃Z�b�g
	if (enemymanager->BossDestroy())
	{
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		//�t�F�[�h�O
		if (!camerawork->GetFeedEnd()) {
			enemymanager->SetDeadThrow(true);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_BEFORE);
		}
		//�t�F�[�h��
		else
		{
			PlayPostEffect = false;
			Player::GetInstance()->InitState({ 0.0f,0.0f,-5.0f });
			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_SEVEN);
		}

		if (camerawork->GetEndDeath()) {
			sceneChanger_->ChangeStart();
			sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::NonReverse);

		}

		Player::GetInstance()->DeathUpdate();
	}
	else
	{
		Player::GetInstance()->Update();
	}

	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::Reverse);
	}

	//���y�̉��ʂ��ς��
	VolumManager::GetInstance()->Update();

	//�e�N���X�X�V
	BackObj::GetInstance()->Update();


	enemymanager->BattleUpdate();
	loadobj->SevenUpdate();
	ParticleEmitter::GetInstance()->Update();

	camerawork->Update(camera);

	XMFLOAT3 Position = enemymanager->GetBoss()->GetPosition();
	XMVECTOR tex2DPos = { Position.x, Position.y, Position.z };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewMatrix(), false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetProjectionMatrix(), true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewPort(), false);

	postEffect->SetRadCenter(XMFLOAT2(tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]));
	postEffect->SetRadPower(camerawork->GetEffectPower());
}
//���j�V�[��
void SevenStageActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}
//�X�|�b�g���C�g�̓���
void SevenStageActor::MoveSpotLight() {
	const float l_AddAngle = 5.0f;
	const float l_AddFrame = 0.5f;
	const float l_DirMax = 2.0f;
	const float l_DirMin = -2.0f;
	const float l_PosMax = 100.0f;
	const float l_PosMin = -100.0f;
	//sin�g�ɂ���ď㉺�ɓ���
	if (_AppState == APP_START) {
		for (int i = 0; i < SPOT_NUM; i++) {
			m_Angle[i] += (l_AddAngle - (0.5f * i));
			m_Angle2[i] = m_Angle[i] * (3.14f / 180.0f);
		}

		spotLightDir[0].x = (sin(m_Angle2[0]) * 6.0f + (-2.0f));
		spotLightDir[0].z = (sin(m_Angle2[0]) * 6.0f + (-2.0f));
		spotLightDir[1].x = (sin(m_Angle2[1]) * 6.0f + (-2.0f));
		spotLightDir[1].z = (sin(m_Angle2[1]) * 6.0f + (2.0f));
		spotLightDir[2].x = (sin(m_Angle2[2]) * 6.0f + (2.0f));
		spotLightDir[2].z = (sin(m_Angle2[2]) * 6.0f + (-2.0f));
		spotLightDir[3].x = (sin(m_Angle2[3]) * 6.0f + (2.0f));
		spotLightDir[3].z = (sin(m_Angle2[3]) * 6.0f + (2.0f));

	}
	else if (_AppState == APP_NOTICE) {
		SpotSet(spotLightDir[0], { l_DirMin,{},l_DirMin }, l_AddFrame);
		SpotSet(spotLightDir[1], { l_DirMin,{},l_DirMax }, l_AddFrame);
		SpotSet(spotLightDir[2], { l_DirMax,{},l_DirMin }, l_AddFrame);
		SpotSet(spotLightDir[3], { l_DirMax,{},l_DirMax }, l_AddFrame);
	}
	else if (_AppState == APP_VANISH) {
		//�p�x
		SpotSet(spotLightDir[0], {}, l_AddFrame);
		SpotSet(spotLightDir[1], {}, l_AddFrame);
		SpotSet(spotLightDir[2], {}, l_AddFrame);
		SpotSet(spotLightDir[3], {}, l_AddFrame);
		//���W
		SpotSet(spotLightPos[0], { l_PosMax,spotLightPos[0].y,l_PosMax }, l_AddFrame);
		SpotSet(spotLightPos[1], { l_PosMax,spotLightPos[1].y,l_PosMin }, l_AddFrame);
		SpotSet(spotLightPos[2], { l_PosMin,spotLightPos[2].y,l_PosMax }, l_AddFrame);
		SpotSet(spotLightPos[3], { l_PosMin,spotLightPos[3].y,l_PosMin }, l_AddFrame);
	}
}
//�X�|�b�g���C�g
void SevenStageActor::SpotSet(XMFLOAT3& Pos, const XMFLOAT3& AfterPos, const float AddFrame) {
	Pos = { Ease(In,Cubic,AddFrame,Pos.x,AfterPos.x),
		Pos.y,
		Ease(In,Cubic,AddFrame,Pos.z,AfterPos.z),
	};
}