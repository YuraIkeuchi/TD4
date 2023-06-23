#include "LoadStageObj.h"
#include "CsvLoader.h"
#include "HungerGauge.h"
#include "Collision.h"
#include "Helper.h"
#include "Player.h"
#include "VariableCommon.h"
#include <Easing.h>
EnemyManager* LoadStageObj::m_EnemyManager = nullptr;
//�S�[�X�g�̃��[�h
void LoadStageObj::GhostLoad() {
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/ghost/ghost.csv", "Quantity")));

	ghosts.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);

}
//�H�ו����[�h
void LoadStageObj::FoodLoad(const std::string& sceneName) {
	size_t Size;
	if (sceneName == "FIRSTSTAGE") {
		Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Stage1")));
	} else if (sceneName == "SECONDSTAGE") {
		Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Stage2")));
	} else if (sceneName == "TUTORIAL") {
		Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Stage1")));
	} else {
		assert(0);
	}

	foods.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);
}
//���ׂă��[�h
void LoadStageObj::AllLoad(const std::string& sceneName) {
	m_SceneName = sceneName;
	//�S�[�X�g�֌W
	GhostLoad();
	//������
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i] = new Ghost();
		ghosts[i]->Initialize();
	}
	//�H�ו��֌W
	FoodLoad(sceneName);

	//������
	for (auto i = 0; i < foods.size(); i++) {
		foods[i] = new Food();
		foods[i]->Initialize();
	}
}
//���C�g�Z�b�g(�H�ו��Ɏg��)
void LoadStageObj::LightSet(LightGroup* light) {
	lightgroup = light;
	for (auto i = 0; i < foods.size(); i++) {
		lightgroup->SetCircleShadowActive(i + 2, true);
	}
}
//������
void LoadStageObj::Initialize() {
	//Load();
}
void LoadStageObj::TutorialUpdate() {

	CommonUpdate();
}
//�X�V(�X�e�[�W1)
void LoadStageObj::FirstUpdate() {
	//�X�V
	CommonUpdate();
}

