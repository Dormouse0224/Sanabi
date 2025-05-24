#pragma once


#include "framework.h"
#include <crtdbg.h>

// 엔진 라이브러리 인클루드
#include <Engine/global.h>
#include <Engine/CEngine.h>

#ifndef _DEBUG
#pragma comment(lib, "Engine//Engine")
#else
#pragma comment(lib, "Engine//Engine_d")
#endif

// Sanabi 게임용 스크립트 라이브러리 인클루드
#ifndef _DEBUG
#pragma comment(lib, "SanabiScript//SanabiScript")
#else
#pragma comment(lib, "SanabiScript//SanabiScript_d")
#endif