#include "ViewBullet.h"
#include "VariableCommon.h"
#include "CsvLoader.h"
#include "Helper.h"
ViewBullet::ViewBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool ViewBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/bullet/bullet.csv", "ViewSpeed")));
	m_Alive = false;
	return true;
}

//ImGui描画
void ViewBullet::ImGui_Origin() {
}
//オリジナル描画
void ViewBullet::Draw_Origin() {
}
//弾の特有処理
void ViewBullet::Action() {
	if (m_Alive) {
		//移動を加算
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		if ((m_Charge) && (Helper::GetInstance()->CheckValueRange(m_Scale.x,0.1f,2.5f))) {
			m_Scale = Helper::GetInstance()->Float3AddFloat(m_Scale, 0.1f);
		}
	}
	Obj_SetParam();
}
//固有の描画
void ViewBullet::View_Draw() {
	if (m_Alive) {
		Obj_Draw();
	}
}