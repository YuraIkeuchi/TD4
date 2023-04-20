#include "Helper.h"
#include"WinApp.h"

Helper* Helper::GetInstance()
{
	static Helper instance;

	return &instance;
}
//max min��bool���ŕԂ�
bool Helper::CheckMax(float& Num, const float Max, const float Add) {
	Num += Add;

	Num = max(Num, Max);


	if (Num <= Max) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
//Min������
bool Helper::CheckMin(float& Num, const float Min, const float Add) {
	Num += Add;
	Num = min(Num, Min);

	if (Num >= Min) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
//Clamp��Ԃ��֐�
//int�^
void Helper::IntClamp(int& Num, const int Min, const int Max) {
	Num = min(max(Num, Min), Max);
}
//float�^
void Helper::FloatClamp(float& Num, const float Min, const float Max) {
	Num = min(max(Num, Min), Max);
}
//XMFLOAT3��float�����Z����֐�
Helper::XMFLOAT3 Helper::Float3AddFloat(const XMFLOAT3& Num, float Add) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x + Add;
	l_Result.y = Num.y + Add;
	l_Result.z = Num.z + Add;

	return l_Result;
}
//XMFLOAT3��XMFLOAT3�����Z����֐�
Helper::XMFLOAT3 Helper::Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x + Add.x;
	l_Result.y = Num.y + Add.y;
	l_Result.z = Num.z + Add.z;

	return l_Result;
}
//XMFLOAT3��float�����Z����֐�
Helper::XMFLOAT3 Helper::Float3SubFloat(const XMFLOAT3& Num, float Sub) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x - Sub;
	l_Result.y = Num.y - Sub;
	l_Result.z = Num.z - Sub;

	return l_Result;
}
//XMFLOAT3��XMFLOAT3�����Z����֐�
Helper::XMFLOAT3 Helper::Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x - Sub.x;
	l_Result.y = Num.y - Sub.y;
	l_Result.z = Num.z - Sub.z;

	return l_Result;
}
//������Ԃ�
float Helper::GetPercent(const float amount, const float num)
{
	return num / amount * 100.00f;
}
//2D->3D�ϊ��Ɏg��(�X�v���C�g�̍��W���v���C���[�̃��[���h���W�ɕ\�������肷��̂Ɏg��)
//���W�n�����m�łȂ��ꍇ->�s�񂩂�v�Z->W���Z
Helper::XMVECTOR Helper::WDivision(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub)
{
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = pos.m128_f32[3];
	if (sub) {
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{ x, y, z, w };
}
//���W�n�����m
Helper::XMVECTOR Helper::WDivision(const XMVECTOR& pos, const bool sub)
{
	float x, y, z, w;
	x = pos.m128_f32[0];
	y = pos.m128_f32[1];
	z = pos.m128_f32[2];
	w = pos.m128_f32[3];

	if (sub)
	{
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{ x, y, z, w };
}
//���[���h���W�ɕϊ�
Helper::XMVECTOR Helper::PosDivi(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub)
{
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = 1.0f;

	if (sub)
	{
		w = z;
	}

	return XMVECTOR{ x, y, z, w };
}
Helper::XMVECTOR Helper::WorldDivision(const XMVECTOR& pos, const bool sub) {
	XMVECTOR l_Result;
	l_Result = PosDivi(l_Result, m_Camera->GetViewMatrix(), false);
	l_Result = PosDivi(l_Result, m_Camera->GetProjectionMatrix(), true);
	l_Result = WDivision(pos, true);
	l_Result = PosDivi(l_Result, m_Camera->GetViewPort(), false);

	return l_Result;
}

//�Ǐ]
Helper::XMFLOAT3 Helper::FollowMove(const XMFLOAT3& pos, const XMFLOAT3& pos2, const float vel) {
	XMFLOAT3 l_Distance;//��_�̋���
	XMFLOAT3 l_AddVel;//���Z�����ړ���
	XMFLOAT3 l_Result{};//�Ԃ��l

	//�Ƃ肠����X����Z���̂ݒǏ]
	l_Distance.x = pos.x - pos2.x;
	l_Distance.z = pos.z - pos2.z;

	l_AddVel.x = (float)(sin(atan2f(l_Distance.x, l_Distance.z)) * vel);
	l_AddVel.z = (float)(cos(atan2f(l_Distance.x, l_Distance.z)) * vel);

	l_Result.y = pos.y;
	l_Result.x += l_AddVel.x;
	l_Result.z += l_AddVel.z;

	//�܂���]�Ƃ��͍l�����ĂȂ���

	return l_Result;
}
//�~�^��
Helper::XMFLOAT3 Helper::CircleMove(const XMFLOAT3& basepos, const float scale, const float speed) {
	float l_Radius;//
	XMFLOAT3 l_AddVel;//�ړ���
	XMFLOAT3 l_Result{};//�Ԃ��l

	//�~�^���̏���(X����Z��)
	l_Radius = speed * PI / PI_180;
	l_AddVel.x = cosf(l_Radius) * scale;
	l_AddVel.z = sinf(l_Radius) * scale;
	l_Result.x = l_AddVel.x + basepos.x;
	l_Result.z = l_AddVel.z + basepos.z;
	l_Result.y = basepos.y;
	return l_Result;
}
//�l���͈͊O�ɂ��邩
bool Helper::CheckNotValueRange(float& Num, const float Min, const float Max) {
	if ((Num > Max || Num < Min)) {
		return true;
	}
	else {
		return false;
	}

	return true;
}
//�l���͈͓��ɂ��邩
bool Helper::CheckValueRange(float& Num, const float Min, const float Max) {
	if ((Num < Max && Num > Min)) {
		return true;
	}
	else {
		return false;
	}

	return true;
}
//�X�N���[�����W�ɕϊ�
Helper::XMFLOAT2 Helper::WorldtoScreen(const XMVECTOR& pos, const XMMATRIX& mat) {
	const float kDistancePlayerTo3DReticle = 50.0f;
	XMVECTOR l_OffSet;
	XMMATRIX MatViewPort;
	l_OffSet = { 0.0,0.0,1.0f };
	l_OffSet = XMVector3TransformNormal(l_OffSet, mat);
	l_OffSet = m_Camera->CameraNormalize(l_OffSet, kDistancePlayerTo3DReticle);
	XMVECTOR PositionRet = pos;

	ChangeViewPort(MatViewPort, l_OffSet);

	XMMATRIX MatVP = MatViewPort;

	XMMATRIX View = m_Camera->GetViewMatrix();
	XMMATRIX Pro = m_Camera->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}
//�r���[�|�[�g�ϊ�
void Helper::ChangeViewPort(XMMATRIX& matviewport, const XMVECTOR& offset) {
	matviewport.r[0].m128_f32[0] = WinApp::window_width / 2;
	matviewport.r[0].m128_f32[1] = 0;
	matviewport.r[0].m128_f32[2] = 0;
	matviewport.r[0].m128_f32[3] = 0;

	matviewport.r[1].m128_f32[0] = 0;
	matviewport.r[1].m128_f32[1] = -(WinApp::window_height / 2);
	matviewport.r[1].m128_f32[2] = 0;
	matviewport.r[1].m128_f32[3] = 0;

	matviewport.r[2].m128_f32[0] = 0;
	matviewport.r[2].m128_f32[1] = 0;
	matviewport.r[2].m128_f32[2] = 1;
	matviewport.r[2].m128_f32[3] = 0;

	matviewport.r[3].m128_f32[0] = WinApp::window_width / 2 + offset.m128_f32[0];
	matviewport.r[3].m128_f32[1] = WinApp::window_height / 2 + offset.m128_f32[1];
	matviewport.r[3].m128_f32[2] = 0;
	matviewport.r[3].m128_f32[3] = 1;
}
//XMFLOAT��XMVECTOR�ɕϊ�
Helper::XMVECTOR Helper::ChangeFLOAT(const XMFLOAT3& pos) {
	XMVECTOR l_Result;
	l_Result.m128_f32[0] = pos.x;
	l_Result.m128_f32[1] = pos.y;
	l_Result.m128_f32[2] = pos.z;

	return l_Result;
}
//XMFLOAT��XMVECTOR�ɕϊ�
Helper::XMFLOAT3 Helper::ChangeVECTOR(const XMVECTOR& pos) {
	XMFLOAT3 l_Result;
	l_Result.x = pos.m128_f32[0];
	l_Result.y = pos.m128_f32[1];
	l_Result.z = pos.m128_f32[2];

	return l_Result;
}