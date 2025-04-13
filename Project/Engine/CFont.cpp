#include "pch.h"
#include "CFont.h"

#include "CPathMgr.h"

#include "CDevice.h"

CFont::CFont()
	:CAsset(ASSET_TYPE::FONT)
{
	m_SpriteBatch = new DirectX::SpriteBatch(CONTEXT);
	m_SpriteFont = nullptr;
}

CFont::~CFont()
{
	delete m_SpriteBatch;
	delete m_SpriteFont;
}

int CFont::Load(const wstring& _FilePath)
{
	std::wstring path = CPathMgr::GetContentDir() + _FilePath;
	if (m_SpriteFont)
		delete m_SpriteFont;
	m_SpriteFont = new DirectX::SpriteFont(DEVICE, path.c_str());

	return S_OK;
}

void CFont::Render(wstring _Text, Vec2 _Pos, Vec4 _Color, float _Rot, Vec2 _Origin, float _Scale, float _Depth)
{
	// 블렌드 설정
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(BS_TYPE::ALPHABLEND).Get(), nullptr, 0xffffffff);
	m_SpriteBatch->Begin();
	if (m_SpriteFont)
	{
		//m_SpriteFont->DrawString(m_SpriteBatch, _Text.c_str(), Vec2(100, 100), _Color, _Rot, Vec2(0, 0), _Scale, SpriteEffects_None, _Depth);
		m_SpriteFont->DrawString(m_SpriteBatch, _Text.c_str(), _Pos, _Color, _Rot, Vec2(0, 0), _Scale, SpriteEffects_None, _Depth);
	}
	m_SpriteBatch->End();	// 드로우콜
}
