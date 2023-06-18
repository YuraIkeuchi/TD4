#include "SelectSceneActor.h"
#include "SelectSceneActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include <HungerGauge.h>
#include "BackObj.h"
#include"SelectScene.h"

const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMVECTOR kPink{ 0.9f,0.6f,0.8f,1.f };

//������
void SelectSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	//�I�[�f�B�I
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	Audio::GetInstance()->LoopWave(1, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);
	//�|�X�g�G�t�F�N�g
	PlayPostEffect = false;
	//�p�[�e�B�N���S�폜
	ParticleEmitter::GetInstance()->AllDelete();

	SelectScene::GetIns()->Init();

	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	//�e�N���X
	//�v���C���[
	Player::GetInstance()->InitState({ 0.0f,5.0f,-70.0f });

	Player::GetInstance()->SetCanShot(false);
	Player::GetInstance()->MoveStop(true);
	//�G
	enemymanager = std::make_unique<EnemyManager>("THIRDSTAGE");
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
	
	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);
}
//�X�V
void SelectSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {


	const int IntervalTextC = 200;

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

	
	//���y�̉��ʂ��ς��
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	ui->Update();


	XMFLOAT3 Position = enemymanager->GetBoss()->GetPosition();
	XMVECTOR tex2DPos = { Position.x, Position.y, Position.z };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewMatrix(), false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetProjectionMatrix(), true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewPort(), false);

	postEffect->SetRadCenter(XMFLOAT2(tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]));
	postEffect->SetRadPower(camerawork->GetEffectPower());
	sceneChanger_->Update();

	SelectScene::GetIns()->Upda();
	camerawork->SetEye({ SelectScene::GetIns()->GetPedestalPos().x,
						SelectScene::GetIns()->GetPedestalPos().y + 20.f,
						SelectScene::GetIns()->GetPedestalPos().z - 50.f
		});
	camerawork->SetTarget(SelectScene::GetIns()->GetPedestalPos());

	camerawork->DefUpda(camera);

	lightgroup->Update();
}
//�`��
void SelectSceneActor::Draw(DirectXCommon* dxCommon) {
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
	} else {
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
void SelectSceneActor::Finalize() {
}
//���̕`��
void SelectSceneActor::BackDraw(DirectXCommon* dxCommon) {
	
	SelectScene::GetIns()->Draw_Obj(dxCommon);
}
//�|�X�g�G�t�F�N�g��������Ȃ�
void SelectSceneActor::FrontDraw(DirectXCommon* dxCommon) {


	//ParticleEmitter::GetInstance()->DeathDrawAll();

	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_BEFORE && camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
		ui->Draw();

	sceneChanger_->Draw();	//���S�ɑO�ɏ����X�v���C�g
	//if (camerawork->GetAppearType() == APPEAR_SEVEN || camerawork->GetAppearType() == APPEAR_EIGHT) {
	IKESprite::PreDraw();
	//blackwindow->Draw();
	SelectScene::GetIns()->Draw_Sprite();
	camerawork->feedDraw();
	IKESprite::PostDraw();
}
//IMGui�̕`��
void SelectSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//loadobj->ImGuiDraw();
	//SceneSave::GetInstance()->ImGuiDraw();
}

