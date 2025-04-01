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
    // ���� ������Ʈ�� ���������� �����մϴ�.
    void SaveAsPrefab(CGameObject* _Obj);
    // ���������� ���� ������Ʈ �ν��Ͻ��� �����մϴ�.
    CGameObject* Instantiate();


    virtual int Save(const wstring& _FileName);
    virtual int Load(const wstring& _FilePath);
};

