#include "PreCompile.h"
#include "GameEngineShaderResHelper.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineTextureManager.h"
#include "GameEngineTexture.h"
#include "GameEngineSamplerManager.h"
#include "GameEngineSampler.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"

GameEngineShaderResHelper::GameEngineShaderResHelper() 
{
}

GameEngineShaderResHelper::~GameEngineShaderResHelper() 
{
	Clear();
}

void GameEngineShaderResHelper::Clear() 
{
	for (auto& Setting : AllTextureData_)
	{
		if (nullptr != Setting.second)
		{
			delete Setting.second;
			Setting.second = nullptr;
		}
	}
	AllTextureData_.clear();

	for (auto& Setting : AllSamplerData_)
	{
		if (nullptr != Setting.second)
		{
			delete Setting.second;
			Setting.second = nullptr;
		}
	}
	AllSamplerData_.clear();

	for (auto& Setting : AllConstantBufferData_)
	{
		if (nullptr != Setting.second)
		{
			delete Setting.second;
			Setting.second = nullptr;
		}
	}
	AllConstantBufferData_.clear();
}

bool GameEngineShaderResHelper::IsConstantBuffer(const std::string& _SettingName)
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineConstantBufferSetting*>::iterator FindIter = AllConstantBufferData_.find(UpperName);

	if (FindIter == AllConstantBufferData_.end())
	{
		return false;
	}

	return true;
}

void GameEngineShaderResHelper::ShaderResourcesCheck(GameEngineRenderingPipeLine* _Pipe) 
{
	Clear();
	ShaderResourcesCheck(_Pipe->GetVertexShader());
	ShaderResourcesCheck(_Pipe->GetPixelShader());
}

void GameEngineShaderResHelper::ShaderResourcesCheck(GameEngineShader* _Shader) 
{
	for (auto& ConstantBuffer : _Shader->GetConstantBuffers())
	{
		GameEngineConstantBufferSetting* SettingData = new GameEngineConstantBufferSetting();
		SettingData->Shader = _Shader;
		SettingData->Res_ = ConstantBuffer.second;
		SettingData->SettingIndex_ = ConstantBuffer.first;
		auto Result = AllConstantBufferData_.insert(std::make_pair(ConstantBuffer.second->GetName(), SettingData));

		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("���� �̸��� ������۰� �̹� �����մϴ�." + ConstantBuffer.second->GetName());
		}
	}

	for (auto& Sampler : _Shader->GetSamplers())
	{
		GameEngineSamplerSetting* SettingData = new GameEngineSamplerSetting();
		SettingData->Shader = _Shader;
		SettingData->Res_ = Sampler.second;
		SettingData->SettingIndex_ = Sampler.first;
		auto Result = AllSamplerData_.insert(std::make_pair(Sampler.second->GetName(), SettingData));

		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("���� �̸��� ���÷��� �̹� �����մϴ�." + Sampler.second->GetName());
		}
	}

	GameEngineTexture* ErrorTexture = GameEngineTextureManager::GetInst().Find("NotSettingError.png");
	for (auto& Texture : _Shader->GetTextures())
	{
		GameEngineTextureSetting* SettingData = new GameEngineTextureSetting();
		SettingData->Shader = _Shader;
		SettingData->Res_ = ErrorTexture;
		SettingData->SettingIndex_ = Texture.first;
		auto Result = AllTextureData_.insert(std::make_pair(Texture.second, SettingData));

		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("���� �̸��� �ؽ�ó�� �̹� �����մϴ�." + Texture.second);
		}
	}
}

void GameEngineShaderResHelper::Setting() 
{
	for (auto& Setting : AllConstantBufferData_)
	{
		if (Setting.second->Mode_ == SettingMode::MAX)
		{
			GameEngineDebug::MsgBoxError("������ ������۰� ���õ��� �ʾҽ��ϴ�. >>> " + Setting.first);
		}
		
		Setting.second->ChangeData();
		Setting.second->ShaderSetting();
	}

	for (auto& Setting : AllTextureData_)
	{
		Setting.second->ShaderSetting();
	}

	for (auto& Setting : AllSamplerData_)
	{
		Setting.second->ShaderSetting();
	}
}

void GameEngineShaderResHelper::ReSet()
{
	for (auto& Setting : AllConstantBufferData_)
	{
		if (Setting.second->Mode_ == SettingMode::MAX)
		{
			GameEngineDebug::MsgBoxError("������ ������۰� ���õ��� �ʾҽ��ϴ�. >>> " + Setting.first);
		}

		Setting.second->ChangeData();
		Setting.second->ShaderReSet();
	}

	for (auto& Setting : AllTextureData_)
	{
		Setting.second->ShaderReSet();
	}

	for (auto& Setting : AllSamplerData_)
	{
		Setting.second->ShaderReSet();
	}
}

void GameEngineShaderResHelper::SettingTexture(const std::string& _SettingName, GameEngineTexture* _Texture) 
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineTextureSetting*>::iterator FindIter = AllTextureData_.find(UpperName);
	if (FindIter == AllTextureData_.end())
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ؽ�ó ���Կ� �����Ϸ��� �߽��ϴ�." + UpperName);
		return;
	}

	if (nullptr == _Texture)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�. >>> " + _Texture->GetName());
		return;
	}

	FindIter->second->Res_ = _Texture;
}

void GameEngineShaderResHelper::SettingTexture(const std::string& _SettingName, const std::string& _ImageName)
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineTextureSetting*>::iterator FindIter = AllTextureData_.find(UpperName);
	if (FindIter == AllTextureData_.end())
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ؽ�ó ���Կ� �����Ϸ��� �߽��ϴ�." + UpperName);
		return;
	}

	std::string UpperNameImage = GameEngineString::toupper(_ImageName);

	GameEngineTexture* FindTexture = GameEngineTextureManager::GetInst().Find(UpperNameImage);
	if (nullptr == FindTexture)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�. >>> " + UpperNameImage);
		return;
	}

	FindIter->second->Res_ = FindTexture;
}

void GameEngineShaderResHelper::SettingSampler(const std::string& _SettingName, const std::string& _Name) 
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineSamplerSetting*>::iterator FindIter = AllSamplerData_.find(UpperName);
	if (FindIter == AllSamplerData_.end())
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ؽ�ó ���Կ� �����Ϸ��� �߽��ϴ�." + UpperName);
		return;
	}

	std::string UpperNameSampler = GameEngineString::toupper(_Name);

	GameEngineSampler* FindSampler = GameEngineSamplerManager::GetInst().Find(UpperNameSampler);
	if (nullptr == FindSampler)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�. >>> " + UpperNameSampler);
		return;
	}

	FindIter->second->Res_ = FindSampler;
}

void GameEngineShaderResHelper::SettingSampler(const std::string& _SettingName, GameEngineSampler* _Value) 
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineSamplerSetting*>::iterator FindIter = AllSamplerData_.find(UpperName);
	if (FindIter == AllSamplerData_.end())
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���÷� ���Կ� �����Ϸ��� �߽��ϴ�." + UpperName);
		return;
	}

	if (nullptr == _Value)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ���÷��� �����Ϸ��� �߽��ϴ�. >>> " + _Value->GetName());
		return;
	}

	FindIter->second->Res_ = _Value;
}
