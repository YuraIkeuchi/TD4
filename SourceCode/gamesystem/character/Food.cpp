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
//初期化
bool Food::Initialize() {
	m_Position = { 0.0f,0.0f,10.0f };
	return true;
}

//更新
void Food::Update() {
	Obj_SetParam();
	//タイプによって色を一旦変えてる
	if (m_Alive) {
		
	}
}

//描画
void Food::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	if (m_Alive) {
	
	}
}
//ImGui描画
void Food::ImGuiDraw() {
	ImGui::Begin("Food");
	ImGui::Text("Pos.X:%f", m_Position.x);
	ImGui::Text("Timer:%d", m_Timer);
	if (m_Alive) {

	}
	ImGui::End();
}