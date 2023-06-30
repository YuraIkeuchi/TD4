#include "Menu.h"

#include "Easing.h"
#include "Helper.h"
#include "ImageManager.h"
#include"Input.h"
#include"imgui.h"
#include "SceneManager.h"

Menu* Menu::GetIns()
{
	static Menu ins;
	return &ins;
}

#define WINW 1280
#define WINH 720
void Menu::Init()
{
	tasks[Home].Sprite = IKESprite::Create(ImageManager::MENU_HOME,tasks[Home].Pos);
	
	tasks[RuleExp].Sprite = IKESprite::Create(ImageManager::MENU_RULE, tasks[Home].Pos);


	tasks[ResetBattle].Sprite = IKESprite::Create(ImageManager::MENU_RESET, tasks[Home].Pos);
	
	tasks[MapChange].Sprite = IKESprite::Create(ImageManager::MENU_SCENECHANGE, tasks[Home].Pos);
	
	frame = IKESprite::Create(ImageManager::MENU_FRAME, tasks[Home].Pos);
	
	ExpTex[0] = IKESprite::Create(ImageManager::EXPLANATION, tasks[Home].Pos);
	ExpTex[1] = IKESprite::Create(ImageManager::CLOSEKIDO, tasks[Home].Pos);
	ExpTex[2] = IKESprite::Create(ImageManager::CLOSECAMERA, tasks[Home].Pos);

	//ExpTex->SetAnchorPoint({ 0.5f,0.5f });
	closeTex = IKESprite::Create(ImageManager::CLOSESYTOPON, tasks[Home].Pos);
}

void Menu::Upda()
{
	//状態移行
	(this->*stateTable[_tasks])();
	
	//各タスクテクスチャのパラメータ設定
	for (auto i = 1; i < tasks.size(); i++) {
		tasks[i].Sprite->SetSize(tasks[i].Scl);
		tasks[i].Sprite->SetPosition(tasks[i].Pos);
	}

	
	if (!sin&&_tasks==MapChange)
	{
		CloseF = true;
		if (closeScl <= 0.f) {
			sin = true;
			SceneManager::GetInstance()->ChangeScene("TITLE");
			_tasks = NON;
		}
	}
	if (closeScl >= 10000.f) {
		CloseF = false;
		sin = false;
	}

	CloseIconView(CloseF);
	Helper::GetInstance()->Clamp(closeScl, 0.f, 12500.f);
	Helper::GetInstance()->Clamp(closeRad, 0.f, 1500.f);

	closeTex->SetAnchorPoint({ 0.5f,0.5f });
	closeTex->SetSize({ closeScl,closeScl });
	closeTex->SetPosition({ WINW / 2,WINH / 2 });
}

void (Menu::* Menu::stateTable[])() = {
	&Menu::Non,
	&Menu::Home_,//ホーム
	
	& Menu::ResetBattle_,//最初から
	&Menu::RuleExp_,//ルール説明
	&Menu::SceneChange,
};

void Menu::Draw()
{
	if (_tasks != NON) {
		tasks[Home].Sprite->Draw();
		if(tasks[2].Sprite->GetSize().x>290.f)
		frame->Draw();

		for (auto i = 2; i < tasks.size(); i++)
		{
			tasks[i].Sprite->Draw();
		}
		for(auto i=0;i<3;i++)
		ExpTex[i]->Draw();
	}
	if (_tasks == MapChange||sin)
	closeTex->Draw();
}

void Menu::Home_()
{
	OpenHomeCount++;
	if(OpenHomeCount>30)
	{
		if(Input::GetInstance()->TriggerButton(Input::START))
		{
			_tasks = NON;
		}
	}

	tasks[RuleExp].ActFlag = false;
	ExpEaseFrame[0] = 0.f;
	ExpEaseFrame[1] = 0.f;
	ExpEaseFrame[2] = 0.f;
	//外枠移動処理
	if (!trigger && Input::GetInstance()->TiltPushStick(Input::L_RIGHT, 0.0f))
	{
		index++;
		trigger = true;
	}
	if (!trigger && Input::GetInstance()->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		index--;
		trigger = true;
	}
	Helper::GetInstance()->Clamp(index, 0, 2);
	if(!Input::GetInstance()->TiltPushStick(Input::L_LEFT, 0.0f)&&
		!Input::GetInstance()->TiltPushStick(Input::L_RIGHT, 0.0f))
	{
		trigger = false;

	}
		frame->SetPosition({YInter[index],300.f });
	frame->SetSize({ 300,180 });



	if (Helper::GetInstance()->FrameCheck(tasks[Home].EaseFrame, 0.02f)) {

		for(auto i=2;i<tasks.size();i++)
		{
			Helper::GetInstance()->FrameCheck(tasks[i].EaseFrame, 0.02f);
			tasks[i].Scl.x = Ease(In, Cubic, tasks[i].EaseFrame, 0,300);
			tasks[i].Scl.y = Ease(In, Cubic, tasks[i].EaseFrame, 0, 180);


			frame->SetSize({ 300,180 });
			if(i>=tasks.size()-1)
				frame->SetSize({ 300,180 });
		}
	}
	tasks[Home].Scl.x = Ease(In, Cubic, tasks[Home].EaseFrame,0,1280);
	tasks[Home].Scl.y = Ease(In, Cubic, tasks[Home].EaseFrame, 0, 780);


	if (Input::GetInstance()->TriggerButton(Input::B))
	{
		if (index == 0)_tasks = RuleExp;
		else if (index == 2)_tasks = MapChange;
	}
	
	tasks[Home].ActFlag = true;
}

void Menu::ResetBattle_()
{
	//if (!Open_Reset())return;


	tasks[ResetBattle].ActFlag = true;

}

