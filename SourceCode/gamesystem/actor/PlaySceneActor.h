#pragma once
#include "BaseActor.h"
#include <vector>
#include<windows.h>
#include"Player.h"
#include "UI.h"
#include "EnemyManager.h"
#include "FirstBoss.h"
#include "BackObj.h"
#include "LoadFood.h"
#include "LoadGhost.h"
/// �^�C�g���V�[��
class PlaySceneActor : public BaseActor {
private:
	enum state {
		CONVERSATION=0,
		FIGHT,
		NONE,
	};
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
	unique_ptr<LoadFood> loadfood;
	unique_ptr<LoadGhost> loadghost;
	unique_ptr<IKESprite> conversationwindow;
	unique_ptr<IKESprite> blackwindow;
	//���W
	XMFLOAT2 window_pos{ WinApp::window_width/2.f,WinApp::window_height+100 };
	XMFLOAT2 window_size{ 0.f,0.f };

	XMFLOAT4 black_color{ 1.f,1.f,1.f,0.f };

	float maxframe = 20.f;
	float nowframe = 0.f;
	float frame = 0.f;
	int nowstate = NONE;
	std::vector<std::vector<int>> map; //�}�b�v�`�b�v(1�}�b�v)
};