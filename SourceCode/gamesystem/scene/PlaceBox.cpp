#include "PlaceBox.h"

#include "CollisionAttribute.h"
#include "ModelManager.h"
#include "SphereCollider.h"

void PlaceBox::Initialize()
{
	SampleObj = std::make_unique<IKEObject3d>();
	SampleObj->Initialize();
	SampleObj->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Box));


	float radius = 5.0f;
	SampleObj->SetCollider(new SphereCollider(XMVECTOR({ 10, radius, 10, 0 }), radius));
	SampleObj->collider->SetAttribute(COLLISION_ATTR_ALLIES);
}


void PlaceBox::Update()
{
	SampleObj->SetScale({ 5.0f,5.0f,5.0f });
	SampleObj->SetPosition(S_Pos);

	SampleObj->Update();

	SampleObj->CollisionField();
	SampleObj->SetColor({ 1.f,1.f,1.f,0.5f });

	if (ArgmentFlag) {

		ArgmentObj(ArgmentFlag, ModelManager::GetInstance()->GetModel(ModelManager::Box));
		
	}
	for (auto i = 0; i < boxes.size(); i++)
	{
		if (boxes[i] == nullptr)continue;
		boxes[i]->SetColor({ 1.f,1.f,1.f,1.f });
		boxes[i]->SetScale({ 5.0f,5.0f,5.0f });
		boxes[i]->Update();
		boxes[i]->CollisionField();
	}
	if (StoneArgment) {
		SampleObj->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Cube));
		std::unique_ptr<IKEObject3d> newobj;
		newobj = std::make_unique<IKEObject3d>();

		newobj->Initialize();
		newobj->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Cube));

		newobj->SetPosition(S_Pos);
		float radius = 5.0f;
		newobj->SetCollider(new SphereCollider(XMVECTOR({ 10, radius, 10, 0 }), radius));
		newobj->collider->SetAttribute(COLLISION_ATTR_ALLIES);
		boxes.push_back(std::move(newobj));

		std::unique_ptr<CreateImGui> newlist;
		newlist = std::make_unique<CreateImGui>(std::to_string(static_cast<int>(boxes.size())), 0.02f, S_Pos);
		newlist->SetModelName("CUBE");
		newlist->SetImguiNumber(static_cast<int>(boxes.size() - 1));

		imguilist.push_back(std::move(newlist));
		SelThis.push_back(false);


		StoneArgment = false;
	}

	
}

void PlaceBox::Draw(DirectXCommon* DxCommon)
{
	SampleObj->Draw();
	for (auto i = 0; i < boxes.size(); i++)
	{
		boxes[i]->Draw();
	}
}

void PlaceBox::FileWriting()
{
	if (!LoadFlag)return;

	file.open("stage1.csv");
	popcom << file.rdbuf();

	file.close();
	//std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Resources/csv/stage/stage1.csv"); // ファイルパスを指定する
	ofs << "Wood_Quantity" << "," << boxes.size() << std::endl;

	for (int i = 0; i < boxes.size(); i++)
	{
		//ofs << "Number" << "," << Number[i] << std::endl;
		ofs << "POSITION" << "," << boxes[i]->GetPosition().x
			<< "," << boxes[i]->GetPosition().y
			<< "," << boxes[i]->GetPosition().z << std::endl;
	}
}

void PlaceBox::ImGui_Draw()
{
	

	ImGui::Begin("BoxParam");

	ImGui::SetWindowPos(ImVec2(0, 400));
	ImGui::SetWindowSize(ImVec2(300, 300));
	if (ImGui::Button("WoodBox", ImVec2(50, 50)))
	{
		ArgmentFlag = true;
	}
	ImGui::SameLine();

	if (ImGui::Button("StoneBox", ImVec2(50, 50)))
	{
		StoneArgment = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("OtherBox", ImVec2(50, 50)))
	{
		OtherArgment = true;
	}
	ImGui::Text("Position");
	ImGui::SliderFloat("Position_X", &S_Pos.x, -2000.f, 2000.f);
	ImGui::SliderFloat("Position_Y", &S_Pos.y, -20.f, 20.f);
	ImGui::SliderFloat("Position_Z", &S_Pos.z, -200.f, 200.f);

	ImGui::Text("Rotation");
	ImGui::SliderFloat("Rotation_X", &S_Rot.x, 0.f, 360.f);
	ImGui::SliderFloat("Rotation_Y", &S_Rot.y, 0.f, 360.f);
	ImGui::SliderFloat("Rotation_Z", &S_Rot.z, 0.f, 360.f);

	ImGui::Text("Scale");
	ImGui::SliderFloat("Sclae_X", &S_Scl.x, 0.f, 10.f);
	ImGui::SliderFloat("Scale_Y", &S_Scl.y, 0.f, 10.f);
	ImGui::SliderFloat("Scale_Z", &S_Scl.z, 0.f, 10.f);
	ImGui::End();

	ImGui::Begin("BoxList");
	ImGui::SetWindowPos(ImVec2(300, 400));
	ImGui::SetWindowSize(ImVec2(300, 300));
	for (int i = 0; i < imguilist.size(); i++)
	{
		if (imguilist[i] == nullptr)
		{
			continue;
		}
		
		imguilist[i]->CreateImguiList();
}
ImGui::End();
}



