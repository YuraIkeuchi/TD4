#include "FirstBoss.h"
#include "ModelManager.h"
#include "Helper.h"
//生成
FirstBoss::FirstBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Boss);

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
bool FirstBoss::Initialize() {

	m_Position = { 0.0f,0.0f,10.0f };
	m_Scale = { 3.5f,3.5f,3.5f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	return true;
}
//行動
void FirstBoss::Action() {

	//OBJのステータスのセット
	Obj_SetParam();
}
//ポーズ
void FirstBoss::Pause() {

}