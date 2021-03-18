#include "stdafx.h"
#include "RevModelConstructionFunctions.h"
#include "../D3D/RevD3DTypes.h"

RevModelData RevModelConstructionFunctions::CreateModelDataType(RevEModelType type)
{
    if(type == RevEModelType::Triangle)
    {
        return RevModelConstructionFunctions::CreateTriangleData();
    }
    else
    if(type == RevEModelType::Plane)
    {
        return RevModelConstructionFunctions::CreatePlaneData();
    }
    return RevModelData();
}

RevModelData RevModelConstructionFunctions::CreateTriangleData()
{
    RevModelData returnData = {};
    returnData.m_type = RevEModelType::Triangle;
    returnData.m_vertexes = {
        {{sqrtf(8.f / 9.f), 0.f, -1.f / 3.f}, {1.f, 0.f, 0.f, 1.f}},
        {{-sqrtf(2.f / 9.f), sqrtf(2.f / 3.f), -1.f / 3.f}, {0.f, 1.f, 0.f, 1.f}},
        {{-sqrtf(2.f / 9.f), -sqrtf(2.f / 3.f), -1.f / 3.f}, {0.f, 0.f, 1.f, 1.f}},
        {{0.f, 0.f, 1.f}, {1, 0, 1, 1}}};

    {
        returnData.m_indices = {0, 1, 2, 0, 3, 1, 0, 2, 3, 1, 3, 2};
    }

    return returnData;
}

RevModelData RevModelConstructionFunctions::CreatePlaneData()
{
    RevModelData returnData = {};
    returnData.m_type = RevEModelType::Plane;
    returnData.m_vertexes = {
        {{-1.5f, -.8f, 1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 0
        {{-1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 1
        {{1.5f, -.8f, 1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 2
        {{1.5f, -.8f, 1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 2
        {{-1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 1
        {{1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}  // 4
    };
    return returnData;
}
