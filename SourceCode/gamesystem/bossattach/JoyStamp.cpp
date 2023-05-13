#include "JoyStamp.h"
#include "VariableCommon.h"
#include "Easing.h"
#include "ImageManager.h"

JoyStamp::JoyStamp() {

}

//‰Šú‰»
bool JoyStamp::Initialize(const XMFLOAT3& pos) {
	texture.reset(IKETexture::Create(ImageManager::JOY, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	texture->TextureCreate();
	m_Birth = true;
	m_Position = { pos.x,0.0f,pos.z };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	return true;
}

//s“®
void JoyStamp::Action() {
	if (m_Birth) {
		m_BirthTimer++;

		if (m_BirthTimer == 5) {
			m_Alive = true;
		}

		if (m_BirthTimer >= 200) {
			if (m_Frame < m_FrameMax) {
				m_Frame += 0.01f;
			}
			else {
				m_Alive = false;
				m_Birth = false;
			}
		
		}
		m_Color.w = Ease(In, Cubic, m_Frame, 1.0f, 0.0f);
	}
}

//ImGui
void JoyStamp::ImGui_Origin() {
	ImGui::Begin("Joy");
	ImGui::Text("m_Color.w:%f", m_Color.w);
	ImGui::End();
}


void JoyStamp::Origin_Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon,AlphaBlendType);

	if (m_Alive) {
		texture->Draw();
	}
	IKETexture::PostDraw();
}