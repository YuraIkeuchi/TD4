#pragma once
#include"InterEnemy.h"
using namespace std;         //  –¼‘O‹óŠÔw’è
//•’Ê‚Ì“G
class NormalEnemy :public InterEnemy {
public:
	NormalEnemy();
	bool Initialize() override;//‰Šú‰»
	void Finalize() override;//ŠJ•ú
	void Action()override;//XV
	void ImGuiDraw()override;
	//void Circle();//‰~‰^“®
	//void Follow();//’Ç]
	void Draw(DirectXCommon* dxCommon) override;//•`‰æ
private:
	void Particle();
protected:

private:
	float m_CircleAngle = 0.0f;
	float m_CircleRadius = 0.0f;
	float m_CircleSpeed = 0.0f;
	float m_CircleScale = 20.0f;
};

