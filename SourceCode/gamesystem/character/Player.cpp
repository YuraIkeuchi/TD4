#include "Player.h"

#include "Input.h"
#include"ModelManager.h"


/*-----------------*/
/*松本エンジン慣れる用*/
/*-----------------*/

//コンストラクタ
Player::Player()
{
	//初期化ぶち込み
	Init();
}

//デストラクタ
Player::~Player()
{
	
}

//初期化
void Player::Init()
{
	Model.reset(new IKEFBXObject3d());
	Model->Initialize();
	Model->SetModel(ModelManager::GetInstance()->GetFBXModel(ModelManager::PLAYER));
	Model->LoadAnimation();
}

//更新処理
void Player::Upda()
{
	/*FBXのカウンタdoubleにしたほうが調整ききやすそう*/

	//攻撃アクション
	if(Input::GetInstance()->TriggerButton(Input::Button_Y))
		AnimationControl(AnimeName::ATTACK, false, 1);

	//アクション後
	if (_animeName != AnimeName::IDLE)
	{
		//FBXのタイムが最終フレーム到達したらアイドル状態に
		if (Model->GetFbxTime_Current() >= Model->GetFbxTime_End())
		{
			AnimationControl(AnimeName::IDLE, true, 1);
		}
	}

	//基礎パラメータ設定
	Model->SetPosition({ 0.0f,8.0f,0.0f });
	Model->SetRotation({ 0.0f,0.0f,0.0f });
	Model->SetScale({ 2.02f,2.02f,2.02f });

	//どっち使えばいいか分からなかったから保留
	Model->Update(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
	Model->FollowUpdate(m_LoopFlag, m_AnimationSpeed, m_StopFlag);
}

//描画
void Player::Draw(DirectXCommon* dxCommon)
{
	Model->Draw(dxCommon->GetCmdList());
}

//FBXのアニメーション管理(アニメーションの名前,ループするか,カウンタ速度)
void Player::AnimationControl(AnimeName name, const bool& loop, int speed)
{
	if (_animeName != name)
	{
		Model->PlayAnimation(static_cast<int>(name));
	}

	//各種パラメータ反映
	_animeName = name;
	m_LoopFlag =loop;
	m_AnimationSpeed = speed;
	
}



