#include "CsvLoader.h"

void LoadCSV::LoadCsvParam(std::string FileName, std::string LoadName, std::any& p)
{
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	
	file.open(FileName);

	popcom << file.rdbuf();
	
	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find(LoadName) == 0)
		{
			std::getline(line_stream, word, ',');
			p =(std::atof(word.c_str()));
			
			break;
		}
	}

	file.close();
}

std::any LoadCSV::LoadCsvParam(std::string FileName, std::string LoadName)
{
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	std::string name1 = "Resources/";
	std::string name2 = "csv/";
	std::string openFileName = name1 + name2 + FileName;

	std::any param;

	file.open(FileName);

	popcom << file.rdbuf();

	file.close();
	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find(LoadName) == 0)
		{
			std::getline(line_stream, word, ',');
			param = (std::atof(word.c_str()));

			break;
		}
	}

	return param;
}


XMFLOAT3 LoadCSV::LoadCsvParam_XMFLOAT3(std::string FileName, std::string LoadName)
{
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	file.open(FileName);

	popcom << file.rdbuf();

	file.close();

	XMFLOAT3 pos;
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find(LoadName) == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				pos = { x, y, z };
				//break;
			}
		}
		return pos;
}
