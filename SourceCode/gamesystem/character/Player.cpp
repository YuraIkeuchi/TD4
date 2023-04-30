#include "Player.h"
#include <any>
#include "CsvLoader.h"
#include"Helper.h"
#include"ModelManager.h"
#include "VariableCommon.h"
#include "HungerGauge.h"
#include "Collision.h"
#include "Input.h"

//コンストラクタ
Player::Player(XMFLOAT3 StartPos)
{
	m_Position = StartPos;
	//初期化ぶち込み
	Initialize();
	//移動処理用
	velocity /= 5.0f;
	//大きさ
	m_Scale = { 2.5f,2.5f,2.5f };
}
//デストラクタ
Player::~Player()
{
	m_fbxObject.reset(nullptr);
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
	/*①*/LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "speed1", sp);/*m_AddSpeedにspを代入*/

	//関数の戻り値から直接値を取る方法(こっちのほうが楽ではある　ただ行数が少し長くなる)
	/*②*/m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "speed2")));

	m_TargetInterVal = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "InterVal")));
	m_TargetRigidityTime = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player.csv", "Rigidity")));

	//飢餓ゲージはプレイヤーで管理する
	HungerGauge::GetInstance()->Initialize();

	return true;
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Player::* Player::stateTable[])() = {
	&Player::Idle,//待機
	&Player::Walk,//移動
	&Player::GhostShot,//ゴーストを捕まえる
	&Player::AttackShot,//攻撃
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


	/*--------キャラの基本動作-------*/
	/*-----------------------------*/
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
	//弾の更新
	BulletUpdate();

	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();

	//基礎パラメータ設定
	Fbx_SetParam();
	
	//どっち使えばいいか分からなかったから保留
	m_fbxObject->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);

	//Stateに入れなくていいやつ
	//攻撃のインターバル
	InterVal();
	//弾の種類選択
	SelectBullet();
	//飢餓ゲージ更新
	HungerGauge::GetInstance()->Update();
}
//描画
void Player::Draw(DirectXCommon* dxCommon)
{
	Fbx_Draw(dxCommon);
	//弾の描画(言霊)
	for (InterBullet* ghostbullet : ghostbullets) {
		if (ghostbullet != nullptr) {
			ghostbullet->Draw(dxCommon);
		}
	}

	//弾の描画(言霊)
	for (InterBullet* attackbullet : attackbullets) {
		if (attackbullet != nullptr) {
			attackbullet->Draw(dxCommon);
		}
	}
}
//ImGui
void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("BULLET")) {
		if (m_BulletType == BULLET_FORROW) {
			ImGui::Text("BULLET_FORROW");
		}
		else {
			ImGui::Text("BULLET_SEARCH");
		}
		ImGui::TreePop();
	}
	ImGui::End();

	HungerGauge::GetInstance()->ImGuiDraw();
	//弾ImGui
	for (InterBullet* bullet : ghostbullets) {
		if (bullet != nullptr) {
			bullet->ImGuiDraw();
		}
	}
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
	XMFLOAT3 pos = m_Position;
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

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
		move = XMVector3TransformNormal(move, matRot);

		//リミット制限
		Helper::GetInstance()->FloatClamp(m_Position.x, -41.0f, 50.0f);
		Helper::GetInstance()->FloatClamp(m_Position.z, -45.0f, 45.0f);
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
void Player::BulletUpdate() {
	const float l_TargetHunger = 2.0f;
	/*-----------------------------*/
	//Aが押されたら弾を撃つ(言霊)
	if (Input::GetInstance()->TriggerButton(Input::A) && m_InterVal == 0)
	{
		m_InterVal = m_TargetInterVal;
		m_RigidityTime = m_TargetRigidityTime;
		_charaState = CharaState::STATE_GHOST;
	}
	//Bが押されたら弾を撃つ(攻撃)
	if (Input::GetInstance()->TriggerButton(Input::B) && m_InterVal == 0 && HungerGauge::GetInstance()->GetNowHunger() >= l_TargetHunger)
	{
		HungerGauge::GetInstance()->SetNowHunger(HungerGauge::GetInstance()->GetNowHunger() - l_TargetHunger);
		m_InterVal = m_TargetInterVal;
		m_RigidityTime = m_TargetRigidityTime;
		_charaState = CharaState::STATE_SHOT;
	}

	//言弾の更新
	for (InterBullet* ghostbullet : ghostbullets) {
		if (ghostbullet != nullptr) {
			ghostbullet->Update();
		}
	}


	//攻撃弾の更新
	for (InterBullet* attackbullet : attackbullets) {
		if (attackbullet != nullptr) {
			attackbullet->Update();
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
}
//弾を打つ処理(ゴーストを捕まえる)
void Player::GhostShot() {
	//弾を撃つ方向を算出するために回転を求める
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	XMFLOAT2 l_Angle;
	l_Angle.x = move.m128_f32[0];
	l_Angle.y = move.m128_f32[2];

	//弾の生成
	GhostBullet* newbullet;
	newbullet = new GhostBullet();
	newbullet->Initialize();
	newbullet->SetPosition(m_Position);
	newbullet->SetBulletType(m_BulletType);
	newbullet->SetAngle(l_Angle);
	ghostbullets.push_back(newbullet);
}
//弾を打つ処理(ゴーストを捕まえる)
void Player::AttackShot() {
	//弾を撃つ方向を算出するために回転を求める
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	move = XMVector3TransformNormal(move, matRot);
	XMFLOAT2 l_Angle;
	l_Angle.x = move.m128_f32[0];
	l_Angle.y = move.m128_f32[2];

	//弾の生成
	InterBullet* newbullet;
	newbullet = new AttackBullet();
	newbullet->Initialize();
	newbullet->SetPosition(m_Position);
	newbullet->SetBulletType(m_BulletType);
	newbullet->SetAngle(l_Angle);
	attackbullets.push_back(newbullet);
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
	Helper::GetInstance()->CheckMaxINT(m_InterVal, 0, -1);
	Helper::GetInstance()->CheckMaxINT(m_RigidityTime, 0, -1);
}
//弾を選ぶ
void Player::SelectBullet() {
	if (Input::GetInstance()->TriggerButton(Input::RB)) {
		m_BulletType = BULLET_FORROW;
	}
	else if (Input::GetInstance()->TriggerButton(Input::LB)) {
		m_BulletType = BULLET_SEARCH;
	}
}
//弾との当たり判定
bool Player::BulletCollide(const XMFLOAT3& pos) {
	float l_Radius = 1.0f;//当たり範囲
	//弾の更新
	for (InterBullet* bullet : ghostbullets) {
		if (bullet != nullptr) {
			if (Collision::CircleCollision(bullet->GetPosition().x, bullet->GetPosition().z, l_Radius, pos.x, pos.z, l_Radius) && (bullet->GetAlive())) {
				bullet->SetAlive(false);
				return true;
			}
			else {
				return false;
			}
		}
	}

	return false;
}
//プレイヤーとの当たり判定
bool Player::PlayerCollide(const XMFLOAT3& pos) {
	float l_Radius = 2.0f;//当たり範囲
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, pos.x, pos.z, l_Radius)) {
		return true;
	}
	else {
		return false;
	}

	return false;
}