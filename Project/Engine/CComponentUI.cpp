#include "pch.h"
#include "CComponentUI.h"

CComponentUI::CComponentUI(COMPONENT_TYPE _Type)
	: CImguiObject(COMPONENT_TYPE_WSTR[(UINT)_Type])
	, m_Type(_Type)
{
}

CComponentUI::~CComponentUI()
{
}
