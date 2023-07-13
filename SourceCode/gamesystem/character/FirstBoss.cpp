#include "FirstBoss.h"
#include "ImageManager.h"
#include"Easing.h"
#include "Helper.h"
#include "Player.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include <random>

void (FirstBoss::* FirstBoss::stateTable[])() = {
	&FirstBoss::InterValMove,
	&FirstBoss::Choice,
	&FirstBoss::RockOnAttack,
	&FirstBoss::RandAttack,
	&FirstBoss::Hit,
	&FirstBoss::EndMove,
};

FirstBoss::FirstBoss()
{
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::MILKCAP_NORMAL);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	confueffect.reset(new ConfuEffect());
	confueffect->Initialize();

	noteeffect.reset(new NoteEffect());
	noteeffect->Initialize();

	m_HP = 5.f;
}

bool FirstBoss::Initialize()
{
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 15.3f,15.3f,15.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = -90.f;
	m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "pos")));
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/Firstboss.csv", "HeartTarget")));
	m_Radius = 5.2f;
	m_MaxHp = m_HP;
	_charstate = CharaState::STATE_INTER;

	return true;
}

void FirstBoss::SkipInitialize()
{
	m_Position = { 0.0f,3.0f,30.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 15.3f,15.3f,15.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
}

void FirstBoss::Pause()
{
}

void FirstBoss::Action()
{
	//状態移行(charastateに合わせる)
	if (m_HP > 0.0f) {
		(this->*stateTable[_charstate])();
	}

	FaceToOrientation();

	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	//プレイヤーの当たり判定
	ColPlayer();
	//OBJのステータスのセット
	Obj_SetParam();
	for (std::unique_ptr<Fraction>& fraction : fraction_) {
		fraction->Update();
	}
	fraction_.remove_if([](std::unique_ptr<Fraction>& fraction) {
		return fraction->IsDelete();
		});
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
}

void FirstBoss::AppearAction()
{
	Obj_SetParam();
}

void FirstBoss::DeadAction()
{
	const float l_AddAngle = 5.0f;
	m_DeathTimer++;
	const int l_BaseTarget = 50;
	if (m_DeathTimer == 1) {
		m_Position = { 0.0f,30.0f,20.0f };
		m_Rotation = { 0.0f,0.0f,0.0f };
	}
	else if (m_DeathTimer >= 2 && m_DeathTimer < 300) {
		//sin波によって上下に動く
		m_Angle += l_AddAngle;
		m_Angle2 = m_Angle * (3.14f / 180.0f);
		m_Position.x = (sin(m_Angle2) * 15.0f + 15.0f);
		DeathParticle();
	}
	else {
		m_Gravity = 0.05f;
		//飛ぶような感じにするため重力を入れる
		m_AddPower -= m_Gravity;
		Helper::GetInstance()->CheckMax(m_Position.y, 6.0f, m_AddPower);
	}

	Obj_SetParam();
}

void FirstBoss::DeadAction_Throw()
{
	Obj_SetParam();
}

void FirstBoss::DeathParticle()
{
	const XMFLOAT4 s_color = { 1.0f,1.0f,1.0f,1.0f };
	const XMFLOAT4 e_color = { 0.0f,0.0f,1.0f,1.0f };
	float s_scale = 5.0f;
	float e_scale = 0.0f;
	float l_velocity = 0.5f;
	for (int i = 0; i < 3; ++i) {
		ParticleEmitter::GetInstance()->DeathEffect(50, { m_Position.x,(m_Position.y - 1.0f),m_Position.z }, s_scale, e_scale, s_color, e_color, l_velocity);
	}
}

void FirstBoss::ImGui_Origin()
{
}

void FirstBoss::EffecttexDraw(DirectXCommon* dxCommon)
{
	if (m_HP < 0.0f)return;

	confueffect->Draw(dxCommon);
	noteeffect->Draw(dxCommon);
}

void FirstBoss::Draw(DirectXCommon* dxCommon)
{
	Obj_Draw();
	for (std::unique_ptr<Fraction>& fraction : fraction_) {
		fraction->Draw(dxCommon);
	}
}
//攻撃後のインターバル
void FirstBoss::InterValMove()
{
	ActionTimer++;
	if (ActionTimer >= 180.f) {
		_charstate = STATE_CHOICE;
		ActionTimer = 0;
	}
}
//攻撃の選択
void FirstBoss::Choice()
{
	fase_ = AttackFase::AttackBefore;
	jump_ = JumpFase::JumpBefore;
	int l_RandState = 0;
	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandomMove(0, 30);
	m_StopTimer++;
	//一定時間立ったらランダムで行動選択
	if (m_StopTimer > m_ChoiceInterval) {
		m_Frame = 0.0f;
		m_StopTimer = 0;
		l_RandState = int(l_RandomMove(mt));
		//_InterValState = UpState;
		//_charaState = STATE_MOVE;
		m_FollowSpeed = 1.0f;
		m_AfterPos.y = 30.0f;
		////RandStateが30以下ならそれに応じた移動にする、
		if (l_RandState < 10) {
			_charstate = CharaState::STATE_RAND;
		}
		else if (10 <= l_RandState && l_RandState < 20) {
			_charstate = CharaState::STATE_ROCKON;
			_rockonstate = RockonState::STATE_AIM;
		}
		else if (20 <= l_RandState && l_RandState <= 30) {
			_charstate = CharaState::STATE_HIT;
		}
	}
}
//ロックオン突進
void FirstBoss::RockOnAttack()
{
	if (fase_ == AttackFase::AttackBefore) {
		if (jump_ == JumpFase::JumpBefore) {
			m_Position.y = s_pos.y;
			e_pos.y = 3;
			commandTimer += 1.0f / kMoveTimeMax;
			Helper::GetInstance()->Clamp(commandTimer, 0.0f, 1.0f);
			m_Position = { 0.f,Ease(Out, Quart, commandTimer, s_pos.y, e_pos.y),0.f };
			if (m_Position.y >= 3) {
				jump_ = JumpFase::JumpAfter;
			}
		}
		if (jump_ == JumpFase::JumpAfter) {
			m_Position.y = s_pos.y;
			e_pos.y = 0;
			commandTimer += 1.0f / kMoveTimeMax;
			Helper::GetInstance()->Clamp(commandTimer, 0.0f, 1.0f);
			m_Position = { 0.f,Ease(Out, Quart, commandTimer, s_pos.y, e_pos.y),0.f };
			if (m_Position.y <= 0) {
				jump_ = JumpFase::JumpBefore;
				jumpCount += 1;
			}
		}
		if (jumpCount >= 2) {
			fase_ = AttackFase::AttackAfter;
		}
	}


	if (fase_ != AttackFase::AttackAfter) { return; }
	RockOn();

	Attack();
}
//動いてる途中に落とす
void FirstBoss::RandAttack()
{
	m_Color = { 0.f,1.f,1.f,1.f };
	m_Rotation.x += 10.f;
	if (m_Rotation.x >= 100) {
		CreateFraction(m_Position);
		m_Rotation.x = 0;
		_charstate = CharaState::STATE_INTER;
	}
}
//中央行ってから拡散
void FirstBoss::Hit()
{
	if (fase_ == AttackFase::AttackBefore) {
		if (move_ == MoveFase::Move) {
			commandTimer += 1.0f / kMoveTimeMax;
			Helper::GetInstance()->Clamp(commandTimer, 0.0f, 1.0f);
			s_pos = m_Position;
			e_pos = { 0.f,0.f,0.f };
			m_Position = {
		Ease(Out, Quart, commandTimer, s_pos.x, e_pos.x),
		0.0f,
		Ease(Out, Quart, commandTimer, s_pos.z, e_pos.z),
			};
			if (m_Position.x == 0.f && m_Position.z == 0) {
				move_ = MoveFase::Stop;
				commandTimer = 0.f;
			}
		}
		if (move_ != MoveFase::Stop) { return; }
		if (jump_ == JumpFase::JumpBefore) {
			commandTimer += 1.0f / kJumpTimeMax;
			Helper::GetInstance()->Clamp(commandTimer, 0.0f, 1.0f);
			//m_Position.y= Ease(In, Quad, commandTimer, 1.0f, 0.0f);
			m_Position.y += 0.5f;
			m_Rotation.x = Ease(In, Quad, commandTimer, 0.0f, 360.0f);
			if (m_Rotation.x < 360) { return; }
			jump_ = JumpFase::JumpAfter;
			m_Rotation.x = 0.f;
		}
		else if (jump_ == JumpFase::JumpAfter) {
			if (m_Position.y >= 0) {
				m_Position.y -= 2.f;
			}
			else if (m_Position.y <= 0) {
				fase_ = AttackFase::AttackAfter;
			}
		}
	}
	else if (fase_ == AttackFase::AttackAfter) {
		mt19937 mt{ std::random_device{}() };
		uniform_int_distribution<int> l_distX(-50, -20);
		uniform_int_distribution<int> l_distZ(20, 50);
		XMFLOAT3 AreaOne = { float(l_distX(mt)),0.0f,float(l_distZ(mt)) };
		uniform_int_distribution<int> l_distX2(-50, -20);
		uniform_int_distribution<int> l_distZ2(-50, -20);
		XMFLOAT3 AreaTwo = { float(l_distX2(mt)),0.0f,float(l_distZ2(mt)) };
		uniform_int_distribution<int> l_distX3(20, 50);
		uniform_int_distribution<int> l_distZ3(-50, -20);
		XMFLOAT3 AreaThree = { float(l_distX3(mt)),0.0f,float(l_distZ3(mt)) };
		uniform_int_distribution<int> l_distX4(20, 50);
		uniform_int_distribution<int> l_distZ4(20, 50);
		XMFLOAT3 AreaFour = { float(l_distX4(mt)),0.0f,float(l_distZ4(mt)) };


		//乱数指定
		uniform_int_distribution<int> l_RandState(0, 40);
		if (attack_count_ < 3) {
			int ans = (l_RandState(mt)) % 4;
			if (ans == 0) {
				CreateFraction(AreaOne);
			}
			else if (ans == 1) {
				CreateFraction(AreaTwo);
			}
			else if (ans == 2) {
				CreateFraction(AreaThree);
			}
			else if (ans == 3) {
				CreateFraction(AreaFour);

			}
			attack_count_ += 1;
			m_Color = { 1.f,1.f,0.f,1.f };
			_charstate = CharaState::STATE_HIT;
			fase_ = AttackFase::AttackBefore;
			commandTimer = 0.f;
			move_ = MoveFase::Move;
		}

		if (attack_count_ >= 3) {
			m_StopTimer++;
			//一定時間立ったらランダムで行動選択
			if (m_StopTimer > m_ChoiceInterval) {
				m_StopTimer = 0;
				_charstate = CharaState::STATE_INTER;
				fase_ = AttackFase::AttackBefore;
				commandTimer = 0.f;
				move_ = MoveFase::Move;
				attack_count_ = 0;
			}
		}
	}
}
//行動の終わり(プレイヤーから逃げる)
void FirstBoss::EndMove()
{

}

void FirstBoss::RockOn()
{
	if (_rockonstate != RockonState::STATE_AIM) { return; }

	commandTimer += 1.0f / kLockOnTimeMax;
	Helper::GetInstance()->Clamp(commandTimer, 0.0f, 1.0f);
	m_Color = { 1.f,0.f,1.f,1.f };
	if (commandTimer == 1.0f) {
		commandTimer = 0.0f;
		jumpCount++;
		rot = m_Rotation.y;
		s_pos = m_Position;
		e_pos = { m_Position.x + sinf(RottoPlayer) * -(20.f * (float)jumpCount),0.f, m_Position.z + cosf(RottoPlayer) * -(20.0f * (float)jumpCount) };
		_rockonstate = RockonState::STATE_ATTACK;
	}
}

void FirstBoss::Attack()
{
	if (_rockonstate != RockonState::STATE_ATTACK) { return; }
	commandTimer += 1.0f / kJumpTimeMax;
	Helper::GetInstance()->Clamp(commandTimer, 0.0f, 1.0f);


	float hight = Ease(In, Quad, commandTimer, 1.0f, 0.0f);

	m_Position = {
	Ease(Out, Quart, commandTimer, s_pos.x, e_pos.x),
	0.0f,
	Ease(Out, Quart, commandTimer, s_pos.z, e_pos.z),
	};
	if (commandTimer == 1.0f) {
		if (jumpCount < kJumpCountMax) {
			_rockonstate = RockonState::STATE_AIM;
		}
		else {
			jumpCount = 1;
			_charstate = CharaState::STATE_INTER;
		}
		commandTimer = 0.0f;
	}
}


void FirstBoss::FaceToOrientation()
{
	XMFLOAT3 l_player = Player::GetInstance()->GetPosition();
	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = { l_player.x,l_player.y,l_player.z };
	XMVECTOR PositionB = { m_Position.x,m_Position.y,m_Position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	m_Rotation.y = RottoPlayer * 60.0f + (PI_90 + PI_180);
}

void FirstBoss::CreateFraction(const XMFLOAT3& FractionPos)
{
	unique_ptr<Fraction> fraction = make_unique<Fraction>();
	fraction->Init(FractionPos);
	fraction->Drop();
	fraction_.push_back(std::move(fraction));
}


void FirstBoss::InitAwake() {

}

void FirstBoss::EndRollAction() {
	m_EndTimer++;
	if (m_EndTimer == 1) {
		m_Position = { 0.0f,2.0f,0.0f };
		m_Rotation = { 0.0f,0.0f,0.0f };
	}
	//OBJのステータスのセット
	Obj_SetParam();
}
