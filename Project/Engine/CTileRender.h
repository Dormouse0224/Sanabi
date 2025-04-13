#pragma once
#include "CRenderComponent.h"

#include "CSprite.h"
#include "CTexture2D.h"
#include "CStructuredBuffer.h"

struct TileInfo {
    Vec2 vLeftTop;

    int padding[2];
};

class CTileRender :
    public CRenderComponent
{
    friend class CTileRnederUI;
public:
    CTileRender();
    CTileRender(const CTileRender& _Other);
    ~CTileRender();
    CLONE(CTileRender);

private:
    UINT                    m_Col;          // Ÿ�ϸ� �� ����
    UINT                    m_Row;          // Ÿ�ϸ� �� ����
    Vec2                    m_TileSize;     // Ÿ�� 1���� ũ��
    AssetPtr<CTexture2D>    m_AtlasTex;     // Ÿ���� ��Ʋ�� �ؽ���
    vector<TileInfo>        m_vecTileInfo;  // �� Ÿ���� ����Ű�� ��������Ʈ�� Left Top ��ǥ

    CStructuredBuffer*      m_GpuBuffer;    // Ÿ�� ��������Ʈ�� ������ GPU�� �����ϴ� ����ȭ����


public:
    void SetColRow(UINT _Col, UINT _Row);
    void SetTileSize(Vec2 _TileSize);
    void SetAtlasTex(AssetPtr<CTexture2D> _Tex) { m_AtlasTex = _Tex; }
    void SetTile(UINT _Col, UINT _Row, Vec2 _LT);

public:
    virtual void FinalTick() override;
    virtual void Render() override;

    virtual int Load(fstream& _Stream) override;
    virtual int Save(fstream& _Stream) override;

private:
    void UpdateBuffer();



};

