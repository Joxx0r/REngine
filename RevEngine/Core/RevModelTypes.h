#pragma once
#include <string>

struct RevVertexPosCol
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};

struct RevVertexPosTexNormBiTan
{
    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT2 m_tex;
    DirectX::XMFLOAT3 m_normal;
    DirectX::XMFLOAT3 m_binormal;
    DirectX::XMFLOAT3 m_tangent;
};


enum RevEModelType : UINT8
{
    Invalid,
    Triangle,
    Plane,
    ModelStatic,
    ModelAnimated
};

struct RevModelRetrievalData
{
    RevModelRetrievalData()
    {
        m_type = RevEModelType::Invalid;
    }

    RevModelRetrievalData(std::wstring path)
    {
        m_type = ModelStatic;
        m_path = path;
    }
    RevModelRetrievalData(RevEModelType type)
    {
        m_type = type;
        m_path = L"";
    }
    
    std::wstring m_path;
    RevEModelType m_type;
};