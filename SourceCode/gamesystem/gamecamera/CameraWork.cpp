#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include "Player.h"
#include  "imgui.h"
#include "Helper.h"
CameraWork::CameraWork(XMFLOAT3 eye, XMFLOAT3 target) {
	m_eyePos = eye;
	m_targetPos = target;

	Shake* shake_ = new Shake();
	shake.reset(shake_);

	Feed* feed_ = new Feed();
	feed.reset(feed_);

#pragma region First
	{
		if (pointsList.size() == 0) {
			pointsList.emplace_back(XMFLOAT3{ 0,160,200 });
			pointsList.emplace_back(XMFLOAT3{ 0,80,120 });
			//pointsList.emplace_back(XMFLOAT3{ 25,30,-15 });
			pointsList.emplace_back(XMFLOAT3{ 60,30,70 });
			//pointsList.emplace_back(XMFLOAT3{ 30,50,45 });
			pointsList.emplace_back(XMFLOAT3{ 30,30,50 });
			pointsList.emplace_back(XMFLOAT3{ 0,30,0});
			pointsList.emplace_back(XMFLOAT3{ -20,10,-30 });
			pointsList.emplace_back(XMFLOAT3{ -10,10,-70 });
		}
		spline = new Spline();
		spline->Init(pointsList, static_cast<int>(pointsList.size()));
	}
#pragma endregion
}
/*CharaStateのState並び順に合わせる*/
void (CameraWork::* CameraWork::stateTable[])() = {
	&CameraWork::DefaultCam,//通常
	&CameraWork::SpecialUpdate,//ロード
	&CameraWork::BossAppear,//ボス登場
	&CameraWork::SetBossDead_Cam,//ボスのやられたとき
	&CameraWork::SetBossDead_Act,//ボスのやられたとき（フェード五）
};
//XV
void CameraWork::Update(DebugCamera* camera) {
	//状態移行(charastateに合わせる)
	
	(this->*stateTable[m_CameraState])();
	camera->SetEye(m_eyePos);
	camera->SetTarget(m_targetPos);
	camera->Update();
}
//通常のカメラ
void CameraWork::DefaultCam()
{
	m_eyePos.x = Player::GetInstance()->GetPosition().x;
	m_eyePos.y = Player::GetInstance()->GetPosition().y+50.0f;

	m_eyePos.z = Player::GetInstance()->GetPosition().z - 20.0f;
	m_targetPos.x = Player::GetInstance()->GetPosition().x;
	m_targetPos.z = Player::GetInstance()->GetPosition().z;

}
//ボス登場
void CameraWork::BossAppear() {
	//シーンによってカメラの関数が違う
	if (SceneName == "FIRSTSTAGE") {
		FirstBossAppear();
	}
	else if (SceneName == "SECONDSTAGE") {
		SecondBossAppear();
	}
}
//ボス撃破
void CameraWork::SetBossDead_Cam()
{
	m_eyePos.x = boss->GetPosition().x;

	m_eyePos.z = boss->GetPosition().z -30.f;
	m_eyePos.y = 20.f;

	m_targetPos.x = boss->GetPosition().x;
	m_targetPos.z = boss->GetPosition().z;


	if (DeathTimer > 120 && !FeedF)
	{
		FeedF = true;
	}

	if (FeedF) {
		DeathTimer = 0;
		feed->FeedIn(Feed::FeedType::WHITE, 0.02f, FeedF);
		if (feed->GetFeedEnd())FeedEndF = true;
	} else
		DeathTimer++;

}

//フェード後の撃破アクション
void CameraWork::SetBossDead_Act()
{
	m_eyePos.x = Player::GetInstance()->GetPosition().x;
	m_eyePos.y = Player::GetInstance()->GetPosition().y+50;
	m_eyePos.z = Player::GetInstance()->GetPosition().z - 20.0f;
	m_targetPos.x = Player::GetInstance()->GetPosition().x;
	m_targetPos.z = Player::GetInstance()->GetPosition().z;

	DeathTimer = 0;
	FeedF = false;
}

void CameraWork::EditorCamera()
{
	m_eyePos.y = 35.f;
	m_targetPos.z = m_eyePos.z + 30.0f;
	m_targetPos.x = m_eyePos.x;
}

