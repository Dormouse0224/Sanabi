#pragma once
#include "CAsset.h"


class CShader :
    public CAsset
{
public:
    CShader(ASSET_TYPE _Type);
    ~CShader();
    CLONE_DISABLE(CShader);

protected:
    ComPtr<ID3DBlob>        m_ErrBlob;


public:
    virtual int Save(const wstring& _FilePath) { return S_OK; }
    virtual int Load(const wstring& _FilePath) { return S_OK; }
};

