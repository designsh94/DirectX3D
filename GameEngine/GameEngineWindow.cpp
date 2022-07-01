#include "PreCompile.h"
#include "GameEngineWindow.h"
#include "KeyboardClass.h"

std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> GameEngineWindow::MessageCallBack_ = nullptr;
bool GameEngineWindow::WindowLoopFlag = true;

// 포인터형 싱글톤
GameEngineWindow* GameEngineWindow::Inst = new GameEngineWindow();

//LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
//{
//    if (nullptr != GameEngineWindow::MessageCallBack_)
//    {
//        if (0 != GameEngineWindow::MessageCallBack_(_hWnd, _message, _wParam, _lParam))
//        {
//            return true;
//        }
//    }
//
//    //if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
//    //    return true;
//
//    switch (_message)
//    {
//    case WM_PAINT:
//    {
//        PAINTSTRUCT ps;
//
//        // HDC 윈도우에 픽셀에 색깔을 바꿀수 있는 권한이입니다.
//        // 윈도우창이 그려지는 2차원 배열에 접근할수 있는 권한입니다.
//        HDC hdc = BeginPaint(_hWnd, &ps);
//        EndPaint(_hWnd, &ps);
//        break;
//    }
//    case WM_SIZE:
//    {
//        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
//        break;
//    }
//    case WM_DESTROY:
//    {
//        WindowOn = false;
//        break;
//    }
//    default:
//        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
//    }
//
//    return 0;
//}

GameEngineWindow::GameEngineWindow() :
    className_(""),
    windowTitle_(""),
    windowhandle_(nullptr),
    hInstance_(nullptr),
    devicecontext_(nullptr)
{
}

GameEngineWindow::~GameEngineWindow() 
{
    if (nullptr != windowhandle_)
    {
        DestroyWindow(windowhandle_);
        windowhandle_ = nullptr;
    }
}

int GameEngineWindow::CreateMainWindowClass()
{
    hInstance_ = GetModuleHandle(NULL);

    if (nullptr == hInstance_)
    {
        GameEngineDebug::AssertFalse();
        return 0;
    }

    className_ = "DEF";

    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    //wcex.lpfnWndProc = WndProc;
    wcex.lpfnWndProc = WindowEvent;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance_;
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = className_.c_str();
    wcex.hIconSm = nullptr;

    // 아래의 함수의 내용이 
    return RegisterClassExA(&wcex);
}

void GameEngineWindow::CreateMainWindow(const std::string& _titlename, const float4& _size, const float4& _pos)
{
    // 반환값이 '0'인경우 윈도우클래스 등록 실패로 간주
    if (0 == CreateMainWindowClass())
    {
        GameEngineDebug::MsgBoxError("윈도우 클래스 등록에 실패했습니다.");
        return;
    }

    if (nullptr == hInstance_)
    {
        GameEngineDebug::AssertFalse();
        return;
    }

    if ("" == className_)
    {
        GameEngineDebug::AssertFalse();
        return;
    }

    windowhandle_ = nullptr;
    windowTitle_ = _titlename;

    windowhandle_ = CreateWindowA(className_.c_str(), "TEST", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance_, nullptr);
    if (0 == windowhandle_)
    {
        GameEngineDebug::AssertFalse();
        return;
    }

    // 윈도우 클라이언트영역(작업영역) 설정
    SetSizeAndPos(_size, _pos);

    // 윈도우 띄우기 및 갱신
    ShowWindow(windowhandle_, SW_SHOW);
    UpdateWindow(windowhandle_);

    devicecontext_ = ::GetDC(windowhandle_);

    return;
}

void GameEngineWindow::SetSizeAndPos(const float4& _size, const float4& _pos)
{
    size_ = _size;
    pos_ = _pos;

    RECT Rc = { 0, 0, _size.ix(), _size.iy() };

    // 내가 넣어준 렉트에 타이틀바와 메뉴등의 사이즈가 들어간 녀석으로 만들어주세요.
    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);

    // 0넣으면 그냥 보통 기본이다.
    SetWindowPos(windowhandle_, nullptr, _pos.ix(), _pos.iy(), Rc.right - Rc.left, Rc.bottom - Rc.top, 0);
}

void GameEngineWindow::Loop(void(*_loopFunc)()) 
{
    MSG msg;
    while (WindowLoopFlag)
    {
        if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (nullptr == _loopFunc)
            {
                GameEngineDebug::AssertFalse();
                return;
            }

            _loopFunc();

            // Window메세지가 있든 없든 돌아간다.
            if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg); // WNDPROC
            }
        }
        else
        {
            if (nullptr == _loopFunc)
            {
                GameEngineDebug::AssertFalse();
                return;
            }

            _loopFunc();
        }
    }
}

