#include "ImageManager.h"

ImageManager* ImageManager::GetInstance()
{
	static ImageManager instans;
	return &instans;
}

//スプライトの読み込み
void ImageManager::StartLoad2D()
{
	IKESprite::LoadTexture(TITLE, L"Resources/2d/sceneback/title.png");
	IKESprite::LoadTexture(LOADING, L"Resources/2d/objicon/box.png");
	IKESprite::LoadTexture(WHITE, L"Resources/2d/effect/white1x1.png");
	IKESprite::LoadTexture(WINDOW, L"Resources/2d/objicon/ConversationWindow.png");
}

//非同期処理で読み込むスプライト
void ImageManager::SecondLoad2D()
{
	IKESprite::LoadTexture(PLAY, L"Resources/2d/sceneback/game.png");
	IKESprite::LoadTexture(GAMEOVER, L"Resources/2d/sceneback/over.png");
	IKESprite::LoadTexture(GAMECLEAR, L"Resources/2d/sceneback/clear.png");

	IKESprite::LoadTexture(BOX, L"Resources/2d/objicon/box.png");
	IKESprite::LoadTexture(CUBE, L"Resources/2d/objicon/cube.png");
	IKESprite::LoadTexture(NON, L"Resources/2d/objicon/non.png");
}

//テクスチャの読み込み
void ImageManager::StartLoadTex2D()
{
}

//非同期処理でのテクスチャの読み込み
void ImageManager::SecondLoadTex2D()
{
}

void ImageManager::LoadParticle()
{
	ParticleManager::LoadTexture(Normal, "effect1");
	ParticleManager::LoadTexture(Smoke, "hootEffect");
}
