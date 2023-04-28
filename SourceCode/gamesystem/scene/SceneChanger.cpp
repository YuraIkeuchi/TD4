#include "SceneChanger.h"
#include "ImageManager.h"
#include <SceneManager.h>


SceneChanger::SceneChanger() {
}

SceneChanger::~SceneChanger() {
	for (std::unique_ptr<IKESprite>& sprite : sprites) {
		sprite.reset();
	}
	sprites.clear();
}


void SceneChanger::Initialize() {
	for (int i = 0; i < width_num; i++) {
		for (int j = 0; j < height_num; j++) {
			std::unique_ptr<IKESprite> newSprite;
			newSprite = IKESprite::Create(ImageManager::WHITE, { 0,0 });
			newSprite->SetAnchorPoint({ 0.5f,0.5f });
			newSprite->SetPosition({ (float)(i * base_size) + base_size / 2,(float)(j * base_size) + base_size / 2 });
			newSprite->SetSize({ 0,0 });
			DirectX::XMFLOAT4 col = { 1,1,1,1 };
			newSprite->SetColor(col);

			sprites.push_back(std::move(newSprite));
		}
	}
}
void SceneChanger::InitializeOver() {
	for (int i = 0; i < width_num; i++) {
		for (int j = 0; j < height_num; j++) {
			std::unique_ptr<IKESprite> newSprite;
			newSprite = IKESprite::Create(ImageManager::WHITE, { 0,0 });
			newSprite->SetPosition({ (float)(i * base_size),(float)(j * base_size) });
			newSprite->SetSize({ 0,0 });
			DirectX::XMFLOAT4 col = { 0,0,0,1 };
			newSprite->SetColor(col);

			over_sprites.push_back(std::move(newSprite));
		}
	}




}

void SceneChanger::Draw() {
	if (!easing_start) { return; }
	IKESprite::PreDraw();
	for (std::unique_ptr<IKESprite>& sprite : sprites) {
		sprite->Draw();
	}
	for (std::unique_ptr<IKESprite>& sprite : over_sprites) {
		sprite->Draw();
	}
}

bool SceneChanger::ChangeScene(const std::string& sceneName, const ReverseType _reverse) {
	if (!easing_start) { return false; }
	if (ease_frame > 1.0f) {
		SceneManager::GetInstance()->ChangeScene(sceneName);
		return true;
	} else {
		ease_frame += 1.0f / end_frame;
	}
	float rot = 0;
	rot = Ease(InOut, Quint, ease_frame, 0, 360.f * (float)_reverse);
	DirectX::XMFLOAT2 size{};
	size.x = Ease(InOut, Quint, ease_frame, 0, (float)base_size);
	size.y = Ease(InOut, Quint, ease_frame, 0, (float)base_size);
	for (std::unique_ptr<IKESprite>& sprite : sprites) {
		sprite->SetRotation(rot);
		sprite->SetSize({ size.x,size.y });
	}
	return false;
}

void SceneChanger::ChangeGameOver() {

	if (easing_start) {
		ease_frame += 1.0f / 120.0f;
		DirectX::XMFLOAT2 size = { (float)base_size , (float)base_size };
		if (ease_frame <= 1.0f) {
			size.x = Ease(Out, Quint, ease_frame, 0, (float)base_size);
			size.y = Ease(Out, Quint, ease_frame, 0, (float)base_size);
		} else if (ease_frame >= 1.5f) {
			SceneManager::GetInstance()->ChangeScene("TITLE");

		}

		for (std::unique_ptr<IKESprite>& sprite : over_sprites) {
			sprite->SetAnchorPoint({ 0,0 });
			sprite->SetSize({ size.x,size.y });
		}
	}
}
