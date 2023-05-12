#include "InterBoss.h"
#include"Collision.h"
#include "ParticleEmitter.h"
#include "VariableCommon.h"
//�X�V
void InterBoss::Update() {
	//�s��
	Action();
}
//�`��
void InterBoss::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
	EffecttexDraw(dxCommon);
}
//ImGui�`��
void InterBoss::ImGuiDraw() {
	ImGui_Origin();//���ꂼ���ImGui
}

//�e�Ƃ̓����蔻��
void InterBoss::CollideBul(vector<InterBullet*> bullet)
{
	constexpr float BulRad = 1.f;

	constexpr float BossRad = 3.f;

	for (InterBullet* _bullet : bullet) {
		if (_bullet != nullptr) {
			if (Collision::CircleCollision(_bullet->GetPosition().x, _bullet->GetPosition().z, BulRad, m_Position.x, m_Position.z, BossRad))
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Attack_Normal.wav", VolumManager::GetInstance()->GetSEVolum());
				Recv = true;
				_bullet->SetAlive(false);
				if (_bullet->GetScale().x == 1.0f) {
					m_HP--;
				}
				else {
					m_HP -= 2.0f;
				}
			}
		}
	}
}
