#pragma once


#include "framework.h"
#include <crtdbg.h>

// ���� ���̺귯�� ��Ŭ���
#include <Engine/global.h>
#include <Engine/CEngine.h>

#ifndef _DEBUG
#pragma comment(lib, "Engine//Engine")
#else
#pragma comment(lib, "Engine//Engine_d")
#endif

// Sanabi ���ӿ� ��ũ��Ʈ ���̺귯�� ��Ŭ���
#ifndef _DEBUG
#pragma comment(lib, "SanabiScript//SanabiScript")
#else
#pragma comment(lib, "SanabiScript//SanabiScript_d")
#endif