#include "PreCompile.h"
#include "EngineVertex.h"
#include <GameEngineBase/GameEngineDebug.h>

GameEngineLayOut GameEngineVertex::LayOut;

void GameEngineLayOut::AddInputLayOut(const char* _SemanticName, unsigned int _Index, unsigned int _AlignedByteOffset, DXGI_FORMAT _Format, unsigned int _InputSlot, unsigned int _InstanceDataStepRate, D3D11_INPUT_CLASSIFICATION _inputClass) 
{
	// �ø�ƾ�� ����
	SemanticName_.push_back(_SemanticName);

	// InputLayout ��������
	D3D11_INPUT_ELEMENT_DESC LayOutDesc = { 0, };

	LayOutDesc.SemanticName = _SemanticName;
	LayOutDesc.SemanticIndex = _Index;
	LayOutDesc.Format = _Format;
	LayOutDesc.InputSlot = _InputSlot;
	LayOutDesc.InstanceDataStepRate = _InstanceDataStepRate;
	LayOutDesc.AlignedByteOffset = LayOutOffset_;				// �����ּ� ����
	LayOutOffset_ += _AlignedByteOffset;						// �ش� ��ǲ���̾ƿ� ũ�⸦ ������ �� �ּ����� ����

	InputLayoutDesc_.push_back(LayOutDesc);
}

ID3D11InputLayout* GameEngineLayOut::CreateLayOut(GameEngineShader* _Shader) 
{
	ID3D11InputLayout* LayOut_;

	if (S_OK != GameEngineDevice::GetInst().GetDevice()->CreateInputLayout(&InputLayoutDesc_[0], static_cast<unsigned int>(InputLayoutDesc_.size()), _Shader->CodeBlob_->GetBufferPointer(), _Shader->CodeBlob_->GetBufferSize(), &LayOut_))
	{
		GameEngineDebug::MsgBoxError("��ǲ���̾ƿ� ������ �����߽��ϴ�.");
		return nullptr;
	}

	return LayOut_;
}
