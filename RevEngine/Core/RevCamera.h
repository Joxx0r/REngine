#pragma once
#include <DirectXMath.h>

class D3D12HelloTriangle;
struct RevInputState;
using namespace DirectX;

class RevCamera
{
public:
    RevCamera();
    
    XMMATRIX m_matrix;
    
    int m_startX = 0;
    int m_startY = 0;
    float m_yaw = -90.0f;
    float m_pitch = 0;
    float m_aspectRatio = 0.0f;
    std::vector<XMMATRIX> m_matrices;

    XMVECTOR m_worldLoc;
    XMVECTOR m_direction;

    void OnMoveDelta(float deltaX, float deltaY);
    void Initialize(float aspectRatio);
    void Update(float tick, const RevInputState& input);
};


