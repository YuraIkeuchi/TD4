#include "BackObj.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
BackObj::BackObj() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Ground);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//‰Šú‰»
bool BackObj::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.7f,0.7f,0.7f };
	m_Color = { 0.5f,0.5f,0.5f,1.0f };
	return true;
}

//XV
void BackObj::Update() {
	Obj_SetParam();
}

//•`‰æ
void BackObj::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
