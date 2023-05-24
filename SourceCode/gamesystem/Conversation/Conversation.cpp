#include "Conversation.h"
#include"ImageManager.h"
#include"Easing.h"

const XMFLOAT4 kHalfClear{ 0.5f,0.5f,0.5f,0.5f };

void Conversation::Initialize(DirectXCommon* dxcommon)
{
	playertext_ = make_unique<Text>();
	sutopontext_ = make_unique<Text>();
	playertext_->Initialize(dxcommon);
	sutopontext_->Initialize(dxcommon, { 0.f,1.f,1.f,1.f });

	conversationwindow = IKESprite::Create(ImageManager::WINDOW, window_pos);
	conversationwindow->SetAnchorPoint({ 0.5f,0.5f });
	conversationwindow->SetSize(window_size);

	blackwindow = IKESprite::Create(ImageManager::BLACKWINDOW, {});

	girl = IKESprite::Create(ImageManager::GIRL, { -100.f,300.f });
	girl->SetColor(girl_color);

	megahon = IKESprite::Create(ImageManager::SUTOPON, { 1100.f,560.f });
	megahon->SetColor(sutopon_color);
	megahon->SetAnchorPoint({ 0.5f,0.5f });
	megahon->SetSize({ 250.f,250.f });

	for (int i = 0; i < 3; i++) {
		playerward_[i] = L" ";
		sutoponward_[i] = L" ";
	}
}

void Conversation::Update()
{
	girl->SetColor(girl_color);
	megahon->SetColor(sutopon_color);
	conversationwindow->SetPosition(window_pos);
	conversationwindow->SetSize(window_size);
	blackwindow->SetColor(black_color);
	playertext_->Update(playerward_[0], playerward_[1], playerward_[2]);
	sutopontext_->Update(sutoponward_[0], sutoponward_[1], sutoponward_[2]);
}

void Conversation::SproteDraw()
{
	blackwindow->Draw();
	conversationwindow->Draw();
	girl->Draw();
	megahon->Draw();
}

void Conversation::FontDraw(DirectXCommon* dxcommon)
{
	playertext_->Draw(dxcommon);
	sutopontext_->Draw(dxcommon);
}

void Conversation::Tyutorial()
{
	if (conversation == 0) {
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

		playerward_[0] = L"ここはどこだろう?";
		playerward_[1] = L"あたりを見回してみよう";
		playerward_[2] = L"'Lスティックで移動してみよう'";
		
		
	}
	else if (conversation == 1) {
		frame += 0.1f;
		nowframe = frame / maxframe;
		if (frame >= maxframe) {
			frame = maxframe;
		}
		sutopon_color.w = Ease(Out, Sine, nowframe, 0, 1);
		megahon->SetColor(sutopon_color);
		playerward_[0] = L"これは.....メガホン?";
		playerward_[1] = L"でも,動いてる?";
	}
	else if (conversation == 2) {
		girl_color = kHalfClear;
		sutopon_color.w = 1.f;
		sutoponward_[0] = L"う..う........はっ!?";
		sutoponward_[1] = L"敵!?......じゃないみたいだな";
	}
	else if (conversation == 3) {
		sutopon_color = kHalfClear;
		girl_color = { 1.5f,1.5f,1.5f,1.f };
		playerward_[0] = L"え!?メガホンが喋った!";
	}
	else if (conversation == 4) {
		sutopon_color = { 1.f,1.f,1.f,1.f };
		girl_color = kHalfClear;
		sutoponward_[0] = L"メガホンじゃない,オレはストポンだ";
		sutoponward_[1] = L"メガホンに取り憑いた亡霊だ!";
		sutoponward_[2] = L"こうしちゃいられない....追手が来ちまう!";
	}
	else if (conversation == 5) {
		sutopon_color = kHalfClear;
		girl_color = { 1.5f,1.5f,1.5f,1.f };
		playerward_[0] = L"追手ってあれのこと?";
	}

	
}

void Conversation::WardNone()
{
	for (int i = 0; i < 3; i++) {
		frame = 0;
		nowframe = 0;
		playerward_[i] = L" ";
		sutoponward_[i] = L"　";
	}
	conversation += 1;
}
