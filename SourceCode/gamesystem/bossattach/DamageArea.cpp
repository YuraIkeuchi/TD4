#include "DamageArea.h"
#include "ModelManager.h"
#include "ImageManager.h"
#include "Helper.h"
DamageArea::DamageArea(const int Num) {
	obj.resize(Num);
	m_Position.resize(Num);
	m_Rotation.resize(Num);
	m_Scale.resize(Num);
	m_Color.resize(Num);

	tex.resize(Num - 1);
	m_TexPosition.resize(Num - 1);
	m_TexRotation.resize(Num - 1);
	m_TexScale.resize(Num - 1);
	m_TexColor.resize(Num - 1);

	model = ModelManager::GetInstance()->GetModel(ModelManager::Bullet);
	for (size_t i = 0; i < obj.size(); i++) {
		obj[i] = new IKEObject3d();
		obj[i]->Initialize();
		obj[i]->SetModel(model);
	}

	for (size_t i = 0; i < tex.size(); i++) {
		tex[i] = IKETexture::Create(ImageManager::DAMAGEAREA, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
		tex[i]->TextureCreate();
	}
}

void DamageArea::Initialize() {
	m_Position[0] = { 40.0f,0.0f,0.0f };
	m_Position[1] = { 50.0f,0.0f,-30.0f };
	m_Position[2] = { 0.0f,0.0f,0.0f };
	m_Position[3] = { -20.0f,0.0f,30.0f };

	for (size_t i = 0; i < obj.size(); i++) {
		m_Scale[i] = { 1.0f,1.0f,1.0f };
		m_Color[i] = { 0.0f,1.0f,0.0f,1.0f };
		m_Rotation[i] = { 0.0f,0.0f,0.0f };
	}

	for (size_t i = 0; i < tex.size(); i++) {
		m_TexColor[i] = { 1.0f,0.0f,0.0f,1.0f };
		m_TexRotation[i] = { 90.0f,0.0f,0.0f };
		m_TexScale[i] = { 0.2f,3.0f,1.0f };
	}
}

void DamageArea::Update() {
	for (size_t i = 0; i < tex.size(); i++) {
		m_TexPosition[i] = {
		m_TexPosition[i].x = (m_Position[i].x + m_Position[i + 1].x) / 2,
		0.0f,
		m_TexPosition[i].z = (m_Position[i].z + m_Position[i + 1].z) / 2,
		};

		m_TexRotation[i].y = Helper::GetInstance()->DirRotation(m_TexPosition[i], m_Position[i], -PI_180);

		m_TexScale[i].y = Helper::GetInstance()->ChechLength(m_Position[i], m_Position[i + 1]) * 0.1f;
		if (tex[i] != nullptr) {
			tex[i]->Update();
			tex[i]->SetPosition(m_TexPosition[i]);
			tex[i]->SetRotation(m_TexRotation[i]);
			tex[i]->SetScale(m_TexScale[i]);
			tex[i]->SetColor(m_TexColor[i]);
		}
	}

	for (size_t i = 0; i < obj.size(); i++) {
		if (obj[i] != nullptr) {
			obj[i]->Update();
			obj[i]->SetPosition(m_Position[i]);
			obj[i]->SetRotation(m_Rotation[i]);
			obj[i]->SetScale(m_Scale[i]);
			obj[i]->SetColor(m_Color[i]);
		}
	}
}

void DamageArea::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	for (size_t i = 0; i < obj.size(); i++) {
		if (obj[i] != nullptr) {
			obj[i]->Draw();
		}
	}
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (size_t i = 0; i < tex.size(); i++) {
		if (tex[i] != nullptr) {
			tex[i]->Draw();
		}
	}
	IKETexture::PostDraw();
}

void DamageArea::ImGuiDraw() {
	ImGui::Begin("DamageArea");
	for (size_t i = 0; i < tex.size(); i++) {
		ImGui::Text("Scale[%d]:%f", i, m_TexScale[i].y);
	}
	ImGui::SliderFloat("PosX", &m_Position[0].x, -50, 50);
	ImGui::SliderFloat("PosZ", &m_Position[0].z, -50, 50);
	ImGui::End();
}