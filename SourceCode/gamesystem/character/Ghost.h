#pragma once
#include "ObjCommon.h"
#include "Player.h"
//wiÌâÈÇ¤ÊwiÌNX
class Ghost :
	public ObjCommon {
protected:
	// DirectX::ðÈª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Ghost();

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
	//p[eBN
	void Particle();
	//½è»è
	bool Collision();
	//H¿¶Y
	void BirthGhost();
public://getter setter
	void SetPlayer(Player* player) { this->player.reset(player); }
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
private:
	unique_ptr<Player> player;
	bool m_Alive = true;//¶¶tO
	int m_Timer = 0;
};