//ImGui
void CameraWork::ImGuiDraw() {
	ImGui::Begin("Camera");
	ImGui::Text("targetPosX:%f", m_targetPos.x);
	ImGui::Text("targetPosY:%f", m_targetPos.y);
	ImGui::Text("targetPosZ:%f", m_targetPos.z);
	ImGui::Text("eyePosX:%f", m_eyePos.x);
	ImGui::Text("eyePosY:%f", m_eyePos.y);
	ImGui::Text("eyePosZ:%f", m_eyePos.z);
	ImGui::End();
}

void CameraWork::SpecialUpdate() {

}

void CameraWork::feedDraw()
{
	if (!FeedF) return;
	feed->Draw();
}
//最初のボスのカメラ
void CameraWork::FirstBossAppear() {

	if(!Finish)
	spline->Upda(m_eyePos,180.00f);

	if (spline->GetIndex() >=pointsList.size()-1)
	{
		Finish = true;
		if (Helper::GetInstance()->FrameCheck(m_Frame,0.02f)) {
			m_Frame = 1.0f;
		}
		m_AfterEye = { Player::GetInstance()->GetPosition().x,45.0f,Player::GetInstance()->GetPosition().z - 20.0f };
		m_AfterTarget = Player::GetInstance()->GetPosition();
		m_targetPos = {
Ease(In,Cubic,m_Frame,boss->GetPosition().x,m_AfterTarget.x),
Ease(In,Cubic,m_Frame,boss->GetPosition().y,m_AfterTarget.y),
Ease(In,Cubic,m_Frame,boss->GetPosition().z,m_AfterTarget.z),
		};

		m_eyePos = {
Ease(In,Cubic,m_Frame,m_eyePos.x,m_AfterEye.x),
Ease(In,Cubic,m_Frame,m_eyePos.y,m_AfterEye.y),
Ease(In,Cubic,m_Frame,m_eyePos.z,m_AfterEye.z),
		};
	}
	else
	  m_targetPos = { boss->GetPosition() };
}
//2個目のボスのカメラ
void CameraWork::SecondBossAppear() {
	float l_AddFrame = 0.0f;
	if (m_AppearType == APPEAR_START) {
		m_CameraTimer++;
		m_AfterSpeed = m_CameraSpeed;
		m_targetPos.x = 0.0f;
		m_eyePos = {
			Player::GetInstance()->GetPosition().x,
			Player::GetInstance()->GetPosition().y,
			Player::GetInstance()->GetPosition().z + 23.0f,
		};

		if (m_CameraTimer == 10) {
			m_AfterSpeed = 30.0f;
			m_AppearType = APPEAR_SECOND;
		}

		SetCircleCamera();
	}
	//右を見る
	else if (m_AppearType == APPEAR_SECOND) {
		l_AddFrame = 0.01f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_AfterSpeed = 150.0f;
			m_AppearType = APPEAR_THIRD;
			m_Frame = 0.0f;
		}
	
		m_CameraSpeed = Ease(In, Cubic, m_Frame, m_CameraSpeed, m_AfterSpeed);

		SetCircleCamera();
	}
	//左を見る
	else if (m_AppearType == APPEAR_THIRD) {
		l_AddFrame = 0.01f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_AfterTarget = { boss->GetPosition().x,20.0f,boss->GetPosition().z };
			m_AppearType = APPEAR_FOURTH;
			m_Frame = 0.0f;
			m_CameraTimer = 0;
		}
	
		m_CameraSpeed = Ease(In, Cubic, m_Frame, m_CameraSpeed, m_AfterSpeed);

		SetCircleCamera();
	}
	//上を見る
	else if (m_AppearType == APPEAR_FOURTH) {
		l_AddFrame = 0.01f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_AppearType = APPEAR_FIVE;
			m_Frame = 0.0f;
			m_CameraTimer = {};
		}
	
		m_CameraSpeed = Ease(In, Cubic, m_Frame, m_CameraSpeed, m_AfterSpeed);
		m_CameraScale = Ease(In, Cubic, m_Frame, m_CameraScale, m_AfterScale);
		m_targetPos = { Ease(In, Cubic, m_Frame, m_targetPos.x, m_AfterTarget.x),
			Ease(In, Cubic, m_Frame, m_targetPos.y, m_AfterTarget.y),
			Ease(In, Cubic, m_Frame, m_targetPos.z, m_AfterTarget.z),
		};
	}
	//ボスの後ろにいる
	else if(m_AppearType == APPEAR_FIVE) {
		m_targetPos = boss->GetPosition();
		m_eyePos = { boss->GetPosition().x - 10.0f,boss->GetPosition().y,boss->GetPosition().z + 20.0f };
		m_CameraTimer++;

		if (m_CameraTimer == 30) {
			m_AfterEye = { boss->GetPosition().x - 5.0f, boss->GetPosition().y, boss->GetPosition().z - 40.0f };
			m_AppearType = APPEAR_SIX;
			m_Frame = {};
			m_CameraTimer = {};
		}
	}
	//プレイヤーの右後ろに来る
	else if (m_AppearType == APPEAR_SIX) {
		l_AddFrame = 0.01f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 0.0f;
			m_AppearType = APPEAR_SEVEN;
		}

		m_eyePos = {
			Ease(In,Cubic,m_Frame,m_eyePos.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,m_eyePos.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,m_eyePos.z,m_AfterEye.z),
		};
	}
	//カメラを停止させる
	else if (m_AppearType == APPEAR_SEVEN) {
		//カメラが寄るフラグになったら次のシーン移行
		if (m_Approach) {
			m_AfterEye = { boss->GetPosition().x,boss->GetPosition().y,boss->GetPosition().z - 20.0f };
			m_AfterTarget = { boss->GetPosition().x,boss->GetPosition().y,boss->GetPosition().z };
			m_Frame = {};
			m_CameraTimer = {};
			m_AppearType = APPEAR_EIGHT;
			m_Approach = false;
		}
	}
	//カメラをボスの前に
	else if (m_AppearType == APPEAR_EIGHT) {
		l_AddFrame = 0.05f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			//一定時間でカメラを戻す
			if (Helper::GetInstance()->CheckMin(m_CameraTimer, 50, 1)) {
				m_AfterEye = { Player::GetInstance()->GetPosition().x,45.0f,Player::GetInstance()->GetPosition().z - 20.0f };
				m_AfterTarget = { Player::GetInstance()->GetPosition().x,5.0f,Player::GetInstance()->GetPosition().z };
				m_Frame = {};
				m_CameraTimer = {};
				m_AppearType = APPEAR_END;
				boss->SetFinishApp(true);
			}
		}
		m_eyePos = {
		Ease(In,Cubic,m_Frame,m_eyePos.x,m_AfterEye.x),
		Ease(In,Cubic,m_Frame,m_eyePos.y,m_AfterEye.y),
		Ease(In,Cubic,m_Frame,m_eyePos.z,m_AfterEye.z),
		};

		m_targetPos = {
		Ease(In,Cubic,m_Frame,m_targetPos.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,m_targetPos.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,m_targetPos.z,m_AfterTarget.z),
		};
	}
	//バトル前のカメラに戻る
	else if (m_AppearType == APPEAR_END) {
		l_AddFrame = 0.05f;
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
		}

		m_eyePos = {
		Ease(In,Cubic,m_Frame,m_eyePos.x,m_AfterEye.x),
		Ease(In,Cubic,m_Frame,m_eyePos.y,m_AfterEye.y),
		Ease(In,Cubic,m_Frame,m_eyePos.z,m_AfterEye.z),
		};

		m_targetPos = {
		Ease(In,Cubic,m_Frame,m_targetPos.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,m_targetPos.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,m_targetPos.z,m_AfterTarget.z),
		};
	}
}

//円運動の際のカメラ位置更新
void CameraWork::SetCircleCamera() {
	//円運動の計算
	m_CameraRadius = m_CameraSpeed * m_PI / 180.0f;
	m_CameraCircleX = cosf(m_CameraRadius) * m_CameraScale;
	m_CameraCircleZ = sinf(m_CameraRadius) * m_CameraScale;
	m_targetPos.x = m_CameraCircleX;
	m_targetPos.z = m_CameraCircleZ;
}
