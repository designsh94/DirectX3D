#include "PreCompile.h"
#include "GameEnginePixelShader.h"
#include "GameEngineShaderResHelper.h"

GameEnginePixelShader::GameEnginePixelShader() :
	GameEngineShader(ShaderType::PS)
{
}

GameEnginePixelShader::~GameEnginePixelShader() 
{
	if (nullptr != Shader_)
	{
		Shader_->Release();
		Shader_ = nullptr;
	}
}

void GameEnginePixelShader::SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting)
{
	GameEngineDevice::GetContext()->PSSetConstantBuffers(_Setting->SettingIndex_, 1, &_Setting->Res_->GetBuffer());
}

void GameEnginePixelShader::SetTexture(const GameEngineTextureSetting* _Setting) 
{
	GameEngineDevice::GetContext()->PSSetShaderResources(_Setting->SettingIndex_, 1, _Setting->Res_->GetShaderResourcesView());
}

void GameEnginePixelShader::SetSampler(const GameEngineSamplerSetting* _Setting) 
{
	GameEngineDevice::GetContext()->PSSetSamplers(_Setting->SettingIndex_, 1, _Setting->Res_->GetSamplerState());
}

void GameEnginePixelShader::ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting)
{
	static ID3D11Buffer* const ReSetting[1] = { nullptr };
	GameEngineDevice::GetContext()->PSSetConstantBuffers(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEnginePixelShader::ReSetTexture(const GameEngineTextureSetting* _Setting)
{
	static ID3D11ShaderResourceView* ReSetting[1] = { nullptr };

	GameEngineDevice::GetContext()->PSSetShaderResources(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEnginePixelShader::ReSetSampler(const GameEngineSamplerSetting* _Setting)
{
	static ID3D11SamplerState* const ReSetting[1] = { nullptr };

	GameEngineDevice::GetContext()->PSSetSamplers(_Setting->SettingIndex_, 1, ReSetting);
}

bool GameEnginePixelShader::Create(const std::string& _ShaderCode, const std::string& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	SetVersion(_VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);
	SetCode(_ShaderCode);
	CreateVersion("ps");

	return StringCompile();
}

bool GameEnginePixelShader::Load(const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	SetVersion(_VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);
	CreateVersion("ps");

	return FileCompile(_Path);
}

bool GameEnginePixelShader::StringCompile()
{
	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif
	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	ID3DBlob* ResultBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;

	if (S_OK != D3DCompile(Code_.c_str(), Code_.size(), "", nullptr, nullptr, EntryPoint_.c_str(), Version_.c_str(), 0, 0, &ResultBlob, &ErrorBlob))
	{
		std::string ErrorText = (char*)ErrorBlob->GetBufferPointer();
		GameEngineDebug::MsgBox(GetName() + " 컴파일 도중 에러가 발생했습니다.");
		GameEngineDebug::MsgBox(ErrorText);

		return false;
	}

	CodeBlob_ = ResultBlob;

	if (S_OK != GameEngineDevice::GetDevice()->CreatePixelShader(CodeBlob_->GetBufferPointer(), CodeBlob_->GetBufferSize(), nullptr, &Shader_))
	{
		GameEngineDebug::MsgBoxError("버텍스 버퍼 생성에 실패했습니다.");

		return false;
	}

	ResCheck();

	return true;
}

bool GameEnginePixelShader::FileCompile(const std::string& _Path)
{
	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif
	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	ID3DBlob* ResultBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;

	std::wstring Path;
	GameEngineString::AnsiToUnicode(_Path, Path);

	if (S_OK != D3DCompileFromFile(Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint_.c_str(), Version_.c_str(), Flag, 0, &ResultBlob, &ErrorBlob))
	{
		std::string ErrorText = (char*)ErrorBlob->GetBufferPointer();

		GameEngineDebug::MsgBox(GetName() + " 컴파일 도중 에러가 발생했습니다.");
		GameEngineDebug::MsgBox(ErrorText);

		return false;
	}

	CodeBlob_ = ResultBlob;
	if (S_OK != GameEngineDevice::GetDevice()->CreatePixelShader(CodeBlob_->GetBufferPointer(), CodeBlob_->GetBufferSize(), nullptr, &Shader_))
	{
		GameEngineDebug::MsgBoxError("버텍스 버퍼 생성에 실패했습니다.");

		return false;
	}

	ResCheck();

	return true;
}

void GameEnginePixelShader::Setting()
{
	GameEngineDevice::GetInst().GetContext()->PSSetShader(Shader_, nullptr, 0);
}
