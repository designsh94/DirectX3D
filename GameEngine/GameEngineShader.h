#pragma once
#include "GameEngineDevice.h"
#include "GameEngineConstantBuffer.h"

enum class ShaderType
{
	VS,
	PS,
	MAX
};

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineSampler;
class GameEngineTexture;
class GameEngineConstantBuffer;
class GameEngineSamplerSetting;
class GameEngineTextureSetting;
class GameEngineConstantBufferSetting;
class GameEngineShader : public GameEngineObjectNameBase 
{
	friend class GameEngineLayOut;

private:
	std::map<unsigned int, GameEngineConstantBuffer*> ConstanceBuffers_;
	std::map<unsigned int, GameEngineSampler*> Samplers_;
	std::map<unsigned int, std::string> Textures_;

protected:
	UINT VersionHigh_;
	UINT VersionLow_;
	ID3DBlob* CodeBlob_;
	std::string Version_;
	std::string EntryPoint_;
	std::string Code_;
	ShaderType Type_;

public:
	GameEngineShader(ShaderType _Type);
	virtual ~GameEngineShader() = 0;

protected:
	GameEngineShader(const GameEngineShader& _Other) = delete;
	GameEngineShader(GameEngineShader&& _Other) noexcept = delete;

private:
	GameEngineShader& operator=(const GameEngineShader& _Other) = delete;
	GameEngineShader& operator=(GameEngineShader&& _Other) noexcept = delete;

public:
	unsigned int GetTypeIndex()
	{
		return static_cast<unsigned int>(Type_);
	}

public:
	std::map<unsigned int, GameEngineConstantBuffer*>& GetConstantBuffers() 
	{
		return ConstanceBuffers_;
	}

	std::map<unsigned int, GameEngineSampler*>& GetSamplers()
	{
		return Samplers_;
	}

	std::map<unsigned int, std::string>& GetTextures()
	{
		return Textures_;
	}

protected:
	void SetVersion(UINT _VersionHigh, UINT _VersionLow);
	void CreateVersion(const std::string& _ShaderType);
	void SetCode(const std::string& _Code);
	void SetEntryPoint(const std::string& _EntryPoint);

public:
	void ResCheck();

public:
	virtual void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) = 0;
	virtual void SetTexture(const GameEngineTextureSetting* _Setting) = 0;
	virtual void SetSampler(const GameEngineSamplerSetting* _Setting) = 0;

	virtual void ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) = 0;
	virtual void ReSetTexture(const GameEngineTextureSetting* _Setting) = 0;
	virtual void ReSetSampler(const GameEngineSamplerSetting* _Setting) = 0;
};

