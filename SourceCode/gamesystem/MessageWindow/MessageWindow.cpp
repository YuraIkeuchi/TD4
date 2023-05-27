#include "MessageWindow.h"

void MessageWindow::Initialize()
{
	window_ = IKESprite::Create(ImageManager::WINDOW, window_pos);
	window_->SetAnchorPoint({ 0.5f,0.5f });
	window_->SetSize(window_size);

	blackback_ = IKESprite::Create(ImageManager::BLACKWINDOW, {});

	girl_ = IKESprite::Create(ImageManager::GIRL, { -100.f,300.f });
	girl_->SetColor(girl_color);

	sutopon_ = IKESprite::Create(ImageManager::SUTOPON, { 1100.f,560.f });
	sutopon_->SetColor(sutopon_color);
	sutopon_->SetAnchorPoint({ 0.5f,0.5f });
	sutopon_->SetSize({ 250.f,250.f });
}

void MessageWindow::Update()
{
	Display();

	girl_->SetColor(girl_color);
	sutopon_->SetColor(sutopon_color);
	window_->SetPosition(window_pos);
	window_->SetSize(window_size);
	blackback_->SetColor(black_color);
}

void MessageWindow::Draw()
{
	blackback_->Draw();
	window_->Draw();
	girl_->Draw();
	sutopon_->Draw();
}

void MessageWindow::Display()
{
	if (nowframe >= 1) { return; }
	frame++;
	nowframe = frame / maxframe;
	if (frame >= maxframe) {
		frame = maxframe;
	}
	window_pos.y = Ease(Out, Sine, nowframe, WinApp::window_height + 100, WinApp::window_height - 100);
	window_size.x = Ease(Out, Sine, nowframe, 0, 1300);
	window_size.y = Ease(Out, Sine, nowframe, 0, 223);
	black_color.w = Ease(Out, Sine, nowframe, 0, 1);
	girl_color.w = Ease(Out, Sine, nowframe, 0, 1);
}

bool MessageWindow::DisplayCheck()
{
	if (nowframe >= 1) {
		return true;
	}
	return false;
}
