#pragma once
#include <array>
#include <memory>
#include <IKESprite.h>
#include<DirectXMath.h>

using namespace DirectX;
using namespace std;
#define window_Width 1280.0f
#define window_Height 720.0f
#define half_Width 640.0f
#define half_Height 360.0f

class Menu {
public:
	Menu();
	~Menu();

	void Initialize();
	void Update();
	void Draw();
private:
	enum class State :int{
		NONE = 0,
		OPEN,
		SELECT,//セレクト
		CHECK,//最終確認
		CONFIRM,//操作方法
		CLOSE
	}_state = State::NONE;
	//関数ポインタ
	static void(Menu::* stateTable[])();
	//メンバ関数
	void NoneUpdate();
	void OpenUpdate();
	void SelectUpdate();
	void CheckUpdate();
	void ConfirmUpdate();
	void CloseUpdate();
private:
	struct SpriteInfo {
		unique_ptr<IKESprite> sprite=nullptr;
		XMFLOAT2 size = {};
		XMFLOAT2 pos = {};
		XMFLOAT2 start_size = {};
		XMFLOAT2 end_size = {};
		bool isVisible = false;
		bool isAction = false;
		float easingFrame=0.0f;
		float kFrameMax = 60.0f;
	};
	

	enum {
		BackScreen,
		ConfirmButton,
		SceneBackButton,
		ReturnButton,
		SutoponBar,
		//Confirm_FIRST,
		//Confirm_SECOND,
		//Confirm_THIRD,
		//SceneBack,
		SpriteMax,
	};
	std::array<SpriteInfo, SpriteMax> sprites_;

	std::array<XMFLOAT2, 3> buttonPos = {
	XMFLOAT2(half_Width - 320.f,half_Height),
	XMFLOAT2(half_Width + 320.f,half_Height),
	XMFLOAT2(half_Width,half_Height + 180)
	};

	SpriteInfo CreateSprite(UINT num,XMFLOAT2 pos,float easingFrame=60.0f);
};
