#pragma once

#include "../Core/RevCoreDefines.h"
#include "../Core/RevModelTypes.h"

using Microsoft::WRL::ComPtr;

struct AccelerationStructureBuffers
{
    ComPtr<ID3D12Resource> pScratch;      // Scratch memory for AS builder
    ComPtr<ID3D12Resource> pResult;       // Where the AS is
    ComPtr<ID3D12Resource> pInstanceDesc; // Hold the matrices of the instances
};

enum class RevTextureType : UINT8
{
    Invalid,
    Diffuse,
    Normal,
    Substance,
    RoughnessAOEmissive
};

struct RevTexture
{
    RevTexture() { m_type = RevTextureType::Invalid; }
    RevTexture(const std::wstring& path, RevTextureType type)
    {
        m_path = path;
        m_resource = nullptr;
        m_type = type;
    }
    std::wstring m_path;
    ID3D12Resource* m_resource = nullptr;
    RevTextureType m_type;
};

struct RevTextureCollection
{
    std::vector<RevTexture> m_textures;
    int m_numTextures = 0;
};

struct RevModelData
{
    std::vector<RevVertexPosCol> m_vertexes;
    std::vector<RevVertexPosTexNormBiTan> m_staticVertexes;
    std::vector<UINT> m_indices;
    RevTextureCollection m_textures;
    RevEModelType m_type = RevEModelType::Invalid;
    
    int GetModelIndexSize() const { return m_indices.size() * sizeof(UINT); }
    
    int GetModelVertexSize() const
    {
        return GetNumVertexes() * GetVertexStride();
    }
    int GetNumVertexes() const
    {
        if(m_vertexes.size() > 0)
        {
            return m_vertexes.size();   
        }
        else
        {
           return m_staticVertexes.size();   
        }
    }
    int GetVertexStride() const
    {
        if(m_vertexes.size() > 0)
        {
           return sizeof(RevVertexPosCol);   
        }
        else
        {
           return sizeof(RevVertexPosTexNormBiTan);   
        }
    }

    void* GetData()
    {
        if(m_vertexes.size() > 0)
        {
            return m_vertexes.data();   
        }
        else
        {
            return m_staticVertexes.data();   
        }
    }
    
    const void* GetData() const
    {
        if(m_vertexes.size() > 0)
        {
            return m_vertexes.data();   
        }
        else
        {
            return m_staticVertexes.data();   
        }
    }
};

struct RevModelD3DData
{
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    
    int m_vertexCount = REV_INDEX_NONE;
    int m_indexCount = REV_INDEX_NONE;
    int m_vertexStride = REV_INDEX_NONE;

    static RevModelD3DData Create(const RevModelData& data);
    static AccelerationStructureBuffers CreateAccelerationStructure(const RevModelD3DData& inData); 
};