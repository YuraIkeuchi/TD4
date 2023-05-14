#include "AngerStamp.h"
#include "VariableCommon.h"
#include "Easing.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include <random>
AngerStamp::AngerStamp() {
	
}

//������
bool AngerStamp::Initialize(const XMFLOAT3& pos) {
	texture.reset(IKETexture::Create(ImageManager::ANGER, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	texture->TextureCreate();
	m_Birth = true;
	m_Position = { pos.x,0.0f,pos.z };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };

	return true;
}

//�s��
void AngerStamp::Action() {
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
		//�Ԃɋ߂Â�����
		m_Color = {
			m_Color.x,
			m_Color.y = Ease(In, Cubic, m_Frame, 1.0f, 0.0f),
			m_Color.z = Ease(In, Cubic, m_Frame, 1.0f, 0.0f),
			m_Color.w = Ease(In, Cubic, m_Frame, 1.0f, 0.0f),
		};

		//���̃��C���ɂȂ����甚���G�t�F�N�g����
		if (m_Color.w < 0.1f) {
			BirthParticle();
		}
	}
}

//ImGui
void AngerStamp::ImGui_Origin() {
	ImGui::Begin("Anger");
	ImGui::Text("Timer:%d", m_BirthTimer);
	ImGui::End();
}

void AngerStamp::Origin_Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);

	if (m_Alive) {
		texture->Draw();
	}
	IKETexture::PostDraw();
}

void AngerStamp::BirthParticle() {
	float l_AddSize = 2.5f;
	const float RandScale = 3.0f;
	float s_scale = 0.3f * l_AddSize;
	float e_scale = (4.0f + (float)rand() / RAND_MAX * RandScale - RandScale / 2.0f) * l_AddSize;
	
	//�F
	const float RandRed = 0.2f;
	const float red = 0.2f + (float)rand() / RAND_MAX * RandRed;
	const XMFLOAT4 s_color = { 0.9f, red, 0.1f, 1.0f }; //�Z����
	const XMFLOAT4 e_color = { 0, 0, 0, 1.0f }; //���F

	//�����w��
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_Randlife(10, 40);
	int l_Life = int(l_Randlife(mt));

	ParticleEmitter::GetInstance()->Explosion(l_Life, m_Position, l_AddSize, s_scale, e_scale, s_color, e_color);
}