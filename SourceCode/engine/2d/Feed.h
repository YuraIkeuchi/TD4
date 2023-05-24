#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
using namespace DirectX;
class Feed
{
public:
	Feed();
private:
	std::unique_ptr<IKESprite>FeedTex;
	float m_Alpha;
	bool m_FeedFlag;

	XMFLOAT3 Color_RGB;
	int FeedInCounter;
private:
	enum class FeedPhase
	{
		NON,
		FEEDIN,
		FEEDOUT
	}_phase;
public:
	enum class FeedType
	{
		BLACK,
		WHITE
	}_color;

	//type:�t�F�[�h�̐F(������) feedspeed:�t�F�[�h�̃X�s�[�h feedf:�t�F�[�h�J�n�t���O
	void FeedIn(FeedType type,float feedspeed,bool &feedf);
	void Draw();

	inline bool GetFeedEnd() { if (_phase == FeedPhase::NON)return true; return false; }
};

