#pragma once
#include "DebugCamera.h"
#include "Camera.h"
#include "Shake.h"
#include <memory>
#include "Helper.h"
using namespace std;         //  ���O��Ԏw��
//�J�����̎��
enum CameraType {
	CameraTitle,
	CameraIntroduction,
	CameraNormal,
	CameraBossApp,
	CameraBossEnd,
	CameraClear,
};
//�Q�[���̃J�����̓���
class CameraWork {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	CameraWork();
	void Update(DebugCamera* camera);//�X�V
	void ImGuiDraw();
public:
	//getter setter

private:
	//�N���X
	unique_ptr<Helper> helper;
	unique_ptr<Shake> shake = nullptr;
	//�C�[�W���O�̕ϐ�
	float m_Frame = 0.0f;
	XMFLOAT3 m_AfterEye = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_AfterTarget = { 0.0f,0.0f,0.0f };
	//���_���W
	XMFLOAT3 m_eyePos = { 0.0f,0.0f,0.0f };
	//�����_���W
	XMFLOAT3 m_targetPos = { 0.0f,0.0f,0.0f };
};