bool GameEngineWindow::IsWindowRangeOut(const float4& _Pos) 
{
    if (0 > _Pos.x)
    {
        return true;
    }

    if (0 > _Pos.y)
    {
        return true;
    }

    if (_Pos.x > GetSize().x)
    {
        return true;
    }

    if (_Pos.y > GetSize().y)
    {
        return true;
    }

    return false;
}


__int64 GameEngineWindow::WindowEvent(HWND _hWnd, unsigned int _EventType, unsigned __int64 _LValue, __int64 _SubValue)
{
	// IMGUI Message Proc
	if (nullptr != GameEngineWindow::MessageCallBack_)
	{
		if (0 != GameEngineWindow::MessageCallBack_(_hWnd, _EventType, _LValue, _SubValue))
		{
			return true;
		}
	}

	switch (_EventType)
	{
		case WM_CREATE:
		{
			//// 윈도우 생성과 동시에 시스템에 폰트 등록
			//GameEngineDirectory SoundDir;
			//SoundDir.MoveParent("DirectX3D");
			//SoundDir.MoveChild("Resources");
			//SoundDir.MoveChild("Font");

			//std::string FontPathName = SoundDir.GetFullPath();
			//FontPathName += "\\diablo.ttf";

			//AddFontResource(FontPathName.c_str());

			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;

			// HDC 윈도우에 픽셀에 색깔을 바꿀수 있는 권한이입니다.
			// 윈도우창이 그려지는 2차원 배열에 접근할수 있는 권한입니다.
			HDC hdc = BeginPaint(_hWnd, &ps);
			EndPaint(_hWnd, &ps);
			break;
		}
		case WM_SIZE:
		{
			return DefWindowProc(_hWnd, _EventType, _LValue, _SubValue);
			break;
		}
		case WM_DESTROY:
		{
			//// 윈도우 제거와 동시에 등록된 폰트 해제
			//GameEngineDirectory SoundDir;
			//SoundDir.MoveParent("DirectX3D");
			//SoundDir.MoveChild("Resources");
			//SoundDir.MoveChild("Font");

			//std::string FontPathName = SoundDir.GetFullPath();
			//FontPathName += "\\diablo.ttf";

			//RemoveFontResource(FontPathName.c_str());

			WindowLoopFlag = false;

			break;
		}

		// ===================================================== 키보드입력을 그대로 받기위한 처리 ===================================================== //
		case WM_CHAR:
		{
			unsigned char ch = static_cast<unsigned char>(_LValue);

			// 예외처리 : 해당 키들은 input에 등록하여 사용함
			if (ch == '\b') // BackSpace Key Down
			{
				int a = 0;

				break;
			}
			else if (ch == '\r') // Enter Key Down
			{
				int a = 0;

				break;
			}
			else if (ch == ' ') // Space Key Down
			{
				int a = 0;

				break;
			}

			// 문자 입력에 의한 큐 등록
			if (KeyboardClass::GetInst().IsCharsAutoRepeat())
			{
				KeyboardClass::GetInst().OnChar(ch);
			}
			else
			{
				const bool wasPressed = _LValue & 0x40000000;
				if (!wasPressed)
				{
					KeyboardClass::GetInst().OnChar(ch);
				}
			}
			return 0;

			break;
		}
		case WM_KEYDOWN:
		{
			unsigned char keycode = static_cast<unsigned char>(_LValue);

			if (KeyboardClass::GetInst().IsKeysAutoRepeat())
			{
				KeyboardClass::GetInst().OnKeyPressed(keycode);
			}
			else
			{
				const bool wasPressed = _SubValue & 0x40000000;
				if (!wasPressed)
				{
					KeyboardClass::GetInst().OnKeyPressed(keycode);
				}
			}

			return 0;
		}
		case WM_KEYUP:
		{
			unsigned char keycode = static_cast<unsigned char>(_LValue);
			KeyboardClass::GetInst().OnKeyReleased(keycode);

			return 0;
		}
		default:
		{
			return DefWindowProc(_hWnd, _EventType, _LValue, _SubValue);
		}
	}

	return 0;
}

void GameEngineWindow::CloseWindow()
{
	// Window Message Send
	DestroyWindow(windowhandle_);
}
