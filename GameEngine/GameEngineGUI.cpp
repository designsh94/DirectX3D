#include "PreCompile.h"
#include "GameEngineGUI.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "GameEngineWindow.h"
#include "GameEngineDevice.h"

GameEngineGUI* GameEngineGUI::Inst_ = new GameEngineGUI();

GameEngineGUI::GameEngineGUI()
{
}

GameEngineGUI::~GameEngineGUI()
{
    for (auto& Window : Windows_)
    {
        delete Window;
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void GameEngineGUI::Initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    GameEngineDirectory Dir;
    Dir.MoveParent("DirectX3D");
    Dir.MoveChild("EngineResources");
    Dir.MoveChild("Font");
    io.Fonts->AddFontFromFileTTF(Dir.PathToPlusFileName("malgun.ttf").c_str(), 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    ImGui_ImplWin32_Init(GameEngineWindow::GetInst().GetWindowHWND());
    ImGui_ImplDX11_Init(GameEngineDevice::GetDevice(), GameEngineDevice::GetContext());

    GameEngineWindow::SetMessageCallBack(ImGui_ImplWin32_WndProcHandler);
}

void GameEngineGUI::GUIRenderStart()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    for (auto& Window : Windows_)
    {
        if (false == Window->IsUpdate())
        {
            continue;
        }

        Window->Begin();
        Window->OnGUI();
        Window->End();
    }
}

void GameEngineGUI::GUIRenderEnd()
{
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

GameEngineGUIWindow::GameEngineGUIWindow() :
    Style_(0)
{
}

GameEngineGUIWindow::~GameEngineGUIWindow()
{
}

GameEngineGUIWindow* GameEngineGUI::FindGUIWindow(const std::string& _Name)
{
    for (auto FindIter : Windows_)
    {
        if (FindIter->GetName() == _Name)
        {
            return FindIter;
        }
    }

    return nullptr;
}

std::list<GameEngineGUIWindow*> GameEngineGUI::FindGUIWindowForList(const std::string& _Name)
{
    std::list<GameEngineGUIWindow*> NewList;
    for (auto FindIter : Windows_)
    {
        if (FindIter->GetName() == _Name)
        {
            NewList.push_back(FindIter);
        }
    }

    return NewList;
}
