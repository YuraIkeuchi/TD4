#include "Ghost.h"
#include "VariableCommon.h"
#include "Collision.h"
#include "HungerGauge.h"
#include <random>
#include "Player.h"
Ghost::Ghost() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Ghost);
	model_follow.reset(ModelManager::GetInstance()->GetModel(ModelManager::Buddy));
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool Ghost::Initialize() {
	//�����w��
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_distX(-50, 60);
	uniform_int_distribution<int> l_distZ(-55, 55);
	m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
	m_Rotation.y = -PI_90;
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Color = { 1.0f,1.0f,1.0f,0.7f };
	uniform_int_distribution<int> spawn(60, 90);
	kSpawnTimerMax = float(spawn(mt));
	_charaState = CharaState::STATE_SPAWN;
	_searchState = SearchState::SEARCH_NO;
	_followState = FollowState::Follow_NO;
	return true;
}
//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (Ghost::* Ghost::stateTable[])() = {
	&Ghost::None,//�ҋ@
	&Ghost::Spawm,
	&Ghost::Follow,//�ړ�
	&Ghost::Search,//�U��
	&Ghost::Jack
};
//�X�V
void Ghost::Update() {
	float l_AddScale = 1.5f;//OBB�p�ɏ����傫�߂̃X�P�[�������
	m_OldPos = m_Position;
	m_OBBScale = { m_Scale.x + l_AddScale,m_Scale.y + l_AddScale, m_Scale.z + l_AddScale };
	if (m_IsPostionCheck) { _charaState = CharaState::STATE_JACK; }
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_charaState])();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();
	//�H������
	BirthGhost();
	//�����蔻��(�v���C���[)
	PlayerCollision();
	//�H�ו����͂���
	CarryFood();
	Particle();
	//�����蔻��i�e�j
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_ghost();
	CollideBullet(_playerBulA);
}
//�`��
void Ghost::Draw(DirectXCommon* dxCommon) {
	//if (!m_Alive) { return; }
	Obj_Draw();
}
//ImGui�`��
void Ghost::ImGuiDraw() {
	ImGui::Begin("Ghost");
	ImGui::Text("ColorR:%f", m_Color.x);
	ImGui::Text("ColorG:%f", m_Color.y);
	ImGui::Text("ColorB:%f", m_Color.z);
	ImGui::Text("ColorW:%f", m_Color.w);
	ImGui::End();
}

