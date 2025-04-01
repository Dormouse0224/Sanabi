#pragma once
#include "CAsset.h"


struct tConstData
{
    CONST_PARAM m_Param;
    string      m_Desc;
};

struct tTexData
{
    TEX_PARAM   m_Param;
    string      m_Desc;
};

class CShader :
    public CAsset
{
public:
    CShader(ASSET_TYPE _Type);
    ~CShader();
    CLONE_DISABLE(CShader);

protected:
    ComPtr<ID3DBlob>                m_ErrBlob;

    vector<tConstData>              m_vecConstData;
    vector<tTexData>                m_vecTexData;

public:
    const vector<tConstData>& GetConstData() { return m_vecConstData; }
    const vector<tTexData>& GetTexData() { return m_vecTexData; }

    void SetConstData(CONST_PARAM _Param, string _Desc) { m_vecConstData.push_back(tConstData{ _Param, _Desc }); }
    void SetTexData(TEX_PARAM _Param, string _Desc) { m_vecTexData.push_back(tTexData{ _Param, _Desc }); }

};

