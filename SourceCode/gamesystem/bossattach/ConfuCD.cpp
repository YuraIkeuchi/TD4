#include "ConfuCD.h"
#include "ModelManager.h"

ConfuCD::ConfuCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

bool ConfuCD::Initialize() {
	m_Position = { -40.0f,0.0f,-40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };

	return true;
}

void ConfuCD::Action() {
	Obj_SetParam();
}

void ConfuCD::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}

void ConfuCD::ImGui_Origin() {

}