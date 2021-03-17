#pragma once

#include "../Core/RevModelTypes.h"

using Microsoft::WRL::ComPtr;

struct AccelerationStructureBuffers
{
    ComPtr<ID3D12Resource> pScratch;      // Scratch memory for AS builder
    ComPtr<ID3D12Resource> pResult;       // Where the AS is
    ComPtr<ID3D12Resource> pInstanceDesc; // Hold the matrices of the instances
};

struct RevModelData
{
    std::vector<Vertex> m_vertexes;
    std::vector<UINT> m_indices;
    
    int GetIndexSize() { return m_indices.size() * sizeof(UINT); }
    int GetVertexSize(){ return m_vertexes.size() * sizeof(Vertex); }
};

struct RevModelD3DData
{
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};


struct RevVertInitializationData
{
    RevModelData m_generatedData;
};
    
struct RevVertIndexData
{
    RevModelData m_modelData;
    RevModelD3DData m_d3dData;

    static RevVertIndexData Create(const RevVertInitializationData& data);
    static AccelerationStructureBuffers CreateAccelerationStructure(
            const RevVertIndexData& inData); 
    
};

