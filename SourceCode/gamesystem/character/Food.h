#pragma once
#include "ObjCommon.h"
//�w�i�̊�Ȃǋ��ʔw�i�̃N���X
class Food :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Food();

	bool Initialize() override;//������
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void ImGuiDraw();
private:

public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }
	void SetFoodType(const int FoodType) { m_FoodType = FoodType; }
private:
	XMFLOAT2 m_Angle = {};//�e�̊p�x
	float m_AddSpeed = {};//�����x
	int m_FoodType = {};//�e�̎��
	int m_Timer = {};//�o������
	int m_TargetTimer = {};//�o�����Ԃ̖ڕW
	bool m_Alive = true;//�����t���O
};
