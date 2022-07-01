#include "PreCompile.h"

// GameEngineBase(내부)
#pragma comment (lib, "GameEngineBase.lib")

// DirectX Tex(외부)
#pragma comment(lib, "DirectXTex.lib")

// FW1FontWrapper(외부)
#pragma comment(lib, "FW1FontWrapper.lib")

// FBX SDK(외부)
#pragma comment(lib, "zlib-md.lib")			// 문제없음
#pragma comment(lib, "libxml2-md.lib")		// 문제없음
//#pragma comment(lib, "libfbxsdk.lib")		// 특정 static 함수 링크에러발생
#pragma comment(lib, "libfbxsdk-md.lib")	// 메모리누수발생
