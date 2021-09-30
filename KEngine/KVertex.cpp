#include "KVertex.h"


bool KVertex::LoadObject(std::wstring filename)
{
    FILE* FObject = nullptr;
    _tfopen_s(&FObject, filename.c_str(), _T("rt"));
    if (FObject == nullptr)
    {
        return false;
    }
    TCHAR obejct[256] = { 0, };
    float fVersion = 1.0f;
    _fgetts(obejct, 256, FObject);
    TCHAR tmp[256] = { 0, };
    _stscanf_s(obejct, _T("%s%f"), tmp, _countof(tmp), &fVersion);


    int inumVertex = 0;
    _fgetts(obejct, 256, FObject);
    _stscanf_s(obejct, _T("%d"), &inumVertex);

    int index = 0;
    for (int iLine = 0; iLine < inumVertex; iLine++)
    {
        SimpleVertex vtmp;
        _fgetts(obejct, 256, FObject);
        _stscanf_s(obejct, _T("%d %f %f %f %f %f %f %f"), &index, &vtmp.pos.x, &vtmp.pos.y, &vtmp.pos.z,
            &vtmp.color.x, &vtmp.color.y, &vtmp.color.z, &vtmp.color.w);
        m_VertexList.push_back(vtmp);
    }
    fclose(FObject);


}
HRESULT KVertex::CreateVertexBuffer()
{
    HRESULT hr;

    D3D11_BUFFER_DESC pDec;
    ZeroMemory(&pDec, sizeof(D3D11_BUFFER_DESC));
    pDec.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
    pDec.Usage = D3D11_USAGE_DEFAULT;
    pDec.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA pInitData;
    ZeroMemory(&pInitData, sizeof(D3D11_SUBRESOURCE_DATA));
    pInitData.pSysMem = &m_VertexList.at(0);

    hr = m_pDevice->CreateBuffer(&pDec, &pInitData, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;

    return hr;
}

HRESULT KVertex::LoadShaderAndInputLayout()
{
    HRESULT hr;

    hr = LoadShader();
    if (FAILED(hr)) return hr;


    D3D11_INPUT_ELEMENT_DESC pInputLayout[]
    {
        {"POSI", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numLayout = sizeof(pInputLayout) / sizeof(pInputLayout[0]);



    hr = m_pDevice->CreateInputLayout(pInputLayout, numLayout, m_pVStemp->GetBufferPointer(), m_pVStemp->GetBufferSize(), &m_pVertexLayout);
    if (FAILED(hr)) return hr;

    m_pVStemp->Release();

    return hr;
}

HRESULT KVertex::LoadShader()
{
    HRESULT hr;

    hr = D3DCompileFromFile(L"VertexShader.txt", nullptr, nullptr, "VertexS", "vs_5_0", 0, 0, &m_pVStemp, nullptr);
    if (FAILED(hr)) return hr;
    hr = D3DCompileFromFile(L"PixelShader.txt", nullptr, nullptr, "PixelS", "ps_5_0", 0, 0, &m_pPStemp, nullptr);
    if (FAILED(hr)) return hr;

    hr = m_pDevice->CreatePixelShader(m_pPStemp->GetBufferPointer(), m_pPStemp->GetBufferSize(), NULL, &m_pPS);
    if (FAILED(hr)) return hr;
    m_pPStemp->Release();

    hr = m_pDevice->CreateVertexShader(m_pVStemp->GetBufferPointer(), m_pVStemp->GetBufferSize(), NULL, &m_pVS);
    if (FAILED(hr)) return hr;



    return hr;
}

bool	KVertex::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;
    LoadObject(L"ObjectData.txt");
    CreateVertexBuffer();
    LoadShaderAndInputLayout();
    return true;
}
bool	KVertex::Frame()
{

    return true;
}
bool	KVertex::Render()
{
    m_pContext->VSSetShader(m_pVS, NULL, 0);
    m_pContext->PSSetShader(m_pPS, NULL, 0);
    m_pContext->IASetInputLayout(m_pVertexLayout);

    UINT pStrides = sizeof(SimpleVertex);
    UINT pOffsets = 0;

    m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &pStrides, &pOffsets);
    m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    m_pContext->Draw(m_VertexList.size(), 0);
    return true;
}
bool	KVertex::Release()
{
    return true;
}


