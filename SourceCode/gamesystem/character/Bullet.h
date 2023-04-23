#pragma once
#include "ObjCommon.h"
//wiÌâÈÇ¤ÊwiÌNX
class Bullet :
	public ObjCommon {
protected:
	// DirectX::ðÈª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Bullet();

	bool Initialize() override;//ú»
	/// <summary>
	/// t[
	/// </summary>
	void Update() override;

	/// <summary>
	/// `æ
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	/// <summary>
	/// ImGui`æ
	/// </summary>
	void ImGuiDraw();
private:
	//eÌ®­
	void Move();
public:
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }

private:
	XMFLOAT2 m_Angle = {};
	float m_AddSpeed = {};
};
