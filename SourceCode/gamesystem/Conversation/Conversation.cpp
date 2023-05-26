#include "Conversation.h"
#include"ImageManager.h"
#include"Easing.h"

const XMFLOAT4 kHalfClear{ 0.5f,0.5f,0.5f,0.5f };


Conversation* Conversation::GetInstance()
{
	static Conversation ins;

	return &ins;
}


void Conversation::SetText(wchar_t* text,CharaText chara,int index)
{
	Conversation* con;
	con = new Conversation();
}



void Conversation::CreateText(DirectXCommon*comn,ChatName num, wchar_t* text[3])
{
	//èâä˙âª
	Font* tex[3];

	for(auto i=0;i<3;i++)
	{
		tex[i] = new Font();
	//ì«Ç›çûÇ›
		tex[i]->LoadFont(comn);
		tex[i]->SetString(text[i]);
	}

	CharaState state;

	state.index = num;
	
	FontList.emplace_back(tex[0]);//0
	FontList.emplace_back(tex[1]);//1
	FontList.emplace_back(tex[2]);//2
	for (auto i = 0; i < 3; i++)
		state.font[i] = FontList[i];

	//åƒÇ—èoÇµàÍçs
	textList.emplace_back(state);
}

void Conversation::TextUpdate(XMFLOAT2* pos, XMFLOAT3* color)
{
	for (auto i = 0; i < textList.size(); i++) {
		for (auto j = 0; j < 3; j++) {
			textList[i].font[j]->SetPos(pos[j]);
			textList[i].font[j]->SetColor({ color[i].x,color[i].y,color[i].z,1});
		}
	}
}


void Conversation::Draw(DirectXCommon* comn,ChatName num)
{
	for(auto i=0;i<3;i++)
		textList[num].font[i]->Draw(comn);
}
