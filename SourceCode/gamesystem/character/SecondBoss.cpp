#include "SecondBoss.h"
#include "ModelManager.h"
#include "Helper.h"
#include <any>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include "Input.h"
//����
SecondBoss::SecondBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Tyuta);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool SecondBoss::Initialize() {
	IKETexture::LoadTexture(8, L"Resources/2d/effect/impact.png");

	impact1.reset(IKETexture::Create(ImageManager::IMPACT2, { 0,0,0 }, { 10,10,10 }, { 1,1,1,1 }));
	impact1->TextureCreate();

	for (auto i = 0; i < 2; i++)
	{
		texalpha[i] = 1.f;
		texscl[i] = { 0.f,0.f,0.f };
	}

	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 1.f,1.f,1.f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = 90.f;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "pos")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss.csv", "hp2")));
	return true;
}
//�s��
void SecondBoss::Action() {

	impact1->SetPosition({ m_Position.x,8.f,m_Position.z });
	impact1->SetIsBillboard(false);
	impact1->SetScale({ texscl[0].x,texscl[0].y,5.f });
	impact1->SetRotation({ 90,0,0 });
	impact1->SetColor({ 1,1,1,texalpha[0] });

	impact1->Update();


	//�����蔻��i�e�j
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA);
	//��퓬�J�n��
	if (!BattleStartF) {
		NoBattleMove();
	}
	//�퓬�J�n��
	else {
		//�ړ�
		Move();
		//��_���[�W
		DamAction();
	}

	//�U������
	_attack.Action();

	//OBJ�̃X�e�[�^�X�̃Z�b�g
	Obj_SetParam();
}
//�|�[�Y
void SecondBoss::Pause() {


}
void SecondBoss::EffecttexDraw(DirectXCommon* dxCommon)
{

	IKETexture::PreDraw2(dxCommon, ImageManager::IMPACT);
	impact1->Draw2(dxCommon);
	IKETexture::PostDraw();
}

void SecondBoss::Move()
{

	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	if (!Recv && !_attack.GetAttackF()) {

		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
		XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
		//��]�����v���C���[�̕���

		float RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		m_Rotation.y = RotY * 60.f + 90.f;

		if (SearchPlayer) {
			m_move = XMVector3TransformNormal(m_move, m_matRot);
			//m_Position.x = m_move.m128_f32[0] * movespeedX;
			//m_Position.z = m_move.m128_f32[2] * movespeedZ;

		}
	}
}

void SecondBoss::DamAction()
{
	if (Recv)
	{
		DamColSetF = true;
	}

	if (DamColSetF)
	{
		m_Color.y = 1.f;
		m_Color.z = 0.f;
		m_Color.x = 1.f;
		m_Color.w = 1.f;
	}

}

void SecondBoss::NoBattleMove()
{
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 90.f));

	move = XMVector3TransformNormal(move, matRot);

	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();

	constexpr float MoveSpeed = 4.f;

	constexpr float EaseAddVal = 0.04f;

	bool SearchF = Collision::GetLength(m_Position, l_player) < 20.f;

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	XMVECTOR PositionA = { l_player.x + sinf(0 * (PI / 180.0f)) * 10.0f,l_player.y, l_player.z + cosf(0 * (PI / 180.0f)) * 10.0f };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	if (!SearchF) {
		OldPos = m_Position;
		//�ړ�����
		Helper::GetInstance()->FloatClamp(m_Position.z, 20.f, 45.f);
		Helper::GetInstance()->FloatClamp(m_Position.x, -10.f, 10.f);
	}

	//�v���C���[��������
	if (SearchF) {
		//�ǐՂƂ��U��
		//�U���t���O���Ă�
		_attack.SetAttackF(true);
	}
}

void SecondBoss::Attack::Action()
{
	if (!AttackFlag)return;
	//�U�������Ƃ�


	/*�I������*/
	//AttackFlag=false;
}

//ImGui
void SecondBoss::ImGui_Origin() {
}
