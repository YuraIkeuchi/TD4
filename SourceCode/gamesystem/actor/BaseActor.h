#pragma once
#include "LightGroup.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "PostEffect.h"
#include "ParticleManager.h"
#include "VolumManager.h"
#include "IKEFBXObject3d.h"
#include "IKEObject3d.h"
#include "CameraWork.h"
using namespace std;         //  ���O��Ԏw��
//Actor�N���X�̊���
class BaseActor
{
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//���z�f�X�g���N�^
	virtual ~BaseActor() = default;

	//������
	virtual void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) = 0;
	//�J��
	virtual void Finalize() = 0;
	//�X�V
	virtual void Update(DirectXCommon* dxCommon, DebugCamera* camera,LightGroup* lightgroup) = 0;
	//�`��
	virtual void Draw(DirectXCommon* dxCommon) = 0;
	//���ʏ�����
	void BaseInitialize(DirectXCommon* dxCommon);
protected:

	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//�_����
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 15.0f,15.0f,15.0f };
	float pointLightPower[3] = { 20.0f,20.0f,20.0f };
	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.8f };
	unique_ptr<PostEffect> postEffect = nullptr;
	unique_ptr<CameraWork> camerawork;
	bool PlayPostEffect = false;
};