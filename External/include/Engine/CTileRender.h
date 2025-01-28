#pragma once
#include "CRenderComponent.h"

#include "CSprite.h"
#include "CTexture.h"
#include "CStructuredBuffer.h"

struct tTileInfo
{
    Ptr<CSprite>    Sprite;
};

class CTileRender :
    public CRenderComponent
{
private:
    UINT                m_Col;          // 열
    UINT                m_Row;          // 행
    Vec2                m_TileSize;     // 타일 1개의 크기
    vector<tTileInfo*>  m_vecTileInfo;  // 각 타일의 정보
    CStructuredBuffer* m_GpuBuffer;    // 타일 정보를 저장하고 텍스쳐 레지스터로 바인딩하기 위함


public:
    void SetColRow(UINT _Col, UINT _Row);
    void SetTileSize(Vec2 _TileSize);
    void SetSprite(UINT _Col, UINT _Row, Ptr<CSprite> _Sprite);

public:
    virtual void FinalTick() override;
    virtual void Render() override;

private:
    void CreateTileRenderMtrl();
    void UpdateBuffer();
    Ptr<CTexture> GetAtlasTex();



public:
    CLONE(CTileRender);
    CTileRender();
    ~CTileRender();
};

