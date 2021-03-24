#include "stdafx.h"
#include "RevModelConstructionFunctions.h"

#include "RevModelLoader.h"
#include "../D3D/RevD3DTypes.h"

RevModelData RevModelConstructionFunctions::CreateModelDataType(RevModelInitializationData inData)
{
    if(inData.m_type == RevEModelType::Triangle)
    {
        return RevModelConstructionFunctions::CreateTriangleData();
    }
    else
    if(inData.m_type == RevEModelType::Plane)
    {
        return RevModelConstructionFunctions::CreatePlaneData();
    }
    else
    if(inData.m_type == RevEModelType::ModelStatic)
    {
        return RevModelLoader::CreateModelDataFromFile(inData.m_path.c_str());
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
    returnData.m_inputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};
    returnData.m_shaderPath = L"Data//Shaders//Shaders.hlsl";
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
    returnData.m_shaderPath = L"Data//Shaders//Shaders.hlsl";
    returnData.m_inputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};
    return returnData;
}
