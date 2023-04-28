#pragma once
#include "BaseActor.h"
#include <vector>
#include<windows.h>
#include"Player.h"
#include "UI.h"
#include "EnemyManager.h"
#include "FirstBoss.h"
#include "BackObj.h"
#include "LoadStageObj.h"
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
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
private:
	//�N���X
	Player* player = nullptr;
	unique_ptr<EnemyManager> enemymanager;
	unique_ptr<InterBoss> boss;
	unique_ptr<UI>ui;
	unique_ptr<BackObj> backobj;
	unique_ptr<LoadStageObj> loadobj;
	std::vector<std::vector<int>> map; //�}�b�v�`�b�v(1�}�b�v)
};