#pragma once
#include "CShader.h"
#include "CStructuredBuffer.h"

#include "CTexture2D.h"

class CComputeShader :
    public CShader
{
public:
    CComputeShader();
    CComputeShader(const wstring& _RelativePath, const string& _FuncName
        , UINT _GroupPerX, UINT _GroupPerY, UINT _GroupPerZ);
    ~CComputeShader();
    CLONE_DISABLE(CComputeShader);

protected:
    ComPtr<ID3DBlob>                m_CSBlob;
    ComPtr<ID3D11ComputeShader>     m_CS;

    tMtrlConst                      m_Const;
    pair<AssetPtr<CTexture2D>, int> m_CSTex[TEX_PARAM::TEX_END];    // �ؽ��� ���̴� ���ҽ��� �ؽ��İ� ���ε� �� �������� �ѹ�

    const UINT                      m_ThreadPerGroupX;
    const UINT                      m_ThreadPerGroupY;
    const UINT                      m_ThreadPerGroupZ;

    UINT                            m_GroupX;
    UINT                            m_GroupY;
    UINT                            m_GroupZ;


protected:
    int CreateComputeShader(const wstring& _RelativePath, const string& _FuncName);

public:
    tMtrlConst GetConstParam() { return m_Const; }
    pair<AssetPtr<CTexture2D>, int>& GetCSTex(TEX_PARAM _Param) { return m_CSTex[_Param]; }
    Vec3 GetThreadPerGroup() { return Vec3(m_ThreadPerGroupX, m_ThreadPerGroupY, m_ThreadPerGroupZ); }
    Vec3 GetGroupCount() { return Vec3(m_GroupX, m_GroupY, m_GroupZ); }

    int Execute();
    virtual void UIRender() = 0;

private:
    virtual int Binding() = 0;
    virtual void CalcGroupNum() = 0;
    virtual void Clear() = 0;
};

