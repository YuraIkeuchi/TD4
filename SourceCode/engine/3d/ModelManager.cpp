#include"ModelManager.h"
#include "IKEFbxLoader.h"

std::map<ModelManager::ModelName, IKEModel*> ModelManager::model;
std::map<ModelManager::FBXName, IKEFBXModel*>  ModelManager::fbxModel;

ModelManager* ModelManager::GetInstance() {
	static ModelManager instans;
	return &instans;
}

//モデル読み込み(最初から読み込むもの)
void ModelManager::StartInitialize() {
	
}
//モデル読み込み(非同期処理で読み込むもの)
void ModelManager::SecondInitialize() {
	LoadModel(Ground, "ground");
	LoadModel(Cube, "cube");
	LoadModel(Box, "UVSphere");
	LoadModel(Boss, "chr_knight");
	LoadFBXModel(PLAYER, "PlayerFBXNew");
}

void ModelManager::Finalize() {
	model.clear();
	fbxModel.clear();
}
void ModelManager::LoadModel(const ModelName modelName, std::string fileName) {
	model[modelName] = new IKEModel();
	model[modelName] = IKEModel::LoadFromOBJ(fileName, true);
}
void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName) {
	fbxModel[modelName] = new IKEFBXModel();
	fbxModel[modelName] = IKEFbxLoader::GetInstance()->LoadModelFromFile(fileName);
}

