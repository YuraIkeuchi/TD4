#pragma once
#include <DirectXMath.h>
#include "MapChip.h"
#include "IKEObject3d.h"
#include <memory>
using namespace std;         //  ���O��Ԏw��
//�}�b�v�`�b�v�̃N���X
class Block {
private:
	// DirectX::���ȗ�
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

	//�}�b�v�`�b�v����
	void MapCreate(int mapNumber,int StageNumber);

	//�}�b�v�`�b�v�����蔻��(�v���C���[)
	bool PlayerMapCollideCommon(XMFLOAT3 pos, XMFLOAT2 radius,
		const XMFLOAT3 old_pos,XMFLOAT3& velocity);

private:
	//�}�b�v�`�b�v1�̑傫��
	const float LAND_SCALE = 5.0f;
	//�}�b�v�`�b�v�֌W
	IKEModel* modelNormalBlock = nullptr;
	//�e�}�b�v�̓ǂݍ���
	std::vector<std::vector<int>> stagemap; //1�}�b�v
	std::vector<std::vector<int>> bossmap; //�{�X�}�b�v
	unique_ptr <IKEObject3d> objNormalBlock[map_max_y][map_max_x]; //�X�e�[�W�u���b�N
	
};