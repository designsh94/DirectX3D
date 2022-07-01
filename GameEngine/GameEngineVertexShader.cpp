#include "PreCompile.h"
#include "GameEngineVertexShader.h"
#include "GameEngineShaderResHelper.h"

GameEngineVertexShader::GameEngineVertexShader() :
	GameEngineShader(ShaderType::VS)
{
}

GameEngineVertexShader::~GameEngineVertexShader()
{
	if (nullptr != LayOut_)
	{
		LayOut_->Release();
		LayOut_ = nullptr;
	}

	if (nullptr != Shader_)
	{
		Shader_->Release();
		Shader_ = nullptr;
	}

	if (nullptr != CodeBlob_)
	{
		CodeBlob_->Release();
		CodeBlob_ = nullptr;
	}

}

void GameEngineVertexShader::LayOutCheck()
{
	LayOutClear();

	if (nullptr == CodeBlob_)
	{
		return;
	}

	ID3D11ShaderReflection* CompilInfo = {};

	if (S_OK != D3DReflect(CodeBlob_->GetBufferPointer(), CodeBlob_->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&CompilInfo)))
	{
		GameEngineDebug::MsgBoxError("쉐이더 컴파일 정보를 얻어오지 못했습니다.");
		return;
	}

	D3D11_SHADER_DESC Info;

	CompilInfo->GetDesc(&Info);

	std::string Name = "";
	int PrevIndex = 0;
	for (unsigned int i = 0; i < Info.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC Input = { 0, };
		CompilInfo->GetInputParameterDesc(i, &Input);

		DXGI_FORMAT Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		D3D_REGISTER_COMPONENT_TYPE Reg = Input.ComponentType;

		unsigned int ParameterSize = 0;

		switch (Input.Mask)
		{
			case 1:
			{
				ParameterSize = 4;
				switch (Reg)
				{
					case D3D_REGISTER_COMPONENT_UNKNOWN:
					{
						break;
					}
					case D3D_REGISTER_COMPONENT_UINT32: // unsigned int형 정보라는 뜻
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
						break;
					}
					case D3D_REGISTER_COMPONENT_SINT32: // int형 정보라는 뜻
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
						break;
					}
					case D3D_REGISTER_COMPONENT_FLOAT32: // float형 정보라는 뜻
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			case 3:
			{
				ParameterSize = 8;
				switch (Reg)
				{
					case D3D_REGISTER_COMPONENT_UNKNOWN:
					{
						break;
					}
					case D3D_REGISTER_COMPONENT_UINT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
						break;
					}
					case D3D_REGISTER_COMPONENT_SINT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_SINT;
						break;
					}
					case D3D_REGISTER_COMPONENT_FLOAT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			case 7:
			{
				ParameterSize = 12;
				switch (Reg)
				{
					case D3D_REGISTER_COMPONENT_UNKNOWN:
					{
						break;
					}
					case D3D_REGISTER_COMPONENT_UINT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
						break;
					}
					case D3D_REGISTER_COMPONENT_SINT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_SINT;
						break;
					}
					case D3D_REGISTER_COMPONENT_FLOAT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			case 15:
			{
				ParameterSize = 16;
				switch (Reg)
				{
					case D3D_REGISTER_COMPONENT_UNKNOWN:
					{
						break;
					}
					case D3D_REGISTER_COMPONENT_UINT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
						break;
					}
					case D3D_REGISTER_COMPONENT_SINT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT;
						break;
					}
					case D3D_REGISTER_COMPONENT_FLOAT32:
					{
						Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			default:
			{
				break;
			}
		}

		std::string NextName = Input.SemanticName;

		if (Name == "")
		{
			Name = NextName;
			++PrevIndex;
		}
		else
		{
			if (Name == NextName)
			{
				if (PrevIndex != Input.SemanticIndex)
				{
					GameEngineDebug::MsgBoxError("시맨틱의 인덱스 순서가 잘못되었습니다 오름차순이 아닙니다..");
					return;
				}

				++PrevIndex;
			}
			else
			{
				Name = NextName;
				PrevIndex = 0;
			}
		}

		if (DXGI_FORMAT::DXGI_FORMAT_UNKNOWN == Format)
		{
			GameEngineDebug::MsgBoxError("쉐이더 인풋 파라미터 타입이 정상적이지 않습니다.");
		}

		AddInputLayOut(Input.SemanticName, Input.SemanticIndex, ParameterSize, Format, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);
	}

	CreateLayOut();
}

void GameEngineVertexShader::LayOutClear()
{
	if (nullptr != LayOut_)
	{
		LayOut_->Release();
	}

	LayOut_ = nullptr;
	InputLayoutDesc_.clear();
	SemanticName_.clear();
	LayOutOffset_ = 0;
}

bool GameEngineVertexShader::Create(const std::string& _ShaderCode, const std::string& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	SetVersion(_VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);
	SetCode(_ShaderCode);
	CreateVersion("vs");

	return StringCompile();
}

bool GameEngineVertexShader::Load(const std::string& _Path, const std::string& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	SetVersion(_VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);
	CreateVersion("vs");

	return FileCompile(_Path);
}

