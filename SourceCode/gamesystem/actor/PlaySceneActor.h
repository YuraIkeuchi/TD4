#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "IKEObject3d.h"
#include "IKEModel.h"
#include <vector>
#include<windows.h>
#include"Player.h"
#include "UI.h"
#include "EnemyManager.h"
#include "FirstBoss.h"

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
	void NormalUpdate();//���ʂ̍X�V
	void ImGuiDraw(DirectXCommon* dxCommon);
private:
	//OBJ
	unique_ptr<IKEObject3d> objGround;
	unique_ptr<Player>player;
	unique_ptr<EnemyManager> enemymanager;
	unique_ptr<InterBoss> boss;
	unique_ptr<UI>ui;
	std::vector<std::vector<int>> map; //�}�b�v�`�b�v(1�}�b�v)
	IKEModel* modelGround = nullptr;
};