#include "SceneFactory.h"
#include "FirstStage.h"
#include "SecondStage.h"
#include "ThirdStage.h"
#include "FourthStage.h"
#include "TitleScene.h"
#include "IntroductionScene.h"
#include "ClearScene.h"
#include "GameoverScene.h"
#include "LoadScene.h"
#include "EditorScene.h"
#include "TutorialScene.h"
#include"SelectStageScene.h"
BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンの作成
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	else if (sceneName == "INTRODUCTION") {
		newScene = new IntroductionScene();
	}
	else if (sceneName == "LOAD") {
		newScene = new LoadScene();
	}
	else if (sceneName == "TUTORIAL") {
		newScene = new TutorialScene();
	}
	else if(sceneName == "FIRSTSTAGE"){
		newScene = new FirstStage();
	}
	else if (sceneName == "SECONDSTAGE") {
		newScene = new SecondStage();
	}
	else if (sceneName == "THIRDSTAGE") {
		newScene = new ThirdStage();
	}
	else if (sceneName == "FOURTHSTAGE") {
		newScene = new FourthStage();
	}
	else if (sceneName == "EDITORSCENE") {
		newScene = new EditorScene();
	}
	else if (sceneName == "GAMEOVER") {
		newScene = new GameoverScene();
	}
	else if (sceneName == "GAMECLEAR") {
		newScene = new ClearScene();
	}
	else if (sceneName == "SELECT") {
		newScene = new SelectStageScene();
	}
	return newScene;
}
