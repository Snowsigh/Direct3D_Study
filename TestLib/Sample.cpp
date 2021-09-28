#include "Sample.h"


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Sample g_Sample;
    g_Sample.InitWindow(
        hInstance,
        nCmdShow,
        L"CreateWindows");
    g_Sample.Run();
    //MessageBox(NULL, L"Exit", L"Click", MB_OK);
}

bool Sample::Init()
{
    return false;
}

bool Sample::Frame()
{
    return false;
}

bool Sample::Render()
{
    return false;
}

bool Sample::Release()
{
    return false;
}