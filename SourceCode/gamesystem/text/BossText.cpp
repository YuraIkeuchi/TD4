#include "BossText.h"
#include "Helper.h"
BossText::BossText() {
	window_ = IKESprite::Create(ImageManager::WINDOW, window_pos);
	window_->SetAnchorPoint({ 0.5f,0.5f });
	window_->SetSize(window_size);
	window_->SetPosition(window_pos);
}

void BossText::Initialize(DirectXCommon* dxCommon) {
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::AISATU);
}

void BossText::SelectText(TextManager::Name name) {
	text_->SetConversation(name);
}
void BossText::SelectText(TextManager::Name_First name) {
	text_->SetConversation(name);
}
void BossText::SpriteDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	window_->Draw();
	IKESprite::PostDraw();
	text_->Draw(dxCommon);
}

void BossText::Display()
{
	if (Helper::GetInstance()->FrameCheck(m_Frame, 0.05f)) {
		m_Frame = 1.0f;
	}
	window_pos.y = Ease(Out, Sine, m_Frame, WinApp::window_height + 100, WinApp::window_height - 100);
	window_size.x = Ease(Out, Sine, m_Frame, 0, 1300);
	window_size.y = Ease(Out, Sine, m_Frame, 0, 223);

	window_->SetPosition(window_pos);
	window_->SetSize(window_size);
}