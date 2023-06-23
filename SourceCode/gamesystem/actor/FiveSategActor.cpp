#include "FiveSategActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>
#include "BackObj.h"
#include "Menu.h"

void FiveSategActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
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

	enemymanager = std::make_unique<EnemyManager>("SECONDSTAGE");
	//enemymanager->Initialize(dxCommon);
	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::ANGER_TALK);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("FIRSTSTAGE");
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();

	ui->SetBoss(enemymanager->GetBoss());
	BackObj::GetInstance()->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->LightSet(lightgroup);
	LoadStageObj::SetEnemyManager(enemymanager.get());

	m_SceneState = SceneState::IntroState;

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);

	Menu::GetIns()->Init();
}

void FiveSategActor::Finalize()
{
}

void FiveSategActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	//�֐��|�C���^�ŏ�ԊǗ�
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	sceneChanger_->Update();

	//�v���C���[
	if (enemymanager->BossDestroy() && camerawork->GetFeedEnd()) {
		SceneSave::GetInstance()->SetClearFlag(kSecondStage, true);
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

	Menu::GetIns()->Upda();
	postEffect->SetCloseRad(Menu::GetIns()->GetCloseIconRad());
}

void FiveSategActor::Draw(DirectXCommon* dxCommon)
{
	//�`����@
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw(dxCommon);
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
		dxCommon->PostDraw();
	}
}

void FiveSategActor::FrontDraw(DirectXCommon* dxCommon)
{
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
		if ((camerawork->GetAppearType() == APPEAR_SEVEN) || (camerawork->GetAppearType() == APPEAR_EIGHT)) {
			text_->SpriteDraw(dxCommon);
		}
	}
	IKESprite::PostDraw();
	sceneChanger_->Draw();
	Menu::GetIns()->Draw();
	camerawork->feedDraw();
}

void FiveSategActor::BackDraw(DirectXCommon* dxCommon)
{
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

void FiveSategActor::IntroUpdate(DebugCamera* camera)
{
	if (camerawork->GetAppearType() == APPEAR_SEVEN || camerawork->GetAppearType() == APPEAR_EIGHT) {
		text_->Display();
		//�ŏ��̌��t(�{��)
		if (m_AppState == AppState::ANGER_START) {
			text_->SelectText(TextManager::ANGER_TALK);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::ANGER_SECOND;
			}
		}
		//2�ڂ̌��t(�{��)
		else if (m_AppState == AppState::ANGER_SECOND) {
			text_->SelectText(TextManager::ANGER_TALK2);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::JOY_START;
				enemymanager->DirSet(DIR_JOY);
			}
		}
		//�ŏ��̌��t(���)
		else if (m_AppState == AppState::JOY_START) {
			text_->SelectText(TextManager::JOY_TALK);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::JOY_SECOND;
			}
		}
		//2�ڂ̌��t(���)
		else if (m_AppState == AppState::JOY_SECOND) {
			text_->SelectText(TextManager::JOY_TALK2);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::JOY_THIRD;
			}
		}
		//3�߂̌��t(���)
		else if (m_AppState == AppState::JOY_THIRD) {
			text_->SelectText(TextManager::JOY_TALK3);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::SELECT_EMO;
			}
		}
		//�I����
		else if (m_AppState == AppState::SELECT_EMO) {
			text_->ChangeColor(1, { 1.0f,0.0f,0.0f,1.0f });
			text_->ChangeColor(2, { 0.5f,1.0f,0.1f,1.0f });
			text_->SelectText(TextManager::SELECT_TALK);
			if (Input::GetInstance()->TriggerButton(Input::Y)) {
				enemymanager->DirSet(DIR_ANGER);
				m_AppState = AppState::EMO_ANGER;
			}
			else if (Input::GetInstance()->TriggerButton(Input::X)) {
				enemymanager->DirSet(DIR_JOY);
				m_AppState = AppState::EMO_JOY;
			}
		}
		//�C�J����I�񂾏ꍇ
		else if (m_AppState == AppState::EMO_ANGER) {
			for (int i = 0; i < 3; i++) {
				text_->ChangeColor(i, { 1.0f,1.0f,1.0f,1.0f });
			}
			text_->SelectText(TextManager::SELECT_ANGER);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::EMO_ANGER2;
				camerawork->SetApproach(true);
			}
		}
		else if (m_AppState == AppState::EMO_ANGER2) {
			for (int i = 0; i < 3; i++) {
				text_->ChangeColor(i, { 1.0f,0.0f,0.0f,1.0f });
			}
			text_->SelectText(TextManager::SELECT_ANGER2);
		}
		//��낱�т�I�񂾏ꍇ
		else if (m_AppState == AppState::EMO_JOY) {
			for (int i = 0; i < 3; i++) {
				text_->ChangeColor(i, { 1.0f,1.0f,1.0f,1.0f });
			}
			text_->SelectText(TextManager::SELECT_JOY);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::EMO_JOY2;
				camerawork->SetApproach(true);
			}
		}
		else if (m_AppState == AppState::EMO_JOY2) {
			for (int i = 0; i < 3; i++) {
				text_->ChangeColor(i, { 1.0f,0.0f,0.0f,1.0f });
			}
			text_->SelectText(TextManager::SELECT_JOY2);
		}
	}

	//�Ō�܂Ńe�L�X�g������
	if (enemymanager->GetEnemyFinishAppear()) {
		m_SceneState = SceneState::MainState;
		camerawork->SetCameraState(CAMERA_NORMAL);
	}

	//���o�X�L�b�v
	if (Input::GetInstance()->TriggerButton(Input::A)) {
		camerawork->SetCameraSkip(true);
	}

	if (camerawork->GetAppearEndF()) {
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
}

void FiveSategActor::MainUpdate(DebugCamera* camera)
{
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
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_SECOND);
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
	loadobj->SecondUpdate();
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

void FiveSategActor::FinishUpdate(DebugCamera* camera)
{
	Input* input = Input::GetInstance();
}
