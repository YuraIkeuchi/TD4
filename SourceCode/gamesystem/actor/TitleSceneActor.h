#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "Helper.h"
/// �^�C�g���V�[��
class TitleSceneActor : public BaseActor {
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
	void FrontDraw();
private:
	static const int TITLE_MAX = 3;//�^�C�g���p�[�c�̍ő吔
	static const int MODE_MAX = 3;//���[�h�p�[�c�̍ő吔
	static const int EXPLAIN_MAX = 3;//�������̍ő吔
private://�����o�ϐ�
	//�N���X
	unique_ptr<Helper> helper;
	//�X�v���C�g
	unique_ptr<IKESprite> TitleSprite;
};