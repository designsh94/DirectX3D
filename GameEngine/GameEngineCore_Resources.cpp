#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineWindow.h"
#include "GameEngineResourcesManager.h"
#include "GameEngineCollision.h"
#include "GameEngineBase/GameEngineDirectory.h"
#include "GameEngineBase/GameEngineFile.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineDepthStencil.h"
#include "EngineVertex.h"

void GameEngineCore::EngineResourcesCreate_Mesh() 
{
	GameEngineVertex::LayOut.AddInputLayOut("POSITION", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("TEXTURECOORD", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("COLOR", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("NORMAL", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("TANGENT", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("BINORMAL", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("BLENDWEIGHT", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("BLENDINDICES", 0, 16, DXGI_FORMAT_R32G32B32A32_SINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);

	// Sphere
	{
		GameEngineVertex V;
		std::vector<GameEngineVertex> VBVector;
		std::vector<UINT> IBVector;

		float Radius = 0.5f;
		// 북극점부터 시작합니다.
		V.POSITION = float4(0.0f, Radius, 0.0f, 1.0f);
		V.TEXTURECOORD = float4(0.5f, 0.0f);
		// 노말 백터 혹은 법선백터라고 불리며
		// 면에 수직인 벡터를 의미하게 된다.
		// 빛을 반사할때 필수.
		V.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize3D();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, 1.0f, 0.0f);

		VBVector.push_back(V);

		UINT iStackCount = 40; // 가로 분할 개수입니다.
		UINT iSliceCount = 40; // 세로분할 개수

		float yRotAngle = GameEngineMath::PI / (float)iStackCount;
		float zRotAngle = (GameEngineMath::PI * 2) / (float)iSliceCount;

		// UV의 가로세로 간격값을 구한다.
		float yUvRatio = 1.0f / (float)iStackCount;
		float zUvRatio = 1.0f / (float)iStackCount;

		for (UINT y = 1; y < iStackCount; ++y)
		{
			// 각 간격에 대한 각도값
			float phi = y * yRotAngle;
			for (UINT z = 0; z < iSliceCount + 1; ++z)
			{
				float theta = z * zRotAngle;
				V.POSITION = float4{
					Radius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
					Radius * cosf(y * yRotAngle),
					Radius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
					1.0f // 위치 크기 값에 영향을 주기 위해서
				};

				// V.Pos *= GameEngineRandom::RandomFloat(-0.9f, 0.1f);

				V.TEXTURECOORD = float4(yUvRatio * z, zUvRatio * y);
				V.NORMAL = V.POSITION.NormalizeReturn3D();
				V.NORMAL.w = 0.0f;

				V.TANGENT.x = -Radius * sinf(phi) * sinf(theta);
				V.TANGENT.y = 0.0f;
				V.TANGENT.z = Radius * sinf(phi) * cosf(theta);
				V.TANGENT = V.TANGENT.NormalizeReturn3D();
				V.TANGENT.w = 0.0f;

				V.BINORMAL = float4::Cross3D(V.TANGENT, V.NORMAL);
				V.BINORMAL = V.BINORMAL.NormalizeReturn3D();
				V.BINORMAL.w = 0.0f;

				VBVector.push_back(V);
			}
		}

		// 남극점
		V.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.TEXTURECOORD = float4(0.5f, 1.0f);
		V.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize3D();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(-1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, -1.0f, 0.0f);
		VBVector.push_back(V);

		// 인덱스 버퍼를 만듭니다.
		IBVector.clear();

		// 북극점을 이루는 점을 만드는건.
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// 시작은 무조건 북극점
			IBVector.push_back(0);
			IBVector.push_back(i + 2);
			IBVector.push_back(i + 1);
		}

		for (UINT y = 0; y < iStackCount - 2; y++)
		{
			for (UINT z = 0; z < iSliceCount; z++)
			{
				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);

			}
		}

		// 마지막으로 남극점 인덱스
		UINT iBotIndex = (UINT)VBVector.size() - 1;
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// 시작은 무조건 북극점
			IBVector.push_back(iBotIndex);
			IBVector.push_back(iBotIndex - (i + 2));
			IBVector.push_back(iBotIndex - (i + 1));
		}

		GameEngineVertexBufferManager::GetInst().Create("Sphere", VBVector, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		GameEngineIndexBufferManager::GetInst().Create("Sphere", IBVector, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	// Box
	{
		std::vector<GameEngineVertex> Vertex = std::vector<GameEngineVertex>(4 * 6);
		{
			Vertex[0] = { float4({ -0.5f, 0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };
			Vertex[1] = { float4({ 0.5f, 0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };
			Vertex[2] = { float4({ 0.5f, -0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };
			Vertex[3] = { float4({ -0.5f, -0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };

			Vertex[4] = { float4::RotateXDegree(Vertex[0].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
			Vertex[5] = { float4::RotateXDegree(Vertex[1].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
			Vertex[6] = { float4::RotateXDegree(Vertex[2].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
			Vertex[7] = { float4::RotateXDegree(Vertex[3].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
		}

		{
			Vertex[8] = { float4::RotateYDegree(Vertex[0].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[9] = { float4::RotateYDegree(Vertex[1].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[10] = { float4::RotateYDegree(Vertex[2].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[11] = { float4::RotateYDegree(Vertex[3].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };

			Vertex[12] = { float4::RotateYDegree(Vertex[0].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[13] = { float4::RotateYDegree(Vertex[1].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[14] = { float4::RotateYDegree(Vertex[2].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[15] = { float4::RotateYDegree(Vertex[3].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
		}

		{
			Vertex[16] = { float4::RotateXDegree(Vertex[0].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
			Vertex[17] = { float4::RotateXDegree(Vertex[1].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
			Vertex[18] = { float4::RotateXDegree(Vertex[2].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
			Vertex[19] = { float4::RotateXDegree(Vertex[3].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };

			Vertex[20] = { float4::RotateXDegree(Vertex[0].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
			Vertex[21] = { float4::RotateXDegree(Vertex[1].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
			Vertex[22] = { float4::RotateXDegree(Vertex[2].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
			Vertex[23] = { float4::RotateXDegree(Vertex[3].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
		}

		for (size_t i = 0; i < Vertex.size(); i += 4)
		{
			Vertex[i + 0].TEXTURECOORD = { 0.0f, 0.0f };
			Vertex[i + 1].TEXTURECOORD = { 1.0f, 0.0f };
			Vertex[i + 2].TEXTURECOORD = { 1.0f, 1.0f };
			Vertex[i + 3].TEXTURECOORD = { 0.0f, 1.0f };
		}

		GameEngineVertexBufferManager::GetInst().Create("Box", Vertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> BoxIndex;

		for (int i = 0; i < 6; i++)
		{
			BoxIndex.push_back(i * 4 + 2);
			BoxIndex.push_back(i * 4 + 1);
			BoxIndex.push_back(i * 4 + 0);

			BoxIndex.push_back(i * 4 + 3);
			BoxIndex.push_back(i * 4 + 2);
			BoxIndex.push_back(i * 4 + 0);
		}

		GameEngineIndexBufferManager::GetInst().Create("Box", BoxIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			// 앞면
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
			RectVertex[1] = { float4({ 0.5f, 0.5f, 0.0f }),  { 1.0f, 0.0f } };
			RectVertex[2] = { float4({ 0.5f, -0.5f, 0.0f }),  { 1.0f, 1.0f } };
			RectVertex[3] = { float4({ -0.5f, -0.5f, 0.0f }),  { 0.0f, 1.0f } };
		}

		GameEngineVertexBufferManager::GetInst().Create("Rect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);

		RectIndex.push_back(0);
		RectIndex.push_back(2);
		RectIndex.push_back(3);

		GameEngineIndexBufferManager::GetInst().Create("Rect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(5);

		{
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
			RectVertex[1] = { float4({ 0.5f, 0.5f, 0.0f }),  { 1.0f, 0.0f } };
			RectVertex[2] = { float4({ 0.5f, -0.5f, 0.0f }),  { 1.0f, 1.0f } };
			RectVertex[3] = { float4({ -0.5f, -0.5f, 0.0f }),  { 0.0f, 1.0f } };
			RectVertex[4] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
		}

		GameEngineVertexBufferManager::GetInst().Create("DebugRect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);
		RectIndex.push_back(3);
		RectIndex.push_back(4);

		GameEngineIndexBufferManager::GetInst().Create("DebugRect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			RectVertex[0] = { float4({ -1.0f, 1.0f, 0.0f }),float4({ 0.0f, 0.0f }) };
			RectVertex[1] = { float4({ 1.0f, 1.0f, 0.0f }), float4({ 1.0f, 0.0f }) };
			RectVertex[2] = { float4({ 1.0f, -1.0f, 0.0f }), float4({ 1.0f, 1.0f }) };
			RectVertex[3] = { float4({ -1.0f, -1.0f, 0.0f }),  float4({ 0.0f, 1.0f }) };
		}

		GameEngineVertexBufferManager::GetInst().Create("FullRect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);

		RectIndex.push_back(0);
		RectIndex.push_back(2);
		RectIndex.push_back(3);

		GameEngineIndexBufferManager::GetInst().Create("FullRect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}
}

void GameEngineCore::EngineResourcesCreate_Rasterizer()
{
	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_SOLID, };
		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;
		Info.DepthClipEnable = true;
		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizerBack", Info);
		Ptr->AddWindowSizeViewPort();
	}

	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_SOLID, };
		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;
		Info.DepthClipEnable = true;
		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizerFront", Info);
		Ptr->AddWindowSizeViewPort();
	}
}

void GameEngineCore::EngineResourcesLoad()
{
	{
		GameEngineDirectory EngineDir;

		while (true)
		{
			if (EngineDir.IsRoot())
			{
				GameEngineDebug::MsgBoxError("엔진 리소스 폴더가 존재하지 않습니다.");
				return;
			}

			std::vector<GameEngineDirectory> AllDir = EngineDir.GetAllDirectory("EngineResources");

			if (0 == AllDir.size())
			{
				EngineDir.MoveParent();
				continue;
			}

			EngineDir.MoveChild("EngineResources");
			break;
		}

		EngineDir.MoveChild("Texture");

		std::vector<GameEngineFile> AllFile = EngineDir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory EngineDir;

		while (true)
		{
			if (EngineDir.IsRoot())
			{
				GameEngineDebug::MsgBoxError("엔진 리소스 폴더가 존재하지 않습니다.");
				return;
			}

			std::vector<GameEngineDirectory> AllDir = EngineDir.GetAllDirectory("EngineResources");

			if (0 == AllDir.size())
			{
				EngineDir.MoveParent();
				continue;
			}

			EngineDir.MoveChild("EngineResources");
			break;
		}

		EngineDir.MoveChild("Shader");

		std::vector<GameEngineFile> AllShader = EngineDir.GetAllFile("fx");

		for (auto& ShaderFile : AllShader)
		{
			ShaderFile.Open("rt");

			std::string FileName = ShaderFile.GetFileNameWithOutExtension();
			std::string AllCode = ShaderFile.GetString();

			if (std::string::npos != AllCode.find(FileName + "_VS"))
			{
				GameEngineVertexShader* Ptr = GameEngineVertexShaderManager::GetInst().Load(FileName + "_VS", ShaderFile.GetFullPath(), FileName + "_VS");
			}

			if (std::string::npos != AllCode.find(FileName + "_PS"))
			{
				GameEnginePixelShader* Ptr = GameEnginePixelShaderManager::GetInst().Load(FileName + "_PS", ShaderFile.GetFullPath(), FileName + "_PS");
			}

		}
	}

	GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().Find("PointSmp");
	NewRes->Info_.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	NewRes->ReCreate();
}

void GameEngineCore::EngineResourcesCreate()
{
	EngineResourcesCreate_Mesh();
	EngineResourcesCreate_Rasterizer();

	{
		D3D11_BLEND_DESC BlendInfo = { 0 };

		BlendInfo.AlphaToCoverageEnable = FALSE;
		BlendInfo.IndependentBlendEnable = FALSE;
		BlendInfo.RenderTarget[0].BlendEnable = true;
		BlendInfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendInfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		BlendInfo.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		BlendInfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		BlendInfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		GameEngineBlendManager::GetInst().Create("AlphaBlend", BlendInfo);
	}

	{
		D3D11_BLEND_DESC BlendInfo = { 0 };

		BlendInfo.AlphaToCoverageEnable = FALSE;
		BlendInfo.IndependentBlendEnable = FALSE;
		BlendInfo.RenderTarget[0].BlendEnable = true;
		BlendInfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendInfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
		BlendInfo.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		BlendInfo.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
		BlendInfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		BlendInfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		GameEngineBlendManager::GetInst().Create("Trans", BlendInfo);
	}


	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = {0};

		DepthInfo.DepthEnable = true;
		DepthInfo.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		DepthInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("BaseDepthOn", DepthInfo);
	}

	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = { 0 };

		DepthInfo.DepthEnable = true;
		DepthInfo.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		DepthInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("MergeDepth", DepthInfo);
	}


	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = { 0 };

		DepthInfo.DepthEnable = false;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("BaseDepthOff", DepthInfo);
	}

	{
		D3D11_SAMPLER_DESC Smp_Decs = {};

		memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));
		Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		Smp_Decs.MipLODBias = 0.0f;
		Smp_Decs.MaxAnisotropy = 1;
		Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Smp_Decs.MinLOD = -FLT_MAX;
		Smp_Decs.MaxLOD = FLT_MAX;

		GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().CreateAndFind("LINEARSmp", Smp_Decs);
	}


	{
		D3D11_SAMPLER_DESC Smp_Decs = {};

		memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));
		Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		Smp_Decs.MipLODBias = 0.0f;
		Smp_Decs.MaxAnisotropy = 1;
		Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Smp_Decs.MinLOD = -FLT_MAX;
		Smp_Decs.MaxLOD = FLT_MAX;

		GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().CreateAndFind("PointSmp", Smp_Decs);
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DebugRect");
		Pipe->SetInputAssembler1VertexBufferSetting("DebugRect");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("DebugRect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		Pipe->SetPixelShader("Color_PS");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TargetMerge");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetVertexShader("TargetMerge_VS");
		Pipe->SetPixelShader("TargetMerge_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Color");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("Color_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ColorAni");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("ColorAni_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("ColorAni_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureAtlas");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TextureAtlas_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("TextureAtlas_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Texture");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("Texture_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("Texture_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Fade");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetVertexShader("Fade_VS");
		Pipe->SetPixelShader("Fade_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Blur");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetVertexShader("Blur_VS");
		Pipe->SetPixelShader("Blur_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	// ============================================= 타일맵 관련 ============================================= //
	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TileMap");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TileMap_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("TileMap_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
	}

	// ============================================= 특수기능 UI 관련 ============================================= //
	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ProgressBarUI");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("ProgressBar_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("ProgressBar_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
	}

	// ========================================= 알파블렌딩(검정색제외) 관련 ======================================= //
	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureTrans");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("Texture_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("Texture_PS");
		Pipe->SetOutputMergerBlend("Trans");
	}
}