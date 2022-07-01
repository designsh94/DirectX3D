#include "PreCompile.h"
#include "SmallPostBlur.h"
#include "GameEngineTexture.h"
#include "GameEngineTextureManager.h"
#include "GameEngineWindow.h"
#include "GameEngineRasterizer.h"

SmallPostBlur::SmallPostBlur() :
	Data(),
	Filter(nullptr)
{
}

SmallPostBlur::~SmallPostBlur()
{
}

void SmallPostBlur::Initialize()
{
	float4 RenderTargetSize = GameEngineWindow::GetInst().GetSize().halffloat4();

	CreateResultTarget(RenderTargetSize);
	SetEffect("Blur");
	Res_.SettingConstantBufferLink("BlurData", Data);

	Effect_ = Effect_->Clone();
	Effect_->RasterizerClone();
	Effect_->GetRasterizer()->SetViewPort(RenderTargetSize.x, RenderTargetSize.y, 0.0f, 0.0f);
}

void SmallPostBlur::Effect(float _DeltaTime)
{
	Result_->Clear();
	Result_->Setting();
	Res_.SettingTexture("Target", Target_->GetTexture(0));
	Res_.Setting();
	Effect_->Rendering();
	Effect_->Reset();
	Res_.ReSet();

	Target_->Clear(false);
	Target_->Copy(Result_);
}

void SmallPostBlur::SetFilter(std::string _TextureName)
{
	Filter = GameEngineTextureManager::GetInst().Find(_TextureName);

	if (nullptr == Filter)
	{
		return;
	}

	Data.FilterPixelX = 1.0f / Filter->GetTextureSize().x;
	Data.FilterPixelY = 1.0f / Filter->GetTextureSize().y;
	Data.FilterEndX = Filter->GetTextureSize().ix() / 2;
	Data.FilterEndY = Filter->GetTextureSize().iy() / 2;
	Data.FilterStartX = -Data.FilterEndX;
	Data.FilterStartY = -Data.FilterEndY;

	Data.ImagePixelUVX = 1.0f / Target_->GetTexture(0)->GetTextureSize().x;
	Data.ImagePixelUVY = 1.0f / Target_->GetTexture(0)->GetTextureSize().y;

	Data.FilterSum = 0.0f;
	for (int y = 0; y < Filter->GetTextureSize().iy(); y++)
	{
		for (int x = 0; x < Filter->GetTextureSize().ix(); x++)
		{
			Data.FilterSum += Filter->GetPixel(x, y).x;
		}
	}

	Data.FilterCount = Filter->GetTextureSize().ix() * Filter->GetTextureSize().iy();

	Res_.SettingTexture("Filter", Filter);
}
