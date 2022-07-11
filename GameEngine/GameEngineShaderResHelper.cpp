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
}

void GameEngineShaderResHelper::Clear()
{
}

bool GameEngineShaderResHelper::IsConstantBuffer(const std::string& _SettingName)
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineConstantBufferSetting>::iterator FindIter = AllConstantBufferData_.find(UpperName);
	if (FindIter == AllConstantBufferData_.end())
	{
		return false;
	}

	return true;
}

bool GameEngineShaderResHelper::IsStructuredBuffer(const std::string& _SettingName)
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineStructuredBufferSetting>::iterator FindIter = AllStructuredBufferData_.find(UpperName);
	if (FindIter == AllStructuredBufferData_.end())
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
		GameEngineConstantBufferSetting SettingData = ConstantBuffer.second;
		SettingData.Shader = _Shader;
		auto Result = AllConstantBufferData_.insert(std::make_pair(SettingData.GetName(), SettingData));
		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("같은 이름의 상수버퍼가 이미 존재합니다." + SettingData.GetName());
		}
	}

	for (auto& Sampler : _Shader->GetSamplers())
	{
		GameEngineSamplerSetting SettingData = Sampler.second;
		SettingData.Shader = _Shader;
		auto Result = AllSamplerData_.insert(std::make_pair(SettingData.GetName(), SettingData));
		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("같은 이름의 샘플러가 이미 존재합니다." + SettingData.GetName());
		}
	}

	for (auto& Texture : _Shader->GetTextures())
	{
		GameEngineTextureSetting SettingData = Texture.second;
		SettingData.Shader = _Shader;
		auto Result = AllTextureData_.insert(std::make_pair(SettingData.GetName(), SettingData));
		if (false == Result.second)
		{
			GameEngineDebug::MsgBoxError("같은 이름의 텍스처가 이미 존재합니다." + SettingData.GetName());
		}
	}

	for (auto& StructuredBuffers : _Shader->GetStructuredBuffers())
	{
		GameEngineStructuredBufferSetting SettingData = StructuredBuffers.second;
		SettingData.Shader = _Shader;
		auto Result = AllStructuredBufferData_.insert(std::make_pair(SettingData.GetName(), SettingData));
		if (false == Result.second)
		{
			 GameEngineDebug::MsgBoxError("같은 이름의 구조화버퍼가 이미 존재합니다." + SettingData.GetName());
		}
	}
}

void GameEngineShaderResHelper::SettingTexture(const std::string& _SettingName, GameEngineTexture* _Texture) 
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineTextureSetting>::iterator FindIter = AllTextureData_.find(UpperName);
	if (FindIter == AllTextureData_.end())
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 텍스처 슬롯에 세팅하려고 했습니다." + UpperName);
		return;
	}

	if (nullptr == _Texture)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 텍스처를 세팅하려고 했습니다. >>> " + _Texture->GetName());
		return;
	}

	FindIter->second.Res_ = _Texture;
}

void GameEngineShaderResHelper::SettingTexture(const std::string& _SettingName, const std::string& _ImageName)
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineTextureSetting>::iterator FindIter = AllTextureData_.find(UpperName);
	if (FindIter == AllTextureData_.end())
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 텍스처 슬롯에 세팅하려고 했습니다." + UpperName);
		return;
	}

	std::string UpperNameImage = GameEngineString::toupper(_ImageName);

	GameEngineTexture* FindTexture = GameEngineTextureManager::GetInst().Find(UpperNameImage);
	if (nullptr == FindTexture)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 텍스처를 세팅하려고 했습니다. >>> " + UpperNameImage);
		return;
	}

	FindIter->second.Res_ = FindTexture;
}

void GameEngineShaderResHelper::SettingSampler(const std::string& _SettingName, const std::string& _Name) 
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineSamplerSetting>::iterator FindIter = AllSamplerData_.find(UpperName);
	if (FindIter == AllSamplerData_.end())
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 텍스처 슬롯에 세팅하려고 했습니다." + UpperName);
		return;
	}

	std::string UpperNameSampler = GameEngineString::toupper(_Name);

	GameEngineSampler* FindSampler = GameEngineSamplerManager::GetInst().Find(UpperNameSampler);
	if (nullptr == FindSampler)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 텍스처를 세팅하려고 했습니다. >>> " + UpperNameSampler);
		return;
	}

	FindIter->second.Res_ = FindSampler;
}

void GameEngineShaderResHelper::SettingSampler(const std::string& _SettingName, GameEngineSampler* _Value) 
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineSamplerSetting>::iterator FindIter = AllSamplerData_.find(UpperName);
	if (FindIter == AllSamplerData_.end())
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 샘플러 슬롯에 세팅하려고 했습니다." + UpperName);
		return;
	}

	if (nullptr == _Value)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 샘플러를 세팅하려고 했습니다. >>> " + _Value->GetName());
		return;
	}

	FindIter->second.Res_ = _Value;
}

void GameEngineShaderResHelper::SettingStructuredBuffer(const std::string& _SettingName, GameEngineStructuredBuffer* _Buffer)
{
	std::string UpperName = GameEngineString::toupper(_SettingName);

	std::map<std::string, GameEngineStructuredBufferSetting>::iterator FindIter = AllStructuredBufferData_.find(UpperName);
	if (FindIter == AllStructuredBufferData_.end())
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 스트럭처드 버퍼 슬롯에 세팅하려고 했습니다." + UpperName);
		return;
	}

	if (nullptr == _Buffer)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 스트럭처드 버퍼를 세팅하려고 했습니다. >>> " + _Buffer->GetName());
		return;
	}

	FindIter->second.Res_ = _Buffer;
}

void GameEngineShaderResHelper::Setting()
{
	for (auto& Setting : AllConstantBufferData_)
	{
		if (Setting.second.Mode_ == SettingMode::MAX)
		{
			GameEngineDebug::MsgBoxError("다음의 상수버퍼가 세팅되지 않았습니다. >>> " + Setting.first);
		}

		Setting.second.ChangeData();
		Setting.second.ShaderSetting();
	}

	for (auto& Setting : AllTextureData_)
	{
		Setting.second.ShaderSetting();
	}

	for (auto& Setting : AllSamplerData_)
	{
		Setting.second.ShaderSetting();
	}

	for (auto& Setting : AllStructuredBufferData_)
	{
		Setting.second.ChangeData();
		Setting.second.ShaderSetting();
	}
}

void GameEngineShaderResHelper::ReSet()
{
	for (auto& Setting : AllConstantBufferData_)
	{
		if (Setting.second.Mode_ == SettingMode::MAX)
		{
			GameEngineDebug::MsgBoxError("다음의 상수버퍼가 세팅되지 않았습니다. >>> " + Setting.first);
		}

		Setting.second.ChangeData();
		Setting.second.ShaderReSet();
	}

	for (auto& Setting : AllTextureData_)
	{
		Setting.second.ShaderReSet();
	}

	for (auto& Setting : AllSamplerData_)
	{
		Setting.second.ShaderReSet();
	}
}
