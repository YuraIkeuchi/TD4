#pragma once
#include"IKESprite.h"
#include<memory>
#include<array>
#include<vector>
#include<DirectXMath.h>

using namespace DirectX;
using namespace std;
class Menu
{
public:
	static Menu* GetIns();

	enum Task
	{
		NON,
		Home,
		ResetBattle,
		RuleExp,
		MapChange
	}_tasks;


private:
	bool MenuOpenF;

	struct Texparam
	{
		unique_ptr<IKESprite>Sprite;
		XMFLOAT2 Scl;
		XMFLOAT2 Pos;
		bool ActFlag;
		float EaseFrame;
	};

	static constexpr float SizeY=250.f;
	//�^�X�N��
	static constexpr int taskNum = 5;

	array<Texparam, taskNum>tasks;

	/**
	 * \brief �O�g
	 */
	unique_ptr<IKESprite>frame;
	/**
	 * \brief �A�C�R��
	 */
	unique_ptr<IKESprite>closeTex;
	float closeScl = 6500.f;
	float closeRad=1500.f;
	bool k;
	bool sin = false;;
	float SclingSpeed=55.f,CorrSpeed=0.48f;
	int MenuResetTimer = 0;
	int MenuRestartTimer = 0;
	int CoolTime;
	/**
	 * \brief ��������e�N�X�`��
	 */
	array<unique_ptr<IKESprite>,3>ExpTex;
	array<float,3> ExpEaseFrame;
	array<float, 3> ReExpEaseFrame;
	array<bool, 3>nextExp;
	array<bool, 3>removeExp;
	/**
	 * \brief �g
	 */
	static constexpr float YInter[taskNum - 1] = { 400,700,1000 };
	int index=0;

	std::string SceneName;

	bool CloseF=false;

	int OpenHomeCount=0;

	bool trigger;
public:
	//������
	void Init();
	//�X�V
	void Upda();
	//�`��
	void Draw();

	void CloseIconView(bool closeF);
	//�ǂ��̉�ʉ�����t���O
	bool GetTaskActFlag(Task task) { return tasks[task].ActFlag; }
	//�|�X�g�G�t�F�N�g�p
	float GetCloseIconRad() { return closeRad; }
	//
	bool GetMenuOpen() { return MenuOpenF; }
	void SetSceneName(std::string name) { SceneName = name; }
private:
	/*�e�^�X�N�J�������ݒ�*/
	inline bool Open_Home() { if (tasks[Home].ActFlag)return true; return false; }
	inline bool Open_Rule(){ if (tasks[RuleExp].ActFlag)return true; return false; }
	inline bool Open_Reset(){ if (tasks[ResetBattle].ActFlag)return true; return false; }

private:
	void Non();
	void Home_();
	void RuleExp_();
	void ResetBattle_();
	void SceneChange();

	void ResetAllTask();
private:
	static void (Menu::* stateTable[])();
};

