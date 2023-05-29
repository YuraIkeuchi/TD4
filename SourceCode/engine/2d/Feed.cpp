#include "Feed.h"

#include "FirstBoss.h"
#include"Helper.h"
#include"ImageManager.h"
#define WindowW 1920
#define WindowH 1020
Feed::Feed()
{
	//初期化
	FeedTex = IKESprite::Create(ImageManager::FEED, { 0,0 });
	FeedTex->SetAnchorPoint({ 0.f,0.f });
	FeedTex->SetSize({ WindowW,WindowH });

	Color_RGB = { 0,0,0 };
	m_Alpha = 0.f;
}

void Feed::FeedIn(FeedType type, float feedspeed, bool& feedf)
{
	if (FeedTex == nullptr)return;
	//テク椅子茶色指定
	if (type == FeedType::BLACK)Color_RGB = { 0,0,0 };
	else Color_RGB = { 1,1,1 };

	//遷移
	constexpr int inStaymax = 60;
	switch (_phase)
	{
	case FeedPhase::NON:
		m_Alpha = 0.f;
		if (feedf)_phase = FeedPhase::FEEDIN;
		break;

	case FeedPhase::FEEDIN:
		m_Alpha += feedspeed;
		if (m_Alpha >= 1.f)FeedInCounter++;
		//フェードイン語の待機時間
		if (FeedInCounter > inStaymax)_phase = FeedPhase::FEEDOUT;
		break;
	case FeedPhase::FEEDOUT:
		m_Alpha -= (feedspeed-0.01f);
		if (m_Alpha <= 0.f) {
			//各種初期化
			feedf = false;
			FeedInCounter = 0;
			_phase = FeedPhase::END;
		}
		break;

		case FeedPhase::END:
			if (feedf)_phase = FeedPhase::NON;
			break;

	}

	FeedTex->SetColor({ Color_RGB.x,Color_RGB.y,Color_RGB.z,m_Alpha });
	//アルファ値の範囲指定
	Helper::GetInstance()->Clamp(m_Alpha, 0.f, 1.f);
}

void Feed::Draw()
{
	if (FeedTex == nullptr)return;
	IKESprite::PreDraw();
	FeedTex->Draw();
	IKESprite::PostDraw();
}