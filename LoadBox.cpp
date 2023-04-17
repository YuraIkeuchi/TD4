#include "LoadBox.h"

#include "CsvLoader.h"
#include "ModelManager.h"

void LoadBox::Load()
{
	auto Size= static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/stage/stage1.csv", "Quantity")));
	boxes.resize(Size);

	for (auto i = 0; i < boxes.size(); i++) {
		boxes[i].reset(new IKEObject3d());
		boxes[i]->Initialize();
		boxes[i]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Box));
		
	}
	file.open("Resources/csv/stage/stage1.csv");

	popcom << file.rdbuf();

	file.close();

	std::vector<XMFLOAT3>Pos(Size);
	for (auto i = 0; i < boxes.size(); i++) {
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find("POSITION") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				Pos[i] = { x, y, z };
				break;
			}
		}
		
	}
	for (auto i = 0; i < boxes.size(); i++) {
			boxes[i]->SetPosition(Pos[i]);
		}
}

void LoadBox::Initialize()
{
	Load();
	
}

void LoadBox::Update()
{
	for(auto i=0;i<boxes.size();i++)
	{

		boxes[i]->Update();
	}
}


void LoadBox::Draw()
{
	for (auto i = 0; i < boxes.size(); i++)
	{
		boxes[i]->Draw();
	}
	
}


