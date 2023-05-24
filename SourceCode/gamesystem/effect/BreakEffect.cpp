#include "BreakEffect.h"
#include <random>

BreakEffect::BreakEffect() {

}

bool BreakEffect::Initialize() {
	m_Alive = true;
	return true;
}

void BreakEffect::Action() {
	const int l_TargetTimer = 5;
	if (m_Alive) {
		BirthEffect();

		if (Helper::GetInstance()->CheckMaxINT(m_AliveTimer, l_TargetTimer, 1)) {
			m_Alive = false;
		}
	}
}

void BreakEffect::BirthEffect() {
	float l_AddSize = 2.5f;
	const float RandScale = 3.0f;
	float s_scale = 0.3f * l_AddSize;
	float e_scale = (4.0f + (float)rand() / RAND_MAX * RandScale - RandScale / 2.0f) * l_AddSize;

	//êF
	const float RandRed = 0.2f;
	const float red = 0.2f + (float)rand() / RAND_MAX * RandRed;
	const XMFLOAT4 s_color = { 0.9f, red, 0.1f, 1.0f }; //îZÇ¢ê‘
	const XMFLOAT4 e_color = { 0, 0, 0, 1.0f }; //ñ≥êF

	//óêêîéwíË
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_Randlife(10, 40);
	int l_Life = int(l_Randlife(mt));

	ParticleEmitter::GetInstance()->Explosion(l_Life, m_Position, l_AddSize, s_scale, e_scale, s_color, e_color);
}