#include "UI.h"
#include"ImageManager.h"
#include <Player.h>
#include <HungerGauge.h>

UI::~UI() {
	TexList.clear();
}

//������
void UI::Initialize() {
	for (int i = HeartOne; i < UnderStatusGauge; i++) {
		sprites[i] = CreateUi(ImageManager::HEART, pos[i], { 50.f,50.f }, { 1.5f, 1.5f, 1.5f,1 });
		TexList.emplace_back(std::move(sprites[i]));
	}

	{//�Q�[�W���~��
		sprites[UnderStatusGauge] = CreateUi(ImageManager::UnderGauge, gaugePos, gaugeSize, { 1.5f, 1.5f, 1.5f,1 });
		TexList.emplace_back(std::move(sprites[UnderStatusGauge]));
	}
	{//�Q�[�W
		sprites[StatusGauge] = CreateUi(ImageManager::Gauge, gaugePos_, gaugeSize, { 1.5f, 1.5f, 1.5f,1 });
		sprites[StatusGauge].Tex->SetAnchorPoint({0.f,0.5f});
		TexList.emplace_back(std::move(sprites[StatusGauge]));
	}
	{//�Q�[�W��
		sprites[TopStatusGauge] = CreateUi(ImageManager::TopGauge, gaugePos, gaugeSize, { 1.5f, 1.5f, 1.5f,1 });
		TexList.emplace_back(std::move(sprites[TopStatusGauge]));
	}

}

//�X�V
void UI::Update() {
	//Gauge����
	if (HungerGauge::GetInstance()->GetCatchCount() == 0.f) {
		TexList[UnderStatusGauge].IsVisible = false;
		TexList[StatusGauge].IsVisible = false;
		TexList[TopStatusGauge].IsVisible = false;
	} else {
		TexList[StatusGauge].Size = { HungerGauge::GetInstance()->GetPercentage() * gaugeSize.x,gaugeSize.y };
		TexList[UnderStatusGauge].IsVisible = true;
		TexList[StatusGauge].IsVisible = true;
		TexList[TopStatusGauge].IsVisible = true;
	}
	//���C�t����
	for (int i = HeartOne; i < UnderStatusGauge; i++) {
		if (i < Player::GetInstance()->GetHP()) {
			TexList[i].IsVisible =true;
		} else {
			TexList[i].IsVisible = false;
		}
	}






	for (auto i = 0; i < TexList.size(); i++) {
		if (TexList[i].Tex == nullptr)continue;
		TexList[i].Tex->SetPosition(TexList[i].Position);
		TexList[i].Tex->SetSize(TexList[i].Size);
		TexList[i].Tex->SetRotation(TexList[i].Rotation);
		TexList[i].Tex->SetColor(TexList[i].Color);
	}
}

//�`��
void UI::Draw() {
	IKESprite::PreDraw();
	for (auto i = 0; i < TexList.size(); i++) {
		if (TexList[i].Tex == nullptr) { continue; }
		if (TexList[i].IsVisible) {
			TexList[i].Tex->Draw();
		}
	}
	IKESprite::PostDraw();
}

UI::SpriteData UI::CreateUi(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color) {
	SpriteData itr;
	itr.Tex = IKESprite::Create(texNumber, pos);
	itr.Tex->SetAnchorPoint({ 0.f,0.f });
	itr.Position = pos;
	itr.Size = size;
	itr.Color = color;
	return itr;
}

