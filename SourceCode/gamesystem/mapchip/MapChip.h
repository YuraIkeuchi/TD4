#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

//�}�b�v�̃T�C�Y(��{��1)
const int map_size = 1;
//�c���̍ő吔(�}�b�v�ɂ���ĕύX)
const int map_max_x = 10, map_max_y = 10;

class MapChip
{
public:
	//csv�ǂݍ���
	static void LoadCsvMap(std::vector<std::vector<int>>& mapData, std::string fName);
	//�}�b�v�ԍ�
	static int GetChipNum(int x,int y, std::vector<int> map);
};

