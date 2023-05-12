#include "Font.h"



void Font::Initialize(DirectXCommon* dxcommon)
{
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
		L"font/myfile.spritefont",
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
	_spritefont->DrawString(_spritebatch, L"こんにちはピカチュウ",
		DirectX::XMFLOAT2(102, 102), DirectX::Colors::Black);

	_spritefont->DrawString(_spritebatch, L"こんにちはピカチュウ",
		DirectX::XMFLOAT2(100, 100), DirectX::Colors::Yellow);
	_spritebatch->End();
	_gmemory->Commit(dxcommon->GetQue());
}
