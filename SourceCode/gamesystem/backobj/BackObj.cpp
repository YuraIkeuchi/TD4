#include "BackObj.h"
#include "JsonLoader.h"
#include "ParticleEmitter.h"
#include "LeftLimit.h"
#include "RightLimit.h"
#include "DownLimit.h"
#include "UpLimit.h"
BackObj* BackObj::GetInstance()
{
	static BackObj instance;

	return &instance;
}
//マップ読み込み
void BackObj::LoadMap() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Ground);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	jsonData = JsonLoader::LoadFile("GameScene");

	//リスポーン位置
	for (auto& objectData : jsonData->objects) {
		if (objectData.GimmickName == "No") {
			// ファイル名から登録済みモデルを検索
			IKEModel* model = IKEModel::LoadFromOBJ(objectData.fileName, true);
			decltype(models)::iterator it = models.find(objectData.fileName);
			if (it != models.end()) {
				model = it->second;
			}

			// モデルを指定して3Dオブジェクトを生成
			IKEObject3d* newObject = IKEObject3d::Create();
			newObject->SetModel(model);
			XMFLOAT3 Startpos;

			// 座標
			DirectX::XMStoreFloat3(&Startpos, objectData.translation);

			newObject->SetPosition(Startpos);

			// 回転角
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			newObject->SetRotation(rot);

			// 座標
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);

			newObject->SetTiling(30.0f);
			// 配列に登録
			objects.push_back(newObject);
		}
	}
}

//初期化
bool BackObj::Initialize() {
	m_Position = { 0.0f,15.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 0.5f,0.5f,0.5f,1.0f };
	return true;
}

//更新
void BackObj::Update() {
	//Json用
	for (auto& object : objects) {
		object->Update();
	}

	//移動制御のラインにパーティクル設置
	DownLimit::GetInstance()->LimitParticle();
	UpLimit::GetInstance()->LimitParticle();
	LeftLimit::GetInstance()->LimitParticle();
	RightLimit::GetInstance()->LimitParticle();
	//Obj_SetParam();
}

//描画
void BackObj::Draw(DirectXCommon* dxCommon) {
	//Json用
	for (auto& object : objects) {
		object->Draw();
	}
	//Obj_Draw();
}
