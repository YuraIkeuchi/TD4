#include "FourthBoss.h"
#include <any>
#include <random>

#include "Collision.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include "Shake.h"
#include "Player.h"

void (FourthBoss::* FourthBoss::stateTable[])() = {
	&FourthBoss::WaitUpdate,//�v�f0
	&FourthBoss::MoveUpdate, //�v�f1
	&FourthBoss::ControlUpdate,
};


//����
FourthBoss::FourthBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Tyuta);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

bool FourthBoss::Initialize() {
	m_Position = { 0.0f,0.0f,30.0f };
	m_Scale = { 1.0f,1.4f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = -90.f;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "pos")));
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/fourth/fourthboss.csv", "HeartTarget")));
	m_MaxHp = m_HP;

	ActionTimer = 1;

	m_Radius = 5.0f;
	return true;
}

void FourthBoss::SkipInitialize() {

}
//�s��
void FourthBoss::Action() {
	if (m_HP < 0.1) return;



	/*^^^^�����蔻��^^^^*/
	//�e�ƃ{�X�̓����蔻��
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//OBJ�̃X�e�[�^�X�̃Z�b�g
	Obj_SetParam();
	//���~�b�g����
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
}
//�|�[�Y
void FourthBoss::Pause() {


}

void FourthBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.1f)return;
}
//�`��
void FourthBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	EffecttexDraw(dxCommon);
}

void FourthBoss::WaitUpdate() {
}

void FourthBoss::MoveUpdate() {
}

void FourthBoss::ControlUpdate() {
}
