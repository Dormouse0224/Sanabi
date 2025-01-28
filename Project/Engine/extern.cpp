#include "pch.h"


tTransform g_Trans = {};

const Matrix Matrix::Identity = Matrix(1.f, 0.f, 0.f, 0.f
	, 0.f, 1.f, 0.f, 0.f
	, 0.f, 0.f, 1.f, 0.f
	, 0.f, 0.f, 0.f, 1.f);