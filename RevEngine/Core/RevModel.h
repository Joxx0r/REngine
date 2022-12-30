#pragma once

#include "RevCoreDefines.h"
#include "../TopLevelASGenerator.h"
#include "../D3D/RevD3DTypes.h"

struct RevDrawData;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

class RevModel
{

public:
    RevModel() {};

    void Initialize(const RevModelData& modelData, REV_ID_HANDLE handle);

    void DrawRasterized(const RevDrawData& data) const;

    AccelerationStructureBuffers CreateStructureBuffer();

    AccelerationStructureBuffers m_relevantBuffers;
    RevModelD3DData m_d3dData;
    RevModelData m_modelData;

    RevEModelType m_type;
    REV_ID_HANDLE m_handle = REV_INDEX_NONE;
};
