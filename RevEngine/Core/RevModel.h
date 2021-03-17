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

    void Initialize(int type, int handle, ID3D12Device5* device);

    void DrawRasterized(ID3D12GraphicsCommandList4* list) const;
    
    AccelerationStructureBuffers CreateStructureBuffer(
           ID3D12Device5* device, ID3D12GraphicsCommandList4* list); 
    
    RevVertIndexData m_baseData;

    int m_type;
    REV_ID_HANDLE m_handle = REV_INDEX_NONE;
};