bool GameEngineVertexShader::StringCompile()
{
	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif
	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	ID3DBlob* ResultBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;

	if (S_OK != D3DCompile(Code_.c_str(), Code_.size(), "", nullptr, nullptr, EntryPoint_.c_str(), Version_.c_str(), Flag, 0, &ResultBlob, &ErrorBlob))
	{
		std::string ErrorText = (char*)ErrorBlob->GetBufferPointer();
		GameEngineDebug::MsgBox(GetName() + " 컴파일 도중 에러가 발생했습니다.");
		GameEngineDebug::MsgBox(ErrorText);

		return false;
	}

	CodeBlob_ = ResultBlob;

	if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader(CodeBlob_->GetBufferPointer(), CodeBlob_->GetBufferSize(), nullptr, &Shader_))
	{
		GameEngineDebug::MsgBoxError("버텍스 버퍼 생성에 실패했습니다.");

		return false;
	}

	LayOutCheck();
	ResCheck();

	return true;
}

bool GameEngineVertexShader::FileCompile(const std::string& _Path)
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

	if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader(CodeBlob_->GetBufferPointer(), CodeBlob_->GetBufferSize(), nullptr, &Shader_))
	{
		GameEngineDebug::MsgBoxError("버텍스 버퍼 생성에 실패했습니다.");

		return false;
	}

	LayOutCheck();
	ResCheck();

	return true;
}

void GameEngineVertexShader::SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting)
{
	GameEngineDevice::GetContext()->VSSetConstantBuffers(_Setting->SettingIndex_, 1, &_Setting->Res_->GetBuffer());
}

void GameEngineVertexShader::SetTexture(const GameEngineTextureSetting* _Setting)
{
	GameEngineDevice::GetContext()->VSSetShaderResources(_Setting->SettingIndex_, 1, _Setting->Res_->GetShaderResourcesView());
}

void GameEngineVertexShader::SetSampler(const GameEngineSamplerSetting* _Setting)
{
	GameEngineDevice::GetContext()->VSSetSamplers(_Setting->SettingIndex_, 1, _Setting->Res_->GetSamplerState());
}

void GameEngineVertexShader::ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting)
{
	static ID3D11Buffer* const ReSetting[16] = { nullptr };
	GameEngineDevice::GetContext()->VSSetConstantBuffers(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEngineVertexShader::ReSetTexture(const GameEngineTextureSetting* _Setting)
{
	static ID3D11ShaderResourceView* ReSetting[16] = { nullptr };
	GameEngineDevice::GetContext()->VSSetShaderResources(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEngineVertexShader::ReSetSampler(const GameEngineSamplerSetting* _Setting)
{
	static ID3D11SamplerState* const ReSetting[16] = { nullptr };
	GameEngineDevice::GetContext()->VSSetSamplers(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEngineVertexShader::LayOutReset()
{
	SemanticName_.clear();
	InputLayoutDesc_.clear();
}

void GameEngineVertexShader::CreateLayOut()
{
	if (nullptr != LayOut_)
	{
		LayOut_->Release();
		LayOut_ = nullptr;
	}

	if (S_OK != GameEngineDevice::GetInst().GetDevice()->CreateInputLayout(&InputLayoutDesc_[0], static_cast<unsigned int>(InputLayoutDesc_.size()), CodeBlob_->GetBufferPointer(), CodeBlob_->GetBufferSize(), &LayOut_))
	{
		GameEngineDebug::MsgBoxError("인풋레이아웃 생성에 실패했습니다.");
		return;
	}
}

void GameEngineVertexShader::AddInputLayOut(const char* _SemanticName, unsigned int _Index, unsigned int _AlignedByteOffset, DXGI_FORMAT _Format, unsigned int _InputSlot, unsigned int _InstanceDataStepRate, D3D11_INPUT_CLASSIFICATION _inputClass)
{
	SemanticName_.push_back(_SemanticName);

	D3D11_INPUT_ELEMENT_DESC LayOutDesc = { 0, };

	LayOutDesc.SemanticName = _SemanticName;
	LayOutDesc.SemanticIndex = _Index;
	LayOutDesc.Format = _Format;
	LayOutDesc.InputSlot = _InputSlot;
	LayOutDesc.InstanceDataStepRate = _InstanceDataStepRate;
	LayOutDesc.AlignedByteOffset = LayOutOffset_;

	LayOutOffset_ += _AlignedByteOffset;

	InputLayoutDesc_.push_back(LayOutDesc);
}

void GameEngineVertexShader::InputLayOutSetting()
{
	if (nullptr == LayOut_)
	{
		GameEngineDebug::MsgBoxError("쉐이더 인풋 파라미터가 존재하지 않습니다.");
	}

	GameEngineDevice::GetInst().GetContext()->IASetInputLayout(LayOut_);
}

void GameEngineVertexShader::Setting()
{
	GameEngineDevice::GetInst().GetContext()->VSSetShader(Shader_, nullptr, 0);
}
