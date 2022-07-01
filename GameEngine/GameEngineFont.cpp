#include "PreCompile.h"
#include "GameEngineFont.h"

IFW1Factory* GameEngineFont::FontFactory_ = nullptr;

GameEngineFont::GameEngineFont() :
	FontWrapper_(nullptr)
{
}

GameEngineFont::~GameEngineFont() 
{
	if (nullptr !=  FontWrapper_)
	{
		FontWrapper_->Release();
		FontWrapper_ = nullptr;
	}
}

void GameEngineFont::Load(const std::string& _Path) 
{
	if (nullptr == FontFactory_)
	{
		if (S_OK != FW1CreateFactory(FW1_VERSION, &FontFactory_))
		{
			GameEngineDebug::MsgBoxError("��Ʈ �ε��� �����߽��ϴ�.");
			return;
		}
	}

	std::wstring wText = GameEngineString::StringToWStringReturn(_Path);

	if (S_OK != FontFactory_->CreateFontWrapper(GameEngineDevice::GetDevice(), wText.c_str(), &FontWrapper_))
	{
		GameEngineDebug::MsgBoxError("��Ʈ ĳĪ�� �����߽��ϴ� ������ ���� �ʾҽ��ϴ�.");
		return;
	}
}

void GameEngineFont::DrawFont(const std::string& _Text, float _Size, const float4& _Pos, const float4& _Color, unsigned int _Pivot)
{
	std::wstring wText = GameEngineString::StringToWStringReturn(_Text);
	FontWrapper_->DrawString(GameEngineDevice::GetContext(), wText.c_str(), _Size, _Pos.x, _Pos.y, _Color.ColorToUint(), _Pivot);
}
