#include "stdafx.h"
#include "Windowsx.h"
#include "RevModel.h"

#include "../BottomLevelASGenerator.h"
#include "../DXRHelper.h"
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
        m_vertexes = {
              {{-1.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 0
              {{-1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 1
              {{01.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 2
              {{01.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 2
              {{-1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 1
              {{01.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}  // 4
          };
        
          const UINT planeBufferSize = m_vertexes.size() * sizeof(Vertex);
        
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
          memcpy(pVertexDataBegin, m_vertexes.data(), planeBufferSize);
          m_vertexBuffer->Unmap(0, nullptr);
        
          // Initialize the vertex buffer view.
          m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
          m_vertexBufferView.StrideInBytes = sizeof(Vertex);
          m_vertexBufferView.SizeInBytes = planeBufferSize;
    }
}
AccelerationStructureBuffers RevModel::CreateStructureBuffer(ID3D12Device5* device, ID3D12GraphicsCommandList4* list)
{
    nv_helpers_dx12::BottomLevelASGenerator bottomLevelAS;
	// Adding all vertex buffers and not transforming their position.
	if(m_vertexBuffer.Get() != nullptr)
	{
		if(m_indexBuffer.Get() != nullptr)
		{
			int numVertex = m_vertexes.size();
			int numIndicies = m_indices.size();
			bottomLevelAS.AddVertexBuffer(m_vertexBuffer.Get(), 0,
                               numVertex, sizeof(Vertex),
                               m_indexBuffer.Get(), 0,
                               numIndicies, nullptr, 0, true);	
		}
		else
		{
			int numVertex = m_vertexes.size();
			bottomLevelAS.AddVertexBuffer(m_vertexBuffer.Get(), 0,
                                          numVertex, sizeof(Vertex), 0,
                                          0);
		}
	}

	// The AS build requires some scratch space to store temporary information.
	// The amount of scratch memory is dependent on the scene complexity.
	UINT64 scratchSizeInBytes = 0;
	// The final AS also needs to be stored in addition to the existing vertex
	// buffers. It size is also dependent on the scene complexity.
	UINT64 resultSizeInBytes = 0;

	bottomLevelAS.ComputeASBufferSizes(device, false, &scratchSizeInBytes,
	                                   &resultSizeInBytes);

	// Once the sizes are obtained, the application is responsible for allocating
	// the necessary buffers. Since the entire generation will be done on the GPU,
	// we can directly allocate those on the default heap
	AccelerationStructureBuffers buffers;
	buffers.pScratch = nv_helpers_dx12::CreateBuffer(
		device, scratchSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON,
		nv_helpers_dx12::kDefaultHeapProps);
	buffers.pResult = nv_helpers_dx12::CreateBuffer(
		device, resultSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		nv_helpers_dx12::kDefaultHeapProps);

	// Build the acceleration structure. Note that this call integrates a barrier
	// on the generated AS, so that it can be used to compute a top-level AS right
	// after this method.
	bottomLevelAS.Generate(list, buffers.pScratch.Get(),
	                       buffers.pResult.Get(), false, nullptr);

	return buffers;
}
