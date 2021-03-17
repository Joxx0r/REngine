#pragma once

#include "RevCoreDefines.h"
#include "../TopLevelASGenerator.h"
#include "../D3D/RevD3DTypes.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class RevModel
{
    
public:
    RevModel() {};

    void Initialize(RevEModelType type, REV_ID_HANDLE handle);

    void DrawRasterized() const;
    
    AccelerationStructureBuffers CreateStructureBuffer(); 
    
    RevVertIndexData m_baseData;
    AccelerationStructureBuffers m_relevantBuffers;

    RevEModelType m_type;
    REV_ID_HANDLE m_handle = REV_INDEX_NONE;
};
