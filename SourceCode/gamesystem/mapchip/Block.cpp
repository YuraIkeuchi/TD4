#include "Block.h"
#include "imgui.h"
#include "ModelManager.h"
#include "VariableCommon.h"
Block* Block::GetInstance()
{
	static Block instance;

	return &instance;
}

//モデル読み込み
void Block::ModelInit() {
	modelNormalBlock = ModelManager::GetInstance()->GetModel(ModelManager::Block);
	//マップチップ用のオブジェクトの初期化
	for (int y = 0; y < map_max_y; y++)
	{
		for (int x = 0; x < map_max_x; x++)
		{
			objNormalBlock[y][x] = make_unique<IKEObject3d>();
			objNormalBlock[y][x]->Initialize();
			objNormalBlock[y][x]->SetModel(modelNormalBlock);
			objNormalBlock[y][x]->SetScale({ 5.0f,5.0f,5.0f });
			objNormalBlock[y][x]->SetTiling({ 5.0f });
			objNormalBlock[y][x]->SetPosition({ 1000.0f,1000.0f,1000.0f });
		}
	}
}
//初期化
void Block::Initialize(std::vector<std::vector<int>>& map, int mapNumber,int StageNumber) {

	stagemap = map;
	MapChip::LoadCsvMap(stagemap, "Resources/csv/stage/stage1.csv");
	MapCreate(0,StageNumber);
}
//更新
void Block::Update() {
	//ステージごとに違う
		//各ブロックのアップデート
	for (int y = 0; y < map_max_y; y++)
	{
		for (int x = 0; x < map_max_x; x++)
		{
			if ((objNormalBlock[y][x]->GetPosition().y != 1000.0f)) {
				objNormalBlock[y][x]->Update();
			}
		}
	}
}
//描画
void Block::Draw() {
	IKEObject3d::PreDraw();
	//マップチップの描画
	for (int y = 0; y < map_max_y; y++)
	{
		for (int x = 0; x < map_max_x; x++)
		{
			if ((objNormalBlock[y][x]->GetPosition().y != 1000.0f)) {
				objNormalBlock[y][x]->Draw();
			}
		}
	}
}
//ブロック配置
void Block::MapCreate(int mapNumber, int StageNumber)
{
	
	for (int y = 0; y < map_max_y; y++) {//(yが15)
		for (int x = 0; x < map_max_x; x++) {//(xが59)

			if (MapChip::GetChipNum(x, y, stagemap[mapNumber]) == 1)
			{
				//位置と大きさの変更
				objNormalBlock[y][x]->SetPosition({ x * LAND_SCALE,-2.0f,-y * LAND_SCALE});
			}
		}
	}
}

//プレイヤーとブロック当たり判定
bool Block::PlayerMapCollideCommon(XMFLOAT3 pos, XMFLOAT2 radius,
	const XMFLOAT3 old_pos,XMFLOAT3& velocity)
{

	//マップチップ
	//X, Y
	float l_MapPosX = 0.0f;
	float l_MapPosZ = 0.0f;
	//Radius
	float l_RadiusX = 2.0f;
	float l_RadiusZ = 2.0f;

	//フラグ
	bool l_IsHit = false;

	//判定
	int l_MapMaxX = static_cast<int>((pos.x + radius.x + LAND_SCALE / 2) / LAND_SCALE);
	int l_MapMinX = static_cast<int>((pos.x - radius.x + LAND_SCALE / 2) / LAND_SCALE);
	int l_MapMaxZ = -static_cast<int>((pos.z - radius.y + LAND_SCALE / 2) / LAND_SCALE - 1);
	int l_MapMinZ = -static_cast<int>((pos.z + radius.y + LAND_SCALE / 2) / LAND_SCALE - 1);

	for (int h = l_MapMinZ; h <= l_MapMaxZ; h++)
	{
		if (h < 0)
		{
			continue;
		}
		for (int w = l_MapMinX; w <= l_MapMaxX; w++)
		{
			if (w < 0)
			{
				continue;
			}

			if (MapChip::GetChipNum(w, h, stagemap[0]) == 1)
			{
				l_MapPosX = objNormalBlock[h][w]->GetPosition().x;
				l_MapPosZ = objNormalBlock[h][w]->GetPosition().z;

				if (pos.x <= l_MapPosX + l_RadiusX && l_MapPosX - l_RadiusX <= pos.x)
				{
					//下の当たり判定
					if (l_MapPosZ + l_RadiusZ + radius.y > pos.z && l_MapPosZ < old_pos.z)
					{
						l_IsHit = true;
						velocity.z *= -1;
					}
					//上の当たり判定
					else if (l_MapPosZ - l_RadiusZ - radius.y < pos.z && l_MapPosZ > old_pos.z)
					{
						l_IsHit = true;
						velocity.z *= -1;
					}
				}
				if (pos.z <= l_MapPosZ + l_RadiusZ && l_MapPosZ - l_RadiusZ <= pos.z)
				{
					//左の当たり判定
					if (l_MapPosX + l_RadiusX + radius.x > pos.x && l_MapPosX < old_pos.x)
					{
						l_IsHit = true;
						velocity.x *= -1;
					}
					//右の当たり判定
					if (l_MapPosX - l_RadiusX - radius.x < pos.x && l_MapPosX > old_pos.x)
					{
						l_IsHit = true;
						velocity.x *= -1;
					}
				}

			}
		}
	}
	return l_IsHit;
}

//マップチップの初期化
void Block::ResetBlock() {
	for (int y = 0; y < map_max_y; y++)
	{
		for (int x = 0; x < map_max_x; x++)
		{
			objNormalBlock[y][x]->SetPosition({ 1000.0f,1000.0f,1000.0f });
		}
	}
}