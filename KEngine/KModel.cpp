#include "KModel.h"

void KModel::SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj)
{
    if (pMatWorld != nullptr)
    {
        m_kbData.matWorld = pMatWorld->Transpose();
    }
    if (pMatView != nullptr)
    {
        m_kbData.matView = pMatView->Transpose();
    }
    if (pMatProj != nullptr)
    {
        m_kbData.matProj = pMatProj->Transpose();
    }
}
bool KModel::LoadObject(std::wstring filename)
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

    return true;
}
KModel::KModel()
{
    m_pVertexBuffer = nullptr;
    m_pVertexLayout = nullptr;
    m_pVS = nullptr;
    m_pPS = nullptr;
}

HRESULT KModel::CreateVertexBuffer()
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

    hr = g_pd3dDevice->CreateBuffer(&pDec, &pInitData, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;

    return hr;
}
HRESULT KModel::CreateIndexBuffer()
{
    HRESULT hr;

    D3D11_BUFFER_DESC pDec;
    ZeroMemory(&pDec, sizeof(D3D11_BUFFER_DESC));
    pDec.ByteWidth = sizeof(DWORD) * m_IndexList.size();
    pDec.Usage = D3D11_USAGE_DEFAULT;
    pDec.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA pInitData;
    ZeroMemory(&pInitData, sizeof(D3D11_SUBRESOURCE_DATA));
    pInitData.pSysMem = &m_IndexList.at(0);

    hr = g_pd3dDevice->CreateBuffer(&pDec, &pInitData, &m_pIndexBuffer);
    if (FAILED(hr)) return hr;

    return hr;
}

HRESULT KModel::LoadShaderAndInputLayout()
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



    hr = g_pd3dDevice->CreateInputLayout(pInputLayout, numLayout, m_pVStemp->GetBufferPointer(), m_pVStemp->GetBufferSize(), &m_pVertexLayout);
    if (FAILED(hr)) return hr;

    m_pVStemp->Release();

    return hr;
}
HRESULT KModel::CreateConstantBuffer()
{
    HRESULT hr;

    D3D11_BUFFER_DESC pDec;
    ZeroMemory(&pDec, sizeof(D3D11_BUFFER_DESC));
    pDec.ByteWidth = sizeof(KB_Data);
    pDec.Usage = D3D11_USAGE_DEFAULT;
    pDec.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    D3D11_SUBRESOURCE_DATA pInitData;
    ZeroMemory(&pInitData, sizeof(D3D11_SUBRESOURCE_DATA));
    pInitData.pSysMem = &m_kbData;

    hr = g_pd3dDevice->CreateBuffer(&pDec, &pInitData, &m_pConstantBuffer);
    if (FAILED(hr)) return hr;

    return hr;
}

HRESULT KModel::LoadShader()
{
    HRESULT hr;
    ID3DBlob* error = nullptr;
    hr = D3DCompileFromFile(L"VertexShader.txt", nullptr, nullptr, "VertexS", "vs_5_0", 0, 0, &m_pVStemp, &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return hr;
    }

    hr = D3DCompileFromFile(L"PixelShader.txt", nullptr, nullptr, "PixelS", "ps_5_0", 0, 0, &m_pPStemp, &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return hr;
    }

    hr = g_pd3dDevice->CreatePixelShader(m_pPStemp->GetBufferPointer(), m_pPStemp->GetBufferSize(), NULL, &m_pPS);
    if (FAILED(hr)) return hr;
    m_pPStemp->Release();

    hr = g_pd3dDevice->CreateVertexShader(m_pVStemp->GetBufferPointer(), m_pVStemp->GetBufferSize(), NULL, &m_pVS);
    if (FAILED(hr)) return hr;



    return hr;
}

bool KModel::CreateVertexData()
{
    if (m_VertexList.size() > 0)
    {
        return true;
    }
    return false;
}

bool KModel::CreateIndexData()
{
    if (m_IndexList.size() > 0)
    {
        return true;
    }
    return false;
}


bool KModel::Init(ID3D11DeviceContext* pContext)
{
    m_pContext = pContext;
    if (CreateVertexData() && CreateIndexData())
    {
        CreateVertexBuffer();
        CreateIndexBuffer();
        CreateConstantBuffer();
        LoadShaderAndInputLayout();
        return true;
    }
    return false;
}

bool KModel::Frame()
{
   

    return true;
}
bool KModel::PreRender()
{
    if (m_VertexList.size() <= 0) return true;

    m_pContext->UpdateSubresource(
        m_pConstantBuffer, 0, NULL, &m_kbData, 0, 0);
    m_pContext->VSSetConstantBuffers(
        0, 1, &m_pConstantBuffer);
    m_pContext->VSSetShader(m_pVS, NULL, 0);
    m_pContext->PSSetShader(m_pPS, NULL, 0);
    m_pContext->IASetInputLayout(m_pVertexLayout);
    UINT pStrides = sizeof(SimpleVertex);
    UINT pOffsets = 0;
    m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,
        &pStrides, &pOffsets);
    m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    
    return true;
}

bool KModel::Render()
{
    
    if (PreRender() == false) return false;
    if (PostRender(m_IndexList.size()) == false) return false;
    return true;

}
bool KModel::PostRender(UINT iNumIndex)
{
    m_pContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pContext->DrawIndexed(iNumIndex, 0, 0);
    return true;
}

bool KModel::Release()
{
    m_pVertexBuffer->Release();
    m_pIndexBuffer->Release();
    m_pVertexLayout->Release();
    m_pConstantBuffer->Release();
    m_pVS->Release();
    m_pPS->Release();
    return false;
}
