#pragma once


class CEntity
{
public:
	CEntity();
	CEntity(wstring _Name);
	CEntity(const CEntity& _Origin);
	virtual ~CEntity();
	virtual CEntity* Clone() = 0;	// �ڱ��ڽ��� ������ü�� ��ȯ���ִ� �Լ�

private:
	static UINT g_NextID;
	UINT		m_ID;
	wstring		m_Name;

public:
	void SetName(wstring _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }

	UINT GetID() { return m_ID; }

};


typedef void(CEntity::* DELEGATE_0)();
typedef void(CEntity::* DELEGATE_1)(DWORD_PTR);
typedef void(CEntity::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);

