#include "Font.h"
#include<wchar.h>

DirectX::GraphicsMemory* Font::_gmemory = nullptr;

Font::~Font()
{
	/*delete _spritefont;
	delete _spritebatch;*/
}

void Font::Initialize(DirectXCommon* dxcommon)
{
	_gmemory = new DirectX::GraphicsMemory(dxcommon->GetDev());
}

void Font::LoadFont(DirectXCommon* dxcommon)
{
	DirectX::ResourceUploadBatch resUploadBatch(dxcommon->GetDev());

	resUploadBatch.Begin();
	DirectX::RenderTargetState rtState(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT);
	DirectX::SpriteBatchPipelineStateDescription pd(rtState);

	_spritebatch = new DirectX::SpriteBatch(dxcommon->GetDev(), resUploadBatch, pd);

	_heapForSpriteFont = dxcommon->CreateDescriptorHeapForSproteFont();
	_spritefont = new DirectX::SpriteFont(dxcommon->GetDev(),
		resUploadBatch,
		L"Resources/font/newUD.spritefont",
		_heapForSpriteFont->GetCPUDescriptorHandleForHeapStart(),
		_heapForSpriteFont->GetGPUDescriptorHandleForHeapStart());

	auto future = resUploadBatch.End(dxcommon->GetQue());
	dxcommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
	future.wait();
	_spritebatch->SetViewport(dxcommon->GetViewPort());
}


void Font::StringReset()
{
	ward_ = L" ";
}

void Font::Draw(DirectXCommon* dxcommon)
{
	dxcommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
	_spritebatch->Begin(dxcommon->GetCmdList());
	_spritefont->DrawString(_spritebatch, ward_,
		disply_place_, DirectX::Colors::Black, {}, shadow_position_);

	_spritefont->DrawString(_spritebatch, ward_,
		disply_place_, color_, {},position_);
	_spritebatch->End();
}

void Font::PostDraw(DirectXCommon* dxcommon)
{
	_gmemory->Commit(dxcommon->GetQue());
}

void Font::SetString(wchar_t* ward)
{
	ward_ = ward;
}

void Font::TestSet(wchar_t* ward, size_t len)
{
	time_ += 0.1f;
	if (time_ > 1) {
		len_ += 1;
		time_ = 0.f;
	}
	if (len_ > len) { return; }
	for (size_t i = 0; i < len_; i++) {
		ward_ = ward;
	}
}
