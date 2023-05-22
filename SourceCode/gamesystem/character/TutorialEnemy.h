#pragma once
#include"InterEnemy.h"
using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class TutorialEnemy :public InterEnemy {
public:
	TutorialEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGuiDraw()override;
	//void Circle();//�~�^��
	//void Follow();//�Ǐ]
	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	void Appearance()override;
	void Particle();//�p�[�e�B�N��
	void RushAction();
	
protected:

private:
	//�~�^���̕ϐ�
	float m_CircleAngle = 0.0f;
	float m_CircleRadius = 0.0f;
	float m_CircleSpeed = 0.0f;
	float m_CircleScale = 20.0f;
	//�Ǐ]�p�ϐ�
	XMFLOAT3 m_FollowVel{};
	float old;
	bool ret;
	bool appF;
	XMVECTOR PositionA, PositionB;
	XMVECTOR SubVector;
	float t; float RotY;
	int randMove;
	bool Rush;
	float s_scale = 0.0f;
	int MoveTimer;

	bool canRot;
};
