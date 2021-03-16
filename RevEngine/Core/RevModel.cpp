#include "stdafx.h"
#include "Windowsx.h"
#include "RevModel.h"
#include "../DXSampleHelper.h"

void RevModel::Initialize(int type, ID3D12Device5* device)
{
    if(type == 0)
    {
        {
            // Define the geometry for a triangle.
            m_vertexes = {
                {{sqrtf(8.f / 9.f), 0.f, -1.f / 3.f}, {1.f, 0.f, 0.f, 1.f}},
{{-sqrtf(2.f / 9.f), sqrtf(2.f / 3.f), -1.f / 3.f}, {0.f, 1.f, 0.f, 1.f}},
{{-sqrtf(2.f / 9.f), -sqrtf(2.f / 3.f), -1.f / 3.f}, {0.f, 0.f, 1.f, 1.f}},
{{0.f, 0.f, 1.f}, {1, 0, 1, 1}}};

            const UINT vertexBufferSize = sizeof(Vertex) * static_cast<UINT>(m_vertexes.size());

            // Note: using upload heaps to transfer static data like vert buffers is not
            // recommended. Every time the GPU needs it, the upload heap will be
            // marshalled over. Please read up on Default Heap usage. An upload heap is
            // used here for code simplicity and because there are very few verts to
            // actually transfer.
            ThrowIfFailed(device->CreateCommittedResource(
                // ReSharper disable once CppMsExtAddressOfClassRValue
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
                // ReSharper disable once CppMsExtAddressOfClassRValue
                &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
                D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                IID_PPV_ARGS(&m_vertexBuffer)));

            // Copy the triangle data to the vertex buffer.
            UINT8* pVertexDataBegin;
            CD3DX12_RANGE readRange(
                0, 0); // We do not intend to read from this resource on the CPU.
            ThrowIfFailed(m_vertexBuffer->Map(
                0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
            memcpy(pVertexDataBegin, m_vertexes.data(), vertexBufferSize);
            m_vertexBuffer->Unmap(0, nullptr);

            // Initialize the vertex buffer view.
            m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
            m_vertexBufferView.StrideInBytes = sizeof(Vertex);
            m_vertexBufferView.SizeInBytes = vertexBufferSize;            
        }

        {
            //----------------------------------------------------------------------------------------------
            // Indices
            m_indices = {0, 1, 2, 0, 3, 1, 0, 2, 3, 1, 3, 2};
            const UINT indexBufferSize = static_cast<UINT>(m_indices.size()) * sizeof(UINT);

            CD3DX12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
            CD3DX12_RESOURCE_DESC bufferResource = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
            ThrowIfFailed(device->CreateCommittedResource(
                &heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource, //
                D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_indexBuffer)));

            // Copy the triangle data to the index buffer.
            UINT8* pIndexDataBegin;
            CD3DX12_RANGE readRange(
            0, 0); // We do not intend to read from this resource on the CPU.
            ThrowIfFailed(m_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin)));
            memcpy(pIndexDataBegin, m_indices.data(), indexBufferSize);
            m_indexBuffer->Unmap(0, nullptr);

            // Initialize the index buffer view.
            m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
            m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
            m_indexBufferView.SizeInBytes = indexBufferSize;     
        }
    }
    else
    {
          // Define the geometry for a plane.
          Vertex planeVertices[] = {
              {{-1.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 0
              {{-1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 1
              {{01.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 2
              {{01.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 2
              {{-1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 1
              {{01.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}  // 4
          };
        
          const UINT planeBufferSize = sizeof(planeVertices);
        
          // Note: using upload heaps to transfer static data like vert buffers is not
          // recommended. Every time the GPU needs it, the upload heap will be
          // marshalled over. Please read up on Default Heap usage. An upload heap is
          // used here for code simplicity and because there are very few verts to
          // actually transfer.
          CD3DX12_HEAP_PROPERTIES heapProperty =
              CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
          CD3DX12_RESOURCE_DESC bufferResource =
              CD3DX12_RESOURCE_DESC::Buffer(planeBufferSize);
          ThrowIfFailed(device->CreateCommittedResource(
              &heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource, //
              D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
              IID_PPV_ARGS(&m_vertexBuffer)));
        
          // Copy the triangle data to the vertex buffer.
          UINT8 *pVertexDataBegin;
          CD3DX12_RANGE readRange(
              0, 0); // We do not intend to read from this resource on the CPU.
          ThrowIfFailed(m_vertexBuffer->Map(
              0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
          memcpy(pVertexDataBegin, planeVertices, sizeof(planeVertices));
          m_vertexBuffer->Unmap(0, nullptr);
        
          // Initialize the vertex buffer view.
          m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
          m_vertexBufferView.StrideInBytes = sizeof(Vertex);
          m_vertexBufferView.SizeInBytes = planeBufferSize;
    }
}
