#include "Font.h"



Font::~Font()
{
	delete _gmemory;
	delete _spritefont;
	delete _spritebatch;
}

void Font::Initialize(DirectXCommon* dxcommon, const XMVECTOR& color, const XMFLOAT2& position)
{
	color_ = color;
	position_ = shadow_position_ =position;
	shadow_position_.x = position.x -2;
	shadow_position_.y = position.y - 2;
	_gmemory = new DirectX::GraphicsMemory(dxcommon->GetDev());

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

void Font::Draw(DirectXCommon* dxcommon)
{
	dxcommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
	_spritebatch->Begin(dxcommon->GetCmdList());
	_spritefont->DrawString(_spritebatch, L"aA1ピカチュウ\n aaaa",
		shadow_position_, DirectX::Colors::Black);

	_spritefont->DrawString(_spritebatch, L"aA1ピカチュウ\n aaa",
		position_, color_);
	_spritebatch->End();
	_gmemory->Commit(dxcommon->GetQue());
}
