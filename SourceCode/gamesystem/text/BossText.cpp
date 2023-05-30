#include "BossText.h"

BossText::BossText() {

}

void BossText::Initialize(DirectXCommon* dxCommon) {
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::AISATU);
}

void BossText::SpriteDraw(DirectXCommon* dxCommon) {
	text_->Draw(dxCommon);
}