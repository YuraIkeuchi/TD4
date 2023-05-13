#include "SecondStageActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>

//������
void SecondStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	//�I�[�f�B�I
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/Boss.wav");
	//�|�X�g�G�t�F�N�g
	PlayPostEffect = true;
	//�p�[�e�B�N���S�폜
	ParticleEmitter::GetInstance()->AllDelete();

	//�e�N���X
	Player::GetInstance()->InitState({ 0.0f,0.0f,0.0f });
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();

	conversationwindow = IKESprite::Create(ImageManager::WINDOW, window_pos);
	conversationwindow->SetAnchorPoint({ 0.5f,0.5f });
	conversationwindow->SetSize(window_size);

	blackwindow = IKESprite::Create(ImageManager::BLACKWINDOW, {});


	enemymanager = std::make_unique<EnemyManager>("SECONDSTAGE");

	backobj = std::make_unique<BackObj>();
	backobj->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad();
}
//�X�V
void SecondStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();

	if (enemymanager->BossDestroy()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	if (PlayerDestroy()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	//���y�̉��ʂ��ς��
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();

	if (input->Pushkey(DIK_A) &&
		nowstate != CONVERSATION) {
		nowstate = CONVERSATION;
		frame = {};
	}
	if (input->Pushkey(DIK_S) &&
		nowstate != FIGHT) {
		nowstate = FIGHT;
		frame = {};
	}
	if (nowstate == CONVERSATION) {
		frame++;
		nowframe = frame / maxframe;
		if (frame >= maxframe) {
			frame = maxframe;
		}
		window_pos.y = Ease(Out, Sine, nowframe, WinApp::window_height + 100, WinApp::window_height - 100);
		window_size.x = Ease(Out, Sine, nowframe, 0, 1300);
		window_size.y = Ease(Out, Sine, nowframe, 0, 223);
		black_color.w = Ease(Out, Sine, nowframe, 0, 1);
	}
	else if (nowstate == FIGHT) {
		frame++;
		nowframe = frame / maxframe;
		if (frame >= maxframe) {
			frame = maxframe;
		}
		window_pos.y = Ease(Out, Sine, nowframe, WinApp::window_height - 100, WinApp::window_height + 100);
		window_size.x = Ease(Out, Sine, nowframe, 1300, 0);
		window_size.y = Ease(Out, Sine, nowframe, 225, 0);
		black_color.w = Ease(Out, Sine, nowframe, 1, 0);
	}


	conversationwindow->SetPosition(window_pos);
	conversationwindow->SetSize(window_size);
	blackwindow->SetColor(black_color);

	//�e�N���X�X�V
	backobj->Update();
	if (nowstate != CONVERSATION) {
		Player::GetInstance()->Update();
		enemymanager->Update();
		loadobj->Update();
		ParticleEmitter::GetInstance()->Update();
	}
	camerawork->Update(camera);
	lightgroup->Update();
}
//�`��
void SecondStageActor::Draw(DirectXCommon* dxCommon) {
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
		dxCommon->PostDraw();
	}
}
//���
void SecondStageActor::Finalize() {
}
//���̕`��
void SecondStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	////�e�N���X�̕`��
	Player::GetInstance()->Draw(dxCommon);
	loadobj->Draw(dxCommon);
	backobj->Draw(dxCommon);
	enemymanager->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//�|�X�g�G�t�F�N�g��������Ȃ�
void SecondStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->FlontDrawAll();
	//���S�ɑO�ɏ����X�v���C�g
	IKESprite::PreDraw();
	blackwindow->Draw();
	conversationwindow->Draw();
	//ui->Draw();
	IKESprite::PostDraw();
}
//IMGui�̕`��
void SecondStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
	enemymanager->ImGuiDraw();
}
