#pragma once
#include "CAsset.h"

class CGameObject;

class CPrefab :
    public CAsset
{
public:
    CPrefab();
    ~CPrefab();
    CLONE_DISABLE(CPrefab);

private:
    CGameObject* m_PrefabObject;

public:
    // 게임 오브젝트를 프리펩으로 저장합니다.
    void SaveAsPrefab(CGameObject* _Obj);
    // 프리펩으로 게임 오브젝트 인스턴스를 생성합니다.
    CGameObject* Instantiate();


    virtual int Save(const wstring& _FileName);
    virtual int Load(const wstring& _FilePath);
};

