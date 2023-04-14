#pragma once
#include <DirectXMath.h>
#include "MapChip.h"
#include "IKEObject3d.h"
#include <memory>
using namespace std;         //  名前空間指定
//マップチップのクラス
class Block {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static Block* GetInstance();
	void ModelInit();
	void Initialize(std::vector<std::vector<int>>& map, int mapNumber,int StageNumber);
	void Update();
	void Draw();

	void ResetBlock();

	//マップチップ生成
	void MapCreate(int mapNumber,int StageNumber);

	//マップチップ当たり判定(プレイヤー)
	bool PlayerMapCollideCommon(XMFLOAT3 pos, XMFLOAT2 radius,
		const XMFLOAT3 old_pos,XMFLOAT3& velocity);

private:
	//マップチップ1つの大きさ
	const float LAND_SCALE = 5.0f;
	//マップチップ関係
	IKEModel* modelNormalBlock = nullptr;
	//各マップの読み込み
	std::vector<std::vector<int>> stagemap; //1マップ
	std::vector<std::vector<int>> bossmap; //ボスマップ
	unique_ptr <IKEObject3d> objNormalBlock[map_max_y][map_max_x]; //ステージブロック
	
};