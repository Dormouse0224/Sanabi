#include "pch.h"
#include "PathMgr.h"

vector<wstring> g_vecScriptNames;
vector<wstring> g_vecStateNames;
vector<wstring> g_vecTFNames;

int main()
{
	CPathMgr::init();
	wstring strProjPath = CPathMgr::GetProjectPath();
	wstring strCppPath = strProjPath + L"SanabiScript\\CScriptInit.cpp";

	// Script, State, TriggerFunc 파일 위치
	wstring strScriptIncludePath = CPathMgr::GetIncludePath();
	wstring strScriptCode = strScriptIncludePath + L"SanabiScript\\Scripts\\";
	wstring strStateCode = strScriptIncludePath + L"SanabiScript\\States\\";
	wstring strTFCode = strScriptIncludePath + L"SanabiScript\\TriggerFunc.h";

	// 예외 리스트 목록을 알아낸다.
	std::wfstream Exception(L"Exeptionlist.txt", ios::in);

	vector<wstring> vecExcept;

	if (Exception.is_open())
	{
		wstring Name = L"";
		while (getline(Exception, Name))
		{
			vecExcept.push_back(Name);
		}
		Exception.close();
	}

	// Scriipt 목록 벡터 작성
	WIN32_FIND_DATA tData = {};
	HANDLE handle = FindFirstFile(wstring(strScriptCode + L"\\*.h").c_str(), &tData);
	if (INVALID_HANDLE_VALUE != handle)
	{

		while (true)
		{
			// 예외가 아닌경우, 스크립트 이름으로 본다.
			bool bExeption = false;
			for (size_t i = 0; i < vecExcept.size(); ++i)
			{
				if (!wcscmp(tData.cFileName, vecExcept[i].c_str()))
				{
					bExeption = true;
					break;
				}
			}

			if (!bExeption)
			{
				// -2 는 .h 확장자를 지우기 위함.
				g_vecScriptNames.push_back(wstring(tData.cFileName).substr(0, wcslen(tData.cFileName) - 2));
			}

			if (!FindNextFile(handle, &tData))
				break;
		}
		FindClose(handle);
	}

	// State 목록 벡터 작성
	tData = {};
	handle = FindFirstFile(wstring(strStateCode + L"\\*.h").c_str(), &tData);
	if (INVALID_HANDLE_VALUE != handle)
	{
		while (true)
		{
			// 예외가 아닌경우, 스크립트 이름으로 본다.
			bool bExeption = false;
			for (size_t i = 0; i < vecExcept.size(); ++i)
			{
				if (!wcscmp(tData.cFileName, vecExcept[i].c_str()))
				{
					bExeption = true;
					break;
				}
			}

			if (!bExeption)
			{
				// -2 는 .h 확장자를 지우기 위함.
				g_vecStateNames.push_back(wstring(tData.cFileName).substr(0, wcslen(tData.cFileName) - 2));
			}

			if (!FindNextFile(handle, &tData))
				break;
		}
		FindClose(handle);
	}

	// Trigger Function 목록 작성
	fstream TFfile(strTFCode, std::ios::in);
	if (TFfile.is_open())
	{
		string line;
		while (getline(TFfile, line))
		{
			int pos0 = 0;
			if ((pos0 = line.find("bool ")) != string::npos)
			{
				pos0 += 5;
				while (line[pos0] == ' ')
					++pos0;
				// pos0 는 함수 이름 시작 인덱스
				int pos1 = 0;
				if ((pos1 = line.find('(', pos0)) != string::npos)
				{
					while (line[pos1 - 1] == ' ')
						--pos1;
					// pos1 는 함수 이름 끝 인덱스
					string FuncName = line.substr(pos0, pos1 - pos0);
					int pos2 = 0;
					if ((pos2 = line.find("CFSM_State*", pos1)) != string::npos)
					{
						if ((pos2 = line.find("CFSM_State*", pos2 + 11)) != string::npos)
						{
							if (line.find("CFSM_State*", pos2 + 11) == string::npos && line.find(';', pos2 + 11) != string::npos)
							{
								g_vecTFNames.push_back(wstring(FuncName.begin(), FuncName.end()));
							}
						}
					}
				}
			}
		}
	}

	// 이전 실행 시 스크립트 목록 체크 후 같으면 즉시 종료하는 기능 - 제거됨 - 항상 코드를 갱신.
	//if (false)
	//{
	//	// 이전에 CodeGen 이 실행할때 체크해둔 스크립트 목록
	//	wfstream PrevScriptList(L"ScriptList.txt", ios::in);

	//	if (PrevScriptList.is_open())
	//	{
	//		wstring ScriptName = L"";
	//		vector<wstring> vecCurScriptList;
	//		while (getline(PrevScriptList, ScriptName))
	//		{
	//			vecCurScriptList.push_back(ScriptName);
	//		}
	//		PrevScriptList.close();


	//		if (g_vecScriptNames.size() == vecCurScriptList.size())
	//		{
	//			bool bSame = true;
	//			for (UINT i = 0; i < g_vecScriptNames.size(); ++i)
	//			{
	//				if (g_vecScriptNames[i] != vecCurScriptList[i])
	//				{
	//					// 같지 않은게 1개이상 있다
	//					bSame = false;
	//					break;
	//				}
	//			}

	//			// 이전 목록과, 현재 스크립트 목록이 완전 일치한다(변경사항 없다)
	//			if (bSame)
	//				return 0;
	//		}
	//	}
	//}


	//====================
	// CScriptInit cpp 작성
	//====================

	wfstream file(strCppPath, ios::out);
	if (!file.is_open())
		return 0;
	file.imbue(std::locale("kor"));

	// 헤더 입력
	file << L"#include \"pch.h\"" << std::endl;
	file << L"#include \"CScriptInit.h\"" << std::endl;
	file << L"#include \"TriggerFunc.h\"" << std::endl;
	file << L"#include \"Engine\\CComponentMgr.h\"" << std::endl;
	file << L"#include \"Engine\\CFSMMgr.h\"" << std::endl;
	file << std::endl;

	file << L"// ========================================================" << std::endl;
	file << L"// 이 코드는 CodeGen 코드 생성기에 의해 작성되었습니다." << std::endl;
	file << L"// Exeptionlist.txt 는 해당 파일 내 이름들을 예외처리합니다." << std::endl;
	file << L"// ScriptList.txt 는 코드가 생성된 스크립트 목록입니다." << std::endl;
	file << L"// 코드는 항상 갱신됩니다." << std::endl;
	file << L"// ========================================================" << std::endl;
	file << std::endl;

	// Script 헤더
	for (UINT i = 0; i < g_vecScriptNames.size(); ++i)
	{
		file << L"#include \"Scripts\\" << g_vecScriptNames[i] << L".h\"" << std::endl;
	}
	file << std::endl;
	//State 헤더
	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		file << L"#include \"States\\" << g_vecStateNames[i] << L".h\"" << std::endl;
	}
	file << std::endl;

	// Script 등록 함수 작성
	file << L"void CScriptInit::ScriptInit()" << std::endl;
	file << L"{" << std::endl;
	for (UINT i = 0; i < g_vecScriptNames.size(); ++i)
	{
		file << L"\tREGISTER_SCRIPT(" << g_vecScriptNames[i] << L");" << std::endl;
	}
	file << L"}" << std::endl;
	file << std::endl;

	// State, TriggerFunc 등록 함수 작성
	file << L"void CScriptInit::FSMInit()" << std::endl;
	file << L"{" << std::endl;
	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		file << L"\tREGISTER_STATE(" << g_vecStateNames[i] << L");" << std::endl;
	}
	file << std::endl;
	for (UINT i = 0; i < g_vecTFNames.size(); ++i)
	{
		file << L"\tREGISTER_TRIGGER(" << g_vecTFNames[i] << L");" << std::endl;
	}
	file << L"}" << std::endl;
	file << std::endl;

	file.close();


	// 다음번 실행시 비교하기위한 정보 저장
	wfstream ScriptList(L"ScriptList.txt", ios::out);
	if (!ScriptList.is_open())
		return 0;

	for (UINT i = 0; i < g_vecScriptNames.size(); ++i)
	{
		ScriptList << g_vecScriptNames[i] << std::endl;
	}
	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		ScriptList << g_vecStateNames[i] << std::endl;
	}
	ScriptList.close();


	wfstream FuncList(L"FuncList.txt", ios::out);
	if (!FuncList.is_open())
		return 0;

	for (UINT i = 0; i < g_vecTFNames.size(); ++i)
	{
		FuncList << g_vecTFNames[i] << std::endl;
	}
	FuncList.close();

	return 0;
}