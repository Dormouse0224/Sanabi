#pragma once
#include "CImguiObject.h"

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

public:
    void Render();
    void AddChild(TreeNode* _Child) { m_vecChild.push_back(_Child); }
    void Clear();
};

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


public:
    virtual void Update();
    virtual void Render();

    TreeNode* GetRoot() { return m_Root; }

    void AddChildNode(TreeNode* _Parent, string _Name, DWORD_PTR m_Data);
};

