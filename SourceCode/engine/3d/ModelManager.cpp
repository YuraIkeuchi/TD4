#include"ModelManager.h"
#include "IKEFbxLoader.h"

std::map<ModelManager::ModelName, IKEModel*> ModelManager::model;
std::map<ModelManager::FBXName, IKEFBXModel*>  ModelManager::fbxModel;

ModelManager* ModelManager::GetInstance() {
	static ModelManager instans;
	return &instans;
}
void ModelManager::StartInitialize() {
	LoadModel(Ground, "ground");
	LoadModel(Cube, "cube");
}
//ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
void ModelManager::SecondInitialize() {
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

