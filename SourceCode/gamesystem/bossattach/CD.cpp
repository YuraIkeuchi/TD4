#include "CD.h"
#include "ModelManager.h"
#include "ImageManager.h"
CD::CD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	for (size_t i = 0; i < m_Object.size(); i++) {
		m_Object[i].reset(new IKEObject3d());
		m_Object[i]->Initialize();
		m_Object[i]->SetModel(m_Model);
	}

}

bool CD::Initialize() {
	for (size_t i = 0; i < m_Object.size(); i++) {
		m_Scale[i] = {2.0f,2.0f,2.0f};
		m_Rotation[i] = { 1.0f,1.0f,1.0f };
		m_Color[i] = { 1.0f,1.0f,1.0f,1.0f };
	}

	m_Position[FOLLOW_NOTE] = { 40.0f,0.0f,40.0f };
	m_Position[DIFF_NOTE] = { -40.0f,0.0f,40.0f };
	m_Position[CONFU_NOTE] = { 0.0f,0.0f,-40.0f };

	return true;
}

void CD::Update() {
	for (size_t i = 0; i < m_Object.size(); i++) {
		m_Object[i]->Update();
		m_Object[i]->SetPosition(m_Position[i]);
		m_Object[i]->SetScale(m_Scale[i]);
		m_Object[i]->SetRotation(m_Rotation[i]);
		m_Object[i]->SetColor(m_Color[i]);
	}

}

void CD::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	for (size_t i = 0; i < m_Object.size(); i++) {
		m_Object[i]->Draw();
	}
	IKEObject3d::PostDraw();
}

void CD::ImGuiDraw() {
	
}