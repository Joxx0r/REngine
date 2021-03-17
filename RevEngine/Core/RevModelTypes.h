#pragma once

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};

enum RevEModelType : UINT8
{
    Invalid,
    Triangle,
    Plane
};