#pragma once
#include "CImguiObject.h"

// Tree Node

struct TreeNode
{
    friend class CTreeUI;
public:
    TreeNode(string _Name);
    TreeNode(string _Name, DWORD_PTR _Data, TreeNode* _Parent);
    ~TreeNode();

public:
    static UINT g_NextTreeID;

private:
    string              m_Name;
    UINT                m_TreeID;
    DWORD_PTR           m_Data;
    TreeNode*           m_Parent;
    vector<TreeNode*>   m_vecChild;
    CTreeUI*            m_Owner;

public:
    void Render();
    void AddChild(TreeNode* _Child) { m_vecChild.push_back(_Child); }
    void Clear();
    void DoubleClickCheck();
};


// Tree UI Class

class CTreeUI :
    public CImguiObject
{
public:
    CTreeUI(wstring _Name);
    ~CTreeUI();
    CLONE_DISABLE(CTreeUI);

private:
    TreeNode*   m_Root;
    bool        m_ShowRoot;
    CEntity*    m_Owner;
    DELEGATE_1  m_DoubleClickFunc;

public:
    virtual void Update();
    virtual void Render();

    TreeNode* GetRoot() { return m_Root; }
    DELEGATE_1 GetDoubleClickFunc() { return m_DoubleClickFunc; }
    CEntity* GetOwner() { return m_Owner; }

    void SetDoubleClickFunc(DELEGATE_1  _SelectFunc) { m_DoubleClickFunc = _SelectFunc; }
    void SetOwner(CEntity* _Owner) { m_Owner = _Owner; }

    void AddChildNode(TreeNode* _Parent, string _Name, DWORD_PTR m_Data);

};

