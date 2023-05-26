﻿#pragma once
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
		LOADING_LO,
		LOADING_O,
		LOADING_DI,
		LOADING_N,
		LOADING_G,
		PLAY,
		GAMEOVER,
		GAMECLEAR,
		BOX,
		CUBE,
		NON,
		WHITE,
		WINDOW,
		BLACKWINDOW,
		HEART,
		UnderGauge,
		Gauge,
		TopGauge,
		GIRL,
		SUTOPON,
		FEED
	};
	//テクスチャの名前
	enum tex2dName
	{
		IMPACT,
		IMPACT2,
		ANGER,
		JOY,
		MARK,
	
	};
	enum particleName
	{
		Normal,
		Smoke
	};

public:
	static ImageManager* GetInstance(); //取得用
	void StartLoad2D();
	void SecondLoad2D();
	void StartLoadTex2D();
	void SecondLoadTex2D();
	void LoadParticle();
};
