﻿#include "ImageManager.h"

ImageManager* ImageManager::GetInstance()
{
	static ImageManager instans;
	return &instans;
}

//スプライトの読み込み
void ImageManager::StartLoad2D()
{
	IKESprite::LoadTexture(TITLE, L"Resources/2d/sceneback/title2.png");
	IKESprite::LoadTexture(TITLEWORD, L"Resources/2d/sceneback/gameTitle.png");
	IKESprite::LoadTexture(LOADING_LO, L"Resources/2d/objicon/LoadText/01_Lo.png");
	IKESprite::LoadTexture(LOADING_O, L"Resources/2d/objicon/LoadText/02_O.png");
	IKESprite::LoadTexture(LOADING_DI, L"Resources/2d/objicon/LoadText/03_Di.png");
	IKESprite::LoadTexture(LOADING_N, L"Resources/2d/objicon/LoadText/04_N.png");
	IKESprite::LoadTexture(LOADING_G, L"Resources/2d/objicon/LoadText/05_G.png");
	IKESprite::LoadTexture(WHITE, L"Resources/2d/effect/white1x1.png");
	IKESprite::LoadTexture(WINDOW, L"Resources/2d/objicon/ConversationWindow.png");
	IKESprite::LoadTexture(BLACKWINDOW, L"Resources/2d/objicon/BlackWindow.png");
	IKESprite::LoadTexture(GIRL, L"Resources/2d/objicon/40E32961-2FAD-4C47-B412-6CAD5C0BFEE2.png");
	IKESprite::LoadTexture(SUTOPON, L"Resources/2d/objicon/sutopon.png");
	IKESprite::LoadTexture(FEED, L"Resources/2d/sceneback/feedwhite.png");
	IKESprite::LoadTexture(PLAY, L"Resources/2d/sceneback/game.png");

}

//非同期処理で読み込むスプライト
void ImageManager::SecondLoad2D()
{
	IKESprite::LoadTexture(GAMEOVER, L"Resources/2d/sceneback/over.png");
	IKESprite::LoadTexture(GAMECLEAR, L"Resources/2d/sceneback/clear.png");
	IKESprite::LoadTexture(MASTERCLEAR, L"Resources/2d/sceneback/lastclear.png");
	IKESprite::LoadTexture(BOX, L"Resources/2d/objicon/box.png");
	IKESprite::LoadTexture(CUBE, L"Resources/2d/objicon/cube.png");
	IKESprite::LoadTexture(NON, L"Resources/2d/objicon/non.png");
	IKESprite::LoadTexture(HEART, L"Resources/2d/objicon/PlayerHeart.png");
	IKESprite::LoadTexture(UnderGauge, L"Resources/2d/objicon/UnderGauge.png");
	IKESprite::LoadTexture(Gauge, L"Resources/2d/objicon/Gauge.png");
	IKESprite::LoadTexture(TopGauge, L"Resources/2d/objicon/TopGauge.png");

	IKESprite::LoadTexture(MENU_HOME, L"Resources/2d/sceneback/MenuH.png");
	IKESprite::LoadTexture(MENU_RESET, L"Resources/2d/sceneback/MenuT.png");
	IKESprite::LoadTexture(MENU_RULE, L"Resources/2d/sceneback/MenuR.png");
	IKESprite::LoadTexture(MENU_SCENECHANGE, L"Resources/2d/sceneback/MenuS.png");
	IKESprite::LoadTexture(MENU_FRAME, L"Resources/2d/sceneback/TaskFrame.png");
	IKESprite::LoadTexture(EXPLANATION, L"Resources/2d/sceneback/Explanation.png");
	IKESprite::LoadTexture(CLOSESYTOPON, L"Resources/2d/objicon/closeSutopon.png");

	IKESprite::LoadTexture(PHOTO_IN, L"Resources/2d/effect/camera_soto.png");
	IKESprite::LoadTexture(PHOTO_OUT, L"Resources/2d/effect/camera_naka.png");
	IKESprite::LoadTexture(CIRCLE, L"Resources/2d/objicon/circle.png");
	IKESprite::LoadTexture(CIRCLECOVER, L"Resources/2d/objicon/circlecover.png");
}

//テクスチャの読み込み
void ImageManager::StartLoadTex2D()
{
}

//非同期処理でのテクスチャの読み込み
void ImageManager::SecondLoadTex2D()
{
	IKETexture::LoadTexture(IMPACT, L"Resources/2d/effect/impact.png");
	IKETexture::LoadTexture(IMPACT2, L"Resources/2d/effect/inpact2.png");
	IKETexture::LoadTexture(ANGER, L"Resources/2d/objicon/AngerStamp.png");
	IKETexture::LoadTexture(JOY, L"Resources/2d/objicon/JoyStamp.png");
	IKETexture::LoadTexture(MARK, L"Resources/2d/objicon/box.png");
	IKETexture::LoadTexture(DAMAGEAREA, L"Resources/2d/effect/DArea.png");
	IKETexture::LoadTexture(GROUND, L"Resources/2d/SceneBack/Ground.png");
	IKETexture::LoadTexture(PHOTOSPOT, L"Resources/2d/effect/PhotoSpot.png");
}

void ImageManager::LoadParticle()
{
	ParticleManager::LoadTexture(Normal, "effect1");
	ParticleManager::LoadTexture(Smoke, "hootEffect");
	ParticleManager::LoadTexture(Heal, "effect2");
}
