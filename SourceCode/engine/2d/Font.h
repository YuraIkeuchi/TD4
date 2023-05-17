#pragma once
#include<SpriteFont.h>
#include<ResourceUploadBatch.h>
#include<Windows.h>
#include"DirectXCommon.h"
#include"WinApp.h"

#pragma comment(lib,"DirectXTK12.lib")

class Font
{
protected:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	// Microsoft::WRL::Çè»ó™
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	~Font();
	void Initialize(DirectXCommon* dxcommon,const XMVECTOR& color={1.f,1.f,1.f,1.f}, const XMFLOAT2& position={0.f,0.f});
	void Draw(DirectXCommon* dxcommon);
private:
	DirectX::GraphicsMemory* _gmemory = nullptr;
	DirectX::SpriteFont* _spritefont = nullptr;
	DirectX::SpriteBatch* _spritebatch = nullptr;

	XMVECTOR color_{};
	XMFLOAT2 position_{};
	XMFLOAT2 shadow_position_ = {};

	ComPtr<ID3D12DescriptorHeap> _heapForSpriteFont;
};

