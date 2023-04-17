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
	//CSV読み込み
	//間接的にするやつ　この関数内で例外処理とかかける
	void LoadCsvParam(std::string FileName,std::string LoadName,std::any&p );

	//直接的にするやつ　コードが短くすむ・格納用の変数も必要ない
	std::any LoadCsvParam(std::string FileName, std::string LoadName);

	XMFLOAT3 LoadCsvParam_XMFLOAT3(std::string FileName, std::string LoadName);
}


