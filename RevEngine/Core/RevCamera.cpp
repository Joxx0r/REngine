#include "stdafx.h"
#include "RevCamera.h"
#include "../Misc/RevTypes.h"

RevCamera::RevCamera()
    :m_matrices()
{
    float pitchRadian = m_pitch * (XM_PI / 180.0f);
    float yawRadian = m_yaw* (XM_PI / 180.0f);
    m_direction = XMVectorSet(XMScalarCos(yawRadian * XMScalarCos(pitchRadian)), XMScalarSin(pitchRadian), XMScalarSin(yawRadian* XMScalarCos(pitchRadian)), 0.0f);
    m_worldLoc = XMVectorSet(0, 0, 1.5f, 1.0f);
    m_direction = XMVectorSet(0, 0, -1.0f, 0.0f);

}

void RevCamera::OnMoveDelta(float deltaX, float deltaY)
{
    const float sensitivity = 0.06f;
    m_yaw += sensitivity * deltaX;
    m_pitch += sensitivity * deltaY;
    if(m_yaw > 180.f)
    {
        float remaining = abs(m_yaw) - 180.0f;
        m_yaw = -180.0f + remaining;
    }
    else
        if(m_yaw < -180.f)
        {
            float remaining = -180.0f - abs(m_yaw);
            m_yaw = 180.0f + remaining;
        }
    if(m_pitch > 180.f)
    {
        float remaining = abs(m_pitch) - 180.0f;
        m_pitch = -180.0f + remaining;
    }
    else
        if(m_pitch < -180.f)
        {
            float remaining = -180.0f - abs(m_pitch);
            m_pitch = 180.0f + remaining;
        }

    float pitchRadian = m_pitch * (XM_PI / 180.0f);
    float yawRadian = m_yaw* (XM_PI / 180.0f);
    m_direction = XMVectorSet(XMScalarCos( yawRadian) * XMScalarCos(pitchRadian), XMScalarSin(pitchRadian), XMScalarSin(yawRadian )* XMScalarCos(pitchRadian), 0.0f);
}

void RevCamera::Initialize(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
    m_matrices.reserve(4);
    XMVECTOR Eye =m_worldLoc;
    XMVECTOR At =  m_worldLoc + m_direction;
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_matrices.push_back(XMMatrixLookAtRH(Eye, At, Up));
	
    float fovAngleY = 45.0f * XM_PI / 180.0f;
    m_matrices.push_back(
        XMMatrixPerspectiveFovRH(fovAngleY, m_aspectRatio, 0.1f, 1000.0f));

    // Raytracing has to do the contrary of rasterization: rays are defined in
    // camera space, and are transformed into world space. To do this, we need to
    // store the inverse m_matrices as well.
    XMVECTOR det2;
    m_matrices.push_back(XMMatrixInverse(&det2, m_matrices[0]));
    m_matrices.push_back(XMMatrixInverse(&det2, m_matrices[1]));
}

void RevCamera::Update(float tick, const RevInputState& input)
{
    float movementSpeed = 0.1;
    XMVECTOR det;
    XMMATRIX cameraView = XMMatrixInverse(&det, m_matrices[0]);
    XMVECTOR worldLoc = cameraView.r[3];
    XMVECTOR right =  cameraView.r[0];
    XMVECTOR up = cameraView.r[1];
    XMVECTOR forward = cameraView.r[2];
    if(input.m_right)
    {
        worldLoc += right * movementSpeed;
    }
    if(input.m_left)
    {
        worldLoc += -right * movementSpeed;
    }
    if(input.m_down)
    {
        worldLoc += -up * movementSpeed;
    }
    if(input.m_up)
    {
        worldLoc += up * movementSpeed;
    }
    if(input.m_forward)
    {
        worldLoc += -forward * movementSpeed;
    }
    if(input.m_back)
    {
        worldLoc += forward * movementSpeed;
    }
	
    m_worldLoc = worldLoc;
    // Initialize the view matrix, ideally this should be based on user
    // interactions The lookat and perspective matrices used for rasterization are
    // defined to transform world-space vertices into a [0,1]x[0,1]x[0,1] camera
    // space
    XMVECTOR Eye =m_worldLoc;
    XMVECTOR At =  m_worldLoc + m_direction;
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_matrices[0] = XMMatrixLookAtRH(Eye, At, Up);
	
    float fovAngleY = 45.0f * XM_PI / 180.0f;
    m_matrices[1] =
        XMMatrixPerspectiveFovRH(fovAngleY, m_aspectRatio, 0.1f, 1000.0f);

    // Raytracing has to do the contrary of rasterization: rays are defined in
    // camera space, and are transformed into world space. To do this, we need to
    // store the inverse m_matrices as well.
    XMVECTOR det2;
    m_matrices[2] = XMMatrixInverse(&det2, m_matrices[0]);
    m_matrices[3] = XMMatrixInverse(&det2, m_matrices[1]);
}
