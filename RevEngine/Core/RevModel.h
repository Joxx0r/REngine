#pragma once

#include "../TopLevelASGenerator.h"
#include "../ShaderBindingTableGenerator.h"
#include "../Core/RevCamera.h"
#include "../Core/RevModelManager.h"
#include "../Misc/RevTypes.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct AccelerationStructureBuffers
{
    ComPtr<ID3D12Resource> pScratch;      // Scratch memory for AS builder
    ComPtr<ID3D12Resource> pResult;       // Where the AS is
    ComPtr<ID3D12Resource> pInstanceDesc; // Hold the matrices of the instances
};


class RevModel
{
    
public:
    RevModel() {};

    void Initialize(int type, ID3D12Device5* device);

    AccelerationStructureBuffers CreateStructureBuffer(ID3D12Device5* device,ID3D12GraphicsCommandList4* list);
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    std::vector<Vertex> m_vertexes;
    std::vector<UINT> m_indices;

    ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};
