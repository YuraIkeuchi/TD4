#pragma once
#include <map>
#include <string>
#include "IKESprite.h"
#include "IKETexture.h"
#include "ParticleManager.h"

//2Dの読み込み処理
class ImageManager final
{
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager& r) = default;
	ImageManager& operator=(const ImageManager& r) = default;

public:
	//スプライトの名前
	enum texName
	{
		TITLE,
		TITLEWORD,
		LOADING_LO,
		LOADING_O,
		LOADING_DI,
		LOADING_N,
		LOADING_G,
		PLAY,
		GAMEOVER,
		GAMEOVER_1,
		GAMEOVER_2,
		GAMEOVER_3,
		GAMECLEAR,
		MASTERCLEAR,
		BOX,
		CUBE,
		NON,
		WHITE,
		WINDOW,
		BLACKWINDOW,
		HEART,
		PlayerHPGauge,
		BossHPGauge,
		UnderGauge,
		Gauge,
		TopGauge,
		GIRL,
		SUTOPON,
		FEED,
		MENU_HOME,
		MENU_RESET,
		MENU_RULE,
		MENU_SCENECHANGE,
		MENU_FRAME,
		EXPLANATION,
		CLOSESYTOPON,
		CLOSEKIDO,
		CLOSEDJ,
		CLOSECAMERA,
		PHOTO_IN,
		PHOTO_IN_Change,
		PHOTO_IN_Control,
		PHOTO_IN_Sub,
		PHOTO_IN_Ult,
		PHOTO_OUT,
		CIRCLE,
		CIRCLECOVER,
		SELECT_FIRST,
		SELECT_SECOND,
		SELECT_THIRD,
		SELECT_FOUR,
		SELECT_FIVE,
		SELECT_SIX,
		SELECT_SEVEN,
		RBBUTTON,
		LBBUTTON,
		tip1,
		Challenge,
		Cancel,
		BOSS_ARROW,
		NOTE,
		NOTE2,
		DARKBOM,

	};
	//テクスチャの名前
	enum tex2dName
	{
		IMPACT,
		IMPACT2,
		ANGER,
		JOY,
		MARK,
		DAMAGEAREA,
		DAMAGEAREA2,
		GROUND,
		PHOTOSPOT,
		STUN,
		CATCH,
		SLASHAREA
	};
	enum particleName
	{
		Normal,
		Smoke,
		Heal,
		Photo,

	};

public:
	static ImageManager* GetInstance(); //取得用
	void StartLoad2D();
	void SecondLoad2D();
	void StartLoadTex2D();
	void SecondLoadTex2D();
	void LoadParticle();
};
