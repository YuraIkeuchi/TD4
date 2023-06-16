#include "BarrangeCD.h"
#include "ModelManager.h"

BarrangeCD::BarrangeCD() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}

bool BarrangeCD::Initialize() {
	m_Position = { -40.0f,0.0f,40.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.5f,0.0f,0.5f,1.0f };

	return true;
}

void BarrangeCD::Action() {
	Obj_SetParam();
}

void BarrangeCD::Origin_Draw(DirectXCommon* dxCommon) {
	//Obj_Draw();
}
void BarrangeCD::ImGui_Origin() {

}