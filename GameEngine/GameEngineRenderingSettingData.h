#pragma once
#include "GameEngineShader.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"
#include "GameEngineStructuredBuffer.h"

enum class SettingMode
{
	Link,
	Set,
	MAX,
};

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineShader;
class GameEngineConstantBuffer;
class GameEngineConstantBufferSetting : public GameEngineObjectNameBase
{
public:
	GameEngineShader* Shader;
	GameEngineConstantBuffer* Res_;
	SettingMode Mode_;
	const char* SettingData_;
	char* NewData_;
	size_t SettingDataSize_;
	int SettingIndex_;

public:
	GameEngineConstantBufferSetting() :
		SettingData_(nullptr), 
		Mode_(SettingMode::MAX),
		Res_(nullptr),
		SettingDataSize_(0),
		SettingIndex_(0),
		Shader(nullptr),
		NewData_(nullptr)
	{
	}

	~GameEngineConstantBufferSetting() 
	{
		Clear();
	}

public:
	void Clear()
	{
		if (nullptr != NewData_)
		{
			delete NewData_;
		}

		NewData_ = nullptr;
		SettingData_ = nullptr;
	}

	void ChangeData()
	{
		Res_->ChangeData(SettingData_, SettingDataSize_);
	}

	void ShaderSetting()
	{
		Shader->SetConstantBuffers(this);
	}

	void ShaderReSet()
	{
		Shader->ReSetConstantBuffers(this);
	}
};

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineShader;
class GameEngineSampler;
class GameEngineSamplerSetting : public GameEngineObjectNameBase
{
public:
	GameEngineShader* Shader;
	GameEngineSampler* Res_;
	int SettingIndex_;

public:
	GameEngineSamplerSetting() :
		Res_(nullptr),
		SettingIndex_(0),
		Shader(nullptr)
	{
	}

	~GameEngineSamplerSetting()
	{
	}

public:
	void ShaderSetting()
	{
		Shader->SetSampler(this);
	}

	void ShaderReSet()
	{
		Shader->ReSetSampler(this);
	}
};

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineShader;
class GameEngineTexture;
class GameEngineTextureSetting : public GameEngineObjectNameBase
{
public:
	GameEngineShader* Shader;
	GameEngineTexture* Res_;
	int SettingIndex_;

public:
	GameEngineTextureSetting() :
		Res_(nullptr),
		Shader(nullptr),
		SettingIndex_(0)
	{
	}

	~GameEngineTextureSetting()
	{
	}

public:
	void ShaderSetting()
	{
		Shader->SetTexture(this);
	}

	void ShaderReSet()
	{
		Shader->ReSetTexture(this);
	}
};

class GameEngineShader;
class GameEngineStructuredBuffer;
class GameEngineStructuredBufferSetting : public GameEngineObjectNameBase
{
private:
	static const char* LastSettingAddress;

public:
	static void ResetLastSetting()
	{
		LastSettingAddress = nullptr;
	}

public:
	GameEngineShader* Shader;
	GameEngineStructuredBuffer* Res_;
	int SettingIndex_;
	SettingMode Mode_;
	const char* SettingData_;
	char* NewData_;
	size_t SettingDataSize_;

public:
	GameEngineStructuredBufferSetting() :
		Shader(nullptr),
		Res_(nullptr),
		SettingIndex_(0),
		Mode_(SettingMode::MAX),
		SettingData_(nullptr),
		NewData_(nullptr),
		SettingDataSize_()
	{

	}

	~GameEngineStructuredBufferSetting()
	{
		Clear();
	}

public:
	void ShaderSetting()
	{
		Shader->SetStructuredBuffer(this);
	}

	void ShaderReSet()
	{
		Shader->ReSetStructuredBuffer(this);
	}

	void Clear()
	{
		if (nullptr != NewData_)
		{
			delete NewData_;
		}

		NewData_ = nullptr;
		SettingData_ = nullptr;
	}

	void ChangeData()
	{
		if (SettingData_ != LastSettingAddress)
		{
			LastSettingAddress = SettingData_;
			Res_->ChangeData(SettingData_, SettingDataSize_);
		}
	}
};
