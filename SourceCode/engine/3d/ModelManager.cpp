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
	LoadModel(Bullet, "UVSphere");
	LoadModel(GhostBullet, "voice");
	LoadModel(BulEnemy, "tri");
	LoadModel(Tyuta, "tieyosi_idol");
	LoadModel(Sutopon, "sutopon");
	LoadModel(Food,"food");
	LoadModel(Ghost,"ghost");
	LoadModel(Buddy,"buddy");
	LoadModel(MobUsa, "mobusa");
	LoadModel(Camera, "A&J");
	LoadModel(Heart, "heart");
	LoadModel(Pedestal, "dodai");
	LoadModel(Skydome, "skydome");
	LoadModel(KIDO_OBJ, "kidokunobj");
	LoadModel(DJ, "DJ");
	LoadModel(CD, "CD");
	LoadModel(SKIRT, "skirt");
	LoadModel(NOTE, "NOTE");

	LoadModel(GLASS1, "glass1");
	LoadModel(GLASS2, "glass2");
	LoadModel(GLASS3, "glass3");
	LoadModel(MILKCAP_NORMAL, "milkCap_normal");
	LoadModel(MILKCAP_BREAK, "milkCap_break");
	LoadModel(CAP, "cap");
	LoadModel(NAIL, "Nail");

	LoadFBXModel(PLAYER, "kotoko");
	LoadFBXModel(SIORI, "siori_idol");
	LoadFBXModel(KIDO, "kidokun_idol");
	LoadFBXModel(TIEYOSHI, "tieyosi_anime");
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
