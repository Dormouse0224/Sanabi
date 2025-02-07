#include "pch.h"


tTransform g_Trans = {};

tGlobal g_global = {};

const Matrix Matrix::Identity = Matrix(1.f, 0.f, 0.f, 0.f
	, 0.f, 1.f, 0.f, 0.f
	, 0.f, 0.f, 1.f, 0.f
	, 0.f, 0.f, 0.f, 1.f);