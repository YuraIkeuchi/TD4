#include "SceneManager.h"
#include "ImageManager.h"
#include "ModelManager.h"
#include<cassert>
void SceneManager::Finalize() {
	//�Ō�̃V�[���̏I���ƊJ��
	scene_->Finalize();
	delete scene_;
}

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::Update(DirectXCommon* dxCommon) {
	//�V�[���؂�ւ������邩�ǂ���
	if (nextScene_) {
		//���V�[���̏I��
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		scene_ = nextScene_;
		nextScene_ = nullptr;
		scene_->Initialize(dxCommon);
	}
	//���[�f�B���O
	if (m_Load == true) {
		switch (m_loadType)
		{
		case SceneManager::NoLoad://���[�h���Ă��Ȃ��Ƃ�
			m_th = std::thread([&] {AsyncLoad(); });
			m_loadType = LoadStart;
			break;
		case SceneManager::LoadStart://���[�h���Ă���Ƃ�
			break;
		case SceneManager::LoadEnd://���[�h�I�������
			m_th.join();
			m_loadType = NoLoad;
			m_Load = false;
			break;
		default:
			break;
		}
	}
	scene_->Update(dxCommon);
}

void SceneManager::Draw(DirectXCommon* dxCommon) {
	scene_->Draw(dxCommon);
}

void SceneManager::ChangeScene(const std::string& sceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);
	//���̃V�[������
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}


void SceneManager::AsyncLoad()
{
	std::thread t = std::thread([&] {
		ImageManager::GetInstance()->SecondLoad2D(), ImageManager::GetInstance()->SecondLoadTex2D(), ModelManager::GetInstance()->SecondInitialize(); });

	//�_�~�[��1�b�҂�
	auto sleepTime = std::chrono::seconds(1);
	std::this_thread::sleep_for(sleepTime);

	t.join();
	// ���[�h���=���[�h�I��
	m_loadType = LoadEnd;
}
