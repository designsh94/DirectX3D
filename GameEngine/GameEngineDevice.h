#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// 다이렉트 x는 프로세스마다 GUIID를 부여서해 X프로세스가 디바이스를 이용하려고 하는지 알아서 처리해줘요.
#pragma comment(lib, "dxguid")

// 분류 : 
// 용도 : 
// 설명 : 그래픽카드에 접근할수 있는 포인터
class GameEngineDepthBuffer;
class GameEngineRenderTarget;
class GameEngineDevice
{
private:
	static GameEngineDevice* Inst;

private:
	static ID3D11Device* Device_;
	static ID3D11DeviceContext* Context_;
	static IDXGISwapChain* SwapChain_;
	static GameEngineRenderTarget* BackBufferTarget_;

public:
	static GameEngineDevice& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

public:
	static GameEngineRenderTarget* GetBackBufferTarget();
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetContext();
	static void ShaderReset();
	static void ResourceReset();
	static void RenderStart();
	static void RenderEnd();

private:

public:
	GameEngineDevice();
	~GameEngineDevice();

protected:
	GameEngineDevice(const GameEngineDevice& _other) = delete;
	GameEngineDevice(GameEngineDevice&& _other) = delete;

private:
	GameEngineDevice& operator=(const GameEngineDevice& _other) = delete;
	GameEngineDevice& operator=(const GameEngineDevice&& _other) = delete;

public: // 디바이스 초기화
	void Initialize();

public: // 스왑체인 생성
	void CreateSwapChain();
};

