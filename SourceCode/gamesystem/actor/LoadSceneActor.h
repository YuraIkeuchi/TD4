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


private://�����o�ϐ�
	int m_LoadTimer = 0;
	
	JsonData* m_JsonData = nullptr;
	std::list<std::unique_ptr<IKEObject3d>> grounds;
	std::map<std::string, std::unique_ptr<IKEModel>> models;

	std::unique_ptr<IKEModel> modelSkydome = nullptr;
	std::unique_ptr<IKEModel> modelGround = nullptr;
	std::unique_ptr<IKEModel> modelFighter = nullptr;
	std::unique_ptr<IKEModel> modelSphere = nullptr;
	std::unique_ptr<IKEModel> modelPine = nullptr;

};