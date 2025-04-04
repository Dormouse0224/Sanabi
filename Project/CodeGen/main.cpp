#include "pch.h"
#include "PathMgr.h"

vector<wstring> g_vecName;

int main()
{
	CPathMgr::init();
	wstring strProjPath = CPathMgr::GetProjectPath();
	wstring strCppPath = strProjPath + L"SanabiScript\\CScriptInit.cpp";

	// 1. 현재 존재하는 모든 스크립트를 알아내야함.
	wstring strScriptIncludePath = CPathMgr::GetIncludePath();
	wstring strScriptCode = strScriptIncludePath + L"SanabiScript\\Scripts\\";

	WIN32_FIND_DATA tData = {};
	HANDLE handle = FindFirstFile(wstring(strScriptCode + L"\\*.h").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == handle)
		return 0;

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
			g_vecName.push_back(wstring(tData.cFileName).substr(0, wcslen(tData.cFileName) - 2));
		}		
				
		if (!FindNextFile(handle, &tData))
			break;
	}

	FindClose(handle);

	// 이전 실행 시 스크립트 목록 체크 후 같으면 즉시 종료하는 기능 - 제거됨 - 항상 코드를 갱신.
	if (false)
	{
		// 이전에 CodeGen 이 실행할때 체크해둔 스크립트 목록
		wfstream PrevScriptList(L"ScriptList.txt", ios::in);

		if (PrevScriptList.is_open())
		{
			wstring ScriptName = L"";
			vector<wstring> vecCurScriptList;
			while (getline(PrevScriptList, ScriptName))
			{
				vecCurScriptList.push_back(ScriptName);
			}
			PrevScriptList.close();


			if (g_vecName.size() == vecCurScriptList.size())
			{
				bool bSame = true;
				for (UINT i = 0; i < g_vecName.size(); ++i)
				{
					if (g_vecName[i] != vecCurScriptList[i])
					{
						// 같지 않은게 1개이상 있다
						bSame = false;
						break;
					}
				}

				// 이전 목록과, 현재 스크립트 목록이 완전 일치한다(변경사항 없다)
				if (bSame)
					return 0;
			}
		}
	}


	//====================
	// ScriptMgr cpp 작성
	//====================

	wfstream file(strCppPath, ios::out);
	if (!file.is_open())
		return 0;
	file.imbue(std::locale("kor"));

	// 헤더 입력
	file << L"#include \"pch.h\"" << std::endl;
	file << L"#include \"CScriptInit.h\"" << std::endl;
	file << L"#include \"Engine\\CComponentMgr.h\"" << std::endl;
	file << std::endl;

	file << L"// ========================================================" << std::endl;
	file << L"// 이 코드는 CodeGen 코드 생성기에 의해 작성되었습니다." << std::endl;
	file << L"// Exeptionlist.txt 는 해당 파일 내 이름들을 예외처리합니다." << std::endl;
	file << L"// ScriptList.txt 는 코드가 생성된 스크립트 목록입니다." << std::endl;
	file << L"// 코드는 항상 갱신됩니다." << std::endl;
	file << L"// ========================================================" << std::endl;
	file << std::endl;


	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		file << L"#include \"Scripts\\" << g_vecName[i] << L".h\"" << std::endl;
	}
	file << std::endl;

	// 첫 번째 함수 작성
	file << L"void CScriptInit::ScriptInit()" << std::endl;
	file << L"{" << std::endl;
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		file << L"\tREGISTER_SCRIPT(" << g_vecName[i] << L");" << std::endl;
	}
	file << L"}" << std::endl;

	file.close();


	// 다음번 실행시 비교하기위한 정보 저장
	wfstream ScriptList(L"ScriptList.txt", ios::out);
	//if (!std::filesystem::exists(_Path.parent_path()))
	//	std::filesystem::create_directories(_Path.parent_path());
	if (!ScriptList.is_open())
		return 0;

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		ScriptList << g_vecName[i] << std::endl;
	}

	ScriptList.close();

	return 0;
}