#include "NoteEffect.h"
#include "ImageManager.h"
#include <random>
//ì«Ç›çûÇ›
NoteEffect::NoteEffect() {

	for (int i = 0; i < NoteSprite.size(); i++) {
		NoteSprite[i] = IKESprite::Create(ImageManager::FEED, { 0,0 });
		NoteSprite[i]->SetAnchorPoint({ 0.f,0.f });
		NoteSprite[i]->SetSize({ 300.0f,300.0f });
	}
}

//èâä˙âª
void NoteEffect::Initialize() {
	for (int i = 0; i < NoteSprite.size(); i++) {
		m_NotePos[i] = { 1920.0f,200.0f };
		m_NoteColor[i] = {};
	}
}

//çXêV
void NoteEffect::Update() {
	FlowNote();
	for (int i = 0; i < NoteSprite.size(); i++) {
		NoteSprite[i]->SetPosition(m_NotePos[i]);
		NoteSprite[i]->SetColor(m_NoteColor[i]);
	}
}
//ï`âÊ
void NoteEffect::Draw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	if (m_Alive) {
		for (int i = 0; i < NoteSprite.size(); i++) {
			NoteSprite[i]->Draw();
		}
	}
	IKESprite::PostDraw();
}
//ImGui
void NoteEffect::ImGuiDraw() {
	ImGui::Begin("NOTEEFFECT");
	ImGui::Text("Alive:%d", m_Alive);
	ImGui::Text("POSX:%f", m_NotePos[0].x);
	ImGui::Text("POSX4:%f", m_NotePos[4].x);
	ImGui::End();
}

void NoteEffect::FlowNote() {
	const float l_AddPos = 40.0f;
	const int l_LimitFlow = 200;
	if (m_Alive) {
		if (m_NoteState == NOTE_SET) {
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_RandomR(70, 100);
			uniform_int_distribution<int> l_RandomG(70, 100);
			uniform_int_distribution<int> l_RandomB(70, 100);
			for (int i = 0; i < NoteSprite.size(); i++) {
				if (i < 3) {
					m_NotePos[i].x = ((1920.0f) + (i * 300.0f));
					m_NotePos[i].y = 0.0f;
				}
				else if (i >= 3 && i < 6) {
					m_NotePos[i].x = ((1920.0f) + ((i - 3) * 300.0f));
					m_NotePos[i].y = 300.0f;
				}
				else {
					m_NotePos[i].x = ((1920.0f) + ((i - 6) * 300.0f));
					m_NotePos[i].y = 600.0f;
				}
				m_NoteColor[i] = { float(l_RandomR(mt)) / 100.0f,float(l_RandomG(mt)) / 100.0f,float(l_RandomR(mt)) / 100.0f,1.0f };
			}
			m_NoteState = NOTE_MOVE;
		}
		else {
			m_FlowTimer++;
			for (int i = 0; i < NoteSprite.size(); i++) {
				m_NotePos[i].x -= l_AddPos;
			}

			if (m_FlowTimer == l_LimitFlow) {
				m_FlowTimer = 0;
				m_NoteState = NOTE_SET;
				m_Alive = false;
			}
		}
	}
}
