#pragma once
// 해당 프로젝트에서 중복되는 헤더(Ex Window Standard Header)를 해당 파일에 선언하여
// 중복을 제거하고 프로젝트 빌드시 미리컴파일된 헤더를 재빌드하는 경우의 수를 없애기 위함

#include <iostream>
#include <Windows.h>
#include <process.h>

// 자료구조
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>

// std::
#include <string>
#include <functional>
#include <thread>

// IO
#include <io.h>
#include <filesystem>
#include <conio.h>
#include <comdef.h>

// 
#include <mutex>

// Debug
#include <assert.h>
#include <crtdbg.h>

// Math
#include <math.h>
#include <random>
#include <algorithm>
#include <DirectXPackedVector.h>	// simd 연산(16Byte를 한번에 연산)용 기능 제공(x64 빌드를 사용해야한다)

// Memory
#include <memory>

// FMOD Sound Lib
#include "FMOD/fmod.hpp"
