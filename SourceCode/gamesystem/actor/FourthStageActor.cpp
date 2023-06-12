#include "FourthStageActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include <HungerGauge.h>
#include "BackObj.h"

const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMVECTOR kPink{ 0.9f,0.6f,0.8f,1.f };

//������
void FourthStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	//�I�[�f�B�I
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	Audio::GetInstance()->LoopWave(1, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);
	//�|�X�g�G�t�F�N�g
	PlayPostEffect = true;

	//�p�[�e�B�N���S�폜
	ParticleEmitter::GetInstance()->AllDelete();

	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	backScreen_ = IKESprite::Create(ImageManager::PLAY, { 0,0 });
	backScreen_->SetSize({ 1280.0f,720.0f });

	//�e�N���X
	//�v���C���[
	Player::GetInstance()->InitState({ 0.0f,5.0f,-70.0f });

	Player::GetInstance()->SetCanShot(false);
	Player::GetInstance()->MoveStop(true);
	//�G
	enemymanager = std::make_unique<EnemyManager>("FOURTHSTAGE");
	//�J����
	camerawork->Update(camera);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("FIRSTSTAGE");
	//UI
	ui = std::make_unique<UI>();
	ui->Initialize();
	ui->SetBoss(enemymanager->GetBoss());
	//�w�i
	BackObj::GetInstance()->Initialize();
	//�X�e�[�WOBJ
	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->LightSet(lightgroup);
	LoadStageObj::SetEnemyManager(enemymanager.get());

	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::Name_First::VIEWBOSS);

	messagewindow_ = make_unique<MessageWindow>();
	messagewindow_->Initialize();
	messagewindow_->Display();

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);
}
//�X�V
void FourthStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {


	const int IntervalTextC = 200;


	messagewindow_->DisplayCharacter(sutopon_color_);
	
	Input* input = Input::GetInstance();

	//�v���C���[
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.0f, Player::GetInstance()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	//�{�X
	lightgroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemymanager->GetBoss()->GetPosition().x, 0.0f, 	enemymanager->GetBoss()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));


	if (enemymanager->BossDestroy()) {
		Audio::GetInstance()->StopWave(1);
		SceneSave::GetInstance()->SetClearFlag(kFirstStage, true);
		if (camerawork->GetCameraState() == CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
		{
			PlayPostEffect = false;
		}
	}

	if (camerawork->GetAppearEndF()) {
		Player::GetInstance()->MoveStop(false);
		Player::GetInstance()->SetCanShot(true);
		camerawork->SetCameraState(CAMERA_NORMAL);
		//enemymanager->SkipInitialize();
	}

	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(1);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::NonReverse);
	}
	//���y�̉��ʂ��ς��
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	ui->Update();
	//�e�N���X�X�V
	BackObj::GetInstance()->Update();


	if (enemymanager->BossDestroy())
	{
		Player::GetInstance()->DeathUpdate();
	}
	else
	{
		Player::GetInstance()->Update();
	}

	enemymanager->BattleUpdate();
	ColEnemy(enemymanager->GetBulEnemy());
	loadobj->FirstUpdate();
	ParticleEmitter::GetInstance()->Update();
	if (input->TriggerKey(DIK_X)) {
		Audio::GetInstance()->StopWave(1);
		SceneManager::GetInstance()->ChangeScene("SECONDSTAGE");

	}
	//�J�������[�N�̃Z�b�g
	if (enemymanager->BossDestroy())
	{
		//�t�F�[�h�O
		if (!camerawork->GetFeedEnd()) {
			enemymanager->SetDeadThrow(true);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_BEFORE);
		}
		//�t�F�[�h��
		else
		{
			Player::GetInstance()->InitState({ 0.0f,0.0f,-5.0f });

			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_FIRST);
		}


		if (camerawork->GetEndDeath()) {
			sceneChanger_->ChangeStart();
			sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::ReverseType::NonReverse);
		}
	}
	else
	{
		if (camerawork->FinishAppear()) {
			//m_SceneState = SceneState::MainState;
		//	camerawork->SetCameraState(CAMERA_NORMAL);
		}
	}
	if (PlayerDestroy()) {
		std::string str = "GAMEOVER";
		Audio::GetInstance()->StopWave(1);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene(str, SceneChanger::Reverse);
	}

	XMFLOAT3 Position = enemymanager->GetBoss()->GetPosition();
	XMVECTOR tex2DPos = { Position.x, Position.y, Position.z };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewMatrix(), false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetProjectionMatrix(), true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewPort(), false);

	postEffect->SetRadCenter(XMFLOAT2(tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]));
	postEffect->SetRadPower(camerawork->GetEffectPower());
	sceneChanger_->Update();

	//if(_Tscne!=TextScene::ENDTEXT)
	//text_->Display();

	messagewindow_->Update(girl_color_, sutopon_color_);

	camerawork->Update(camera);
	lightgroup->Update();
}
//�`��
void FourthStageActor::Draw(DirectXCommon* dxCommon) {
	//�`����@
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw(dxCommon);
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
void FourthStageActor::Finalize() {
}
//���̕`��
void FourthStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	backScreen_->Draw();
	IKESprite::PostDraw();

	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);

	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSAPPEAR &&
		camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
		if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_BEFORE && camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
			ParticleEmitter::GetInstance()->BackDrawAll();
		}
	}

	ParticleEmitter::GetInstance()->DeathDrawAll();
	////�e�N���X�̕`��
	if (!camerawork->GetFeedEnd()) {
		Player::GetInstance()->Draw(dxCommon);
		loadobj->Draw(dxCommon);
	}
	enemymanager->Draw(dxCommon);

	IKEObject3d::PostDraw();
}
//�|�X�g�G�t�F�N�g��������Ȃ�
void FourthStageActor::FrontDraw(DirectXCommon* dxCommon) {


	//�p�[�e�B�N���`��
	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSAPPEAR &&
		camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
		//ParticleEmitter::GetInstance()->FlontDrawAll();
		if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSAPPEAR &&
			camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {

			ParticleEmitter::GetInstance()->FlontDrawAll();
		}



	//ParticleEmitter::GetInstance()->DeathDrawAll();

	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_BEFORE && camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
		ui->Draw();

	//IKESprite::PreDraw();
	//messagewindow_->Draw();
	//IKESprite::PostDraw();

	//if (messagewindow_->DisplayCheck()) {
	//	text_->SpriteDraw(dxCommon);
	//}
	
	sceneChanger_->Draw();	//���S�ɑO�ɏ����X�v���C�g

	IKESprite::PreDraw();
	//blackwindow->Draw();
	camerawork->feedDraw();
	IKESprite::PostDraw();
}
//IMGui�̕`��
void FourthStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
	enemymanager->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//SceneSave::GetInstance()->ImGuiDraw();
}


void FourthStageActor::ColEnemy(std::vector<InterEnemy*> enelist)
{
	for (auto i = 0; i < enelist.size(); ++i) {
		for (auto j = 0; j < enelist.size(); ++j) {
			XMFLOAT3 ghostpos = enelist[i]->GetPosition();
			XMFLOAT3 ghostpos2 = enelist[j]->GetPosition();
			if ((i == j)) { continue; }
			if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
				enelist[i]->EnemyColNormal(ghostpos2);
				enelist[j]->EnemyColNormal(ghostpos);
			}
		}
	}
}