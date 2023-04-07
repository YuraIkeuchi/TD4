#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "IKEObject3d.h"
#include "IKEModel.h"
#include <vector>
#include<windows.h>
#include"Player.h"

/// �^�C�g���V�[��
class PlaySceneActor : public BaseActor {
public:
	/// ������
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �`��
	void Draw(DirectXCommon* dxCommon) override;
	void PlaySceneInitialize();//�v���C�V�[���݂̂̏�����
	void ModelDraw(DirectXCommon* dxCommon);
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
	void NormalDraw(DirectXCommon* dxCommon);//���ʂ̕`��
	void BossAppDraw(DirectXCommon* dxCommon);//�{�X�o��V�[���̕`��
	void BossEndDraw(DirectXCommon* dxCommon);//�{�X�o��V�[���̕`��
	void NormalUpdate();//���ʂ̍X�V
	void BossAppUpdate();//�{�X�o��V�[���̍X�V
	void BossEndUpdate();//�{�X�I���V�[���̍X�V
	void ImGuiDraw(DirectXCommon* dxCommon);
private:
	//�X�v���C�g
	unique_ptr<IKESprite> PlaySprite;
	//OBJ
	unique_ptr<IKEObject3d> objGround;
	unique_ptr<IKEObject3d> objCube;
	unique_ptr<Player>player;
	IKEModel* modelGround = nullptr;
	IKEModel* modelCube = nullptr;
};