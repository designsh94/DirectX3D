#include "PreCompile.h"
#include "GameEngineDevice.h"
#include "GameEngineWindow.h"
#include "GameEngineTextureManager.h"
#include "GameEngineRenderTargetManager.h"
#include "GameEngineDepthBufferManager.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineDepthBuffer.h"

// 포인터형 싱글톤
GameEngineDevice* GameEngineDevice::Inst = new GameEngineDevice();

GameEngineRenderTarget* GameEngineDevice::BackBufferTarget_ = nullptr;
ID3D11Device* GameEngineDevice::Device_ = nullptr;
ID3D11DeviceContext* GameEngineDevice::Context_ = nullptr;
IDXGISwapChain* GameEngineDevice::SwapChain_ = nullptr;

GameEngineRenderTarget* GameEngineDevice::GetBackBufferTarget()
{
	return BackBufferTarget_;
}

ID3D11Device* GameEngineDevice::GetDevice() 
{
	return Device_;
}

ID3D11DeviceContext* GameEngineDevice::GetContext() 
{
	return Context_;
}

void GameEngineDevice::ShaderReset()
{
	Context_->VSSetShader(nullptr, nullptr, 0);
	Context_->HSSetShader(nullptr, nullptr, 0);
	Context_->GSSetShader(nullptr, nullptr, 0);
	Context_->DSSetShader(nullptr, nullptr, 0);
	Context_->PSSetShader(nullptr, nullptr, 0);
}

void GameEngineDevice::ResourceReset()
{
	ShaderReset();
}

void GameEngineDevice::RenderStart()
{
	// 백버퍼 ClearColor로 Clear
	BackBufferTarget_->Clear();

	// 셋팅 : SwapChain이 가지는 렌더타겟뷰(텍스쳐)에 렌더링파이프라인을 거친 정점들을
	// 그리며 이는 바로 화면에 표시된다.
	BackBufferTarget_->Setting();
}

void GameEngineDevice::RenderEnd()
{
	// 화면에 뿌려라
	HRESULT Result = SwapChain_->Present(0, 0);
	if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
	{
		int a = 0;
	}
}

GameEngineDevice::GameEngineDevice()
{
}

GameEngineDevice::~GameEngineDevice()
{
	if (nullptr != SwapChain_)
	{
		SwapChain_->Release();
		SwapChain_ = nullptr;
	}

	if (nullptr != Context_)
	{
		Context_->Release();
		Context_ = nullptr;
	}

	if (nullptr != Device_)
	{
		Device_->Release();
		Device_ = nullptr;
	}
}

void GameEngineDevice::Initialize() 
{
	if (nullptr == GameEngineWindow::GetInst().GetWindowHWND())
	{
		GameEngineDebug::MsgBoxError("Window Hwnd ERROR");
		return;
	}

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Direct 11 Version 사용
	D3D_FEATURE_LEVEL eLV = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// Device 생성
	if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, &Device_, &eLV, &Context_))
	{
		GameEngineDebug::MsgBoxError("D3D11 Create Device ERROR");
	}

	// DirectX 11Version 사용이 불가능하다면 에러
	if (eLV != D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0)
	{
		GameEngineDebug::MsgBoxError("D3D11 Not FEATURE LEVEL 11 ERROR");
	}

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
}

void GameEngineDevice::CreateSwapChain()
{
	if (0 >= GameEngineWindow::GetInst().GetSize().x && 0 >= GameEngineWindow::GetInst().GetSize().y)
	{
		GameEngineDebug::MsgBoxError("Screen Size Zero");
	}

	float4 ScreenSize = GameEngineWindow::GetInst().GetSize();

	// 스왑체인 옵션 설정을 위한 구조
	DXGI_SWAP_CHAIN_DESC ScInfo = { 0, };

	// 그래픽카드가 버퍼를 가지고있어야한다.
	ScInfo.BufferDesc.Width = ScreenSize.uix();
	ScInfo.BufferDesc.Height = ScreenSize.uiy();

	// 모니터에 간섭해서 1초에 60프레임 백버퍼를 스왑해라.
	ScInfo.BufferDesc.RefreshRate.Denominator = 1;
	ScInfo.BufferDesc.RefreshRate.Numerator = 60;

	ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 화면에 띄우기 위해서는 용도를 알려주어야한다.
	ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	// 샘플링
	ScInfo.SampleDesc.Quality = 0;	// 앤티앨리어싱이 없는 기본 샘플러 모드의 개수는 1이고 품질 수준은 0입니다
	ScInfo.SampleDesc.Count = 1;	// 픽셀내 배치할 점 갯수

	// 
	ScInfo.OutputWindow = GameEngineWindow::GetInst().GetWindowHWND();

	// 버퍼개수
	ScInfo.BufferCount = 2;

	// 스왑 방법
	ScInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// 윈도우 창모드 -> 전체화면 전환시 알아서 처리
	ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// true: 전체화면안함, false: 전체화면사용
	ScInfo.Windowed = true;

	// 그래픽카드의 화면출력에 관련된 리소스에 관여할 수 있는 기능들의 포인터를 얻어오고,
	// 그 기능들을 통해서 백버퍼의 텍스쳐를 직접 얻어오는 것
	// ID3D11Device* != IDXGIDevice;
	// 그래픽 카드의 메모리에 관리자로 접근한다.
	IDXGIDevice* pD = nullptr;
	IDXGIAdapter* pA = nullptr;
	IDXGIFactory* pF = nullptr;

	// __uuidof : IDXGIDevice를 찾기위한 키로 변환하는 형변환
// MIDL_INTERFACE("54ec77fa-1377-44e6-8c32-88fd5f44c84c") 에 해당하는 클래스를 찾아와라라는 뜻
	Device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pD));
	if (nullptr == pD)
	{
		GameEngineDebug::MsgBoxError("IDXGIDevice null");
	}

	pD->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pA));
	if (nullptr == pA)
	{
		GameEngineDebug::MsgBoxError("IDXGIAdapter null");
	}

	pA->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pF));
	if (nullptr == pF)
	{
		GameEngineDebug::MsgBoxError("IDXGIFactory null");
	}
	
	// Create SwapChain
	if (S_OK != pF->CreateSwapChain(Device_, &ScInfo, &SwapChain_))
	{
		GameEngineDebug::MsgBoxError("SwapChain Create Error");
	}

	// 받아온 권한 모두 반납
	pF->Release();
	pA->Release();
	pD->Release();

	// 현재 SwapChain이 가지고있는 텍스쳐(렌더타겟뷰)를 얻어온다.
	ID3D11Texture2D* BackBufferTexture = nullptr;
	if (S_OK != SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBufferTexture)))
	{
		GameEngineDebug::MsgBoxError("SwapChain Texture Error");
	}

	// BackBuffer Texture 생성 및 RenderTarget 지정
	GameEngineTextureManager::GetInst().Create("BackBuffer", BackBufferTexture);
	BackBufferTarget_ = GameEngineRenderTargetManager::GetInst().Create("BackBuffer", "BackBuffer", float4::BLUE);
}
