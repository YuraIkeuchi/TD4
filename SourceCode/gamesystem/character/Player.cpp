#include "Player.h"
#include"Helper.h"
#include"ModelManager.h"


/*-----------------*/
/*松本エンジン慣れる用*/
/*-----------------*/

//コンストラクタ
Player::Player(XMFLOAT3 StartPos)
	:Position(StartPos)//シーンまたいだ時初期座標とか設定用(流石にインスタンス一つで回さないと思うので)
{
	//初期化ぶち込み
	Init();
	//移動処理用
	velocity /= 5.0f;
	//大きさ
	Scale = { 2.f,2.f,2.f };
}

//デストラクタ
Player::~Player()
{
	Model.reset(nullptr);
}

//初期化
void Player::Init()
{
	Model.reset(new IKEFBXObject3d());
	Model->Initialize();
	Model->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::PLAYER));
	Model->LoadAnimation();
}

//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Player::* Player::stateTable[])() = {
	&Player::Idle,//待機
	&Player::Walk,//移動
	&Player::Attack,//攻撃
};

//更新処理
void Player::Upda()
{
	input = Input::GetInstance();
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
	//Yが押されたら攻撃状態(まず作るゲーム攻撃あるのかどうか分からんから普通に後で消すかも)
	else if (Input::GetInstance()->TriggerButton(Input::Y))
	{
		_charaState = CharaState::STATE_ATTACK;
	}
	//何もアクションがなかったらアイドル状態
	else
	{
		_charaState = CharaState::STATE_IDLE;
	}
	/*-----------------------------*/



	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();

	//基礎パラメータ設定
	Model->SetPosition(Position);
	Model->SetRotation(Rotation);
	Model->SetScale(Scale);

	//どっち使えばいいか分からなかったから保留
	Model->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
}

//描画
void Player::Draw(DirectXCommon* dxCommon)
{
	Model->Draw(dxCommon->GetCmdList());
}

//FBXのアニメーション管理(アニメーションの名前,ループするか,カウンタ速度)
void Player::AnimationControl(AnimeName name, const bool& loop, int speed)
{
	//アニメーションを引数に合わせる
	if (_animeName != name)
	{
		Model->PlayAnimation(static_cast<int>(name));
	}

	//各種パラメータ反映
	_animeName = name;
	m_LoopFlag =loop;
	m_AnimationSpeed = speed;
	
}

//歩き(コントローラー)
void Player::Walk()
{
	XMFLOAT3 pos = Position;
	XMFLOAT3 rot = Rotation;

	constexpr float AddSpeed=2.f;

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
		Rotation = { rot.x, rot.y, rot.z };

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));
		move = XMVector3TransformNormal(move, matRot);

		//向いた方向に進む
		Position.x += move.m128_f32[0] * AddSpeed;
		Position.z += move.m128_f32[2] * AddSpeed;

}

XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
	return pos;
}

//攻撃アクション
void Player::Attack()
{
	//アニメーションを攻撃に
	AnimationControl(AnimeName::ATTACK, false, 1);
}

//待機ション
void Player::Idle()
{
	//条件少しおかしいので後で修正
	if (_animeName != AnimeName::IDLE)
	{
		//FBXのタイムが最終フレーム到達したらアイドル状態に
		if (Model->GetFbxTime_Current() >= Model->GetFbxTime_End())
		{
			AnimationControl(AnimeName::IDLE, true, 1);
		}
	}
}

