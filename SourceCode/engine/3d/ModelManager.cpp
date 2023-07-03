#include"ModelManager.h"
#include "IKEFbxLoader.h"

std::map<ModelManager::ModelName, IKEModel*> ModelManager::model;
std::map<ModelManager::FBXName, IKEFBXModel*> ModelManager::fbxModel;

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instans;
	return &instans;
}

//ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý(Å‰‚©‚ç“Ç‚Ýž‚Þ‚à‚Ì)
void ModelManager::StartInitialize()
{
}

//ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý(”ñ“¯Šúˆ—‚Å“Ç‚Ýž‚Þ‚à‚Ì)
void ModelManager::SecondInitialize()
{
	LoadModel(Ground, "ground");
	LoadModel(Box, "UVSphere");
	LoadModel(Bullet, "UVSphere");
	LoadModel(GhostBullet, "voice");
	LoadModel(BulEnemy, "tri");
	LoadModel(Tyuta, "tieyosi_idol");
	LoadModel(Sutopon, "sutopon");
	LoadModel(Food,"food");
	LoadModel(Ghost,"ghost");
	LoadModel(Buddy,"buddy");
	LoadModel(MobUsa, "mobusa");
	LoadModel(Heart, "heart");
	LoadModel(Pedestal, "dodai");
	LoadModel(Skydome, "skydome");
	LoadModel(KIDO_OBJ, "kidokunobj");
	LoadFBXModel(PLAYER, "kotoko");
	LoadModel(DJ, "DJ");
	LoadModel(CD, "CD");
	LoadFBXModel(SIORI, "siori_idol");
	LoadFBXModel(KIDO, "kidokun_idol");
	LoadFBXModel(TIEYOSHI, "tieyosi_anime");
	LoadModel(GLASS1, "glass1");
	LoadModel(GLASS2, "glass2");
	LoadModel(GLASS3, "glass3");
	LoadModel(MILKCAP_NORMAL, "milkCap_normal");
	LoadModel(MILKCAP_BREAK, "milkCap_break");
	LoadModel(CAP, "cap");
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
