#pragma once
#include "Easing.h"
#include "IKESprite.h"
#include <WinApp.h>

#include<vector>
#include <memory>
#include<string>
#include <list>

class SceneChanger {
public:
	SceneChanger();
	~SceneChanger();

	/// <summary>
	/// ����k����
	/// </summary>
	void Initialize();
	/// <summary>
	/// ����k����
	/// </summary>
	void InitializeOver();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	enum ReverseType {
		NonReverse = 1,
		Reverse = -1
	};
	/// <summary>
	/// �V�[���؂�ւ�
	/// </summary>
	/// <param name="sceneName"></param>
	bool ChangeScene(const std::string& sceneName, const ReverseType _reverse);
	/// <summary>
/// �V�[���؂�ւ�
/// </summary>
/// <param name="sceneName"></param>
	bool ChangeSceneExtra(const std::string& sceneName, const ReverseType _reverse);

	/// <summary>
	/// �V�[���؂�ւ�
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeGameOver();



	/// <summary>
	/// �C�[�W���O�X�^�[�g
	/// </summary>
	void  ChangeStart() { easing_start = true; }
	const bool& GetEasingStart() { return easing_start; }
private:
	std::vector<std::unique_ptr<IKESprite>> sprites;

	std::list<std::unique_ptr<IKESprite>> over_sprites;

	const float width = WinApp::window_width;
	const float height = WinApp::window_height;


	const int base_size = 80;
	const int width_num = (int)width / base_size;
	const int height_num = (int)height / base_size;

	std::vector<float> frame;
	std::vector<DirectX::XMFLOAT4> color_;

	bool easing_start = false;
	float ease_frame = 0.0f;
	//���t���[���ŏI��邩
	float end_frame = 100.0f;
};