void Menu::RuleExp_()
{

	if (Input::GetInstance()->TriggerButton(Input::START))_tasks = NON;

		if (Input::GetInstance()->TriggerButton(Input::A))
		{
			ExpEaseFrame[0] = 0;
			ExpEaseFrame[1] = 0;
			ExpEaseFrame[2] = 0;
			tasks[RuleExp].ActFlag = true;
		}
	if(!tasks[RuleExp].ActFlag)
	{
		Helper::GetInstance()->FrameCheck(ExpEaseFrame[0], 0.05f);
		ExpTex[0]->SetPosition({Ease(In, Cubic, ExpEaseFrame[0], WINW, 0),0});

		if (!removeExp[1]&& ExpTex[0]->GetPosition().x <= 0.f)
		{

			ReExpEaseFrame[1] = 0;
			if (Input::GetInstance()->TiltPushStick(Input::L_RIGHT, 0.0f))
				nextExp[1] = true;

			if (nextExp[1])
			{
				Helper::GetInstance()->FrameCheck(ExpEaseFrame[1], 0.05f);
				ExpTex[1]->SetPosition({ Ease(In, Cubic, ExpEaseFrame[1], WINW, 0),0 });
			}
		}
		if (!removeExp[2] && ExpTex[1]->GetPosition().x <= 0.f)
		{
			ReExpEaseFrame[2] = 0;

			if (Input::GetInstance()->TiltPushStick(Input::L_RIGHT, 0.0f))
				nextExp[2] = true;

			if (nextExp[2])
			{
				Helper::GetInstance()->FrameCheck(ExpEaseFrame[2], 0.05f);
				ExpTex[2]->SetPosition({ Ease(In, Cubic, ExpEaseFrame[2], WINW, 0),0 });
			}
			
		}


		if (Input::GetInstance()->TiltPushStick(Input::L_LEFT, 0.0f)) {
			if (ExpTex[2]->GetPosition().x>=WINW&& ExpTex[1]->GetPosition().x <= 0.f)
				removeExp[1] = true;
			if (ExpTex[2]->GetPosition().x <= 0.f)
				removeExp[2] = true;

			for (auto i = 0; i < 3; i++) {
				if(i>0)ExpEaseFrame[i] = 0;
				nextExp[i] = false;
			}
		}

		for (auto i = 0; i < 3; i++) {
			
					if (removeExp[i])
				{
					Helper::GetInstance()->FrameCheck(ReExpEaseFrame[i], 0.05f);
					ExpTex[i]->SetPosition({ Ease(In, Cubic, ReExpEaseFrame[i], 0,WINW) ,0});
				}
			
			if (ExpTex[i]->GetPosition().x >= WINW)
			{
				removeExp[i] = false;
			}
		}
	}
	else
	{
		for (auto i = 0; i < 3; i++) {
			Helper::GetInstance()->FrameCheck(ExpEaseFrame[i], 0.05f);
			ExpTex[i]->SetPosition({ Ease(In, Cubic, ExpEaseFrame[i],0, WINW),0});
		}
			if (ExpEaseFrame[0] >= 1.f)_tasks = Home;
	}
	

}


void Menu::SceneChange()
{

	tasks[MapChange].ActFlag = true;
}


void Menu::Non()
{
	//ホーム画面へ
	if (Input::GetInstance()->TriggerButton(Input::START))_tasks = Home;


	ResetAllTask();
}


void Menu::ResetAllTask()
{
	{
		OpenHomeCount = 0;

		tasks[Home].Sprite->SetAnchorPoint({ 0.5f,0.5f });
		tasks[Home].Sprite->SetColor({ 1,1,1,0.8f });
		tasks[Home].Pos = { WINW / 2,WINH / 2 };

		tasks[RuleExp].Sprite->SetAnchorPoint({ 0.5f,0.5f });
		tasks[RuleExp].Pos = { YInter[0],300.f};

		tasks[ResetBattle].Sprite->SetAnchorPoint({ 0.5f,0.5f });
		tasks[ResetBattle].Pos = { YInter[1],300.f};

		tasks[MapChange].Sprite->SetAnchorPoint({ 0.5f,0.5f });
		tasks[MapChange].Pos = { YInter[2],300.f};

		frame->SetAnchorPoint({ 0.5f,0.5f });
		frame->SetPosition({ YInter[0],300.f });
		frame->SetSize({ 0,0 });

		for (auto i = 0; i < 3; i++) {
			ExpTex[i]->SetSize({WINW + 100,WINH});
			ExpTex[i]->SetPosition({WINW,0.f});
		}
		//操作説明関連初期化
		tasks[RuleExp].ActFlag = false;
		ExpEaseFrame[0] = 0;
		ExpEaseFrame[1] = 0.f;
		ExpEaseFrame[2] = 0.f;
		//
		for (auto i = 1; i < tasks.size(); i++)
		{
			tasks[i].ActFlag = false;
			tasks[i].EaseFrame = 0.f;
			tasks[i].Scl = { 0,0 };
		}

		index = 0;
		//
	}
}


void Menu::CloseIconView(bool closeF)
{
	//定数わっしょい　良い方法模索中。。。
	constexpr float texScl = 6500.f;
	constexpr float MinScl = 2000.f;
	constexpr float SubRad = 0.48f;
	
	
	if (closeF&&!sin) {
		closeScl -= SclingSpeed;
		if (closeScl <= MinScl) {
			SclingSpeed = 55.f;
			closeRad -= SclingSpeed * SubRad;
		}
		else
			SclingSpeed = 100.f;
	}
	if(sin)
	{
		if (closeScl >= MinScl) 
			SclingSpeed = 180.f;
		else
			SclingSpeed = 100.f;
		
		closeScl += SclingSpeed;
		closeRad += SclingSpeed * SubRad;
	}
	

}
