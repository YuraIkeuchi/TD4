#include "Font.h"

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
		L"font/UD.spritefont",
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
