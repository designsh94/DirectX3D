#include "PreCompile.h"
#include "GameEngineShader.h"
#include "GameEngineConstantBufferManager.h"
#include "GameEngineResourcesManager.h"

GameEngineShader::GameEngineShader(ShaderType _Type) :
	VersionHigh_(5),
	VersionLow_(0),
	Type_(_Type)
{
}

GameEngineShader::~GameEngineShader() 
{
}

void GameEngineShader::SetVersion(UINT _VersionHigh, UINT _VersionLow)
{
	VersionHigh_ = _VersionHigh;
	VersionLow_ = _VersionLow;
}

void GameEngineShader::CreateVersion(const std::string& _ShaderType)
{
	Version_ = "";
	Version_ += _ShaderType + "_";
	Version_ += std::to_string(VersionHigh_);
	Version_ += "_";
	Version_ += std::to_string(VersionLow_);
}

void GameEngineShader::SetCode(const std::string& _Code)
{
	Code_ = _Code;
}

void GameEngineShader::SetEntryPoint(const std::string& _EntryPoint)
{
	EntryPoint_ = _EntryPoint;
}

void GameEngineShader::ResCheck() 
{
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

	D3D11_SHADER_INPUT_BIND_DESC ResInfo;

	for (unsigned int i = 0; i < Info.BoundResources; i++)
	{
		CompilInfo->GetResourceBindingDesc(i, &ResInfo);

		std::string Name = GameEngineString::toupper(ResInfo.Name);
		unsigned int BindPoint = ResInfo.BindPoint;
		D3D_SHADER_INPUT_TYPE Type = ResInfo.Type;

		switch (Type)
		{
			case D3D_SIT_CBUFFER:
			{
				ID3D11ShaderReflectionConstantBuffer* Buffer = CompilInfo->GetConstantBufferByName(ResInfo.Name);

				D3D11_SHADER_BUFFER_DESC BufferDesc;
				Buffer->GetDesc(&BufferDesc);

				GameEngineConstantBuffer* NewBuffer = GameEngineConstantBufferManager::GetInst().CreateAndFind(Name, BufferDesc, Buffer);

				if (BufferDesc.Size != NewBuffer->GetBufferSize())
				{
					GameEngineDebug::MsgBoxError("구조가 다른 상수버퍼가 존재합니다.");
					return;
				}

				GameEngineConstantBufferSetting Setting;
				Setting.SetName(Name);
				Setting.SettingIndex_ = ResInfo.BindPoint;
				Setting.Res_ = NewBuffer;

				ConstantBuffers_.insert(std::make_pair(Name, Setting));
				break;
			}
			case D3D_SIT_SAMPLER:
			{
				if (nullptr == GameEngineSamplerManager::GetInst().Find(Name))
				{
					D3D11_SAMPLER_DESC Smp_Decs = {};
					memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));

					std::string CheckName = GameEngineString::toupper(Name);

					Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

					if (std::string::npos != CheckName.find("POINT"))
					{
						Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
					}
					else if (std::string::npos != CheckName.find("LINEAR"))
					{
						Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
					}

					Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
					Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
					Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

					Smp_Decs.MipLODBias = 0.0f;
					Smp_Decs.MaxAnisotropy = 1;
					Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
					Smp_Decs.MinLOD = -FLT_MAX;
					Smp_Decs.MaxLOD = FLT_MAX;
					GameEngineSamplerManager::GetInst().Create(Name, Smp_Decs);
				}

				GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().Find(Name);

				GameEngineSamplerSetting Setting;
				Setting.SetName(Name);
				Setting.SettingIndex_ = ResInfo.BindPoint;
				Setting.Res_ = NewRes;

				Samplers_.insert(std::make_pair(Name, Setting));
				break;
			}
			case D3D_SIT_TEXTURE:
			{
				GameEngineTextureSetting Setting;
				Setting.SetName(Name);
				Setting.SettingIndex_ = ResInfo.BindPoint;
				Setting.Res_ = GameEngineTextureManager::GetInst().Find("NotSettingError.png");

				Textures_.insert(std::make_pair(Name, Setting));
				break;
			}
			case D3D_SIT_STRUCTURED:
			{
				GameEngineStructuredBufferSetting Setting;
				Setting.SetName(Name);
				Setting.SettingIndex_ = ResInfo.BindPoint;
				Setting.Res_ = nullptr;

				StructuredBuffers_.insert(std::make_pair(Name, Setting));
				break;
			}
			default:
			{
				GameEngineDebug::MsgBoxError("처리하지 못하는 타입의 쉐이더 리소스가 발견되었습니다");
				break;
			}
		}
	}
}