#pragma once
#include <any>
#include <string>

#include<fstream>
#include<string>
#include<sstream>
#include "DirectXMath.h"
using namespace DirectX;
namespace LoadCSV
{
	//CSV�ǂݍ���
	//�ԐړI�ɂ����@���̊֐����ŗ�O�����Ƃ�������
	void LoadCsvParam(std::string FileName,std::string LoadName,std::any&p );

	//���ړI�ɂ����@�R�[�h���Z�����ށE�i�[�p�̕ϐ����K�v�Ȃ�
	std::any LoadCsvParam(std::string FileName, std::string LoadName);

	XMFLOAT3 LoadCsvParam_XMFLOAT3(std::string FileName, std::string LoadName);
}


