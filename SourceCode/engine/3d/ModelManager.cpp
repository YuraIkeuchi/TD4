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
	LoadFBXModel(PLAYER, "PlayerFBXNew");
	LoadFBXModel(SIORI, "siori_idol");
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
