#pragma once
#include "PlaceManager.h"

class PlaceBox:public PlaceManager
{
public:
	void Initialize() override;

	void Update() override;

	void Draw(DirectXCommon* DxCommon) override;

	void ImGui_Draw() override;

	void FileWriting() override;

private:
	std::vector<std::unique_ptr<IKEObject3d>> boxes;

	bool StoneArgment;
	bool OtherArgment;

	std::vector<bool> SelThis;
public:

	class CreateImGui
	{
	public:
		CreateImGui(std::string num, float scl, XMFLOAT3 pos);
		void CreateImguiList();
	private:
		std::vector<std::string> listnum;

		float CreateScl;
		XMFLOAT3 CreatePos;
		XMFLOAT3 Pos;
		XMFLOAT3 Rot;
		float Scl;
		XMFLOAT4 Col;
		bool Del;
		bool Select;
		int imnumber;
		std::string modelname;
	public:
		float GetScl() { return Scl; }
		XMFLOAT3 GetPos() { return Pos; }
		bool GetDelF() { return Del; }
		bool GetSelectThis() { return Select; }
		void SetModelName(std::string name) { modelname=name; }
		void SetImguiNumber(int index) { imnumber = index; }
		int GetImguiNumber() { return imnumber; }
	public:
		enum BoxNumber
		{
			NON,
			WOOD,
			STONE,
			OTHER
		} bnumber;


		BoxNumber GetBoxnumber() { return bnumber; }
	};

	std::vector<std::unique_ptr<CreateImGui>> imguilist;

};

