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
KInPut::KInPut() {}
KInPut::~KInPut() {}