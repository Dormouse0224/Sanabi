#include "pch.h"
#include "CAsset.h"

CAsset::CAsset(ASSET_TYPE _Type)
	: m_Type(_Type)
	, m_EngineAsset(false)
{
}

CAsset::~CAsset()
{
}
