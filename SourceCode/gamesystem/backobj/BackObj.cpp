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
//������
bool BackObj::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	return true;
}

//�X�V
void BackObj::Update() {
	Obj_SetParam();
}

//�`��
void BackObj::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}