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
	FontName_("궁서"),
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

	// 텍스트 셋팅이 되지않았다면 처리안함
	if (false == IsText_)
	{
		return;
	}

	// Font 전용 렌더타겟 셋팅으로 변경 및 UI RenderTaret으로 재전환하기위해 현재 마지막 렌더타겟 저장
	GameEngineRenderTarget* RenderTarget = GameEngineRenderTarget::GetLastRenderTarget();

	FontTarget_->Clear();
	FontTarget_->Setting();

	// UIRenderer는 화면의 중앙을 원점(0,0,0)을 기준으로 설정되어있고,
	// GameEngineFont는 스크린좌표(윈도우좌표)를 기준으로 설정되어있으므로,
	// 윈도우의 크기와 UIRenderer의 위치를 이용하여 스크린좌표계에서의 Font의 위치를 
	// 계산한다.

	// 윈도우 크기 Get
	float4 ScreenSize = GameEngineWindow::GetInst().GetSize();
	ScreenSize = ScreenSize.halffloat4();

	// UIRenderer 월드위치 Get
	float4 UIPos = GetTransform()->GetWorldPosition();

	// 폰트 렌더링 위치 계산
	// UIPos가 음수이면 x = ScreenSize - UIPos
	// UIPos가 양수이면 x = ScreenSize + UIPos
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

	// RenderTarget를 UIRender Target으로 변경
	RenderTarget->Setting();
}

void GameEngineUIRenderer::SetRenderGroup(int _Order) 
{
	GetLevel()->GetUICamera()->ChangeRendererGroup(_Order, this);;
}

void GameEngineUIRenderer::TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, unsigned int _Flags, float4 _Color, const float4& _FontPivot, int _MaxLen)
{
	// 해당 UIRenderer는 텍스트를 사용하므로 Flag On
	IsText_ = true;

	// 텍스트 렌더링에 필요한 기본데이터 저장
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
	// 기존의 문자열에 해당 문자열을 병합한다.
	// 단, 입력가능 최대치를 넘어가면 병합불가
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
	// 기존 문자열의 끝의 문자열을 삭제한다.
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
