#include "Player.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "VariableCommon.h"
#include "HungerGauge.h"
#include "Collision.h"
#include "Input.h"
#include "Easing.h"
Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}
//初期化
bool Player::Initialize()
{
	//モデル初期化と読み込み
	m_fbxObject.reset(new IKEFBXObject3d());
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::PLAYER));
	m_fbxObject->LoadAnimation();

	/*CSV読み込み(CSVファイル名,読み込むパラメータの名前,受け取る値)　今は単一の方のみ対応(int float double charとか)*/

	//spから間接的にアクセスする方法 (Update()内で専用の変数に代入する必要あり)
	/*①*/LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "speed1", sp);/*m_AddSpeedにspを代入*/

	//関数の戻り値から直接値を取る方法(こっちのほうが楽ではある　ただ行数が少し長くなる)
	/*②*/m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "speed2")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "HP")));
	m_MaxHP = m_HP;
	m_TargetInterVal = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "InterVal")));
	m_TargetRigidityTime = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "Rigidity")));

	//飢餓ゲージはプレイヤーで管理する
	HungerGauge::GetInstance()->Initialize();

	viewbullet.reset(new ViewBullet());
	viewbullet->Initialize();

	playerattach.reset(new PlayerAttach());
	playerattach->Initialize();
	return true;
}
//ステータスの初期化
void Player::InitState(const XMFLOAT3& pos) {
	m_Position = pos;
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_InterVal = 0;
	m_DamageInterVal = 0;

	m_BulletType = BULLET_FORROW;

	m_BoundPower = { 0.0f,0.0f };
	//初期化ぶち込み
	Initialize();
	//移動処理用
	velocity /= 5.0f;
	//大きさ
	m_Scale = { 2.5f,2.5f,2.5f };
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Player::* Player::stateTable[])() = {
	&Player::Idle,//待機
	&Player::Walk,//移動
};
//更新処理
void Player::Update()
{
	//any_castはdouble型なのでそれをstatic_castでfloatに
	//doubleがatof()関数の戻り値なので変更できない<any_cast<float>で処理通らなかった>
	//つまるところstd::any_cast<double>(〇〇)は固定(static_castで変換)
	/*①*/m_AddSpeed= static_cast<float>(std::any_cast<double>(sp));

	Input* input = Input::GetInstance();
	/*FBXのカウンタdoubleにしたほうが調整ききやすそう*/

	OldPos = m_Position;
	/*--------キャラの基本動作-------*/
	/*-----------------------------*/
	if (!isStop) {
		//スティックが押されてる間は移動する
		if (input->TiltPushStick(Input::L_UP, 0.0f) ||
			input->TiltPushStick(Input::L_DOWN, 0.0f) ||
			input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
			input->TiltPushStick(Input::L_LEFT, 0.0f))
		{
			_charaState = CharaState::STATE_RUN;
		}
		//何もアクションがなかったらアイドル状態
		else
		{
			_charaState = CharaState::STATE_IDLE;
		}
	}
		if (isStop) {
			_charaState = CharaState::STATE_IDLE;
		}
	//弾の管理
	Bullet_Management();

	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();

	//Stateに入れなくていいやつ
	//攻撃のインターバル
	InterVal();
	//飢餓ゲージ更新
	HungerGauge::GetInstance()->Update();

	Helper::GetInstance()->CheckMax(m_DamageInterVal, 0, -1);

	//反発
	ReBound();

	//適当にダメージ食らってるときは赤色
	if (m_DamageInterVal == 0) {
		m_Color = { 1.0f,1.0f,1.0f,1.0f };
	}
	else {
		m_Color = { 1.0f,0.0f,0.0f,1.0f };
	}

	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
	Helper::GetInstance()->Clamp(m_HP, 0.0f, 5.0f);

	//基礎パラメータ設定
	Fbx_SetParam();

	//どっち使えばいいか分からなかったから保留
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);

	//エフェクト
	for (InterEffect* effect : effects) {
		if (effect != nullptr) {
			effect->Update();
		}
	}

	//エフェクトの削除
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i] == nullptr) {
			continue;
		}

		if (!effects[i]->GetAlive()) {
			effects.erase(cbegin(effects) += i);
		}
	}
}
//描画
void Player::Draw(DirectXCommon* dxCommon)
{
	//キャラクター
	Fbx_Draw(dxCommon);
	playerattach->Draw(dxCommon);
	//弾の描画
	BulletDraw(ghostbullets, dxCommon);
	BulletDraw(attackbullets, dxCommon);
}
//弾の描画
void Player::BulletDraw(std::vector<InterBullet*> bullets, DirectXCommon* dxCommon) {
	for (InterBullet* bullet : bullets) {
		if (bullet != nullptr) {
			bullet->Draw(dxCommon);
		}
	}

	//viewbullet->Draw(dxCommon);
}
//ImGui
void Player::ImGuiDraw() {
	for (int i = 0; i < attackbullets.size(); i++) {
		attackbullets[i]->ImGuiDraw();
	}

	HungerGauge::GetInstance()->ImGuiDraw();
	ImGui::Begin("Player");
	ImGui::Text("Num:%d", m_BulletNum);
	ImGui::Text("InterVal:%d", m_InterVal);
	ImGui::Text("Can:%d", m_canShot);
	if (m_BulletType == BULLET_FORROW) {
		ImGui::Text("FOLLOW");
	}
	else if (m_BulletType == BULLET_SEARCH) {
		ImGui::Text("SEARCH");
	}
	else {
		ImGui::Text("ATTACK");
	}
	ImGui::End();
}
//FBXのアニメーション管理(アニメーションの名前,ループするか,カウンタ速度)
void Player::AnimationControl(AnimeName name, const bool& loop, int speed)
{
	//アニメーションを引数に合わせる
	if (_animeName != name)
	{
		m_fbxObject->PlayAnimation(static_cast<int>(name));
	}

	//各種パラメータ反映
	_animeName = name;
	m_LoopFlag =loop;
	m_AnimationSpeed = speed;
	
}
//歩き(コントローラー)
void Player::Walk()
{
	XMFLOAT3 rot = m_Rotation;

	float AddSpeed=2.f;
	Input* input = Input::GetInstance();

	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float STICK_MAX = 32768.0f;
	
		//上入力
		if (input->TiltPushStick(Input::L_UP, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, velocity, 0 }, angle);

		//下入力
		if (input->TiltPushStick(Input::L_DOWN, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, -velocity, 0 }, angle);

		//右入力
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ velocity, 0, 0, 0 }, angle);

		//左入力
		if (input->TiltPushStick(Input::L_LEFT, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -velocity, 0, 0, 0 }, angle);
		

		const float rnd_vel = 0.1f;

		XMFLOAT3 vel{};

		vel.x = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		rot.y = angle + atan2f(StickX, StickY) * (PI_180 / PI);

		//プレイヤーの回転角を取る
		m_Rotation = { rot.x, rot.y, rot.z };

		if (m_Rotation.y <= 0.0f) {
			m_Rotation.y = m_Rotation.y + 360.0f;
		}

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
		move = XMVector3TransformNormal(move, matRot);
		//向いた方向に進む
		if (m_RigidityTime == m_ResetNumber) {
			m_Position.x += move.m128_f32[0] * m_AddSpeed;
			m_Position.z += move.m128_f32[2] * m_AddSpeed;
		}
		AnimationControl(AnimeName::WALK, true, 1);
}
//VECTOR
XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
	return pos;
}
//弾の更新
void Player::Bullet_Management() {
	const int l_TargetCount = 1;
	const int l_Limit = 20;//ショットのチャージ時間
	/*-----------------------------*/
	//RB||LBが押されたら弾を切り替える
	if (((Input::GetInstance()->TriggerButton(Input::RB)) || (Input::GetInstance()->TriggerButton(Input::LB))) && (m_canShot) && (m_ShotTimer == 0))
	{
		isShotNow = true;
		if (Input::GetInstance()->TriggerButton(Input::RB)) {
			if (m_BulletType != BULLET_ATTACK) {
				m_BulletType++;
			}
			else {
				m_BulletType = BULLET_FORROW;
			}
		}
		else if (Input::GetInstance()->TriggerButton(Input::LB)) {
			if (m_BulletType != BULLET_FORROW) {
				m_BulletType--;
			}
			else {
				m_BulletType = BULLET_ATTACK;
			}
		}
	}

	//攻撃
	//Bが押されたら弾のチャージ
	if (m_BulletType == BULLET_ATTACK) {
		if (Input::GetInstance()->PushButton(Input::B) && (m_InterVal == 0) && (HungerGauge::GetInstance()->GetCatchCount() >= l_TargetCount)
			&& (m_canShot)) {
			isShotNow = true;
			m_ShotTimer++;
			viewbullet->SetAlive(true);
		}

		//チャージ時間が一定を超えたら飢餓ゲージの減る速度が上がる
		if (m_ShotTimer > l_Limit) {
			viewbullet->SetCharge(true);
			HungerGauge::GetInstance()->SetSubVelocity(2.0f);
			//チャージ中に飢餓ゲージが切れた場合弾が自動で放たれる
			if (HungerGauge::GetInstance()->GetNowHunger() == 0.0f) {
				BirthShot("Attack", true);
				playerattach->SetAlive(true);
				HungerGauge::GetInstance()->SetSubVelocity(1.0f);
				ResetBullet();
			}
		}

		if (!Input::GetInstance()->PushButton(Input::B) && m_ShotTimer != 0) {
			if (m_ShotTimer < l_Limit) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Shot.wav", VolumManager::GetInstance()->GetSEVolum());
				BirthShot("Attack", false);
				playerattach->SetAlive(true);
			}
			else {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Shot_Charge.wav", VolumManager::GetInstance()->GetSEVolum());
				BirthShot("Attack", true);
				playerattach->SetAlive(true);
				HungerGauge::GetInstance()->SetSubVelocity(1.0f);
			}
			ResetBullet();
		}
	}
	else {			//言魂
		if (Input::GetInstance()->TriggerButton(Input::B) && (m_InterVal == 0) && (m_canShot)) {
			if (m_BulletType == BULLET_FORROW) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Follow.wav", VolumManager::GetInstance()->GetSEVolum());
			}
			else if (m_BulletType == BULLET_SEARCH) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Seach.wav", VolumManager::GetInstance()->GetSEVolum());
			}

			isShotNow = true;
			ResetBullet();
			playerattach->SetAlive(true);
			BirthShot("Ghost", false);
		}
	}

	
	//弾の削除(言霊)
	for (int i = 0; i < ghostbullets.size(); i++) {
		if (ghostbullets[i] == nullptr) {
			continue;
		}

		if (!ghostbullets[i]->GetAlive()) {
			ghostbullets.erase(cbegin(ghostbullets) + i);
		}
	}

	//弾の削除(言霊)
	for (int i = 0; i < attackbullets.size(); i++) {
		if (attackbullets[i] == nullptr) {
			continue;
		}

		if (!attackbullets[i]->GetAlive()) {
			attackbullets.erase(cbegin(attackbullets) + i);
		}
	}

	if (attackbullets.size() <= 0 && ghostbullets.size() <= 0) {
		isShotNow = false;
	}

	//弾の数指定
	if (HungerGauge::GetInstance()->GetCatchCount() < 3) {
		m_BulletNum = 1;
	}
	else if (HungerGauge::GetInstance()->GetCatchCount() >= 3 && HungerGauge::GetInstance()->GetCatchCount() < 6) {
		m_BulletNum = 2;
	}
	else {
		m_BulletNum = 3;
	}

	//弾の更新
	BulletUpdate(ghostbullets);
	BulletUpdate(attackbullets);
	//弾を撃つ方向を算出するために回転を求める
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	XMFLOAT2 l_Angle;
	l_Angle.x = move.m128_f32[0];
	l_Angle.y = move.m128_f32[2];


	//可視化の弾関係
	viewbullet->Update();
	viewbullet->SetAngle(l_Angle);
	viewbullet->SetPosition({ m_Position.x,0.0f,m_Position.z });

	SutoponUpdate();
}
void Player::BulletUpdate(std::vector<InterBullet*> bullets) {
	//弾の更新
	for (InterBullet* bullet : bullets) {
		if (bullet != nullptr) {
			bullet->Update();
		}
	}
	
}
//弾の生成
void Player::BirthShot(const std::string& bulletName, bool Super) {

	const int l_BulletNum = m_BulletNum;
	XMVECTOR move2 = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot2 = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	move2 = XMVector3TransformNormal(move2, matRot2);
	XMFLOAT2 l_Angle2;
	l_Angle2.x = move2.m128_f32[0];
	l_Angle2.y = move2.m128_f32[2];
	//攻撃の弾
	if (bulletName == "Attack") {
		for (int i = 0; i < m_BulletNum; i++) {
			XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };

			XMMATRIX matRot;
			//弾の状況によって数と角度を決めている
			if (l_BulletNum == 1) {
				matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
			}
			else if(l_BulletNum == 2) {
				if (i == 0) {
					matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 5.0f));
				}
				else {
					matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y - 5.0f));
				}
			}
			else {
				if (i == 0) {
					matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y - 10.0f));
				}
				else if (i == 1) {
					matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y + 10.0f));
				}
				else {
					matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
				}
			}
			move = XMVector3TransformNormal(move, matRot);
			XMFLOAT2 l_Angle;
			l_Angle.x = move.m128_f32[0];
			l_Angle.y = move.m128_f32[2];

			InterBullet* newbullet;
			newbullet = new AttackBullet();
			newbullet->Initialize();
			newbullet->SetPosition(viewbullet->GetPosition());
			//チャージショットかどうか
			if (Super) {
				newbullet->SetScale(viewbullet->GetScale());
			}
			else {
				newbullet->SetScale({ 1.5f,1.5f,1.5f });
			}
			newbullet->SetAngle(l_Angle);
			attackbullets.push_back(newbullet);
		}
	}
	//言霊
	else if(bulletName == "Ghost") {
		//弾の生成
		GhostBullet* newbullet;
		newbullet = new GhostBullet();
		newbullet->Initialize();
		newbullet->SetPosition(viewbullet->GetPosition());
		newbullet->SetRotation({m_Rotation.x,m_Rotation.y + 90.0f,m_Rotation.z});
		newbullet->SetBulletType(m_BulletType);
		newbullet->SetAngle(l_Angle2);
		ghostbullets.push_back(newbullet);
	}
	else {
		assert(0);
	}
}
//待機モーション
void Player::Idle()
{
	//条件少しおかしいので後で修正
	if (_animeName == AnimeName::IDLE)return;
	AnimationControl(AnimeName::IDLE, true, 1);
}
//インターバル
void Player::InterVal() {
	Helper::GetInstance()->CheckMax(m_InterVal, 0, -1);
	Helper::GetInstance()->CheckMax(m_RigidityTime, 0, -1);
}
//プレイヤーとの当たり判定
bool Player::PlayerCollide(const XMFLOAT3& pos) {
	float l_Radius = 3.1f;//当たり範囲
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, pos.x, pos.z, l_Radius)) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
//弾のリセット
void Player::ResetBullet() {
	m_InterVal = m_TargetInterVal;
	m_RigidityTime = m_TargetRigidityTime;
	viewbullet->SetAlive(false);
	viewbullet->SetCharge(false);
	m_ShotTimer = {};
}
void Player::isOldPos()
{
	m_Position = OldPos;
}
//プレイヤーのダメージ判定
void Player::RecvDamage(float Damage) {
	Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Damage.wav", VolumManager::GetInstance()->GetSEVolum());
	m_HP -= Damage;
	m_DamageInterVal = 50;
	m_Damage = true;
	BirthParticle();
}
//弾の削除
void Player::BulletDelete() {
	ghostbullets.clear();
	attackbullets.clear();
}
//プレイヤーが敵にあたった瞬間の判定
void Player::PlayerHit(const XMFLOAT3& pos) {
	XMFLOAT2 l_Distance;
	l_Distance.x = m_Position.x - pos.x+0.1f;
	l_Distance.y = m_Position.z - pos.z;
	m_BoundPower.x = (sin(atan2f(l_Distance.x, l_Distance.y)) * 3.0f);
	m_BoundPower.y = (cos(atan2f(l_Distance.x, l_Distance.y)) * 3.0f);
}
//弾かれる処理
void Player::ReBound() {
	if (m_Damage) {
		m_BoundPower = {
	Ease(In,Cubic,0.5f,m_BoundPower.x,0.0f),
	Ease(In,Cubic,0.5f,m_BoundPower.y,0.0f),
		};

		m_Position.x += m_BoundPower.x;
		m_Position.z += m_BoundPower.y;

		if (m_BoundPower.x == 0.0f) {
			m_Damage = false;
		}
	}
}
//銃の処理
void Player::SutoponUpdate(){
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	XMFLOAT2 l_Angle;
	l_Angle.x = move.m128_f32[0];
	l_Angle.y = move.m128_f32[2];
	playerattach->SetAngle(l_Angle);
	playerattach->Update();
	playerattach->SetPosition({ m_Position.x,playerattach->GetPosition().y,m_Position.z });
	playerattach->SetRotation({ m_Rotation.x,m_Rotation.y + 90.0f,m_Rotation.z });
}
//ダメージパーティクル
void Player::BirthParticle() {
	InterEffect* neweffect;
	neweffect = new BreakEffect();
	neweffect->Initialize();
	neweffect->SetPosition(m_Position);
	neweffect->SetDiviSpeed(1.0f);
	neweffect->SetLife(50);
	effects.push_back(neweffect);
}
//ボス登場シーンの更新
void Player::AppearUpdate() {
	//基礎パラメータ設定
	Fbx_SetParam();

	//どっち使えばいいか分からなかったから保留
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
}
//ボス撃破シーンの更新
void Player::DeathUpdate() {
	//基礎パラメータ設定
	Fbx_SetParam();

	//どっち使えばいいか分からなかったから保留
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
}