#include "Food.h"
#include "ModelManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Helper.h"
Food::Food() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Cube);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool Food::Initialize() {
	m_Position = { 0.0f,0.0f,10.0f };
	return true;
}

//�X�V
void Food::Update() {
	Obj_SetParam();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	if (m_Alive) {
		
	}
}

//�`��
void Food::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	if (m_Alive) {
	
	}
}
//ImGui�`��
void Food::ImGuiDraw() {
	ImGui::Begin("Food");
	ImGui::Text("Pos.X:%f", m_Position.x);
	ImGui::Text("Timer:%d", m_Timer);
	if (m_Alive) {

	}
	ImGui::End();
}