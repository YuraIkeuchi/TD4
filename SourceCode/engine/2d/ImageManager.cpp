﻿#include "ImageManager.h"

ImageManager* ImageManager::GetInstance()
{
	static ImageManager instans;
	return &instans;
}

//スプライトの読み込み
void ImageManager::StartLoad2D()
{IKESprite::LoadTexture(FEED, L"Resources/2d/sceneback/feedwhite.png");
	
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
	IKESprite::LoadTexture(PLAY, L"Resources/2d/sceneback/game.png");

}

//非同期処理で読み込むスプライト
void ImageManager::SecondLoad2D()
{
	IKESprite::LoadTexture(GAMEOVER, L"Resources/2d/sceneback/over.png");
	IKESprite::LoadTexture(GAMEOVER_1, L"Resources/2d/sceneback/over_1.png");
	IKESprite::LoadTexture(GAMEOVER_2, L"Resources/2d/sceneback/over_2.png");
	IKESprite::LoadTexture(GAMEOVER_3, L"Resources/2d/sceneback/over_3.png");

	IKESprite::LoadTexture(GAMECLEAR, L"Resources/2d/sceneback/clear.png");
	IKESprite::LoadTexture(MASTERCLEAR, L"Resources/2d/sceneback/lastclear.png");
	IKESprite::LoadTexture(CUBE, L"Resources/2d/objicon/cube.png");
	IKESprite::LoadTexture(NON, L"Resources/2d/objicon/non.png");
	IKESprite::LoadTexture(HEART, L"Resources/2d/objicon/PlayerHeart.png");
	IKESprite::LoadTexture(PlayerHPGauge, L"Resources/2d/objicon/playerHPGauge.png");
	IKESprite::LoadTexture(BossHPGauge, L"Resources/2d/objicon/bossHPGauge.png");

	IKESprite::LoadTexture(UnderGauge, L"Resources/2d/objicon/hangarGauge.png");
	IKESprite::LoadTexture(Gauge, L"Resources/2d/objicon/Gauge.png");
	IKESprite::LoadTexture(TopGauge, L"Resources/2d/objicon/TopGauge.png");
	IKESprite::LoadTexture(EXGauge, L"Resources/2d/objicon/EXGauge.png");
	IKESprite::LoadTexture(MENU_HOME, L"Resources/2d/sceneback/MenuH.png");
	IKESprite::LoadTexture(MENU_RESET, L"Resources/2d/sceneback/MenuT.png");
	IKESprite::LoadTexture(MENU_RULE, L"Resources/2d/sceneback/MenuR.png");
	IKESprite::LoadTexture(MENU_SCENECHANGE, L"Resources/2d/sceneback/MenuS.png");
	IKESprite::LoadTexture(MENU_FRAME, L"Resources/2d/sceneback/TaskFrame.png");
	IKESprite::LoadTexture(EXPLANATION, L"Resources/2d/sceneback/Explanation.png");
	IKESprite::LoadTexture(CLOSESYTOPON, L"Resources/2d/objicon/closeSutopon.png");
	IKESprite::LoadTexture(CLOSEKIDO, L"Resources/2d/objicon/closekido.png");
	IKESprite::LoadTexture(CLOSEDJ, L"Resources/2d/objicon/closeDJ.png");
	IKESprite::LoadTexture(PHOTO_IN, L"Resources/2d/effect/camera_soto.png");
	IKESprite::LoadTexture(PHOTO_IN_Change, L"Resources/2d/effect/camera_soto_Chan.png");
	IKESprite::LoadTexture(PHOTO_IN_Control, L"Resources/2d/effect/camera_soto_Cont.png");
	IKESprite::LoadTexture(PHOTO_IN_Sub, L"Resources/2d/effect/camera_soto_sub.png");
	IKESprite::LoadTexture(PHOTO_IN_Ult, L"Resources/2d/effect/camera_soto_Ult.png");

	IKESprite::LoadTexture(PHOTO_OUT, L"Resources/2d/effect/camera_naka.png");
	IKESprite::LoadTexture(CIRCLE, L"Resources/2d/objicon/circle.png");
	IKESprite::LoadTexture(CIRCLECOVER, L"Resources/2d/objicon/circlecover.png");
	IKESprite::LoadTexture(CLOSECAMERA, L"Resources/2d/objicon/closecamera.png");

	IKESprite::LoadTexture(LBBUTTON, L"Resources/2d/objicon/LB.png");
	IKESprite::LoadTexture(RBBUTTON, L"Resources/2d/objicon/RB.png");
	IKESprite::LoadTexture(tip1, L"Resources/2d/sceneback/tips1.png");
	IKESprite::LoadTexture(Challenge, L"Resources/2d/objicon/Challenge.png");
	IKESprite::LoadTexture(Cancel, L"Resources/2d/objicon/Cancel.png");


	IKESprite::LoadTexture(BOSS_ARROW, L"Resources/2d/objicon/Arrow.png");
	IKESprite::LoadTexture(NOTE, L"Resources/2d/objicon/onp.png");
	IKESprite::LoadTexture(NOTE2, L"Resources/2d/objicon/onp2.png");

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
	IKETexture::LoadTexture(DAMAGEAREA, L"Resources/2d/effect/DArea.png");
	IKETexture::LoadTexture(DAMAGEAREA2, L"Resources/2d/effect/DAreaDArea2.png");
	IKETexture::LoadTexture(GROUND, L"Resources/2d/SceneBack/Ground.png");
	IKETexture::LoadTexture(PHOTOSPOT, L"Resources/2d/effect/PhotoSpot.png");


	IKETexture::LoadTexture(SELECT_FIRST, L"Resources/2d/objicon/firstboss.png");
	IKETexture::LoadTexture(SELECT_SECOND, L"Resources/2d/objicon/secondboss.png");
	IKETexture::LoadTexture(SELECT_THIRD, L"Resources/2d/objicon/thirdboss.png");
	IKETexture::LoadTexture(SELECT_FOUR, L"Resources/2d/objicon/fourboss.png");
	IKETexture::LoadTexture(SELECT_FIVE, L"Resources/2d/objicon/fiveboss.png");
	IKETexture::LoadTexture(SELECT_SIX, L"Resources/2d/objicon/sixboss.png");
	IKETexture::LoadTexture(SELECT_SEVEN, L"Resources/2d/objicon/sevenboss.png");
//	IKETexture::LoadTexture(PHOTOSPOT, L"Resources/2d/objicon/sixboss.png");

	IKETexture::LoadTexture(STUN, L"Resources/2d/effect/stun.png");
	IKETexture::LoadTexture(CATCH, L"Resources/2d/effect/effect1.png");
	IKETexture::LoadTexture(SLASHAREA, L"Resources/2d/effect/DArea3.png");
	IKETexture::LoadTexture(DARKBOM, L"Resources/2d/objicon/missileicon.png");
}

void ImageManager::LoadParticle()
{
	ParticleManager::LoadTexture(Normal, "effect1.png");
	ParticleManager::LoadTexture(Smoke, "hootEffect.png");
	ParticleManager::LoadTexture(Heal, "effect2.png");
	ParticleManager::LoadTexture(Photo, "PhotoSpot.jpg");
}
