#pragma once
#include<SpriteFont.h>
#include<ResourceUploadBatch.h>
#include<Windows.h>
#include"DirectXCommon.h"
#include"WinApp.h"

#pragma comment(lib,"DirectXTK12.lib")

class Font
{
private:
	// Microsoft::WRL::Çè»ó™
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	

	void Initialize(DirectXCommon* dxcommon);
private:
	DirectX::GraphicsMemory* _gmemory = nullptr;
	DirectX::SpriteFont* _spritefont = nullptr;
	DirectX::SpriteBatch* _spritebatch = nullptr;

	ComPtr<ID3D12DescriptorHeap> _heapForSpriteFont;
};

