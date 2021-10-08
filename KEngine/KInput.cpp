#include "KInput.h"

bool KInPut::Init()
{
    ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
    return true;
}
// OR(논리합)
// b   0    1     a
// 0   0    1
// 1   1    1 
// AND(논리곱)
// b   0    1     a
// 0   0    0
// 1   0    1 
// sKey 1000 0000 0000 0000
//   &  1000 0000 0000 0000
DWORD  KInPut::GetKey(DWORD dwKey)
{
    return m_dwKeyState[dwKey];
}
LRESULT KInPut::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int iMouseX = (short)LOWORD(lParam);
    int iMouseY = (short)HIWORD(lParam);
    switch (message)
    {
    case  WM_MOUSEMOVE:
        OnMove(iMouseX, iMouseY);
        return TRUE;
    case WM_LBUTTONDOWN:
        SetCapture(g_hWnd);
        OnBegin(iMouseX, iMouseY);
        return TRUE;
    case WM_LBUTTONUP:
        OnEnd();
        ReleaseCapture();
        return TRUE;
        /*case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:*/
    case WM_MOUSEWHEEL:
    {
        m_iWheel = GET_WHEEL_DELTA_WPARAM(wParam);//120    
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;

}
bool KInPut::Frame()
{
    // 화면좌표
    GetCursorPos(&m_ptPos);
    // 클라이언트 좌표
    ScreenToClient(g_hWnd, &m_ptPos);

    for (int iKey = 0; iKey < 256; iKey++)
    {
       
      SHORT sKey = GetAsyncKeyState(iKey);
        if (sKey & 0x8000)
        {
            if (m_dwKeyState[iKey] == KEY_FREE)
            {
                m_dwKeyState[iKey] = KEY_PUSH;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_HOLD;
            }
        }
        else
        {
            if (m_dwKeyState[iKey] == KEY_PUSH ||
                m_dwKeyState[iKey] == KEY_HOLD)
            {
                m_dwKeyState[iKey] = KEY_UP;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_FREE;
            }
        }
    }
    return true;
}

bool KInPut::Render()
{
    
    return true;
}

bool KInPut::Release()
{
    return true;
}
void KInPut::OnMove(int iX, int iY)
{
    if (m_bDrag)
    {
        m_pDrag.x = iX - m_pDragDown.x;
        m_pDrag.y = iY - m_pDragDown.y;
    }
    m_pDragDown.x = iX;
    m_pDragDown.y = iY;
    //std::string buffer;
    //std::string b = std::to_string(iX);
    //buffer = b + " " + std::to_string(m_pDragDown.x);
    //OutputDebugStringA(buffer.c_str());
    return;
}
void KInPut::OnBegin(int iX, int iY)
{
    m_bDrag = true;
    m_pDragDown.x = iX;
    m_pDragDown.y = iY;
    return;
}
void KInPut::OnEnd()
{
    m_bMove = false;
    m_bDrag = false;
    m_pDrag.x = 0;
    m_pDrag.y = 0;
    return;
}
KInPut::KInPut() {}
KInPut::~KInPut() {}