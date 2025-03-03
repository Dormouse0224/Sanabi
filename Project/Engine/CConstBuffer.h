#pragma once
#include "CEntity.h"

class CConstBuffer :
    public CEntity
{
public:
    CConstBuffer();
    ~CConstBuffer();
    CLONE_DISABLE(CConstBuffer);

private:
    ComPtr<ID3D11Buffer>    m_Buffer;
    D3D11_BUFFER_DESC       m_Desc;

    CB_TYPE            m_Type;

public:
    int Create(CB_TYPE _Type, UINT _BufferSize);
    void SetData(void* _Data, UINT _DataSize);
    void Binding();
    void Binding_CS();
};

