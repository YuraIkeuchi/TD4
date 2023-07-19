#include "Menu.h"
#include "ImageManager.h"
#include <Input.h>
#include <Helper.h>
#include <Easing.h>
void (Menu::* Menu::stateTable[])() = {
	&Menu::NoneUpdate,
	&Menu::OpenUpdate,
	&Menu::SelectUpdate,
	&Menu::CheckUpdate,
	&Menu::CloseUpdate,

};

Menu::Menu() {


}

Menu::~Menu() {

}

void Menu::Initialize() {
	sprites_[BackScreen] = CreateSprite(ImageManager::MENU_HOME, { half_Width,half_Height }, 15.0f);

	sprites_[ConfirmButton] = CreateSprite(ImageManager::MENU_RULE, buttonPos[0], 15.0f);
	sprites_[SceneBackButton] = CreateSprite(ImageManager::MENU_SCENECHANGE, buttonPos[1], 15.0f);
	sprites_[ReturnButton] = CreateSprite(ImageManager::MENU_RESET, buttonPos[2], 15.0f);
	sprites_[SutoponBar] = CreateSprite(ImageManager::MENU_FRAME, buttonPos[0], 15.0f);

	//sprites_[]=CreateSprite(ImageManager::MENU_SCENECHANGE, { 0,0 }, 15.0f));



	for (int i = 0; i < SpriteMax; i++) {
		sprites_[i].sprite->SetAnchorPoint({ 0.5f,0.5f });
	}
}

void Menu::Update() {

	(this->*stateTable[(size_t)_state])();

	for (int i = 0; i < SpriteMax; i++) {
		sprites_[i].sprite->SetSize(sprites_[i].size);
		sprites_[i].sprite->SetPosition(sprites_[i].pos);
	}
}

void Menu::Draw() {
	IKESprite::PreDraw();
	for (int i = 0; i < SpriteMax; i++) {
		if (!sprites_[i].isVisible) { continue; }
		sprites_[i].sprite->Draw();
	}
	IKESprite::PostDraw();
}

void Menu::NoneUpdate() {
	if (Input::GetInstance()->TriggerButton(Input::START)) {
		_state = State::OPEN;
	}
}

void Menu::OpenUpdate() {
	for (int i = BackScreen; i <= SutoponBar; i++) {
		sprites_[(size_t)i].isVisible = true;
		sprites_[(size_t)i].start_size = { 0.0f,0.0f };
	}
	sprites_[(size_t)BackScreen].end_size = { 1280.0f,720.0f };
	sprites_[(size_t)ConfirmButton].end_size = { 300.0f,150.0f };
	sprites_[(size_t)SceneBackButton].end_size = { 300.0f,150.0f };
	sprites_[(size_t)ReturnButton].end_size = { 300.0f,150.0f };
	sprites_[(size_t)SutoponBar].end_size = { 300.0f,150.0f };


	for (int i = 0; i < SpriteMax; i++) {
		if (!Helper::GetInstance()->FrameCheck(sprites_[i].easingFrame, 1 / sprites_[i].kFrameMax)) {
			sprites_[(size_t)i].size = {
				Ease(Out,Quad,sprites_[i].easingFrame,sprites_[i].start_size.x,sprites_[i].end_size.x),
				Ease(Out,Quad,sprites_[i].easingFrame,sprites_[i].start_size.y,sprites_[i].end_size.y)
			};
		}
	}
	if (Helper::GetInstance()->FrameCheck(sprites_[BackScreen].easingFrame, 1 / sprites_[BackScreen].kFrameMax)) {
		_state = State::SELECT;
		for (int i = 0; i < SpriteMax; i++) {
			sprites_[i].easingFrame = 0.0f;
		}
	}
}

void Menu::SelectUpdate() {
	if (TriggerMoveButton()) {
		moveBar = true;
		if (Input::GetInstance()->TiltPushStick(Input::L_RIGHT)) {
			barIndex++;
		} else {
			barIndex--;
		}
		if (barIndex > RETURN) {
			barIndex = CONFIRM;
		}
		if (barIndex < CONFIRM) {
			barIndex = RETURN;
		}
		sprites_[(size_t)SutoponBar].start_pos = sprites_[(size_t)SutoponBar].pos;
		sprites_[(size_t)SutoponBar].end_pos = buttonPos[barIndex];
		sprites_[(size_t)SutoponBar].easingFrame = 0.0f;
	}
	if (moveBar) {
		if (!Helper::GetInstance()->FrameCheck(sprites_[SutoponBar].easingFrame, 1 / sprites_[SutoponBar].kFrameMax)) {
			sprites_[(size_t)SutoponBar].pos = {
			Ease(Out, Quad, sprites_[SutoponBar].easingFrame, sprites_[SutoponBar].start_pos.x, sprites_[SutoponBar].end_pos.x),
			Ease(Out, Quad, sprites_[SutoponBar].easingFrame, sprites_[SutoponBar].start_pos.y, sprites_[SutoponBar].end_pos.y)
			};
		} else {
			moveBar = false;
			sprites_[(size_t)SutoponBar].easingFrame = 0.0f;
		}
	}





}

void Menu::CheckUpdate() {
}

void Menu::ConfirmUpdate() {
}

void Menu::CloseUpdate() {
}

Menu::SpriteInfo Menu::CreateSprite(UINT num, XMFLOAT2 pos, float easingFrame) {
	SpriteInfo itr = {};
	itr.sprite = IKESprite::Create(num, pos);
	itr.size = itr.sprite->GetSize();
	itr.pos = pos;
	itr.start_size = itr.size;
	itr.end_size = itr.size;
	itr.start_pos = pos;
	itr.end_pos = pos;
	itr.isAction = false;
	itr.easingFrame = 0.0f;
	itr.kFrameMax = easingFrame;
	return itr;
}

bool Menu::TriggerMoveButton() {
	if (moveBar) { return false; }
	if (Input::GetInstance()->TiltPushStick(Input::L_RIGHT, 0.0f)) {
		return true;
	}
	if (Input::GetInstance()->TiltPushStick(Input::L_LEFT, 0.0f)) {
		return true;
	}
	return false;
}
