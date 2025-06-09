#pragma once
#include "Engine\CScript.h"


class ContainerScript
	: public CScript
{
public:
	ContainerScript();
	ContainerScript(const ContainerScript& _Origin);
	~ContainerScript();
	CLONE(ContainerScript);
	
private:
	float m_Gap;
	float m_Speed;

public:
	virtual void Tick();
	virtual int Save(fstream& _Stream) { return S_OK; }
	virtual int Load(fstream& _Stream) { return S_OK; }

	bool CheckConnection(CGameObject* _Obj);

};

