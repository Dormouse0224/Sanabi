#pragma once
#include "CRenderComponent.h"

#include "CSprite.h"
#include "CTexture2D.h"
#include "CStructuredBuffer.h"

class CTileRender :
    public CRenderComponent
{
public:
    CTileRender();
    CTileRender(const CTileRender& _Other);
    ~CTileRender();
    CLONE(CTileRender);

private:
    UINT                    m_Col;          // ��
    UINT                    m_Row;          // ��
    Vec2                    m_TileSize;     // Ÿ�� 1���� ũ��
    AssetPtr<CTexture2D>    m_AtlasTex;     // Ÿ���� ��Ʋ�� �ؽ���
    vector<Vec2>            m_vecTileLT;    // �� Ÿ�� ��������Ʈ�� Left Top ��ǥ

    CStructuredBuffer*      m_GpuBuffer;    // Ÿ�� ��������Ʈ�� ������ GPU�� �����ϴ� ����ȭ����


public:
    void SetColRow(UINT _Col, UINT _Row);
    void SetTileSize(Vec2 _TileSize);
    void SetAtlasTex(CTexture2D* _Tex) { m_AtlasTex = _Tex; }
    void SetTile(UINT _Col, UINT _Row, Vec2 _LT);

public:
    virtual void FinalTick() override;
    virtual void Render() override;

    virtual int Load(fstream& _Stream) override;
    virtual int Save(fstream& _Stream) override;

private:
    void CreateTileRenderMtrl();
    void UpdateBuffer();



};

