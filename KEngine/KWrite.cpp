#include "KWrite.h"

HRESULT KWrite::CreateDeviceResources(IDXGISurface1* pSurface)
{
    HRESULT hr = S_OK;

    UINT dpi = GetDpiForWindow(g_hWnd);
    D2D1_RENDER_TARGET_PROPERTIES props;
    props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    props.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    props.dpiX = (float)(dpi);
    props.dpiY = (float)(dpi);
    props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
    props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

    hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
        pSurface,
        &props,
        &m_pRT);
    HRFAILED

    m_pRT->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        &m_pTextBrush);
    HRFAILED
    return S_OK;
}

bool KWrite::Init()
{
    HRESULT hr;
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
    HRFAILED
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pdWriteFactory));
    HRFAILED

    hr = m_pdWriteFactory->CreateTextFormat(
        L"±Ã¼­",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        20,
        L"ko-kr",//L"en-us",//L"ko-kr",
        &m_pTextFormat
    );
    if (FAILED(hr)) return false;
    hr = m_pdWriteFactory->CreateTextFormat(
        L"°íµñ",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        50,
        L"ko-kr",//L"en-us",//L"ko-kr",
        &m_pTextFormat50
    );
    HRFAILED
    return true;
}

bool KWrite::Frame()
{
    return false;
}

bool KWrite::Render()
{
    return false;
}
//D2D1_RECT_L RectL(
//    LONG left = 0.f,
//    LONG top = 0.f,
//    LONG right = 0.f,
//    LONG bottom = 0.f)
//{
//    D2D1::Rect<LONG> = RECTL(left, top, right, bottom);
//    return D2D1::Rect<LONG>(left, top, right, bottom);
//}
bool KWrite::DrawText(RECT rt,
    const TCHAR* data, D2D1::ColorF color,
    IDWriteTextFormat* pTextFormat)
{
    if (m_pRT)
    {
        m_pRT->BeginDraw();
        m_pRT->SetTransform(D2D1::IdentityMatrix());
        
        D2D1_RECT_F rect = (D2D1_RECT_F)(D2D1::RectF(rt.left, rt.top, rt.right, rt.bottom));
        m_pTextBrush->SetColor(color);
        if (pTextFormat == nullptr)
        {
            m_pRT->DrawText(data, wcslen(data),
                m_pTextFormat, rect, m_pTextBrush);
        }
        else
        {
            m_pRT->DrawText(data, wcslen(data),
                pTextFormat, rect, m_pTextBrush);
        }
        m_pRT->EndDraw();
    }
    return true;
}
bool KWrite::Release()
{
    IFRELEASE(m_pd2dFactory)
    IFRELEASE(m_pdWriteFactory)
    IFRELEASE(m_pRT)
    IFRELEASE(m_pTextFormat)
    IFRELEASE(m_pTextFormat50)
    IFRELEASE(m_pTextBrush)

    return false;
}
