#pragma once
#include "ObjCommon.h"
//”wŒi‚ÌŠâ‚È‚Ç‹¤’Ê”wŒi‚ÌƒNƒ‰ƒX
class Bullet :
	public ObjCommon {
protected:
	// DirectX::‚ğÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Bullet();

	bool Initialize() override;//‰Šú‰»
	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update() override;

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	/// <summary>
	/// ImGui•`‰æ
	/// </summary>
	void ImGuiDraw();
private:
	//’e‚Ì“®‚­ˆ—
	void Move();
public:
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }
	void SetBulletType(const int BulletType) { m_BulletType = BulletType; }
private:
	XMFLOAT2 m_Angle = {};
	float m_AddSpeed = {};
	int m_BulletType = {};
};