//�X�V
void LoadStageObj::SecondUpdate() {
	//�X�V
	CommonUpdate();
	//����������L�̏���
	//�H���̍폜(���̃X�e�[�W�̂�)
	for (int i = 0; i < foods.size(); i++) {
		if (foods[i] == nullptr) {
			continue;
		}

		if (!foods[i]->GetAlive()) {
			foods.erase(cbegin(foods) + i);
		}
	}

	//�H������
	if (m_EnemyManager->GetEnemyCheck() && (foods.size() < 5)) {
		Food* newFood;
		newFood = new Food();
		newFood->Initialize();
		newFood->SetPosition({ m_EnemyManager->GetEnemyPosition().x,0.0f,m_EnemyManager->GetEnemyPosition().z });
		//newFood->SetLimit(true);
		foods.push_back(newFood);
		m_EnemyManager->FinishCheck();

		for (int i = 0; i < foods.size(); i++) {
			foods[i]->SetLightSet(true);
		}
	}
}
void LoadStageObj::ThirdUpdate() {
	//�X�V
	CommonUpdate();
	ThirdBossAction();
}
void LoadStageObj::FourthUpdate() {
	//�X�V
	CommonUpdate();
	SubHunger();
}
//�`��
void LoadStageObj::Draw(DirectXCommon* dxCommon) {
	//�S�[�X�g
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i]->Draw(dxCommon);
	}
	//
	//�H�ו�
	for (auto i = 0; i < foods.size(); i++) {
		foods[i]->Draw(dxCommon);
	}

	//�n�[�g
	for (auto i = 0; i < hearts.size(); i++) {
		hearts[i]->Draw(dxCommon);
	}
	//
}
//ImGui
void LoadStageObj::ImGuiDraw() {
	//�S�[�X�g
	//ghosts[0]->ImGuiDraw();
	/*ImGui::Begin("Heart");
	ImGui::Text("m_Division:%f", m_Division);
	ImGui::End();*/
	//m_EnemyManager->ImGuiDraw();
}
//�����蔻��(�S�[�X�g)
void LoadStageObj::Collide() {
	for (auto i = 0; i < ghosts.size(); ++i) {
		for (auto j = 0; j < ghosts.size(); ++j) {
			XMFLOAT3 ghostpos = ghosts[i]->GetPosition();
			XMFLOAT3 ghostpos2 = ghosts[j]->GetPosition();
			if ((i == j)) { continue; }
			if ((!ghosts[i]->GetAlive()) || (!ghosts[j]->GetAlive())) { continue; }
			if ((!ghosts[i]->GetFollow()) || (!ghosts[j]->GetFollow())) { continue; }
			if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
				ghosts[i]->GhostCollision(ghostpos2);
				ghosts[j]->GhostCollision(ghostpos);
			}
		}
	}
}
//�H���̌���
void LoadStageObj::SearchFood() {
	for (auto i = 0; i < ghosts.size(); i++) {
		XMFLOAT3 l_ghostpos = ghosts[i]->GetPosition();
		for (auto j = 0; j < foods.size(); j++) {
			if (!ghosts[i]->GetAlive()) { continue; }
			if (!ghosts[i]->GetCatch()) { continue; }
			if (ghosts[i]->GetFollow()) { continue; }
			XMFLOAT3 l_foodpos = foods[j]->GetPosition();
			float l_dir = Helper::GetInstance()->ChechLength(l_ghostpos, l_foodpos);
			if ((!ghosts[i]->GetSearch()) && (foods[j]->GetAlive()) && (!foods[j]->GetLockOn()) && (!foods[j]->GetIsCarried())) {
				if (l_dir < ghosts[i]->GetLimit()) {
					ghosts[i]->StartSearch(l_foodpos);
					foods[j]->SetLockOn(true);
				} else {
					ghosts[i]->SetLimit(ghosts[i]->GetLimit() + 3.0f);
				}
			}
		}
	}
}
//�H���ƃS�[�X�g�̓����蔻��
void LoadStageObj::CollideFood() {
	float l_Radius = 1.5f;
	for (auto i = 0; i < ghosts.size(); i++) {
		XMFLOAT3 l_ghostpos = ghosts[i]->GetPosition();
		for (auto j = 0; j < foods.size(); j++) {
			XMFLOAT3 l_foodpos = foods[j]->GetPosition();
			if (ghosts[i]->GetFollow()) { continue; }
			float l_dir = Helper::GetInstance()->ChechLength(l_ghostpos, l_foodpos);
			if ((ghosts[i]->GetSearch()) && (l_dir < l_Radius)) {
				ghosts[i]->EndSearch();
				foods[j]->CarryStart(ghosts[i]);
			}
		}
	}
}
//�S�[�X�g��������
void LoadStageObj::VanishGhost() {
	int l_TargetCatchCount = HungerGauge::GetInstance()->GetCatchCount() - 1;
	float l_Value = HungerGauge::m_Hungervalue;
	//���Z������
	m_Division = HungerGauge::GetInstance()->GetNowHunger() / 5.0f;
	for (auto i = 0; i < ghosts.size(); ++i) {
		if (ghosts[i]->GetVanish()) { continue; }
		if (!ghosts[i]->GetAlive()) { continue; }
		if (!ghosts[i]->GetCatch()) { continue; }
		if (!ghosts[i]->GetFollow()) { continue; }
		//����̒l�����������
		if (m_Division <= l_TargetCatchCount) {
			m_Vanish = true;
		}

		//for��������
		if (m_Vanish) {
			ghosts[i]->SetVanish(true);
			HungerGauge::GetInstance()->SetCatchCount(HungerGauge::GetInstance()->GetCatchCount() - 1);
			HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() - l_Value);
			m_Vanish = false;
			break;
		}
	}
}
//���ʂ̍X�V
void LoadStageObj::CommonUpdate() {
	//�S�[�X�g
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i]->Update();
	}

	//
	//�H�ו�
	for (auto i = 0; i < foods.size(); i++) {
		foods[i]->Update();
	}

	//�n�[�g
	for (auto i = 0; i < hearts.size(); i++) {
		hearts[i]->Update();
	}

	//�H���̍폜(���̃X�e�[�W�̂�)
	for (int i = 0; i < hearts.size(); i++) {
		if (hearts[i] == nullptr) {
			continue;
		}

		if (!hearts[i]->GetAlive()) {
			hearts.erase(cbegin(hearts) + i);
		}
	}
	//
	//�����蔻��
	Collide();
	//�H�ו��̓����蔻��
	CollideFood();
	//�H�ו��̌���
	SearchFood();
	//�S�[�X�g��������
	VanishGhost();
	//�n�[�g�̐���
	BirthHeart();
	//���C�g����
	LightReturn();
}
//�n�[�g�̐���
void LoadStageObj::BirthHeart() {
	if (!m_EnemyManager->GetBoss()) { return; }
	if (m_EnemyManager->GetBirthHeart()) {
		Heart* newHeart;
		newHeart = new Heart();
		newHeart->Initialize();
		newHeart->SetPosition({ m_EnemyManager->GetEnemyPosition().x,0.0f,m_EnemyManager->GetEnemyPosition().z });
		hearts.push_back(newHeart);
		m_EnemyManager->FinishHeart();
		/*for (int i = 0; i < hearts.size(); i++) {
			if (m_SceneName == "FIRSTSTAGE") {
				lightgroup->SetCircleShadowActive(((int)hearts.size() + 1) + 12, true);
			}
			else if (m_SceneName == "SECONDSTAGE") {
				lightgroup->SetCircleShadowActive(i + 2, true);
			}
		}*/
	}
}

void LoadStageObj::LightReturn() {
}

