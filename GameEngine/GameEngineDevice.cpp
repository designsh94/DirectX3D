#include "PreCompile.h"
#include "GameEngineDevice.h"
#include "GameEngineWindow.h"
#include "GameEngineTextureManager.h"
#include "GameEngineRenderTargetManager.h"
#include "GameEngineDepthBufferManager.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineDepthBuffer.h"

// �������� �̱���
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
	// ����� ClearColor�� Clear
	BackBufferTarget_->Clear();

	// ���� : SwapChain�� ������ ����Ÿ�ٺ�(�ؽ���)�� ������������������ ��ģ ��������
	// �׸��� �̴� �ٷ� ȭ�鿡 ǥ�õȴ�.
	BackBufferTarget_->Setting();
}

void GameEngineDevice::RenderEnd()
{
	// ȭ�鿡 �ѷ���
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

	// Direct 11 Version ���
	D3D_FEATURE_LEVEL eLV = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// Device ����
	if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, &Device_, &eLV, &Context_))
	{
		GameEngineDebug::MsgBoxError("D3D11 Create Device ERROR");
	}

	// DirectX 11Version ����� �Ұ����ϴٸ� ����
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

	// ����ü�� �ɼ� ������ ���� ����
	DXGI_SWAP_CHAIN_DESC ScInfo = { 0, };

	// �׷���ī�尡 ���۸� �������־���Ѵ�.
	ScInfo.BufferDesc.Width = ScreenSize.uix();
	ScInfo.BufferDesc.Height = ScreenSize.uiy();

	// ����Ϳ� �����ؼ� 1�ʿ� 60������ ����۸� �����ض�.
	ScInfo.BufferDesc.RefreshRate.Denominator = 1;
	ScInfo.BufferDesc.RefreshRate.Numerator = 60;

	ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ȭ�鿡 ���� ���ؼ��� �뵵�� �˷��־���Ѵ�.
	ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	// ���ø�
	ScInfo.SampleDesc.Quality = 0;	// ��Ƽ�ٸ������ ���� �⺻ ���÷� ����� ������ 1�̰� ǰ�� ������ 0�Դϴ�
	ScInfo.SampleDesc.Count = 1;	// �ȼ��� ��ġ�� �� ����

	// 
	ScInfo.OutputWindow = GameEngineWindow::GetInst().GetWindowHWND();

	// ���۰���
	ScInfo.BufferCount = 2;

	// ���� ���
	ScInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// ������ â��� -> ��üȭ�� ��ȯ�� �˾Ƽ� ó��
	ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// true: ��üȭ�����, false: ��üȭ����
	ScInfo.Windowed = true;

	// �׷���ī���� ȭ����¿� ���õ� ���ҽ��� ������ �� �ִ� ��ɵ��� �����͸� ������,
	// �� ��ɵ��� ���ؼ� ������� �ؽ��ĸ� ���� ������ ��
	// ID3D11Device* != IDXGIDevice;
	// �׷��� ī���� �޸𸮿� �����ڷ� �����Ѵ�.
	IDXGIDevice* pD = nullptr;
	IDXGIAdapter* pA = nullptr;
	IDXGIFactory* pF = nullptr;

	// __uuidof : IDXGIDevice�� ã������ Ű�� ��ȯ�ϴ� ����ȯ
// MIDL_INTERFACE("54ec77fa-1377-44e6-8c32-88fd5f44c84c") �� �ش��ϴ� Ŭ������ ã�ƿͶ��� ��
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

	// �޾ƿ� ���� ��� �ݳ�
	pF->Release();
	pA->Release();
	pD->Release();

	// ���� SwapChain�� �������ִ� �ؽ���(����Ÿ�ٺ�)�� ���´�.
	ID3D11Texture2D* BackBufferTexture = nullptr;
	if (S_OK != SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBufferTexture)))
	{
		GameEngineDebug::MsgBoxError("SwapChain Texture Error");
	}

	// BackBuffer Texture ���� �� RenderTarget ����
	GameEngineTextureManager::GetInst().Create("BackBuffer", BackBufferTexture);
	BackBufferTarget_ = GameEngineRenderTargetManager::GetInst().Create("BackBuffer", "BackBuffer", float4::BLUE);
}
