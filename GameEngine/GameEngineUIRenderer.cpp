#include "PreCompile.h"
#include "GameEngineUIRenderer.h"
#include "GameEngineLevel.h"
#include "CameraComponent.h"
#include "GameEngineFont.h"
#include "GameEngineFontManager.h"
#include "GameEngineWindow.h"
#include "GameEngineRenderTarget.h"

GameEngineRenderTarget* GameEngineUIRenderer::FontTarget_ = nullptr;
int GameEngineUIRenderer::UIRendererCount = 0;

GameEngineUIRenderer::GameEngineUIRenderer() :
	IsText_(false),
	FontName_("�ü�"),
	PrintText_(""),
	FontSize_(20),
	FontPivot_(float4::ZERO),
	Color_(float4::WHITE),
	Flags_(FW1_CENTER | FW1_VCENTER),
	MaxLen_(0)
{
	++UIRendererCount;
}

GameEngineUIRenderer::~GameEngineUIRenderer()
{
	--UIRendererCount;
	if (0 == UIRendererCount && nullptr != FontTarget_)
	{
		delete FontTarget_;
		FontTarget_ = nullptr;
	}
}

void GameEngineUIRenderer::Start()
{
	GetLevel()->GetUICamera()->PushRenderer(GetOrder(), this);
	SetRenderingPipeLine("TextureAtlas");
	GetGameEngineRenderingPipeLine()->SetOutputMergerDepthStencil("BaseDepthOff");

	if (nullptr == FontTarget_ && UIRendererCount == 1)
	{
		FontTarget_ = new GameEngineRenderTarget();
		FontTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
	}
}

void GameEngineUIRenderer::Render(float _DeltaTime)
{
	GameEngineRenderer::Render(_DeltaTime);

	// �ؽ�Ʈ ������ �����ʾҴٸ� ó������
	if (false == IsText_)
	{
		return;
	}

	// Font ���� ����Ÿ�� �������� ���� �� UI RenderTaret���� ����ȯ�ϱ����� ���� ������ ����Ÿ�� ����
	GameEngineRenderTarget* RenderTarget = GameEngineRenderTarget::GetLastRenderTarget();

	FontTarget_->Clear();
	FontTarget_->Setting();

	// UIRenderer�� ȭ���� �߾��� ����(0,0,0)�� �������� �����Ǿ��ְ�,
	// GameEngineFont�� ��ũ����ǥ(��������ǥ)�� �������� �����Ǿ������Ƿ�,
	// �������� ũ��� UIRenderer�� ��ġ�� �̿��Ͽ� ��ũ����ǥ�迡���� Font�� ��ġ�� 
	// ����Ѵ�.

	// ������ ũ�� Get
	float4 ScreenSize = GameEngineWindow::GetInst().GetSize();
	ScreenSize = ScreenSize.halffloat4();

	// UIRenderer ������ġ Get
	float4 UIPos = GetTransform()->GetWorldPosition();

	// ��Ʈ ������ ��ġ ���
	// UIPos�� �����̸� x = ScreenSize - UIPos
	// UIPos�� ����̸� x = ScreenSize + UIPos
	float4 RenderPos = float4::ZERO;
	RenderPos.x = ScreenSize.x + UIPos.x;
	RenderPos.y = ScreenSize.y - UIPos.y;

	// 
	GameEngineFont* Font = GameEngineFontManager::GetInst().Find(FontName_);
	if (nullptr != Font)
	{
		Font->DrawFont(PrintText_, FontSize_, ScreenSize - UIPos, Color_, FW1_CENTER);
	}

	// Shader Reset
	GameEngineDevice::ShaderReset();

	// Font RenderTarget Merge To UI Render Target
	RenderTarget->Merge(FontTarget_);

	// RenderTarget�� UIRender Target���� ����
	RenderTarget->Setting();
}

void GameEngineUIRenderer::SetRenderGroup(int _Order) 
{
	GetLevel()->GetUICamera()->ChangeRendererGroup(_Order, this);;
}

void GameEngineUIRenderer::TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, unsigned int _Flags, float4 _Color, const float4& _FontPivot, int _MaxLen)
{
	// �ش� UIRenderer�� �ؽ�Ʈ�� ����ϹǷ� Flag On
	IsText_ = true;

	// �ؽ�Ʈ �������� �ʿ��� �⺻������ ����
	FontName_ = _FontName;
	PrintText_ = _PrintText;
	FontSize_ = _FontSize;
	Color_ = _Color;
	Flags_ = _Flags;
	FontPivot_ = _FontPivot;
	MaxLen_ = _MaxLen;
}

bool GameEngineUIRenderer::AddText(std::string _PrintText)
{
	// ������ ���ڿ��� �ش� ���ڿ��� �����Ѵ�.
	// ��, �Է°��� �ִ�ġ�� �Ѿ�� ���պҰ�
	int Length = static_cast<int>(PrintText_.length());
	if (Length <= MaxLen_)
	{
		PrintText_ += _PrintText;
		return true;
	}

	return false;
}

bool GameEngineUIRenderer::DelText()
{
	// ���� ���ڿ��� ���� ���ڿ��� �����Ѵ�.
	int CurLen = static_cast<int>(PrintText_.length());
	if (CurLen <= 0)
	{
		return false;
	}

	std::string::iterator EndIter = PrintText_.end() - 1;
	PrintText_.erase(EndIter);

	return true;
}

void GameEngineUIRenderer::AllDelText()
{
	if (false == PrintText_.empty())
	{
		PrintText_.clear();
	}
}

void GameEngineUIRenderer::SetPrintText(const std::string& _PrintText)
{
	PrintText_ = _PrintText;
}

void GameEngineUIRenderer::SetTextColor(const float4& _Color)
{
	Color_ = _Color;
}
