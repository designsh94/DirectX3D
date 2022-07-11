#pragma once
#include "GameEngineRenderingSettingData.h"
#include "GameEngineShader.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineRenderingPipeLine;
class GameEngineTexture;
class GameEngineSampler;
class GameEngineConstantBuffer;
class GameEngineStructuredBuffer;
class CameraComponent;
class GameEngineRenderer;
class GameEngineShaderResHelper
{
	friend CameraComponent;
	friend GameEngineRenderer;

private:
	std::map<std::string, GameEngineConstantBufferSetting> AllConstantBufferData_;
	std::map<std::string, GameEngineTextureSetting> AllTextureData_;
	std::map<std::string, GameEngineSamplerSetting> AllSamplerData_;
	std::map<std::string, GameEngineStructuredBufferSetting> AllStructuredBufferData_;

public:
	GameEngineShaderResHelper();
	~GameEngineShaderResHelper();

private:
	void Clear();

public:
	template<typename T>
	void SettingConstantBufferLink(const std::string& _SettingName, const T& _Data) 
	{
		std::string UpperName = GameEngineString::toupper(_SettingName);

		std::map<std::string, GameEngineConstantBufferSetting>::iterator FindIter = AllConstantBufferData_.find(UpperName);
		if (FindIter == AllConstantBufferData_.end())
		{
			GameEngineDebug::MsgBoxError("존재하지 않는 상수버퍼를 세팅하려고 했습니다." + UpperName);
			return;
		}

		GameEngineConstantBufferSetting& SettingData = FindIter->second;
		SettingData.Mode_ = SettingMode::Link;
		SettingData.SettingDataSize_ = sizeof(_Data);
		SettingData.SettingData_ = reinterpret_cast<const char*>(&_Data);
	}

	template<typename T>
	void SettingConstantBufferSet(const std::string& _SettingName, const T& _Data)
	{
		std::string UpperName = GameEngineString::toupper(_SettingName);

		std::map<std::string, GameEngineConstantBufferSetting>::iterator FindIter = AllConstantBufferData_.find(UpperName);
		if (FindIter == AllConstantBufferData_.end())
		{
			GameEngineDebug::MsgBoxError("존재하지 않는 상수버퍼를 세팅하려고 했습니다." + UpperName);
			return;
		}

		GameEngineConstantBufferSetting& SettingData = FindIter->second;
		SettingData.Mode_ = SettingMode::Set;
		SettingData.SettingDataSize_ = sizeof(_Data);
		if (nullptr == SettingData.NewData_)
		{
			SettingData.NewData_ = new char[sizeof(_Data)];
		}

		memcpy_s(SettingData.NewData_, sizeof(_Data), &_Data, sizeof(_Data));
		SettingData.SettingData_ = SettingData.NewData_;
	}

public:
	template<typename T>
	void SettingStructuredBufferLink(const std::string& _SettingName, const T& _Data)
	{
		std::string UpperName = GameEngineString::toupper(_SettingName);
		std::map<std::string, GameEngineStructuredBufferSetting>::iterator FindIter = AllStructuredBufferData_.find(UpperName);
		if (FindIter == AllStructuredBufferData_.end())
		{
			GameEngineDebug::MsgBoxError("존재하지 않는 구조화 버퍼를 세팅하려고 했습니다." + UpperName);
			return;
		}

		GameEngineStructuredBufferSetting& SettingData = FindIter->second;
		SettingData.Mode_ = SettingMode::Link;
		SettingData.SettingDataSize_ = sizeof(_Data);
		SettingData.SettingData_ = reinterpret_cast<const char*>(&_Data);
	}

	void SettingStructuredBufferLink(const std::string& _SettingName, const void* _Data, size_t _Size)
	{
		std::string UpperName = GameEngineString::toupper(_SettingName);
		std::map<std::string, GameEngineStructuredBufferSetting>::iterator FindIter = AllStructuredBufferData_.find(UpperName);
		if (FindIter == AllStructuredBufferData_.end())
		{
			GameEngineDebug::MsgBoxError("존재하지 않는 구조화 버퍼를 세팅하려고 했습니다." + UpperName);
			return;
		}

		GameEngineStructuredBufferSetting& SettingData = FindIter->second;
		SettingData.Mode_ = SettingMode::Link;
		SettingData.SettingDataSize_ = _Size;
		SettingData.SettingData_ = reinterpret_cast<const char*>(_Data);
	}

public:
	void ShaderResourcesCheck(GameEngineRenderingPipeLine* _Shader);
	void ShaderResourcesCheck(GameEngineShader* _Shader);

public:
	bool IsConstantBuffer(const std::string& _SettingName);
	bool IsStructuredBuffer(const std::string& _SettingName);

public:
	void SettingTexture(const std::string& _SettingName, const std::string& _ImageName);
	void SettingTexture(const std::string& _SettingName, GameEngineTexture* _Texture);

public:
	void SettingSampler(const std::string& _SettingName, const std::string& _Name);
	void SettingSampler(const std::string& _SettingName, GameEngineSampler* _Value);

public:
	void SettingStructuredBuffer(const std::string& _SettingName, GameEngineStructuredBuffer* _Buffer);

public:
	void Setting();
	void ReSet();

public:
};
