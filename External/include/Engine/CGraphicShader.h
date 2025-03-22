#pragma once
#include "CShader.h"

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

class CGraphicShader :
    public CShader
{
public:
    CGraphicShader();
    ~CGraphicShader();
    CLONE_DISABLE(CGraphicShader);

private:
    ComPtr<ID3DBlob>                m_VSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    // 정점 구조체 구조정보 레이아웃
    ComPtr<ID3D11InputLayout>       m_Layout;   
    D3D11_PRIMITIVE_TOPOLOGY        m_Topology;

    SHADER_DOMAIN                   m_Domain;

    RS_TYPE                         m_RSType;
    DS_TYPE                         m_DSType;
    BS_TYPE                         m_BSType;

    vector<tConstData>              m_vecConstData;
    vector<tTexData>                m_vecTexData;

public:
    int CreateVertexShader(const wstring& _RelativePath, const string& _FuncName);
    int CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName);
    int CreatePixelShader(const wstring& _RelativePath, const string& _FuncName);
    SHADER_DOMAIN GetDomain() { return m_Domain; }
    const vector<tConstData>& GetConstData() { return m_vecConstData; }
    const vector<tTexData>& GetTexData() { return m_vecTexData; }

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }
    void SetDomain(SHADER_DOMAIN _Domain) { m_Domain = _Domain; }
    void SetConstData(CONST_PARAM _Param, string _Desc) { m_vecConstData.push_back(tConstData{ _Param, _Desc }); }
    void SetTexData(TEX_PARAM _Param, string _Desc) { m_vecTexData.push_back(tTexData{ _Param, _Desc }); }

    void Binding();


};

