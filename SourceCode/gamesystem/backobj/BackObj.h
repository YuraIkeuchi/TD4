#pragma once
#include "ObjCommon.h"
#include <map>

struct JsonData;
//”wŒi‚ÌŠâ‚È‚Ç‹¤’Ê”wŒi‚ÌƒNƒ‰ƒX
class BackObj :
	public ObjCommon {
protected:
	// DirectX::‚ğÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static BackObj* GetInstance();
	//Json“Ç‚İ‚İ
	void LoadMap();
	bool Initialize() override;//‰Šú‰»
	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update() override;

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:
	//Json—p
	JsonData* jsonData = nullptr;
	std::map<std::string, IKEModel*> models;
	std::vector<IKEObject3d*> objects;

	XMFLOAT3 m_ParticlePos = {};
};
