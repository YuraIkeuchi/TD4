#include "DebuffCD.h"
#include "ModelManager.h"

DebuffCD::DebuffCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

bool DebuffCD::Initialize() {
	m_Position = { -40.0f,0.0f,-40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };

	return true;
}

void DebuffCD::Action() {
	Obj_SetParam();
}

void DebuffCD::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}

void DebuffCD::ImGui_Origin() {

}