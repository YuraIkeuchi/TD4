#include "EditorSceneActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ModelManager.h"
#include "VariableCommon.h"
#include "ParticleEmitter.h"

//������
void EditorSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	OpenBrowser* openbrowser_;
	openbrowser_ = new OpenBrowser();
	openbrowser.reset(openbrowser_);
	dxCommon->SetFullScreen(true);
	//���ʂ̏�����
	BaseInitialize(dxCommon);

	//�|�X�g�G�t�F�N�g�̃t�@�C���w��
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");

	ParticleEmitter::GetInstance()->AllDelete();

	placeObj.reset(new PlaceBox());
	placeObj->Initialize();

	ground.reset(new IKEObject3d());
	ground->Initialize();
	ground->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Ground));
	ground->SetRotation({ 0.f,0.f,0.f });
	ground->SetScale({ 1.f,1.f,1.f });
	PlayPostEffect = true;
}
//�X�V
void EditorSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{

	lightgroup->Update();
	//camerawork->SetTarget({ camera->GetEye().x,0.0f,camera->GetEye().z });
	//camera->SetTarget({ camera->GetEye().x,camera->GetEye().y - 10.f,camera->GetEye().z + 20.f, });
	camerawork->EditorCamera();
	camerawork->Update(camera);

	ground->Update();
	//���y�̉��ʂ��ς��
	placeObj->Update();

	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
}
//�`��
void EditorSceneActor::Draw(DirectXCommon* dxCommon)
{
	//�`����@
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		postEffect->ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		ImGuiDraw(dxCommon);

		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//���
void EditorSceneActor::Finalize()
{
}
//���f���̕`��
void EditorSceneActor::ModelDraw(DirectXCommon* dxCommon) {
#pragma region 3D�I�u�W�F�N�g�`��
	//�w�i�͐�ɕ`�悷��
	IKEObject3d::PreDraw();
	placeObj->Draw(dxCommon);

	ground->Draw();
	IKEObject3d::PostDraw();
}
//���̕`��
void EditorSceneActor::BackDraw(DirectXCommon* dxCommon)
{
	ModelDraw(dxCommon);
}
//�|�X�g�G�t�F�N�g��������Ȃ�
void EditorSceneActor::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
#pragma endregion
}
//IMGui�̕`��
void EditorSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {

	placeObj->ImGui_Draw();
	camerawork->ImGuiDraw();
}