#include "Font.h"

#pragma comment(lib,"dxguid.lib")

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
		L"font/fonttest.spritefont",
		_heapForSpriteFont->GetCPUDescriptorHandleForHeapStart(),
		_heapForSpriteFont->GetGPUDescriptorHandleForHeapStart());

	auto future = resUploadBatch.End(dxcommon->GetQue());
	dxcommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
	future.wait();

	dxcommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());

	_spritefont->DrawString(_spritebatch, "Hello World",
		DirectX::XMFLOAT2(102, 102), DirectX::Colors::Black);

	_spritefont->DrawString(_spritebatch, "Hello World",
		DirectX::XMFLOAT2(100, 100), DirectX::Colors::Yellow);

	_gmemory->Commit(dxcommon->GetQue());

}
