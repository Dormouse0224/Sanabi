#pragma once
#include "CEntity.h"

class CAsset :
    public CEntity
{
private:
    const ASSET_TYPE    m_Type;
    int                 m_RefCount; // 참조 횟수(0 이되면 스스로 삭제)

    wstring             m_Key;
    wstring             m_RelativePath;

public:
    ASSET_TYPE GetType() const { return m_Type; }
    wstring GetKey() { return m_Key; }
    wstring GetRelativePath() { return m_RelativePath; }

private:
    void AddRef() { ++m_RefCount; }
    void Release()
    {
        --m_RefCount;
        if (m_RefCount <= 0)
        {
            delete this;
        }
    }

    virtual int Save(const wstring& _FilePath) = 0;
    virtual int Load(const wstring& _FilePath) = 0;
    virtual CAsset* Clone() = 0;
public:
    CAsset(ASSET_TYPE _Type);
    ~CAsset();

    template<typename T>
    friend class Ptr;

    friend class CAssetMgr;
};

