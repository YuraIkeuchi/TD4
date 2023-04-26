#include "LoadSceneActor.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include"Easing.h"
#include "Helper.h"

//������
void LoadSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	BaseInitialize(dxCommon, { 0,10,200 }, { 0,0,-200 });
	if (!s_GameLoop) {
		SceneManager::GetInstance()->SetLoad(true);
	}
	CreateStage();




	for (int i = 0; i < SpriteMax; i++) {
		m_SpritesPos[i].x = (i + 1) * 100.0f;
		m_SpritesPos[i].y = 630.0f;
		m_Sprites[i] = IKESprite::Create(ImageManager::LOADING, m_SpritesPos[i]);
		m_Sprites[i]->SetAnchorPoint({ 0.5f,0.5f });
		m_Sprites[i]->SetSize({ 96.0f, 96.0f });
	}
}
//�X�V
void LoadSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	lightgroup->Update();
	camerawork->LoadActorUpdate(camera);

	//�֐��|�C���^�ŏ�ԊǗ�
	(this->*stateTable[static_cast<size_t>(m_SceneState)])();

	for (std::unique_ptr<IKEObject3d>& obj : grounds) {
		obj->Update();
	}
}
//�`��
void LoadSceneActor::Draw(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		SpriteDraw();
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		ImGuiDraw(dxCommon);
		BackDraw(dxCommon);
		SpriteDraw();
		dxCommon->PostDraw();
	}
}
//�O�ʕ`��
void LoadSceneActor::SpriteDraw() {
	IKESprite::PreDraw();
	for (std::unique_ptr<IKESprite>& sprite : m_Sprites) {
		sprite->Draw();
	}

	IKESprite::PostDraw();
}
void LoadSceneActor::CreateStage() {
	m_JsonData = JsonLoader::LoadFile("Introduction");


	// ���f���ǂݍ���
	modelSkydome = make_unique<IKEModel>();
	modelSkydome->Initialize("skydome", false);
	modelGround = make_unique<IKEModel>();
	modelGround->Initialize("ground", false);
	modelFighter = make_unique<IKEModel>();
	modelFighter->Initialize("House", false);
	modelPine = make_unique<IKEModel>();
	modelPine->Initialize("Pine", false);

	models.insert(std::make_pair("skydome", std::move(modelSkydome)));
	models.insert(std::make_pair("ground", std::move(modelGround)));
	models.insert(std::make_pair("House", std::move(modelFighter)));
	models.insert(std::make_pair("Pine", std::move(modelPine)));


	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : m_JsonData->objects) {
		// �t�@�C��������o�^�ς݃��f��������
		IKEModel* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second.get();
		}

		// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		std::unique_ptr<IKEObject3d> newObj = make_unique<IKEObject3d>();
		newObj->Initialize();
		newObj->SetModel(model);

		// ���W
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObj->SetPosition(pos);

		// ��]�p
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObj->SetRotation(rot);

		// ���W
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObj->SetScale(scale);

		// �z��ɓo�^
		grounds.push_back(std::move(newObj));
	}

}

void LoadSceneActor::IntroUpdate() {
	//�ŏ��̕������������Ɠ�����
	m_SpritesAngle[0] += AddMovingVal;

	for (int i = 0; i < SpriteMax; i++) {
		if (i != 0 && m_SpritesAngle[i - 1] > AddMovingVal * 5.0f) {
			m_SpritesAngle[i] += AddMovingVal;
		}
		m_SpritesPos[i].x = CenterPos.x + static_cast<float>(i) * WordsInter;
		m_SpritesPos[i].y = CenterPos.y + sinf(m_SpritesAngle[i] * PI / PI_180) * space;
		m_Sprites[i]->SetPosition(m_SpritesPos[i]);
	}
	if (!SceneManager::GetInstance()->GetLoad()) {
		for (int i = 0; i < SpriteMax; i++) {
			m_StopPos[i] = m_SpritesPos[i];
		}
		m_SceneState = SceneState::MainState;
	}
}
void LoadSceneActor::MainUpdate() {
	m_LoadTimer++;
	float frame = (float)m_LoadTimer / (float)LoadTimerMax;
	for (int i = 0; i < SpriteMax; i++) {
		m_SpritesPos[i].x = Ease(In,Linear,frame, m_StopPos[i].x, CenterPos.x + static_cast<float>(i) * WordsInter);
		m_SpritesPos[i].y = Ease(In, Linear, frame, m_StopPos[i].y, 630.0f);
		m_Sprites[i]->SetPosition(m_SpritesPos[i]); 

	}
	//��莞�ԂŃV�[�����ς��
	if (m_LoadTimer >= LoadTimerMax) {
		m_LoadTimer = 0;
		m_SceneState = SceneState::FinishState;
	}

}
void LoadSceneActor::FinishUpdate() {
	m_LoadTimer++;

	float frame = (float)m_LoadTimer / (float)LoadTimerMax;
	for (int i = 0; i < SpriteMax; i++) {
		m_SpritesPos[i].x = (i + 1) * 100.0f;
		m_SpritesPos[i].y = 630.0f;
		m_Sprites[i]->SetPosition(m_SpritesPos[i]);

		float rot = Ease(In, Quad, frame, 0, PI_360);
		m_Sprites[i]->SetRotation(rot);
	}

	//��莞�ԂŃV�[�����ς��
	if (m_LoadTimer >= LoadTimerMax) {
		SceneManager::GetInstance()->ChangeScene("GAMESCENE");
	}
}
//�w�ʕ`��
void LoadSceneActor::BackDraw(DirectXCommon* dxCommon) {
	for (std::unique_ptr<IKEObject3d>& obj : grounds) {
		obj->Draw();
	}

}
//ImGui�̕`��
void LoadSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Load");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(200, 200));
	ImGui::SliderInt("LoadTimer", &m_LoadTimer, 0, 200);

	ImGui::End();
	camerawork->ImGuiDraw();
}
//���
void LoadSceneActor::Finalize() {
}