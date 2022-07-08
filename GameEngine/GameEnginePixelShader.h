#pragma once
#include "GameEngineDevice.h"
#include "GameEngineShader.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineTextureSetting;
class GameEnginePixelShader : public GameEngineShader
{
private:
	ID3D11PixelShader* Shader_;

public:
	GameEnginePixelShader();
	~GameEnginePixelShader();

protected:
	GameEnginePixelShader(const GameEnginePixelShader& _Other) = delete;
	GameEnginePixelShader(GameEnginePixelShader&& _Other) noexcept = delete;

private:
	GameEnginePixelShader& operator=(const GameEnginePixelShader& _Other) = delete;
	GameEnginePixelShader& operator=(GameEnginePixelShader&& _Other) noexcept = delete;

private:
	void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;
	void SetTexture(const GameEngineTextureSetting* _Setting) override;
	void SetSampler(const GameEngineSamplerSetting* _Setting) override;
	void SetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) override;

private:
	void ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;
	void ReSetTexture(const GameEngineTextureSetting* _Setting) override;
	void ReSetSampler(const GameEngineSamplerSetting* _Setting) override;
	void ReSetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) override;

public:
	bool Create(const std::string& _ShaderCode, const std::string& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);
	bool Load(const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);

public:
	bool StringCompile();
	bool FileCompile(const std::string& _Path);

public:
	void Setting();
};