PlaceBox::CreateImGui::CreateImGui(std::string num, float scl, XMFLOAT3 pos)
{
	Scl = scl;
	Pos = pos;
	listnum.push_back("Box" + num);
}

void PlaceBox::CreateImGui::CreateImguiList()
{
	std::string TitName =modelname+ std::to_string(imnumber) + "----------------------";
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), TitName.c_str());
	//ImGui::StyleColorsClassic();

	if (ImGui::CollapsingHeader(TitName.c_str()))
	{
		ImGui::Text("Scale");
		std::string sclname = "Scl" + TitName;
		ImGui::SliderFloat(sclname.c_str(), &Scl, 0.f, 1.f);

		ImGui::Text("Position");

		float pos[3] = { Pos.x, Pos.y, Pos.z };
		std::string posname_x = "Pos.x" + TitName;
		std::string posname_z = "Pos.z" + TitName;
		ImGui::SliderFloat(posname_x.c_str(), &Pos.x, -800.f, 800.f);
		ImGui::SliderFloat(posname_z.c_str(), &Pos.z, -700.f, 700.f);

		ImGui::Text("Rotation");
		std::string rotname = "Rot" + TitName;
		ImGui::SliderFloat(rotname.c_str(), &Rot.y, 0.f, 360.f);

		ImGui::Text("SelectBox");
		std::string enumynum_g = "WoodBox" + TitName;
		std::string enumynum_l = "StoneBox" + TitName;
		std::string enumynum_t = "Other" + TitName;

		if (ImGui::Button(enumynum_g.c_str(), ImVec2(70, 30)))
		{
			bnumber = WOOD;
		}
		ImGui::SameLine();
		if (ImGui::Button(enumynum_l.c_str(), ImVec2(70, 30)))
		{
			bnumber = STONE;
		}
		ImGui::SameLine();
		if (ImGui::Button(enumynum_t.c_str(), ImVec2(70, 30)))
		{
			bnumber = OTHER;
		}
		
		ImGui::Text("DeleteInstance");
		std::string delname = "Delete" + TitName;
		if (ImGui::Button(delname.c_str(), ImVec2(100, 30)))
		{
			Del = true;
		}

		std::string selname = "SelectThis" + TitName;
		ImGui::Checkbox(selname.c_str(), &Select);
	}
}


void PlaceBox::ArgmentObj(bool& aflag, IKEModel* model)
{

	SampleObj->SetModel(model);

	std::unique_ptr<IKEObject3d> newobj;
	newobj = std::make_unique<IKEObject3d>();
	newobj->Initialize();
	newobj->SetModel(model);
	newobj->SetPosition(SampleObj->GetPosition());


	float radius = 5.0f;
	newobj->SetCollider(new SphereCollider(XMVECTOR({ 10, radius, 10, 0 }), radius));
	newobj->collider->SetAttribute(COLLISION_ATTR_ALLIES);
	boxes.push_back(std::move(newobj));

	std::unique_ptr<CreateImGui> newlist;
	newlist = std::make_unique<CreateImGui>("BOX" + std::to_string(static_cast<int>(boxes.size())), 0.02f, S_Pos);
	newlist->SetModelName("BOX");
	newlist->SetImguiNumber(static_cast<int>(boxes.size() - 1));

	imguilist.push_back(std::move(newlist));

	aflag = false;


}
