#include "TItleObj.h"
#include "ModelManager.h"
#include "Helper.h"

TitleObj* TitleObj::GetInstance()
{
	static TitleObj instance;

	return &instance;
}

//ÉÇÉfÉãì«Ç›çûÇ›
void TitleObj::LoadResource() {
	ghostmodel = ModelManager::GetInstance()->GetModel(ModelManager::Ghost);
	foodmodel = ModelManager::GetInstance()->GetModel(ModelManager::Food);
	treemodel = ModelManager::GetInstance()->GetModel(ModelManager::TREE);
	groundmodel = ModelManager::GetInstance()->GetModel(ModelManager::Ground);

	for (auto i = 0; i < ghostobj.size(); i++) {
		ghostobj[i].reset(new IKEObject3d());
		ghostobj[i]->Initialize();
		ghostobj[i]->SetModel(ghostmodel);
		ghostobj[i]->SetScale({ 0.15f,0.15f,0.15f });
		ghostobj[i]->SetRotation({ 0.0f,270.0f,0.0f });
	}
	ghostobj[0]->SetPosition({ -5.0f,-1.0f,-13.0f });
	ghostobj[1]->SetPosition({ -1.0f,-1.0f,-13.0f });

	for (auto i = 0; i < treeobj.size(); i++) {
		treeobj[i].reset(new IKEObject3d());
		treeobj[i]->Initialize();
		treeobj[i]->SetModel(treemodel);
		treeobj[i]->SetPosition({ (i * 20.0f) + (-30.0f),-1.0f,10.0f });
	}


	groundobj.reset(new IKEObject3d());
	groundobj->Initialize();
	groundobj->SetModel(groundmodel);
	groundobj->SetPosition({ 0.0f,15.0f,0.0f });
	groundobj->SetScale({ 0.3f,0.3f,0.3f });
	groundobj->SetColor({ 0.5f,0.5f,0.5f,1.0f });
	groundobj->SetTiling(3.0f);

	foodobj.reset(new IKEObject3d());
	foodobj->Initialize();
	foodobj->SetModel(foodmodel);
	foodobj->SetPosition({ -3.0f,-1.0f,-13.0f });
	foodobj->SetScale({ 0.2f,0.2f,0.2f });
}

//èâä˙âª
void TitleObj::Initialize() {

}

//çXêV
void TitleObj::Update() {
	for (auto i = 0; i < ghostobj.size(); i++) {
		ghostobj[i]->Update();
	}
	for (auto i = 0; i < treeobj.size(); i++) {
		treeobj[i]->Update();
	}

	groundobj->Update();
	foodobj->Update();
}

//ï`âÊ
void TitleObj::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	for (auto i = 0; i < ghostobj.size(); i++) {
		ghostobj[i]->Draw();
	}
	for (auto i = 0; i < treeobj.size(); i++) {
		treeobj[i]->Draw();
	}

	groundobj->Draw();
	foodobj->Draw();
	IKEObject3d::PostDraw();
}