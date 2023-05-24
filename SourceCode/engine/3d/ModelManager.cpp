#include"ModelManager.h"
#include "IKEFbxLoader.h"

std::map<ModelManager::ModelName, IKEModel*> ModelManager::model;
std::map<ModelManager::FBXName, IKEFBXModel*> ModelManager::fbxModel;

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instans;
	return &instans;
}

//モデル読み込み(最初から読み込むもの)
void ModelManager::StartInitialize()
{
}

//モデル読み込み(非同期処理で読み込むもの)
void ModelManager::SecondInitialize()
{
	LoadModel(Ground, "ground");
	LoadModel(Cube, "box1x1x1");
	LoadModel(Box, "UVSphere");
	LoadModel(Boss, "chr_knight");
	LoadModel(Bullet, "UVSphere");
	LoadModel(GhostBullet, "voice");
	LoadModel(BulEnemy, "tri");
	LoadModel(Tyuta, "zako");
	LoadModel(Sutopon, "sutopon");
	LoadFBXModel(PLAYER, "PlayerFBXNew");
	LoadFBXModel(KIDO, "kidokun_idol");
}

void ModelManager::Finalize()
{
	model.clear();
	fbxModel.clear();
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new IKEModel();
	model[modelName] = IKEModel::LoadFromOBJ(fileName, true);
}

void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName)
{
	fbxModel[modelName] = new IKEFBXModel();
	fbxModel[modelName] = IKEFbxLoader::GetInstance()->LoadModelFromFile(fileName);
}
