#include "pch.h"
#include "CPathMgr.h"

wchar_t CPathMgr::g_ContentDir[255] = {};
wchar_t CPathMgr::g_BinPath[255] = {};
wchar_t CPathMgr::g_SolutionPath[255] = {};

void CPathMgr::Init()
{
	wchar_t szBuff[255] = {};

	// ���� ���μ����� ���(����� ���� ������ ��� ����ó�� �ʿ�, ������Ʈ ����->Debug->�۾����͸� ����
	GetCurrentDirectory(255, szBuff);

	wcscpy_s(g_BinPath, 255, szBuff);
#ifndef _DEBUG
	wcscat_s(g_BinPath, 255, L"\\bin\\");
#else
	wcscat_s(g_BinPath, 255, L"\\bin\\debug\\");
#endif

	wcscpy_s(g_ContentDir, 255, szBuff);
	wcscat_s(g_ContentDir, 255, L"\\Content\\");

	for (int i = wcslen(szBuff); 0 <= i; --i)
	{
		if (szBuff[i] == L'\\')
		{
			szBuff[i + 1] = L'\0';
			break;
		}
	}
	wcscpy_s(g_SolutionPath, 255, szBuff);
}

void CPathMgr::CreateParentDir(std::filesystem::path _Path)
{
	if (!std::filesystem::exists(_Path.parent_path()))
		std::filesystem::create_directories(_Path.parent_path());
}
