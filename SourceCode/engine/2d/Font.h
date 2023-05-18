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
	// Microsoft::WRL::Çè»ó™
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	~Font();
	static void Initialize(DirectXCommon* dxcommon);
	void LoadFont(DirectXCommon* dxcommon);
	void SetPosColor(const XMVECTOR& color = { 1.f,1.f,1.f,1.f }, const XMFLOAT2& position = { 0.f,0.f });
	void Draw(DirectXCommon* dxcommon);
	static void PostDraw(DirectXCommon* dxcommon);
	void SetString(wchar_t* ward);
protected:
	static DirectX::GraphicsMemory* _gmemory;
private:
	DirectX::SpriteFont* _spritefont;
	DirectX::SpriteBatch* _spritebatch;
	ComPtr<ID3D12DescriptorHeap> _heapForSpriteFont;
	XMVECTOR color_{};
	XMFLOAT2 position_{};
	XMFLOAT2 shadow_position_ = {};
	XMFLOAT2 disply_place_{ 300.f,370.f };
	wchar_t* ward_=L"Hello World";
};