void LoadStageObj::ThirdBossAction() {
	LockVerseGhost();
	LockAllGhost();
	NonVerseGhost();
	CheckReferGhost();
	ChangeGhost2Enemy();
	ChangeGhost2Hyper();
	SubHunger();
}

void LoadStageObj::LockVerseGhost() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	if (!boss->GetSearch()) { return; }
	if (boss->GetStrong()) {
		kStopGhorstMax = 5;
	} else {
		kStopGhorstMax = 3;
	}
	int  nowStopGhorst = 0;
	int overTime = 0;
	while (nowStopGhorst < kStopGhorstMax) {
		for (auto i = 0; i < ghosts.size(); i++) {
			if (ghosts[i]->GetIsRefer()) { continue; }
			//�L�����X�e�[�g�ς���ۂɋC�����Ă�������
			if (ghosts[i]->GetStateInst() >= 3) { continue; }
			XMFLOAT3 difPos = ghosts[i]->GetPosition();
			float dif = Helper::GetInstance()->ChechLength(difPos, boss->GetPosition());
			if (boss->GetLimit() > dif) {
				stopGhosts[nowStopGhorst] = ghosts[i];
				ghosts[i]->SetIsRefer(true);
				nowStopGhorst++;
				if (nowStopGhorst >= kStopGhorstMax) {
					break;
				}
			}
		}
		boss->SetLimit(boss->GetLimit() + 5.0f);
	}
	boss->SetSearch(false);
}

void LoadStageObj::LockAllGhost() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	if (!boss->GetHyperSearch()) { return; }
	int  nowStopGhorst = 0;
	for (auto i = 0; i < ghosts.size(); i++) {
		if (ghosts[i]->GetIsRefer()) { continue; }
		//�L�����X�e�[�g�ς���ۂɋC�����Ă�������
		if (ghosts[i]->GetStateInst() >= 3) { continue; }
		stopGhosts[nowStopGhorst] = ghosts[i];
		ghosts[i]->SetIsRefer(true);
		nowStopGhorst++;
	}
	boss->SetHyperSearch(false);
}

void LoadStageObj::NonVerseGhost() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	if (boss->GetInstruction() != InterBoss::ThirdBossInst::StopGhost) { return; }
	for (int i = 0; i < kStopGhorstMax;i++) {
		if (!stopGhosts[i]) { continue; }
		stopGhosts[i]->SetColor({1,0,1,1});
		stopGhosts[i]->SetIsPostionCheck(true);
	}
	boss->SetInstruction(InterBoss::ThirdBossInst::FinishMove);
}

bool LoadStageObj::CheckReferGhost() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	int checkNum = 0;
	for (Ghost*& ghost : stopGhosts) {
		if (!ghost) { continue; }
		if (!ghost->GetIsRefer()) { continue; }
		checkNum++;
	}
	if (checkNum == 0) {
		boss->SetIsReferCheck(true);
		return false;
	} else {
		boss->SetIsReferCheck(false);
		return true;
	}
}

void LoadStageObj::ChangeGhost2Enemy() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	if (boss->GetInstruction() != InterBoss::ThirdBossInst::ChangeGhost) { return; }
	int m_GhostPos = 0;
	for (int i = 0; i < kStopGhorstMax; i++) {
		if (!stopGhosts[i]) { continue; }
		stopGhosts[i]->SetColor({1,0,1,1});
		stopGhosts[i]->SetIsVerse(false, 80);
		stopGhosts[i]->SetVanish(true);
		boss->SetJackPos(m_GhostPos, stopGhosts[i]->GetPosition());
		m_GhostPos++;
	}
	boss->SetInstruction(InterBoss::ThirdBossInst::SpawnEnemy);
}

void LoadStageObj::ChangeGhost2Hyper() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	if (boss->GetInstruction() != InterBoss::ThirdBossInst::AllSummon) { return; }
	for (Ghost*& ghost : stopGhosts) {
		if (!ghost) { continue; }
		ghost->SetColor({ 1,1,0,1 });
		ghost->SetIsAllPostionCheck(true);
	}
	boss->SetInstruction(InterBoss::ThirdBossInst::FinishMove);
}

//�Q��Q�[�W���S�[�X�g�O�̕����炷
void LoadStageObj::SubHunger() {
	const float l_AddFrame = 0.1f;
	if (m_EnemyManager->GetEnemyCheck()) {
		m_LimitHunger = HungerGauge::GetInstance()->GetNowHunger() - 15.0f;
		m_SubHunger = true;
		m_EnemyManager->FinishCheck();
	}

	if (m_SubHunger) {
		if (m_Frame < m_FrameMax) {
			m_Frame += l_AddFrame;
		} else {
			m_Frame = {};
			m_LimitHunger = {};
			m_SubHunger = false;
		}
		HungerGauge::GetInstance()->SetNowHunger(Ease(In, Cubic, m_Frame, HungerGauge::GetInstance()->GetNowHunger(), m_LimitHunger));
	}
}