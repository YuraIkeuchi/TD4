#include "Text.h"
#include"VariableCommon.h"
#include<locale.h>
#include<wchar.h>

const XMFLOAT2 kFirstRowPos{ 5.f,0.f };
const XMFLOAT2 kSecondRowPos{ 5.f,-40.f };
const XMFLOAT2 kThirdRowPos{ 5.f, -80.f };

void Text::Initialize(DirectXCommon* dxcommon, const XMVECTOR& color)
{
	//����
	firstrow_ = make_unique<Font>();
	secondrow_ = make_unique<Font>();
	thirdrow_ = make_unique<Font>();
	//�ǂݍ���
	firstrow_->LoadFont(dxcommon);
	secondrow_->LoadFont(dxcommon);
	thirdrow_->LoadFont(dxcommon);
	//���W�Z�b�g
	firstrow_->SetPos(kFirstRowPos);
	secondrow_->SetPos(kSecondRowPos);
	thirdrow_->SetPos(kThirdRowPos);
	//�F
	firstrow_->SetColor(color);
	secondrow_->SetColor(color);
	thirdrow_->SetColor(color);
}

void Text::Update(wchar_t* firstrow,wchar_t* secondrow,wchar_t* thirdrow)
{
	for (int i = 0; i < len[0]; i++) {
		firstrow_->SetString(firstrow);
	}
	for (int i = 0; i < len[1]; i++) {
		secondrow_->SetString(secondrow);
	}
	for (int i = 0; i < len[2]; i++) {
		thirdrow_->SetString(thirdrow);
	}
}

void Text::Draw(DirectXCommon* dxcommon)
{
	firstrow_->Draw(dxcommon);
	secondrow_->Draw(dxcommon);
	thirdrow_->Draw(dxcommon);
}
