#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "JsonLoader.h"

#include <array>
/// �^�C�g���V�[��
class LoadSceneActor : public BaseActor {
public:
	/// ������
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �`��
	void Draw(DirectXCommon* dxCommon) override;
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void SpriteDraw();
private:
	void CreateStage();
	void IntroUpdate()override;
	void MainUpdate()override;
	void FinishUpdate()override;


private://�����o�ϐ�
	int m_LoadTimer = 0;
	const int LoadTimerMax = 60;
	//�㉺�^���̒��S
	const XMFLOAT2 CenterPos = { 100.f,630.0f };
	//�����x
	const float AddMovingVal = 4.0f;
	//�����̊Ԋu
	const float WordsInter = 100.f;
	//�c���̊Ԋu
	const float space = 30.0f;

	JsonData* m_JsonData = nullptr;
	std::list<std::unique_ptr<IKEObject3d>> grounds;
	std::map<std::string, std::unique_ptr<IKEModel>> models;

	std::unique_ptr<IKEModel> modelSkydome = nullptr;
	std::unique_ptr<IKEModel> modelGround = nullptr;
	std::unique_ptr<IKEModel> modelFighter = nullptr;
	std::unique_ptr<IKEModel> modelSphere = nullptr;
	std::unique_ptr<IKEModel> modelPine = nullptr;

	//Loading
	enum {
		text_L=0,
		text_O,
		text_A,
		text_D,
		text_I,
		text_N,
		text_G,
		SpriteMax
	};


	std::array<std::unique_ptr<IKESprite>, SpriteMax> m_Sprites = {};
	std::array<XMFLOAT2, SpriteMax> m_SpritesPos = {};
	std::array<float, SpriteMax> m_SpritesAngle = {};
	std::array<XMFLOAT2, SpriteMax> m_StopPos = {};

};
//m_SceneState = SceneState::FinishState;
