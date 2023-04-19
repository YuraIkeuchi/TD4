#pragma once
#include"InterEnemy.h"
using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class NormalEnemy :public InterEnemy {
public:
	NormalEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGuiDraw()override;
	//void Circle();//�~�^��
	//void Follow();//�Ǐ]
	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	void Particle();
protected:

private:
	float m_CircleAngle = 0.0f;
	float m_CircleRadius = 0.0f;
	float m_CircleSpeed = 0.0f;
	float m_CircleScale = 20.0f;
};

