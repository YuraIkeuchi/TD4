#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <DirectXMath.h>

/*--�~���Ƃ����߂�p--*/
#define PI 3.14f
#define PI_90 90.00f
#define PI_180 180.00f
#define PI_360 360.00f
/*-----------------*/

//�⏕�N���X
class Helper
{
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static Helper* GetInstance();
	//�v�Z�n
	bool CheckMax(float& Num, const float Max, const float Add);
	bool CheckMin(float& Num, const float Min, const float Add);
	//Clamp��Ԃ��֐�
	void IntClamp(int& Num, const int Min, const int Max);
	void FloatClamp(float& Num, const float Min, const float Max);
	//XMFLOAT3��float�����Z����֐�
	XMFLOAT3 Float3AddFloat(const XMFLOAT3& Num, const float Add);
	//XMFLOAT3��XMFLOAT3�����Z����֐�
	XMFLOAT3 Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add);
	//XMFLOAT3��float�����Z����֐�
	XMFLOAT3 Float3SubFloat(const XMFLOAT3& Num, const float Sub);
	//XMFLOAT3��XMFLOAT3�����Z����֐�
	XMFLOAT3 Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub);

	//�Ǐ]�֐�
	XMFLOAT3 FollowMove(const XMFLOAT3& pos, const XMFLOAT3& pos2, const float vel);

	//�~�^��
	XMFLOAT3 CircleMove(const XMFLOAT3& basepos, const float scale, const float speed);
public:
	//������Ԃ�
	float GetPercent(const float amount, const float num);

	//2D->3D�ϊ��Ɏg��(�X�v���C�g�̍��W���v���C���[�̃��[���h���W�ɕ\�������肷��̂Ɏg��)
	//���W�n�����m�łȂ��ꍇ->�s�񂩂�v�Z->W���Z
	XMVECTOR WDivision(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub);
	//���W�n�����m�ȏꍇ->���̂܂܌v�Z->W���Z
	XMVECTOR WDivision(const XMVECTOR& pos, const bool sub);

	//���[���h���W�ɕϊ�
	XMVECTOR PosDivi(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub);
	
};

