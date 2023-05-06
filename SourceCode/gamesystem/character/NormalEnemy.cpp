#include "NormalEnemy.h"

#include "Collision.h"
#include "ModelManager.h"
#include "Helper.h"
#include "ParticleEmitter.h"
//���f���ǂݍ���
NormalEnemy::NormalEnemy() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BulEnemy);
}
//������
bool NormalEnemy::Initialize() {

	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	m_Scale = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Position.y = 5.0f;
	ret = true;
	return true;
}
//�s��
void NormalEnemy::Action() {
	XMFLOAT3 l_player = player->GetPosition();
	float l_Vel = 0.15f;
	//m_CircleSpeed += 1.0f;
	m_Rotation.x = 90.f;
	if (ShotF) {
		ShotCount++;
		Appearance();
		RushAction();
	}

	//m_Position = Helper::GetInstance()->CircleMove({ 0.0f,5.0f,0.0f }, m_CircleScale, m_CircleSpeed);
	if (OnCollision())
	{
		//player->RecvDamage(10);
	}


	Obj_SetParam();
	Particle();
}
//�`��
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	Obj_Draw();
}
//ImGui�`��
void NormalEnemy::ImGuiDraw() {
	ImGui::Begin("Enemy");
	//ImGui::Text("time %f", t);
	ImGui::Text("RotOld %f", old);
	//ImGui::Text("Roty %f", RotY);

	ImGui::Text("PosX:%f", player->GetPosition().x);
	ImGui::Text("PosZ:%f", player->GetPosition().z);
	ImGui::Text("EnePosX:%f", m_Position.x);
	ImGui::Text("EnePosZ:%f", m_Position.z);
	ImGui::End();
}
//�J��
void NormalEnemy::Finalize() {

}

//�p�[�e�B�N��
void NormalEnemy::Particle() {
	XMFLOAT4 s_color = { 1.0f,0.5f,1.0f,1.5f };
	XMFLOAT4 e_color = { 1.0f,0.5f,1.0f,0.1f };
	//float s_scale = 3.0f;
	float e_scale = 0.0f;
	ParticleEmitter::GetInstance()->FireEffect(10, m_Position, s_scale, e_scale, s_color, e_color);
}

void NormalEnemy::Appearance()
{
	if (Rush)return;
	XMFLOAT3 l_player = player->GetPosition();

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	PositionA = { l_player.x,l_player.y,l_player.z };
	PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	constexpr float AddScaling = 0.01f;

	m_Scale.x += AddScaling;
	m_Scale.y += AddScaling;
	m_Scale.z += AddScaling;

	Helper::GetInstance()->FloatClamp(m_Scale.x, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(m_Scale.y, 0.f, 1.f);
	Helper::GetInstance()->FloatClamp(m_Scale.z, 0.f, 1.f);

	float RottoPlayer;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	m_Rotation.y = RottoPlayer * 60.f + 180.f;

	if (m_Scale.x >= 1.f || m_Scale.y >= 1.f || m_Scale.z >= 1.f)
	{
		Rush = true;
	}




}

void NormalEnemy::RushAction()
{
	if (!Rush)return;
	s_scale += 0.02f;

	Helper::GetInstance()->FloatClamp(s_scale, 0.f, 3.f);

	XMFLOAT3 l_player = player->GetPosition();

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	PositionA = { l_player.x,-l_player.y,l_player.z };
	PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
	//��]�����v���C���[�̕���
		//����������
	if (ret)
	{

		RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		//�C�[�W���O�J�E���^�{�{
		t += 0.02f;

		//�����ς������t���O����
		if (t >= 1.f)ret = false;
		//Rotation���f
		if (randMove > 50) {
			m_Rotation.y = Easing::EaseOut(t, old * 60.f + 180.f, RotY * 60.f + 180.f);
		} else {
			m_Rotation.y = Easing::EaseOut(t, old * 60.f + 180.f, RotY * 60.f + 180.f - 360.f);
		}
	} else
	{

		t = 0.f;
		old = RotY;
		if (Collision::GetLength(l_player, m_Position) > 20.f) {
			randMove = rand() % 100;
			ret = true;
		}
	}
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));

	move = XMVector3TransformNormal(move, matRot);

	m_Position = {
			m_Position.x + move.m128_f32[0] * 2.f,
		m_Position.y,
		m_Position.z + move.m128_f32[2] * 2.f
	};


	Helper::GetInstance()->FloatClamp(t, 0.f, 1.f);

}


bool NormalEnemy::OnCollision()
{
	XMFLOAT3 l_player = player->GetPosition();


	//playerOBB.SetParam_Pos(l_player);
	//playerOBB.SetParam_Rot()
	//if(Collision::OBBCollision())
	return false;

}
