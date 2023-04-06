#include "SceneFactory.h"
#include "FirstStage.h"
#include "TitleScene.h"
#include "IntroductionScene.h"
#include "ClearScene.h"
#include "GameoverScene.h"
#include "LoadScene.h"
#include "EditorScene.h"
BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//éüÇÃÉVÅ[ÉìÇÃçÏê¨
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
	else if(sceneName == "FIRSTSTAGE"){
		newScene = new FirstStage();
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
	return newScene;
}
