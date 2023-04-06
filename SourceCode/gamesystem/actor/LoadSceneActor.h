#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
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
	void ModelDraw(DirectXCommon* dxCommon);
	void GameDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void SpriteDraw();
private:
	//�萔
	static const int Now_Max = 4;
	static const int Load_Max = 5;
private://�����o�ϐ�
};