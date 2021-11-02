#include "KModel.h"

void KModel::SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj, TMatrix* pMatNormal)
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
    if (pMatNormal != nullptr)
    {
        m_kbData.matNormal = pMatNormal->Transpose();
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
    _stscanf_s(obejct, _T("%s%f"), tmp, (UINT)(_countof(tmp)), &fVersion);


    int inumVertex = 0;
    _fgetts(obejct, 256, FObject);
    _stscanf_s(obejct, _T("%d"), &inumVertex);

    int index = 0;
    for (int iLine = 0; iLine < inumVertex; iLine++)
    {
        PNCT_VERTEX vtmp;
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

ID3DBlob* KModel::LoadShaderBlob(std::wstring vs, std::string function, std::string version)
{
    HRESULT hr = S_OK;
    ID3DBlob* ret = nullptr;
    ID3DBlob* error = nullptr;
    hr = D3DCompileFromFile(
        vs.c_str(),
        nullptr,
        nullptr,
        function.c_str(),
        version.c_str(),
        0,
        0,
        &ret,
        &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return ret;
    }
    return ret;
}

HRESULT KModel::CreateVertexBuffer()
{
    HRESULT hr;

    D3D11_BUFFER_DESC pDec;
    ZeroMemory(&pDec, sizeof(D3D11_BUFFER_DESC));
    pDec.ByteWidth = (UINT)(sizeof(PNCT_VERTEX) * m_VertexList.size());
    pDec.Usage = D3D11_USAGE_DEFAULT;
    pDec.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA pInitData;
    ZeroMemory(&pInitData, sizeof(D3D11_SUBRESOURCE_DATA));
    pInitData.pSysMem = &m_VertexList.at(0);

    hr = g_pd3dDevice->CreateBuffer(&pDec, &pInitData, &m_pVertexBuffer);
    HRFAILED

    return hr;
}
HRESULT KModel::CreateIndexBuffer()
{
    HRESULT hr;

    D3D11_BUFFER_DESC pDec;
    ZeroMemory(&pDec, sizeof(D3D11_BUFFER_DESC));
    pDec.ByteWidth = UINT(sizeof(DWORD) * m_IndexList.size());
    pDec.Usage = D3D11_USAGE_DEFAULT;
    pDec.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA pInitData;
    ZeroMemory(&pInitData, sizeof(D3D11_SUBRESOURCE_DATA));
    pInitData.pSysMem = &m_IndexList.at(0);

    hr = g_pd3dDevice->CreateBuffer(&pDec, &pInitData, &m_pIndexBuffer);
    HRFAILED


    m_iNumIndex = (UINT)(m_IndexList.size());
    return hr;
}

HRESULT KModel::LoadShaderAndInputLayout(LPCWSTR vsFile, LPCWSTR psFile)
{
    HRESULT hr;

    hr = LoadShader(vsFile, psFile);
    HRFAILED


    D3D11_INPUT_ELEMENT_DESC pInputLayout[]
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}, // 4번째 인자에 넣을 값이 3번째에 들어가있음
        {"TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    UINT numLayout = sizeof(pInputLayout) / sizeof(pInputLayout[0]);



    hr = g_pd3dDevice->CreateInputLayout(pInputLayout, numLayout, m_pVStemp->GetBufferPointer(), m_pVStemp->GetBufferSize(), &m_pVertexLayout);
    HRFAILED

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
    HRFAILED

    return hr;
}

HRESULT KModel::LoadShader(LPCWSTR vsFile, LPCWSTR psFile)
{
    HRESULT hr = S_OK;
    ID3DBlob* error = nullptr;
    m_pVStemp = LoadShaderBlob(vsFile,"VertexS", "vs_5_0");
    if (m_pVStemp ==  nullptr)
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return hr;
    }

    ID3DBlob* PStemp = nullptr;
    PStemp = LoadShaderBlob(psFile,"PixelS", "ps_5_0");
    if (PStemp == nullptr)
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return hr;
    }
  
    hr = g_pd3dDevice->CreatePixelShader(PStemp->GetBufferPointer(), PStemp->GetBufferSize(), NULL, &m_pPS);
    HRFAILED

    m_pMainPS = m_pPS;


    PStemp->Release();
    hr = g_pd3dDevice->CreateVertexShader(m_pVStemp->GetBufferPointer(), m_pVStemp->GetBufferSize(), NULL, &m_pVS);
    HRFAILED
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
bool KModel::Init()
{
    return true;
}

bool KModel::Create(ID3D11DeviceContext* pContext, LPCWSTR vsFile, LPCWSTR psFile)
{
    m_pContext = pContext;

    if (CreateVertexData())
    {
        CreateVertexBuffer();
        if (CreateIndexData())
        {
            CreateIndexBuffer();

        }
        
        if (SUCCEEDED(LoadShaderAndInputLayout(vsFile, psFile)))
        {
            CreateConstantBuffer();
            return true;
        }
    }


    return false;
}

bool KModel::Create(ID3D11DeviceContext* pContext, LPCWSTR vsFile, LPCWSTR psFile, LPCWSTR TexName)
{
    m_pContext = pContext;
    
    if (CreateVertexData())
    {
        CreateVertexBuffer();
        if (CreateIndexData())
        {
            CreateIndexBuffer();
            
        }
        LoadTexture(TexName);
        if (SUCCEEDED(LoadShaderAndInputLayout(vsFile, psFile)))
        {
            CreateConstantBuffer();
            return true;
        }
    }
    
    
    return false;
}
bool KModel::LoadTexture(LPCWSTR TexName)
{
    if (!(TexName == L""))
    {
        return m_kTex.LoadTexture(TexName);
    }
    return false;
}

bool KModel::Frame()
{
    m_kbData.matTime.z = g_fGameTimer;
    
    return true;
}
bool KModel::PreRender()
{
    if (m_VertexList.size() <= 0) return true;

    m_pContext->UpdateSubresource(
        m_pConstantBuffer, 0, NULL, &m_kbData, 0, 0);
    m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
    m_pContext->PSSetShaderResources(0, 1, &m_kTex.m_pTextureSRV);
    m_pContext->VSSetShader(m_pVS, NULL, 0);
    m_pContext->PSSetShader(m_pMainPS, NULL, 0);
    m_pContext->IASetInputLayout(m_pVertexLayout);
    UINT pStrides = sizeof(PNCT_VERTEX);
    UINT pOffsets = 0;
    m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,
        &pStrides, &pOffsets);
    m_pContext->IASetIndexBuffer(m_pIndexBuffer,
        DXGI_FORMAT_R32_UINT, 0);
    return true;
      
    
    return true;
}

bool KModel::Render()
{
    
    if (PreRender() == false) return false;
    if (PostRender(m_iNumIndex) == false) return false;
    return true;

}
bool KModel::PostRender(UINT iNumIndex)
{

    if (iNumIndex > 0)
    {
        m_pContext->DrawIndexed(iNumIndex, 0, 0);
    }
    else
    {
        m_pContext->Draw(m_VertexList.size(), 0);
    }
    return false;
}

bool KModel::Release()
{
    m_kTex.Release();
    IFRELEASE(m_pVertexBuffer)
    IFRELEASE(m_pIndexBuffer)
    IFRELEASE(m_pVertexLayout)
    IFRELEASE(m_pConstantBuffer)
    IFRELEASE(m_pVS)
    IFRELEASE(m_pPS)
    return false;
}
