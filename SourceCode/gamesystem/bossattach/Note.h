#pragma once
#include "ObjCommon.h"
#include <array>
using namespace std;         //  名前空間指定

//音符クラス
class Note {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Note();
	//初期化
	bool Initialize();
	//更新
	void Update();

	//描画
	void Draw(DirectXCommon* dxCommon);

	//ImGui既定
	void ImGuiDraw();
public:
	//gettersetter

private:
	static const int NOTE_MAX = 3;//音符の最大数
private:
	std::array<unique_ptr<IKEObject3d>, NOTE_MAX> m_Object;
	IKEModel* m_Model = nullptr;

	array<XMFLOAT3, NOTE_MAX> m_Position;
	array<XMFLOAT3, NOTE_MAX> m_Rotation;
	array<XMFLOAT3, NOTE_MAX> m_Scale;
	array<XMFLOAT4, NOTE_MAX> m_Color;

	enum NoteType {
		FOLLOW_NOTE,
		DIFF_NOTE,
		CONFU_NOTE,
	};
};