//�p�[�e�B�N��
void Ghost::Particle() {
	XMFLOAT4 s_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 s_color2 = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 e_color2 = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 s_color3 = { 0.0f,1.0f,0.0f,1.0f };
	XMFLOAT4 e_color3 = { 0.0f,1.0f,0.0f,1.0f };
	float s_scale = 3.0f;
	float e_scale = 0.0f;
	if (m_Alive) {
		if (_charaState == CharaState::STATE_NONE) {
			m_Color = { 1.0f,1.0f,1.0f,0.7f };
			m_Scale = { 0.5f,0.5f,0.5f };
			//ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color, e_color);
		} else if (_charaState == CharaState::STATE_FOLLOW) {
			m_Color = { 1.0f,1.0f,1.0f,1.0f };
			m_Scale = { 0.6f,0.6f,0.6f };
			ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color2, e_color2);
		} else if (_charaState == CharaState::STATE_JACK) {
			m_Color = { 1.0f,0.0f,1.0f,1.0f };
			m_Scale = { 0.6f,0.6f,0.6f };
			//ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color2, e_color2);
		} else {
			//ParticleEmitter::GetInstance()->FireEffect(20, m_Position, s_scale, e_scale, s_color3, e_color3);
		}
	}
}
//�����蔻��(�v���C���[)
bool Ghost::PlayerCollision() {
	if (Player::GetInstance()->PlayerCollide(m_Position) && (_charaState == CharaState::STATE_FOLLOW)) {
		m_Position = m_OldPos;
		return true;
	} else {
		return false;
	}

	return true;
}
//�����蔻��(�S�[�X�g���m)
void Ghost::GhostCollision(const XMFLOAT3& pos) {
	m_Position.x += sin(atan2f((m_Position.x - pos.x), (m_Position.z - pos.z))) * 0.1f;
	m_Position.z += cos(atan2f((m_Position.x - pos.x), (m_Position.z - pos.z))) * 0.1f;
}
//�H������
void Ghost::BirthGhost() {
	if (!isVerse) { return; }
	if (m_Alive) { return; }
	m_ResPornTimer++;
	//�`��o�O�N���邩���ɍ��W�Z�b�g
	if (m_ResPornTimer == 20) {
		_charaState = CharaState::STATE_SPAWN;
		_searchState = SearchState::SEARCH_NO;
		//�����w��
		mt19937 mt{ std::random_device{}() };
		uniform_int_distribution<int> l_distX(-50, 60);
		uniform_int_distribution<int> l_distZ(-55, 55);
		m_Object->SetModel(m_Model);
		m_Position = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
		uniform_int_distribution<int> spawn(30, 45);
		kSpawnTimerMax = float(spawn(mt));
		m_Color = { 1.0f,1.0f,1.0f,0.7f };
		m_Catch = false;
		m_Search = false;
		m_Follow = false;
		m_SearchTimer = 0;
		m_SpawnTimer = 0;
	}
	//��莞�ԂŐ��������
	if (m_ResPornTimer == 100) {
		m_Alive = true;
		m_ResPornTimer = 0;
		m_Scale = { 0.5f,0.5f,0.5f };
	}
}
//�����Ȃ����
void Ghost::None() {
	//���V���
	noneTimer += 0.05f;
	float size = sinf(noneTimer) * 0.05f;
	m_Position.x += cosf(m_Rotation.y * (PI_180 / XM_PI)) * size;
	m_Position.y = sinf(noneTimer) * 1.2f;
	m_Position.z += sinf(m_Rotation.y * (PI_180 / XM_PI)) * size;

}
//���܂����
void Ghost::Spawm() {
	m_SpawnTimer += 1.0f / kSpawnTimerMax;

	m_Rotation.y = Ease(In, Quad, m_SpawnTimer, -(PI_360 + PI_90), -PI_90);

	float scale = Ease(Out, Elastic, m_SpawnTimer, 0.0f, 0.5f);
	m_Scale = { scale,scale,scale };

	Helper::GetInstance()->Clamp(m_SpawnTimer, 0.0f, 1.0f);
	if (m_SpawnTimer == 1.0f) {
		_charaState = CharaState::STATE_NONE;
	}
}
//�Ǐ]
void Ghost::Follow() {
	float l_Vel = 0.35f;//���x
	XMFLOAT3 l_playerPos = Player::GetInstance()->GetPosition();
	Helper::GetInstance()->FollowMove(m_Position, l_playerPos, l_Vel);
	m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, l_playerPos, -PI_90);
}
//�T��
void Ghost::Search() {
	const int l_LimitTimer = 300;
	const float l_Vel = 0.3f;
	const float l_Vel2 = 0.5f;
	XMFLOAT3 l_playerPos = Player::GetInstance()->GetPosition();
	//�T�[�`��Ԃ����莞�ԗ��Ƒ��ݏ���
	m_SearchTimer++;
	if (m_SearchTimer >= l_LimitTimer) {
		m_Scale = { 0.0f,0.0f,0.0f };
		m_Alive = false;
	}
	//�Ǐ]
	if (_searchState == SearchState::SEARCH_START) {
		Helper::GetInstance()->FollowMove(m_Position, m_SearchPos, l_Vel2);
		m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, m_SearchPos, -PI_90);
	} else if (_searchState == SearchState::SEARCH_END) {
		Helper::GetInstance()->FollowMove(m_Position, l_playerPos, l_Vel);
		m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, l_playerPos, -PI_90);
	}
}
//������
void Ghost::Jack() {
	if (m_IsPostionCheck) {
		//m_radius = Helper::GetInstance()->ChechLength(m_Position, {0,0,0});
		f_pos = m_Position;
		m_angle = 0;
		m_radius = 0;
		mt19937 mt{ std::random_device{}() };
		// 0�ȏ�9�ȉ��̒l�𓙊m���Ŕ���������
		std::uniform_int_distribution<> dist(0, 1);
		if (dist(mt) > 0) {
			m_dir = addDir;
		} else {
			m_dir = subDir;
		}
		m_IsPostionCheck = false;
	}
	m_angle += 0.04f;
	m_radius += 0.08f * m_dir;
	XMFLOAT3 e_pos = { f_pos.x + sinf(m_angle) * m_radius ,0,f_pos.z + cosf(m_angle) * m_radius };
	m_Rotation.y = Helper::GetInstance()->DirRotation(m_Position, e_pos,-PI_90);
	m_Position.x = e_pos.x;
	m_Position.z = e_pos.z;
	if (Player::GetInstance()->PlayerCollide(m_Position)&&
		Player::GetInstance()->GetDamageInterVal()==0) {
		Player::GetInstance()->PlayerHit(m_Position);
		Player::GetInstance()->RecvDamage(0.5f);
	}

	if (m_Position.x < -55.0f || m_Position.x>65.0f ||
		m_Position.z < -60.0f || m_Position.z>60.0f) {
		m_Alive = false;
		m_Scale = { 0.0f,0.0f,0.0f };
		m_IsRefer = false;
		isVerse = true;
		_charaState = CharaState::STATE_NONE;
	}
}
//�T���X�^�[�g
void Ghost::StartSearch(const XMFLOAT3& pos) {
	_searchState = SearchState::SEARCH_START;
	m_Search = true;
	m_SearchPos = pos;
}
//�T���I��
void Ghost::EndSearch() {
	_searchState = SearchState::SEARCH_END;
}
//�H�ו����^��
void Ghost::CarryFood() {
	float l_Radius = 1.0f;//�����蔻��
	float l_AddHunger = HungerGauge::m_Hungervalue;//���Z�����C���Q�[�W
	XMFLOAT3 l_playerPos = Player::GetInstance()->GetPosition();
	if ((_searchState == SearchState::SEARCH_END) && (m_Alive)) {
		if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_playerPos.x, l_playerPos.z, l_Radius)) {
			m_Scale = { 0.0f,0.0f,0.0f };
			m_Alive = false;
			m_Search = false;
			m_Catch = false;
			m_Limit = {};
			HungerGauge::GetInstance()->RecoveryNowHunger(HungerGauge::GetInstance()->GetNowHunger() + l_AddHunger);
			HungerGauge::GetInstance()->SetAdditional(HungerGauge::GetInstance()->GetAdditional() + l_AddHunger);
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Get_Food.wav", VolumManager::GetInstance()->GetSEVolum());
		}
	}
}
//�����蔻��
bool Ghost::CollideBullet(vector<InterBullet*>bullet) {
	float l_AddHungerMax = HungerGauge::m_Hungervalue;//���Z�����ő�Q��Q�[�W
	m_OBB1.SetParam_Pos(m_Position);
	m_OBB1.SetParam_Rot(m_Object->GetMatrot());
	m_OBB1.SetParam_Scl(m_OBBScale);
	//�e�̍X�V
	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr && _bullet->GetAlive()) {
			m_OBB2.SetParam_Pos(_bullet->GetPosition());
			m_OBB2.SetParam_Rot(_bullet->GetMatRot());
			m_OBB2.SetParam_Scl(_bullet->GetScale());

			if ((Collision::OBBCollision(m_OBB1, m_OBB2)) && (_bullet->GetAlive()) && (!m_Catch) && (m_Alive)) {
				m_Catch = true;
				if (Player::GetInstance()->GetBulletType() == BULLET_FORROW) {
					Audio::GetInstance()->PlayWave("Resources/Sound/SE/Get_Follower.wav", VolumManager::GetInstance()->GetSEVolum() / 2.5f);
					HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() + l_AddHungerMax);
					HungerGauge::GetInstance()->SetNowHunger(HungerGauge::GetInstance()->GetNowHunger() + l_AddHungerMax);
					HungerGauge::GetInstance()->SetCatchCount(HungerGauge::GetInstance()->GetCatchCount() + 1);
					_charaState = CharaState::STATE_FOLLOW;
					_followState = FollowState::Follow_START;
					m_Object->SetModel(model_follow.get());
					m_Follow = true;
				} else {
					_charaState = CharaState::STATE_SEARCH;
					Audio::GetInstance()->PlayWave("Resources/Sound/SE/Get_Searcher.wav", VolumManager::GetInstance()->GetSEVolum() / 2.5f);
				}
				return true;
			} else {
				return false;
			}
		}
	}

	return false;
}