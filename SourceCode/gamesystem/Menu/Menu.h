#pragma once
#include <list>
#include <memory>
#include <IKESprite.h>

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
	enum {
		BackScreen,
		ConfirmButton,
		SceneBackButton,
		Confirm,
		SceneBack,
		SpriteMax,
	};
	std::list<std::unique_ptr<IKESprite>> sprites_ = {};



	std::unique_ptr<IKESprite> CreateSprite(UINT num);
};
