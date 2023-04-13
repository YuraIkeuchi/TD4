#include "PlaceBox.h"

#include "ModelManager.h"

void PlaceBox::Initialize()
{
	SampleObj = std::make_unique<IKEObject3d>();
	SampleObj->Initialize();
	SampleObj->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Box));
	
}


void PlaceBox::Update()
{SampleObj->SetScale({ 5.0f,5.0f,5.0f });

	SampleObj->SetPosition(S_Pos);
	SampleObj->SetColor({ 1.f,1.f,1.f,1.f });
	SampleObj->Update();

	for(auto i=0;i<boxes.size();i++)
	{
		if (boxes[i] == nullptr)continue;
		boxes[i]->SetColor({ 1.f,1.f,1.f,1.f });
		boxes[i]->Update();
	}

	if (ArgmentFlag)
	{
		std::unique_ptr<IKEObject3d> newobj;
		newobj = std::make_unique<IKEObject3d>();

		newobj->Initialize();
		newobj->SetPosition(S_Pos);
		boxes.push_back(std::move(newobj));
		ArgmentFlag = false;
	}
}


void PlaceBox::Draw(DirectXCommon* DxCommon)
{
	SampleObj->Draw();
	for (auto i = 0; i < boxes.size(); i++)
	{
		//boxes[i]->Draw();
	}
}

void PlaceBox::FileWriting()
{
	file.open("box.csv");
	popcom << file.rdbuf();

	file.close();
	//std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Param_CSV/box.csv"); // ファイルパスを指定する
	ofs << "Wood_Quantity" << "," << boxes.size() << std::endl;

	for (int i = 0; i < boxes.size(); i++)
	{
		ofs << "Number" << "," << Number[i] << std::endl;
		ofs << "POSITION" << "," << boxes[i]->GetPosition().x
			<< "," << boxes[i]->GetPosition().y
			<< "," << boxes[i]->GetPosition().z << std::endl;
	}
}

void PlaceBox::ImGui_Draw()
{
	ImGui::Begin("BoxParam");

	if(ImGui::Button("Argment",ImVec2(200, 200)))
	{
		ArgmentFlag = true;
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
}
