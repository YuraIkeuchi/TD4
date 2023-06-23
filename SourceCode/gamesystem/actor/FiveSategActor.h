#pragma once
#include"BaseActor.h"
#include"BossText.h"
#include<windows.h>
#include<vector>

class FiveSategActor:public BaseActor
{
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
private:

	void IntroUpdate(DebugCamera* camera)override;		//�o��V�[��
	void MainUpdate(DebugCamera* camera)override;		//�o�g���V�[��
	void FinishUpdate(DebugCamera* camera)override;		//���j�V�[��

	unique_ptr<BossText> text_;
	unique_ptr<IKESprite> backScreen_ = nullptr;
private:

	//�����V�[��
	enum class AppState : int {
		ANGER_START,
		ANGER_SECOND,
		JOY_START,
		JOY_SECOND,
		JOY_THIRD,
		SELECT_EMO,
		EMO_JOY,
		EMO_JOY2,
		EMO_ANGER,
		EMO_ANGER2,
	};

	//�V�[���ł̑J��
	AppState m_AppState = AppState::ANGER_START;


	//�ۉe(�{�X)
	float BosscircleShadowDir[3] = { 0,-1,0 };
	float BosscircleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float BosscircleShadowFactorAngle[2] = { 1.8f, 1.8f };
};

